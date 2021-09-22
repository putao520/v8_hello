#pragma once
#include <utility>
#include "queue_ring_exception.h"

using namespace std;

template<typename T>
class queue_ring {
public:
	queue_ring();
	queue_ring(unsigned int);

	queue_ring<T>* push(T v) {
		buffer_ring[buffer_ring_max_index][behind_pos] = v;
		behind_pos++;
		if (behind_pos >= max_size) {
			buffer_ring_max_index++;

			// 最大buffer块超过10块,申请2倍大小指针
			if (buffer_ring_max_index >= buffer_block_max) {
				// 当前使用block块总数是否大于当前设定最大block块总量
				unsigned int len = buffer_ring_max_index - buffer_ring_min_index;
				if (len >= buffer_block_max) {
					buffer_block_max *= 2;
				}
				// 申请新 buffer_ring Array
				T** _buffer_ring = (T**)malloc( sizeof(T *) * buffer_block_max);
				for (unsigned int i = 0; i < len; i++) {
					_buffer_ring[i] = buffer_ring[buffer_ring_min_index + i];
				}
				buffer_ring = _buffer_ring;

				buffer_ring_max_index -= buffer_ring_min_index;
				buffer_ring_min_index = 0;
			}

			// 生成新 buffer 
			buffer_ring[buffer_ring_max_index] = (T*)malloc(sizeof(T) * max_size);
			// 计算下次 push 位置
			behind_pos = 0;
		}
		return this;
	}
	T&& pop() {

		if ( buffer_ring_min_index > buffer_ring_max_index ) {
			throw queue_ring_exception();
		}

		if ((buffer_ring_min_index == buffer_ring_max_index) && (front_pos == behind_pos)) {
			throw queue_ring_exception();
		}

		auto r = buffer_ring[buffer_ring_min_index][front_pos];
		front_pos++;
		if (front_pos >= max_size) {
			// 销毁当前 buffer(指针释放BUG)
			if (buffer_ring_min_index < buffer_ring_max_index) {
				free( buffer_ring[buffer_ring_min_index] );
				buffer_ring_min_index++;
			}

			// 计算下次 pop 位置
			front_pos = 0;
		}

		return move(r);
	}
private:
	void init();

	unsigned int max_size = 1000;

	unsigned int front_pos;		// 队头
	unsigned int behind_pos;	// 队尾	

	unsigned int buffer_ring_max_index;
	unsigned int buffer_ring_min_index;

	unsigned int buffer_block_max;
	T** buffer_ring;
};