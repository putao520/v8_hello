#include "v8inject.h"

int x = 1000;

void Getter(Local<String> property,
	const PropertyCallbackInfo<Value>& info) {
	info.GetReturnValue().Set(x);
}

void Setter(Local<String> property, Local<Value> value,
	const PropertyCallbackInfo<void>& info) {
	x = value->Int32Value(
		info.GetIsolate()->GetCurrentContext()
	).ToChecked();
}

void v8inject::Bind(Local<ObjectTemplate> global, Local<String> name) {
	// 构造全局对象
	global->SetAccessor(name, Getter, Setter);
}
