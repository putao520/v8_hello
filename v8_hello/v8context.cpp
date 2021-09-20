#include "v8context.h"
#include "v8inject.h"
#include "v8time.h"
#include "v8print.h"

#define SETUP_FUNC(type, isolate) type::New(isolate)->setup(global);

Local<Context> v8context::New(Isolate* isolate) {
	// 野割斤��
	Local<ObjectTemplate> global = v8::ObjectTemplate::New(isolate);
	v8inject::Bind(global, String::NewFromUtf8(isolate, "Inject", NewStringType::kInternalized).ToLocalChecked());

	// 野割痕方
	SETUP_FUNC(v8time, isolate)
	SETUP_FUNC(v8print, isolate)

	return Context::New(isolate, NULL, global);
}