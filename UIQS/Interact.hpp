#pragma once
#include <functional>
#include <iostream>
#include <map>
#include <utility>
#include <windows.h>

#include "Validators.hpp"
#include "Types.hpp"


namespace Interact {
	void InitIOEnvironment();

	const std::wstring winput_cancel = L"本次输入已取消";
	const std::wstring winput_wrong_type_abort = L"错误的输入，本次输入已终止";
	const std::wstring winput_invaild_abort = L"被拒绝的输入值，本次输入已终止";
	const std::wstring winput_invaild_retry = L"被拒绝的输入值，请重新输入";

	void werror(const std::wstring& msg);

	class Welcome
	{
		std::wstring welcome_title;
	public:
		explicit Welcome(std::wstring welcomeTitle) : welcome_title(std::move(welcomeTitle))
		{}

		void operator()() const
		{
			std::wcout << "  +" << std::wstring(welcome_title.size() * 4, '-') << "+\n"
				<< "  |" << std::wstring(welcome_title.size(), ' ') << welcome_title << std::wstring(
					welcome_title.size(), ' ') << "|\n"
				<< "  +" << std::wstring(welcome_title.size() * 4, '-') << "+\n";
		}
	};

	class Menu
	{
	public:
		struct Item
		{
			int item_id{};
			std::wstring item_name;
			std::function<void()> item_procdure = nullptr;

			Item(const int itemId, std::wstring itemName, std::function<void()> procdure) : item_id(itemId),
				item_name(std::move(itemName)),
				item_procdure(std::move(procdure))
			{}

			Item(const int itemId, std::wstring itemName) : item_id(itemId), item_name(std::move(itemName))
			{}

			Item() = default;
		};

	private:
		std::wstring menu_title;
		std::map<int, Item> menu_items;

		void show()
		{
			std::wcout << "  " << std::wstring(7, '-') << menu_title << std::wstring(7, '-') << std::endl;
			for (const auto& item : menu_items)
			{
				std::wcout << item.first << "." << item.second.item_name << std::endl;
			}
		}

	public:
		Menu(std::wstring menuTitle, std::initializer_list<Item> menuItems) : menu_title(std::move(menuTitle))
		{
			for (const auto& item : menuItems)
			{
				menu_items[item.item_id] = item;
			}
		}

		void operator()()
		{
			show();
			int cmd;
			while (std::wcin >> cmd || std::wcin.fail())
			{
				if (std::wcin.eof())
				{
					werror(L"此菜单的交互已取消");
					std::wcin.clear();
					break;
				}
				if (std::wcin.fail())
				{
					werror(L"请输入数字序号");
					std::wcin.clear();
					std::wstring clearwcin;
					std::getline(std::wcin, clearwcin);
				}
				else if (menu_items.find(cmd) == menu_items.end())
				{
					werror(L"不存在此项");
				}
				else
				{
					if (menu_items[cmd].item_procdure == nullptr)
					{
						break;
					}
					menu_items[cmd].item_procdure();
					show();
				}
			}
		}
	};

	template <typename T>
	class Input
	{
		std::wstring input_placeholder;
		std::function<void(T)> input_callback;
		std::function<bool(T)> input_validator;
	public:
		Input(std::wstring placeholder, std::function<void(T)> callback, std::function<bool(T)> validator = nullptr) :
			input_placeholder(std::move(placeholder)),
			input_callback(std::move(callback)), input_validator(std::move(validator))
		{}

		void operator()()
		{
			std::wcout << input_placeholder << ":\n";
			T in;
			std::wcin >> in;
			if (std::wcin.eof())
			{
				werror(winput_cancel);
				std::wcin.clear();
				return;
			}
			if (std::wcin.fail())
			{
				werror(winput_wrong_type_abort);
				std::wcin.clear();
				std::wstring clearwcin;
				std::getline(std::wcin, clearwcin);
				return;
			}
			if (input_validator != nullptr && !input_validator(in))
			{
				werror(winput_invaild_abort);
				return;
			}
			input_callback(in);
		}
	};

	template <>
	class Input<std::wstring>
	{
		std::wstring input_placeholder;
		std::function<void(std::wstring)> input_callback;
		std::function<bool(std::wstring)> input_validator;
	public:
		Input(std::wstring placeholder, std::function<void(std::wstring)> callback,
			std::function<bool(std::wstring)> validator = nullptr) : input_placeholder(std::move(placeholder)),
			input_callback(std::move(callback)),
			input_validator(std::move(validator))
		{}

		void operator()() const
		{
			std::wcout << input_placeholder << ":\n";
			std::wstring in;
			while (std::wcin.peek() == '\n')std::wcin.get();
			std::getline(std::wcin, in);
			if (std::wcin.eof())
			{
				werror(winput_cancel);
				std::wcin.clear();
				return;
			}
			if (input_validator != nullptr && !input_validator(in))
			{
				werror(winput_invaild_abort);
				return;
			}
			input_callback(in);
		}
	};

	template <>
	class Input<std::string>
	{
		std::string input_placeholder;
		std::function<void(std::string)> input_callback;
		std::function<bool(std::string)> input_validator;
	public:
		Input(std::string placeholder, std::function<void(std::string)> callback,
			std::function<bool(std::string)> validator = nullptr) : input_placeholder(std::move(placeholder)),
			input_callback(std::move(callback)),
			input_validator(std::move(validator))
		{}

