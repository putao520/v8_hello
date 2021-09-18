#include "v8callback.h"

v8callback::v8callback(FunctionCallbackInfo<Value> args_):
	args(args_)
{}


FunctionCallbackInfo<Value> v8callback::getArgs() {
	return args;
}