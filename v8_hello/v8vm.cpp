#include "v8vm.h"
#include "file_until.h"
#include "text_encode.h"
#include "type_traits"
#include "v8env.h"
#include "v8context.h"
#include "v8time.h"

using namespace std::filesystem;
v8vm::v8vm(): _script(nullptr), _isolate(buildIsolate()) {}

v8vm::v8vm(const v8vm& that) : _script(that._script), _isolate(buildIsolate()) {}

v8vm::v8vm(const v8vm&& that) noexcept : _script(that._script), _isolate(that._isolate) {}

Local<Context> v8vm::buildGlobalContext() {
	// ÃÓ≥‰∂‘œÛ
	Local<ObjectTemplate> global = v8::ObjectTemplate::New(_isolate);
	v8context::Bind(global, String::NewFromUtf8(_isolate, "Context", NewStringType::kInternalized).ToLocalChecked());

	// ÃÓ≥‰∫Ø ˝
	v8time::New(_isolate)->setupSetTimeout(global);

	return Context::New(_isolate, NULL, global);
}

Isolate* v8vm::buildIsolate() {
	v8env::New();
	p_create_params = make_unique<Isolate::CreateParams>();
	p_create_params->array_buffer_allocator = ArrayBuffer::Allocator::NewDefaultAllocator();
	return Isolate::New(*p_create_params.get());
}

v8vm::~v8vm() {
	_isolate->Dispose();
	v8env::Dispose();
	delete p_create_params.get()->array_buffer_allocator;
}

v8vm&& v8vm::script(const char * script) {
	_script = script;
	return move(*this);
}

v8vm&& v8vm::load(const char* file) {
	FileUntil fileIo(file);
	char* script = static_cast<char *>(fileIo.toMemory());
	if (script) {
		script = TextEncode::utf8(script);

		this->script(script);
	}
	return move(*this);
}