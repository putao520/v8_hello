#include "v8context.h"
#include "v8inject.h"
#include "v8time.h"
#include "v8interval.h"
#include "v8print.h"

#define SETUP_FUNC(type, isolate) type::current(isolate)->setup(global);

Local<Context> v8context::New(Isolate* isolate) {
	// Ìî³ä¶ÔÏó
	Local<ObjectTemplate> global = v8::ObjectTemplate::New(isolate);
	v8inject::Bind(global, String::NewFromUtf8(isolate, "Inject", NewStringType::kInternalized).ToLocalChecked());

	// Ìî³äº¯Êý
	SETUP_FUNC(v8time, isolate)
	SETUP_FUNC(v8interval, isolate)
	SETUP_FUNC(v8print, isolate)
	
	return Context::New(isolate, NULL, global);
}