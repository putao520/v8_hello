#include "pch.h"
#include "CppUnitTest.h"
#include "../v8_hello/queue_ring.h"
#include "../v8_hello/queue_ring.cpp"
#include "../v8_hello/queue_ring_exception.h"
#include "../v8_hello/queue_ring_exception.cpp"
#include "ClassDemo.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace QueueRingTest
{
	TEST_CLASS(QueueRingTest)
	{
	public:
		
		TEST_METHOD(TestQueueRing)
		{
			queue_ring<ClassDemo<unsigned int>> queue(2);

			// 背景:因为最大 block 块为 5 ，每个块2条数据
			// 触发:块新增
			for (unsigned int i = 0; i < 10; i++) {
				ClassDemo<unsigned int> v(i);
				queue.push(v);
			}

			// 触发:块复制
			ClassDemo<unsigned int> v(10);
			queue.push(v);

			// 触发:块清除
			for (unsigned int i = 0; i < 11; i++) {
				ClassDemo<unsigned int> v = queue.pop();
				printf("[%d]->%d\n", i, v.getValue());
				Assert::AreEqual(v.getValue(), i);
			}
			try {
				// 触发:块清除溢出
				queue.pop();
			}
			catch (queue_ring_exception& e) {
				const char* info = e.what();
				Assert::AreEqual(info, "queue pop overflow");
			}

		}
	};
}
