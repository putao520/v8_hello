#pragma once
#include <stdlib.h>
#include <iostream>
#include <include/libplatform/libplatform.h>
#include <include/v8.h>

using namespace v8;
using namespace std;

class v8print {
public:
	v8print(Isolate* isolate);
	~v8print();
	void setup(Local<ObjectTemplate>);

	static v8print* current(Isolate* isolate);

private:
	Isolate* _isolate;
};