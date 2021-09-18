#include <filesystem>
#include <fstream>
#include "file_until.h"

using namespace std;
using namespace std::filesystem;
FileUntil::FileUntil(const char* file) {
	path _path(file);
	if (!_path.is_absolute())
		_path = canonical(_path);

	this_path = _path;
}

void* FileUntil::toMemory() {
	if (!exists(this_path))
		return nullptr;

	ifstream fileIo;
	fileIo.open(this_path, ios::binary);
    if (!fileIo.is_open())
        return nullptr;

	filebuf* pbuf;
	pbuf = fileIo.rdbuf();
    size_t size = static_cast<long>(pbuf->pubseekoff(0, ios::end, ios::in));
	pbuf->pubseekpos(0, ios::in);

	buffer = new char[size + 1];

	memset(buffer, 0, size + 1);

	pbuf->sgetn(buffer, size);
	
	fileIo.close();

	return buffer;
}