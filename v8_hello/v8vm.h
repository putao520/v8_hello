#pragma once
#include <stdlib.h>
#include <iostream>
#include <include/libplatform/libplatform.h>
#include <include/v8.h>
#include <unordered_map>

using namespace v8;
using namespace std;
/**
* 实现 v8 执行引擎的Cpp封装
*/
class v8vm {
public:
	~v8vm();
	v8vm&& script(const char* script);
	v8vm&& load(const char* file);

	template<typename T>
	T exec() {
		T r;
		Isolate::Scope _isolate_scope(_isolate);
		{
			HandleScope handle_scope(_isolate);

			Local<Context> _context = buildGlobalContext();

			// 注入全局对象

			Context::Scope context_scope(_context);
			{
				MaybeLocal<String> source = String::NewFromUtf8(_isolate, _script);

				Local<Value> result;
				TryCatch trycatch(_isolate);

				MaybeLocal<Script> rScript = Script::Compile(_context, source.ToLocalChecked());
				if (!rScript.IsEmpty()) {
					Local<Script> script = rScript.ToLocalChecked();
					MaybeLocal<Value> rResult = script->Run(_context);
					if (!rResult.IsEmpty()) {
						result = rResult.ToLocalChecked();
					}
					else {
						result = trycatch.Exception();
					}
				}
				else {
					result = trycatch.Exception();
				}

				// 字符串
				if constexpr (is_same<decay<T>::type, string>::value) {
					String::Utf8Value utf8(_isolate, result);
					string str = *utf8;
					r = str;
				}
				// 有符号 32位 整数
				else if constexpr (is_signed<T>::value && is_integral<T>::value && sizeof(T) == 4 ) {
					r = result->Int32Value(_context).ToChecked();
						
				}
				// 无符号 32位 整数
				else if constexpr (is_unsigned<T>::value && is_integral<T>::value && sizeof(T) == 4) {
					r = result->Uint32Value(_context).ToChecked();

				}
				// 有符号 64位 整数
				else if constexpr (is_signed<T>::value && is_integral<T>::value && sizeof(T) == 8) {
					r = result->IntegerValue(_context).ToChecked();

				}
				// 无符号 64位 整数
				else if constexpr (is_unsigned<T>::value && is_integral<T>::value && sizeof(T) == 8) {
					r = static_cast<uint64_t>(result->IntegerValue(_context).ToChecked());
				}
				// double
				else if constexpr (is_floating_point<T>::value && sizeof(T) == 8) {
					r = result->NumberValue(_context).ToChecked();
				}
				// float
				else if constexpr (is_floating_point<T>::value && sizeof(T) == 44) {
					r = static_cast<float>(result->NumberValue(_context).ToChecked());
				}
			}

			auto v = _context->GetMicrotaskQueue()->IsRunningMicrotasks();
		}
		return r;
	}

	v8vm();
	v8vm(const v8vm& that);
	v8vm(const v8vm&& that) noexcept;

private:
	Isolate* buildIsolate();
	Local<Context> buildGlobalContext();

	unique_ptr<Isolate::CreateParams> p_create_params;
	Isolate* _isolate;
	const char* _script;
};