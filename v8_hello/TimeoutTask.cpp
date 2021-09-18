#include "TimeoutTask.h"


void TimeoutTask::Run() {
	printf("test it!");
	this->pfn();
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