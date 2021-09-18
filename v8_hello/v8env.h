#pragma once
#include <stdlib.h>
#include <iostream>
#include <include/libplatform/libplatform.h>
#include <include/v8.h>
using namespace v8;
using namespace std;

class v8env {
public:
	static void New();
	static void Dispose();
};