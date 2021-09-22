#pragma once

template<typename T>
class ClassDemo {
public:
	ClassDemo(T _v) : v(_v) {};
	T getValue() {
		return v;
	}
private:
	T v;
};