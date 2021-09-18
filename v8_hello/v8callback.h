#pragma once
#include <stdlib.h>
#include <iostream>
#include <include/libplatform/libplatform.h>
#include <include/v8.h>

using namespace v8;
using namespace std;
class v8callback {
private:
	FunctionCallbackInfo<Value> args;
public:
	v8callback(FunctionCallbackInfo<Value> args_);

	FunctionCallbackInfo<Value> getArgs();
	
};