#include "queue_ring.h"

template<typename T>
queue_ring<T>::queue_ring() : max_size(1000) {
	init();
}

template<typename T>
queue_ring<T>::queue_ring(unsigned int _max_size) : max_size(_max_size) {
	init();
}

template<typename T>
void queue_ring<T>::init() {
	front_pos = behind_pos = 0;
	buffer_ring_max_index = buffer_ring_min_index = 0;
	buffer_block_max = 5;
	buffer_ring = (T**)malloc(sizeof(T*) * buffer_block_max);
	buffer_ring[buffer_ring_max_index] = (T *)malloc(sizeof(T) * max_size);
}