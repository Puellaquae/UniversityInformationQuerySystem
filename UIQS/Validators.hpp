#pragma once
#include <string>
#include <functional>
#include <utility>


namespace Validator {
	struct Validator
	{
		std::function<bool(std::wstring)> validator;
		std::wstring requirement;
	};

	extern Validator int_Validator;
	extern Validator date_Validator;
}
