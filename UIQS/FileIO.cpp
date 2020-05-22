#include "FileIO.hpp"

namespace FileIO {
	std::wifstream file_auto_encoding_open(const std::string& filePath)
	{
		std::string encoding;
		{
			std::ifstream encodingTest(filePath, std::ios::binary);
			unsigned char byte;
			encodingTest.read(reinterpret_cast<char*>(&byte), sizeof(byte));
			int bom = byte << 8;
			encodingTest.read(reinterpret_cast<char*>(&byte), sizeof(byte));
			bom |= byte;
			switch (bom)
			{
			case 0xfffe:  //65534
				encoding = ".UTF16LE";
				break;
			case 0xfeff://65279
				encoding = ".UTF16BE";
				break;
			case 0xefbb://61371
				encoding = ".UTF8";
				break;
			default:
				encoding = "";
			}
		}
		std::wifstream ifs(filePath);
		ifs.imbue(std::locale(encoding));
		return ifs;
	}
}