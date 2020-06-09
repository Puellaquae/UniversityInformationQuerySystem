#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include "双向链表.hpp"
#include "文件交互.hpp"

template<typename T>
class 数据库
{
	decltype(T::field)& field = T::field;
	const std::string file_path;

	双向链表<T> raw{};
	explicit 数据库(双向链表<T> list) :raw(std::move(list)) {}
	void load()
	{
		std::wifstream ifs = 文件交互::文件格式自动判断并打开(file_path);
		if (ifs.is_open() && !ifs.bad())
		{
			std::wstring line;
			while (std::getline(ifs, line))
			{
				raw.push_back(T(line));
			}
		}
		else
		{
			std::cerr << "Open File Fail";
		}
	}
public:
	void 保存()
	{
		if (file_path.empty())return;
		std::wofstream ofs(file_path, std::ios::out | std::ios::trunc);
		if (ofs.is_open())
		{
			for (const auto& it : raw)
			{
				ofs << static_cast<std::wstring>(it) << std::endl;
			}
		}
	}

	void 保存(const std::string& 文件路径)
	{
		if (文件路径.empty())return;
		std::wofstream ofs(文件路径, std::ios::out | std::ios::trunc);
		if (ofs.is_open())
		{
			for (const auto& it : raw)
			{
				ofs << static_cast<std::wstring>(it) << std::endl;
			}
		}
	}

	~数据库()
	{
		保存();
	}
	void 添加(const T& 值)
	{
		raw.push_back(值);
	}
	int 删除(std::function<bool(const T&)>筛选条件)
	{
		return raw.erase(筛选条件);
	}
	int 更新(std::wstring 项目名, const std::wstring& 新值, std::function<bool(const T&)>筛选条件)
	{
		if (std::find(field.begin(), field.end(), 项目名) == field.end()) { return 0; }
		int res = 0;
		for (auto it = raw.begin(); it != raw.end(); ++it)
		{
			if (筛选条件(*it))
			{
				(*it)[项目名] = 新值;
				res++;
			}
		}
		return res;
	}
	数据库() = default;
	数据库(const 数据库<T>& db) :数据库(db.raw) {}
	数据库(数据库&& db) noexcept
	{
		raw = std::move(db.raw);
	}
	数据库<T>& operator=(const 数据库<T>& db)
	{
		if (this != &db)
		{
			raw = db.raw;
		}
		return *this;
	}
	数据库<T>& operator=(数据库<T>&& db) noexcept
	{
		if (&db != this)
		{
			raw = std::move(db.raw);
		}
		return *this;
	}

	explicit  数据库(std::string filepath) :file_path(std::move(filepath))
	{
		load();
	}
	数据库<T> 拷贝()
	{
		return 数据库<T>(raw);
	}
	数据库<T> 筛选(std::function<bool(const T& val)>条件)
	{
		数据库<T> result;
		for (const auto& x : raw)
		{
			if (条件(x))
			{
				result.添加(x);
			}
		}
		return result;
	}
	template<typename Tresult>
	数据库<Tresult> 选取(std::function<Tresult(const T&)>selector)
	{
		数据库<Tresult> result;
		for (const auto& x : raw)
		{
			result.添加(selector(x));
		}
		return result;
	}
	数据库<T> 排序(std::function<bool(const T&, const T&)> cmp)
	{
		数据库<T> result(raw);
		result.raw.sort(cmp);
		return result;
	}
	数据库<T> 查询(const std::wstring& cmd)
	{
		数据库<T> res(raw);
		std::wstringstream cmdin(cmd);
		std::wstring cmdA;
		while (cmdin >> cmdA)
		{
			if (cmdA == L"选择")
			{
				std::wstring f, op, val;
				cmdin >> f >> op >> val;
				if (!cmdin.fail() && std::find(field.begin(), field.end(), f) != field.end())
				{
					if (op == L"等于")res = res.筛选([=](const T& u) {return u[f] == val; });
					else if (op == L"不等于")res = res.筛选([=](const T& u) {return u[f] != val; });
					else if (op == L"包含")res = res.筛选([=](const T& u) {return u[f].find(val) != u[f].npos; });
					else if (op == L"不包含")res = res.筛选([=](const T& u) {return u[f].find(val) == u[f].npos; });
					else if (op == L"大于")res = res.筛选([=](const T& u) {return u[f] > val; });
					else if (op == L"小于")res = res.筛选([=](const T& u) {return u[f] < val; });
					else if (op == L"不大于")res = res.筛选([=](const T& u) {return u[f] <= val; });
					else if (op == L"不小于")res = res.筛选([=](const T& u) {return u[f] >= val; });
				}
			}
			else if (cmdA == L"根据")
			{
				std::wstring f, op;
				cmdin >> f >> op;
				if (!cmdin.fail() && std::find(field.begin(), field.end(), f) != field.end())
				{
					if (op == L"升序")res.raw.sort([=](const T& u, const T& v) {return u[f] > v[f]; });
					else if (op == L"降序")res.raw.sort([=](const T& u, const T& v) {return u[f] < v[f]; });
				}
			}
		}
		return res;
	}
	auto begin() { return raw.begin(); }
	auto end() { return raw.end(); }
	auto begin() const { return raw.begin(); }
	auto end() const { return raw.end(); }
	int size() const { return raw.size(); }
};