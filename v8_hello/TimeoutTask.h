#pragma once
#include <stdlib.h>
#include <iostream>
#include <include/libplatform/libplatform.h>
#include <include/v8.h>
#include <functional>

using namespace v8;
using namespace std;
class TimeoutTask : public Task {
private:
	v8::Isolate* _isolate;
	function<void()> pfn;
public:
	TimeoutTask(v8::Isolate* isolate);
	TimeoutTask(TimeoutTask&& that) noexcept;
	TimeoutTask* setTask(const function<void()>&& fn);

	Isolate* getIsolate();
	void Run();
};