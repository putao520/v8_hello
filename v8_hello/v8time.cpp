#include "v8time.h"
#include "TimeoutTask.h"
#include "v8callback.h"
#include "v8context.h"
#include <unordered_map>
#include <functional>

using namespace std;

// 多隔离环境支持
unordered_map<Isolate*, v8time*> time_map;
extern unique_ptr<Platform> g_default_platform;
void SetTimeout(const FunctionCallbackInfo<v8::Value>& args);
v8time::v8time(Isolate* isolate): 
	_isolate(isolate){
}

v8time::~v8time() {
}

void v8time::setup(Local<ObjectTemplate> global) {
	// 构造全局对象
	global->Set(
		String::NewFromUtf8(_isolate, "setTimeout", NewStringType::kNormal).ToLocalChecked(),
		FunctionTemplate::New(_isolate, SetTimeout)
	);
}

v8time* v8time::current(Isolate* isolate) {
	if (!time_map.contains(isolate))
		time_map[isolate] = new v8time(isolate);
	return time_map[isolate];
}

unsigned int v8time::task_inc() {
	taskId++;
	return taskId;
}

void v8time::runner_inc() {
	runnerId++;
}

bool v8time::isEmpty() {
	return taskId <= runnerId;
}
/**
* SetTimeout
*/

unsigned int SetTimeoutImpl(Isolate* isolate, const FunctionCallbackInfo<v8::Value>& args) {
	
	if (args.Length() == 0 || !args[0]->IsFunction()) return 0;
	
	auto taskId = v8time::current(isolate)->task_inc();

	// 此处上下文没有问题
	double delay = 0.0;
	if (args.Length() >= 2) {
		if (args[1]->IsNumber()) {
			delay = args[1]->NumberValue(isolate->GetCurrentContext()).ToChecked();
		}
	}

	Persistent<Function, CopyablePersistentTraits<Function>> pfn(isolate, args[0].As<Function>());

	unique_ptr<TimeoutTask> pTask = make_unique<TimeoutTask>(isolate);
	pTask->setTask([isolate, pfn]() {
		Isolate::Scope isolate_scope(isolate);
		{
			HandleScope handle_scope(isolate);
			Local<Context> context = isolate->GetCurrentContext();
			if (!context.IsEmpty()) {
				Context::Scope context_scope(context);
				{
					TryCatch trycatch(isolate);
					Local<Function> callback = pfn.Get(isolate);
					callback->CallAsFunction(context, Undefined(isolate), 0, nullptr);
					pfn.~Persistent();
				}
			}
		}

		});

	g_default_platform
		->GetForegroundTaskRunner(isolate)
		->PostDelayedTask(move(pTask), delay / static_cast<double>(1000.0));

	return taskId;
}
void SetTimeout(const FunctionCallbackInfo<v8::Value>& args) {
	Isolate* isolate = args.GetIsolate();
	// 返回时间任务id
	args.GetReturnValue().Set(v8::Number::New(isolate, SetTimeoutImpl(isolate, args)));
}
