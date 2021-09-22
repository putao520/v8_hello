#include "v8interval.h"
#include "IntervalTask.h"
#include "v8callback.h"
#include "v8context.h"
#include <functional>

extern unique_ptr<Platform> g_default_platform;

// unordered_map<unsigned int, Persistent_Function> interval_callback_table;

unordered_map<Isolate*, v8interval*> interval_map;
void SetInterval(const FunctionCallbackInfo<v8::Value>& args);
void ClearInterval(const FunctionCallbackInfo<v8::Value>& args);
v8interval::v8interval(Isolate* isolate) :
	task_max(0), 
	_isolate(isolate) {
}

v8interval::~v8interval() {
}

void v8interval::setup(Local<ObjectTemplate> global) {
	// 构造全局对象
	global->Set(
		String::NewFromUtf8(_isolate, "setInterval", NewStringType::kNormal).ToLocalChecked(),
		FunctionTemplate::New(_isolate, SetInterval)
	);
	global->Set(
		String::NewFromUtf8(_isolate, "clearInterval", NewStringType::kNormal).ToLocalChecked(),
		FunctionTemplate::New(_isolate, ClearInterval)
	);
}

v8interval* v8interval::current(Isolate* isolate) {
	if( !interval_map.contains(isolate) )
		interval_map[isolate] = new v8interval(isolate);
	return interval_map[isolate];
}

bool v8interval::isEmpty() {
	return interval_callback_table.empty();
}

unsigned int v8interval::getIdleTaskId() {
	for (auto& v : interval_callback_table) {
		auto p = v.second;
		if (p.IsEmpty()) {
			return v.first;
		}
	}
	// 新增 空句柄
	task_max++;
	Persistent_Function empty_handle;
	interval_callback_table[task_max] = empty_handle;
	return task_max;
}

unsigned int SetIntervalImpl(Isolate* isolate, const FunctionCallbackInfo<v8::Value>& args) {
	if (args.Length() == 0 || !args[0]->IsFunction()) return 0;
	
	v8interval* interval = v8interval::current(isolate);

	auto _taskId = interval->getIdleTaskId();

	// 此处上下文没有问题
	double delay = 0.0;
	if (args.Length() >= 2) {
		if (args[1]->IsNumber()) {
			delay = args[1]->NumberValue(isolate->GetCurrentContext()).ToChecked();
		}
	}

	interval->interval_callback_table[_taskId].Reset(isolate, args[0].As<Function>());
	// asinterval_callback_table[_taskId].Reset(isolate, args[0].As<Function>());

	unique_ptr<IntervalTask> pTask = make_unique<IntervalTask>(isolate);
	pTask->setTask(_taskId, delay);

	g_default_platform
		->GetForegroundTaskRunner(isolate)
		->PostDelayedTask(move(pTask), delay / static_cast<double>(1000.0));

	return _taskId;
}
void SetInterval(const FunctionCallbackInfo<v8::Value>& args) {
	Isolate* isolate = args.GetIsolate();
	// 返回时间任务id
	args.GetReturnValue().Set(v8::Number::New(isolate, SetIntervalImpl(isolate, args)));
}

void ClearInterval(const FunctionCallbackInfo<v8::Value>& args) {
	Isolate* isolate = args.GetIsolate();
	if (args[0]->IsNumber()) {
		v8interval* interval = v8interval::current(isolate);
		auto taskId = args[0]->Int32Value(isolate->GetCurrentContext()).ToChecked();
		
		if (interval->interval_callback_table.contains(taskId)) {
			interval->interval_callback_table[taskId].Reset();
			interval->interval_callback_table.erase(taskId);
		}
	}
}
