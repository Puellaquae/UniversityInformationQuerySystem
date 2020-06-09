#include "文件交互.hpp"

namespace 文件交互 {
	std::wifstream 文件格式自动判断并打开(const std::string& 文件路径)
	{
		std::string encoding;
		{
			std::ifstream encodingTest(文件路径, std::ios::binary);
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
		std::wifstream ifs(文件路径);
		ifs.imbue(std::locale(encoding));
		return ifs;
	}
}