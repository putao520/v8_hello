#include "v8vm.h"
#include "file_until.h"
#include "text_encode.h"
#include "type_traits"
#include "v8env.h"
#include "v8time.h"
#include "v8interval.h"
#include <vector>
#include <chrono>
#include <thread>
#include <restclient-cpp/restclient.h>
using namespace std::filesystem;
vector<Isolate*> isolate_list;

v8vm::v8vm(): _script(nullptr), _isolate(buildIsolate()) {}

v8vm::v8vm(const v8vm& that) : _script(that._script), _isolate(buildIsolate()) {}

v8vm::v8vm(const v8vm&& that) noexcept : _script(that._script), _isolate(that._isolate) {}

vector<Isolate*> v8vm::getAllIsolate() {
	return isolate_list;
}


v8vm* v8vm::wait() {
	// 获得异步队列对象
	while (!v8time::current(_isolate)->isEmpty())
		platform::PumpMessageLoop(g_default_platform.get(), _isolate);

	// 获得异步对象
	while (!v8interval::current(_isolate)->isEmpty())
		platform::PumpMessageLoop(g_default_platform.get(), _isolate);

	while (platform::PumpMessageLoop(g_default_platform.get(), _isolate))
		continue;

	return this;
}

Isolate* v8vm::buildIsolate() {
	v8env::New();
	p_create_params = make_unique<Isolate::CreateParams>();
	p_create_params->array_buffer_allocator = ArrayBuffer::Allocator::NewDefaultAllocator();
	Isolate* p = Isolate::New(*p_create_params.get());
	isolate_list.push_back(p);
	return p;
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

v8vm&& v8vm::from(const char* uri) {
	RestClient::Response r = RestClient::get(uri);
	if (r.code == 200) {
		this->script(r.body.c_str());
	}
	return move(*this);
}