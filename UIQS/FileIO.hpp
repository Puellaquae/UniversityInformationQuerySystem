#pragma once
#include <fstream>
#include <string>

namespace FileIO {
	std::wifstream file_auto_encoding_open(const std::string& filePath);
}