		void operator()() const
		{
			std::cout << input_placeholder << ":\n";
			std::string in;
			while (std::cin.peek() == '\n')std::cin.get();
			std::getline(std::cin, in);
			if (std::cin.eof())
			{
				werror(winput_cancel);
				std::cin.clear();
				return;
			}
			if (input_validator != nullptr && !input_validator(in))
			{
				werror(winput_invaild_abort);
				return;
			}
			input_callback(in);
		}
	};

	BuildHasMemberQ(validators);

	BuildHasMemberQ(field);

	template <typename T, bool ValidQ, bool MsgQ>
	class FormImpl
	{
	public:
		void operator()(std::function<void(const T&)> callback)
		{
			static_assert(true,"This has been deprecated");
			decltype(T::field)& field = T::field;
			T data = T();
			decltype(T::validators)& validators = T::validators;
			decltype(T::validate_fail_msg)& invalidMsg = T::validate_fail_msg;
			std::wstring in;
			std::function<bool(std::wstring)> validator = nullptr;
			for (auto& f : field)
			{
				bool valPass;
				do
				{
					std::wcout << f << ": ";
					std::wcin >> in;
					if (std::wcin.eof())
					{
						werror(winput_cancel);
						std::wcin.clear();
						return;
					}
					if (std::wcin.fail())
					{
						werror(winput_wrong_type_abort);
						std::wcin.clear();
						std::wcin.get();
						return;
					}
					validator = validators.at(f);
					valPass = validator == nullptr ? true : validator(in);
					if (!valPass)
					{
						std::wstring validFailMsg = invalidMsg.at(f);
						werror(validFailMsg.empty() ? winput_invaild_retry : validFailMsg);
					}
				} while (!valPass);
				data[f] = in;
			}
			callback(data);
		}
	};

	template <typename T, bool ValidQ>
	class FormImpl<T, ValidQ, false>
	{
	public:
		void operator()(std::function<void(const T&)> callback)
		{
			decltype(T::field)& field = T::field;
			T data = T();
			decltype(T::validators) validators = T::validators;
			std::wstring in;
			std::function<bool(std::wstring)> validator = nullptr;
			for (auto& f : field)
			{
				bool valPass;
				do
				{
					std::wcout << f << ": ";
					std::wcin >> in;
					if (std::wcin.eof())
					{
						werror(winput_cancel);
						std::wcin.clear();
						return;
					}
					if (std::wcin.fail())
					{
						werror(winput_wrong_type_abort);
						std::wcin.clear();
						std::wcin.get();
						return;
					}
					if (validators.find(f) != validators.end())
					{
						const auto& valid = validators.at(f);
						validator = valid.validator;
						std::wstring require = valid.requirement;
						valPass = validator == nullptr ? true : validator(in);
						if (!valPass)
						{
							werror(require.empty() ? winput_invaild_retry : require);
						}
					}
					else
					{
						valPass = true;
					}
				} while (!valPass);
				data[f] = in;
			}
			callback(data);
		}
	};

	template <typename T>
	class FormImpl<T, false, false>
	{
	public:
		void operator()(std::function<void(const T&)> callback)
		{
			decltype(T::field)& field = T::field;
			T data = T();
			for (auto& f : field)
			{
				std::wcout << f << ": ";
				std::wcin >> data[f];
				if (std::wcin.eof())
				{
					werror(winput_cancel);
					std::wcin.clear();
					return;
				}
				if (std::wcin.fail())
				{
					werror(winput_wrong_type_abort);
					std::wcin.clear();
					std::wcin.get();
					return;
				}
			}
			callback(data);
		}
	};

	template <typename T>
	class Form
	{
		std::function<void(const T&)> callback;
	public:
		explicit Form(std::function<void(T)> callback_) : callback(callback_)
		{
			static_assert(HasMemberfieldQ<T>::value, "Interact::Form: T require member field");
		}

		void operator()()
		{
			FormImpl<T,
				HasMembervalidatorsQ<T>::value,
				false>()(callback);
		}
	};

	class Output
	{
		std::wstring out;
	public:
		template <typename T, typename...Args>
		explicit Output(std::wstring msg, T arg, Args ...args)
		{
			const auto x = msg.find_first_of('#');
			msg = msg.substr(0, x) + std::to_wstring(arg) + msg.substr(x + 1);
			Output(msg, args...);
		}

		template <typename T>
		explicit Output(const std::wstring& msg, T arg)
		{
			const auto x = msg.find_first_of('#');
			out = msg.substr(0, x) + std::to_wstring(arg) + msg.substr(x + 1);
		}

		explicit Output(std::wstring msg) : out(std::move(msg))
		{}

		void operator()() const
		{
			std::wcout << out << std::endl;
		}
	};

	template <typename T>
	class Table
	{
		const T& data;
	public:
		explicit Table(const T& data_) : data(data_)
		{
			static_assert(HasMemberfieldQ<typename T::value_type>::value, "Interact::Table: T require member field");
		}

		void operator()() const
		{
			auto& field = (*data.begin()).field;
			std::wcout << L"--共得到" << data.size() << L"条信息--\n";
			for (auto it : data)
			{
				for (const auto& f : field)
				{
					std::wcout << f << ":\t" << it[f] << std::endl;
				}
				std::wcout << "----------------\n";
			}
		}
	};
};
