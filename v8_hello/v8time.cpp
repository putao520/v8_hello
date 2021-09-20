#include "v8time.h"
#include "TimeoutTask.h"
#include <functional>
#include "v8callback.h"
#include "v8context.h"

v8time* global_v8t = nullptr;

atomic<unsigned int> taskId(0);
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

v8time* v8time::New(Isolate* isolate) {
	return global_v8t ? global_v8t : new v8time(isolate);
}
/**
* SetTimeout
*/

unsigned int SetTimeoutImpl(Isolate* isolate, const FunctionCallbackInfo<v8::Value>& args) {
	
	if (args.Length() == 0 || !args[0]->IsFunction()) return 0;
	taskId++;

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
