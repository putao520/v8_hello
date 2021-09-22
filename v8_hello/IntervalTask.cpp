#include "IntervalTask.h"
#include "v8interval.h"

extern unique_ptr<Platform> g_default_platform;
// extern unordered_map<unsigned int, Persistent<Function, CopyablePersistentTraits<Function>>> interval_callback_table;
void IntervalTask::Run() {
	// 判断任务号是否有效
	v8interval* interval = v8interval::current(_isolate);
	auto pfn = interval->interval_callback_table[taskId];
	if (!pfn.IsEmpty()) {
		Isolate::Scope isolate_scope(_isolate);
		{
			HandleScope handle_scope(_isolate);
			Local<Context> context = _isolate->GetCurrentContext();
			if (!context.IsEmpty()) {
				Context::Scope context_scope(context);
				{
					TryCatch trycatch(_isolate);
					Local<Function> callback = pfn.Get(_isolate);
					callback->CallAsFunction(context, Undefined(_isolate), 0, nullptr);
				}
			}
		}
		// 重新投递实现循环
		unique_ptr<IntervalTask> task = make_unique<IntervalTask>(this);
		g_default_platform
			->GetForegroundTaskRunner(_isolate)
			->PostDelayedTask(move(task), delay / static_cast<double>(1000.0));
	}
}

IntervalTask* IntervalTask::setTask(unsigned taskId, double delay) {
	this->taskId = taskId;
	this->delay = delay;
	return this;
}

IntervalTask::IntervalTask(Isolate* isolate) : _isolate(isolate), delay(0.0), taskId(0) {
}

IntervalTask::IntervalTask(IntervalTask&& that) noexcept {
	this->taskId = that.taskId;
	this->delay = that.delay;
	this->_isolate = that._isolate;
}

IntervalTask::IntervalTask(IntervalTask* that) {
	this->taskId = that->taskId;
	this->delay = that->delay;
	this->_isolate = that->_isolate;
}


Isolate* IntervalTask::getIsolate() {
	return this->_isolate;
}