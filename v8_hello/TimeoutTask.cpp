#include "TimeoutTask.h"
#include "v8time.h"


void TimeoutTask::Run() {
	// printf("test it!\n");
	this->pfn();
	v8time::current(_isolate)->runner_inc();
}

TimeoutTask* TimeoutTask::setTask(const function<void()>&& fn) {
	this->pfn = fn;
	return this;
}

TimeoutTask::TimeoutTask(Isolate* isolate) : _isolate(isolate) {
}

TimeoutTask::TimeoutTask(TimeoutTask&& that) noexcept {
	this->pfn = that.pfn;
	this->_isolate = that._isolate;
}

Isolate* TimeoutTask::getIsolate() {
	return this->_isolate;
}