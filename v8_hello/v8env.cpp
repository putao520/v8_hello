#include "v8env.h"

atomic<uint32_t> ref = 0;

unique_ptr<Platform> g_default_platform;
void v8env::New() {
	if (::ref == 0) {
		V8::InitializeICUDefaultLocation(nullptr);
		V8::InitializeExternalStartupData(nullptr);
		g_default_platform = v8::platform::NewDefaultPlatform();
		V8::InitializePlatform(g_default_platform.get());
		V8::Initialize();
	}
	::ref++;
}

void v8env::Dispose() {
	if(::ref)
		::ref--;
	if (::ref == 0) {
		V8::Dispose();
		V8::ShutdownPlatform();
	}
}