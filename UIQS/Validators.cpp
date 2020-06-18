#include "Validators.hpp"
#include <sstream>

namespace Validator
{
	
	bool IntValidator(const std::wstring& val);
	const std::wstring int_requirement = L"要求一个整数";
	bool DataValidator(const std::wstring& val);
	const std::wstring date_requirement = L"日期格式要求 YYYY-MM-DD";
	inline bool isnum(const int x) { return '0' <= x && x <= '9'; }
	inline bool leap_year(const int y) { return y % 400 == 0 || (y % 4 == 0 && y % 100 != 0); }

	bool ParseInt(std::wstring::const_iterator& it, const std::wstring::const_iterator& end, const int len, int& res)
	{
		res = 0;
		for (int i = 0; i < len; i++)
		{
			if (it != end && isnum(*it))
			{
				res = res * 10 + *it - '0';
				++it;
			}
			else
			{
				return false;
			}
		}
		return true;
	}
	
	bool IntValidator(const std::wstring& val)
	{
		if (val.empty())
		{
			return false;
		}
		std::wstringstream wss(val);
		int intq;
		wss >> intq;
		return !wss.fail() && wss.eof();
	}

	bool DataValidator(const std::wstring& val)
	{
		if (val.empty())
		{
			return false;
		}
		auto it = val.begin();
		int year, month, day;
		if (!ParseInt(it, val.end(), 4, year))
		{
			return false;
		}
		if (it == val.end() || *it++ != '-')
		{
			return false;
		}
		if (!ParseInt(it, val.end(), 2, month))
			return false;
		if (it == val.end() || *it++ != '-')
			return false;
		if (!ParseInt(it, val.end(), 2, day))
			return false;
		int months[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
		if (12 < month || month < 0)return false;
		if (leap_year(year))months[2 - 1] = 29;
		if (months[month - 1] < day || day < 0)
			return false;
		return true;
	}

	Validator int_Validator{IntValidator, int_requirement};
	Validator date_Validator{DataValidator, date_requirement};
}
