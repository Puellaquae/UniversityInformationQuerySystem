#pragma once
#include "Validators.hpp"

struct University
{
	std::wstring id, name, address, province, found_date, website;
	explicit University(const std::wstring& rawdata)
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
	University() = default;
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
	static bool contain(const std::wstring& item)
	{
		return std::find(field.begin(), field.end(), item) != field.end();
	}
	static const std::vector<std::wstring> field;
	static const std::map<std::wstring, std::function<bool(std::wstring)>> validators;
};

const std::vector<std::wstring> University::field = { L"编号",L"名称",L"地址",L"省份",L"建校时间",L"网址" };
const std::map<std::wstring, std::function<bool(std::wstring)>> University::validators = {
	{L"编号",Validator::IntValidator},
	{L"名称",nullptr},
	{L"地址",nullptr},
	{L"省份",nullptr},
	{L"建校时间",Validator::DataValidator},
	{L"网址",nullptr}
};