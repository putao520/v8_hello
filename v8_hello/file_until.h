#pragma once
#include <iostream>
#include <filesystem>

using namespace std::filesystem;

enum class TEXT_TYPE
{
	TEXT_ANSI = 0,
	TEXT_UTF8 = 1,
	TEXT_UTF8_BOM = 2,
	TEXT_UTF16_LE = 3,
	TEXT_UTF16_BE = 4,
	TEXT_UNKNOW = 5,
};

class FileUntil {
public:
	

	FileUntil(const char* file);
	// ~FileUntil();
	void* toMemory();
	TEXT_TYPE getTextEncode();
private:
	bool check_utf8_without_bom();

	path this_path;
	char* buffer;
	size_t size;
};