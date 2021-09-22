#include "queue_ring_exception.h"

const char* queue_ring_exception::what() {
	return "queue pop overflow";
}