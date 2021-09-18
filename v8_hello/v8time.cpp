#include "v8time.h"
#include "TimeoutTask.h"
#include <functional>
#include "v8callback.h"

atomic<unsigned int> taskId(0);
extern unique_ptr<Platform> g_default_platform;

void SetTimeout(const FunctionCallbackInfo<v8::Value>& args);

v8time::v8time(Isolate* isolate): 
	_isolate(isolate){
}

v8time::~v8time() {
}

void v8time::setupSetTimeout(Local<ObjectTemplate> global) {
	// ����ȫ�ֶ���
	global->Set(
		String::NewFromUtf8(_isolate, "setTimeout", NewStringType::kNormal).ToLocalChecked(),
		FunctionTemplate::New(_isolate, SetTimeout)
	);
}

// ע���ڴ�й©
v8time* v8time::New(Isolate* isolate) {
	v8time* v8t = new v8time(isolate);
	return v8t;
}
/**
* SetTimeout
*/
bool runSetTimeout(TimeoutTask* pTask) {
	// �ô�������������Ч
	Isolate* isolate = pTask->getIsolate();
	auto runner = g_default_platform->GetForegroundTaskRunner(isolate);

	unique_ptr<TimeoutTask> _task = make_unique<TimeoutTask>(isolate);

	runner->PostTask(move(_task));

	return true;
}

unsigned int SetTimeoutImpl(Isolate* isolate, const FunctionCallbackInfo<v8::Value>& args) {
	
	if (args.Length() == 0 || !args[0]->IsFunction()) return 0;
	taskId++;
	Local<Function> callback = args[0].As<Function>();
	Local<Context> context = isolate->GetCurrentContext();

	// �˴�������û������
	double delay = 0.0;
	if (args.Length() >= 2) {
		if (args[1]->IsNumber()) {
			delay = args[1]->NumberValue(context).ToChecked();
		}
	}

	TimeoutTask* pTimeroutTask = new TimeoutTask(isolate);
	pTimeroutTask->setTask([pTimeroutTask]() {
		runSetTimeout(pTimeroutTask);
		});

	unique_ptr<TimeoutTask> pTask(pTimeroutTask);

	g_default_platform.get()->CallDelayedOnWorkerThread(move(pTask), delay / static_cast<double>(1000.0));

	return taskId;
}
void SetTimeout(const FunctionCallbackInfo<v8::Value>& args) {
	Isolate* isolate = args.GetIsolate();
	// ����ʱ������id
	args.GetReturnValue().Set(v8::Number::New(isolate, SetTimeoutImpl(isolate, args)));
}