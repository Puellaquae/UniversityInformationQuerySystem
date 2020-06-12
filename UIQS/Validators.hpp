#pragma once
#include <string>
#include <functional>
#include <utility>


namespace Validator {
	struct Validator
	{
		std::function<bool(std::wstring)> validator;
		std::wstring requirement;
		Validator(std::function<bool(std::wstring)> validator, std::wstring require) :
			validator(std::move(validator)), requirement(std::move(require))
		{}
	};

	extern Validator int_Validator;
	extern Validator date_Validator;
}
