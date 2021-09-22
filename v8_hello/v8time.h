#pragma once
#include <stdlib.h>
#include <iostream>
#include <include/libplatform/libplatform.h>
#include <include/v8.h>
#include <atomic>

using namespace v8;
using namespace std;

class v8time {
public:
	
	v8time(Isolate* isolate);
	~v8time();

	void setup(Local<ObjectTemplate>);

	unsigned int task_inc();

	void runner_inc();

	bool isEmpty();

	static v8time* current(Isolate* isolate);
private:
	
	atomic<unsigned int> taskId;
	atomic<unsigned int> runnerId;

	Isolate* _isolate;

};