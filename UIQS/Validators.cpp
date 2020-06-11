#include "Validators.hpp"
#include <sstream>

namespace Validator {
	inline bool isnum(const int x) { return '0' <= x && x <= '9'; }
	inline bool leap_year(const int y) { return y % 400 == 0 || (y % 4 == 0 && y % 100 != 0); }
	bool parseInt(std::wstring::const_iterator& it, const std::wstring::const_iterator& end, const int len, int& res)
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
		if (!parseInt(it, val.end(), 4, year))
		{
			return false;
		}
		if (it == val.end() || *it++ != '-')
		{
			return false;
		}
		if (!parseInt(it, val.end(), 2, month))
			return false;
		if (it == val.end() || *it++ != '-')
			return false;
		if (!parseInt(it, val.end(), 2, day))
			return false;
		int months[] = { 31,28,31,30,31,30,31,31,30,31,30,31 };
		if (12 < month || month < 0)return false;
		if (leap_year(year))months[2 - 1] = 29;
		if (months[month - 1] < day || day < 0)
			return false;
		return true;
	}
}
