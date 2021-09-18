#pragma once
#include <stdlib.h>
#include <iostream>
#include <include/libplatform/libplatform.h>
#include <include/v8.h>
#include <queue>
#include <atomic>

using namespace v8;
using namespace std;

class v8time {
public:
	
	v8time(Isolate* isolate);
	~v8time();

	void setupSetTimeout(Local<ObjectTemplate>);

	static v8time* New(Isolate* isolate);
private:
	
	static atomic<unsigned int> taskId;

	Isolate* _isolate;
	
};