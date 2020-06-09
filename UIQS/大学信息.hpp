#pragma once

struct 大学信息
{
	std::wstring id, name, address, province, found_date, website;
	explicit 大学信息(const std::wstring& rawdata)
	{
		std::wstringstream wss(rawdata);
		wss >> id >> name >> address >> province >> found_date >> website;
	}

	explicit operator std::wstring() const
	{
		std::wstringstream wss;
		wss << id << "\t" << name << "\t" << address << "\t" << province << "\t" << found_date << "\t" << website;
		return wss.str();
	}
	大学信息() = default;
	std::wstring& operator[](const std::wstring& index)
	{
		if (index == L"编号")return id;
		if (index == L"名称")return name;
		if (index == L"地址")return address;
		if (index == L"省份")return province;
		if (index == L"建校时间")return found_date;
		if (index == L"网址")return website;
		throw;
	}
	std::wstring operator[](const std::wstring& index) const
	{
		if (index == L"编号")return id;
		if (index == L"名称")return name;
		if (index == L"地址")return address;
		if (index == L"省份")return province;
		if (index == L"建校时间")return found_date;
		if (index == L"网址")return website;
		return {};
	}
	static bool 存在项目(const std::wstring& 项目)
	{
		return std::find(field.begin(), field.end(), 项目) != field.end();
	}
	static bool 不存在项目(const std::wstring& 项目)
	{
		return std::find(field.begin(), field.end(), 项目) == field.end();
	}
	static const std::vector<std::wstring> field;
};
const std::vector<std::wstring> 大学信息::field = { L"编号",L"名称",L"地址",L"省份",L"建校时间",L"网址" };