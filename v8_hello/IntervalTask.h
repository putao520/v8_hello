#pragma once
#include <stdlib.h>
#include <iostream>
#include <include/libplatform/libplatform.h>
#include <include/v8.h>
#include <functional>

using namespace v8;
using namespace std;
class IntervalTask : public Task {
private:
	v8::Isolate* _isolate;
	double delay;
	unsigned int taskId;
public:
	IntervalTask(v8::Isolate* isolate);
	IntervalTask(IntervalTask* that);
	IntervalTask(IntervalTask&& that) noexcept;
	IntervalTask* setTask(unsigned taskId, double delay);

	Isolate* getIsolate();
	void Run();
};