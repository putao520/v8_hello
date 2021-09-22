#pragma once
#include <exception>
using namespace std;
class queue_ring_exception :
    public exception
{
public:
    const char* what();
};

