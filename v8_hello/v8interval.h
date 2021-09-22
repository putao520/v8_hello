#pragma once
#include <stdlib.h>
#include <iostream>
#include <include/libplatform/libplatform.h>
#include <include/v8.h>
#include <unordered_map>
#include <atomic>

using namespace v8;
using namespace std;
using Persistent_Function = Persistent<Function, CopyablePersistentTraits<Function>>;

class v8interval {
public:
	

	v8interval(Isolate* isolate);
	~v8interval();

	void setup(Local<ObjectTemplate>);

	static v8interval* current(Isolate* isolate);

	unsigned int getIdleTaskId();

	bool isEmpty();

	unordered_map<unsigned int, Persistent_Function> interval_callback_table;
private:
	Isolate* _isolate;

	atomic<unsigned int> task_max;
};