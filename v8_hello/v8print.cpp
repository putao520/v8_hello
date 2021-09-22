#include "v8print.h"


v8print* global_v8t = nullptr;
v8print::v8print(Isolate* isolate) :
	_isolate(isolate) {
}

v8print::~v8print() {
}

void Print(const FunctionCallbackInfo<v8::Value>& args) {
	Isolate* isolate = args.GetIsolate();
	args.GetReturnValue().Set(Undefined(isolate));
	Local<Value> result = args[0].As<Value>();
	String::Utf8Value utf8(isolate, result);
	printf("%s\n", *utf8);
}


void v8print::setup(Local<ObjectTemplate> global) {
	// 构造全局对象
	global->Set(
		String::NewFromUtf8(_isolate, "print", NewStringType::kNormal).ToLocalChecked(),
		FunctionTemplate::New(_isolate, Print)
	);
}

// 注意内存泄漏
v8print* v8print::current(Isolate* isolate) {
	return global_v8t ? global_v8t : new v8print(isolate);
}