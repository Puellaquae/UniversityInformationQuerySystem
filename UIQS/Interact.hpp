#pragma once
#include <functional>
#include <iostream>
#include <map>
#include <utility>

namespace Interact {
	inline void SetChineseEnvironment()
	{
		std::locale::global(std::locale(""));
	}

	const std::wstring winput_cancel = L"本次输入已取消";
	const std::wstring winput_wrong_type_abort = L"错误的输入，本次输入已终止";
	const std::wstring winput_invaild_abort = L"被拒绝的输入值，本次输入已终止";

	const std::string input_cancel = "本次输入已取消";
	const std::string input_wrong_type_abort = "错误的输入，本次输入已终止";
	const std::string input_invaild_abort = "被拒绝的输入值，本次输入已终止";

	class Welcome
	{
		std::wstring welcome_title;
	public:
		explicit Welcome(std::wstring welcomeTitle) :welcome_title(std::move(welcomeTitle)) {}
		void operator()() const
		{
			std::wcout << "  +" << std::wstring(welcome_title.size() * 4, '-') << "+\n"
				<< "  |" << std::wstring(welcome_title.size(), ' ') << welcome_title << std::wstring(welcome_title.size(), ' ') << "|\n"
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
			Item(const int itmeId, std::wstring itemName, void(*procdure)()) :item_id(itmeId), item_name(std::move(itemName)), item_procdure(procdure) {}
			Item(const int itemId, std::wstring itemName) :item_id(itemId), item_name(std::move(itemName)) {}
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
		Menu(std::wstring menuTitle, std::initializer_list<Item> menuItems) :menu_title(std::move(menuTitle))
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
					std::wcerr << L"此菜单的交互已取消\n";
					std::wcin.clear();
					break;
				}
				if (std::wcin.fail())
				{
					std::wcerr << L"请输入数字序号\n";
					std::wcin.clear();
					std::wstring clearwcin;
					std::getline(std::wcin, clearwcin);
				}
				else if (menu_items.find(cmd) == menu_items.end())
				{
					std::wcerr << L"不存在此项\n";
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
	template<typename T> class Input
	{
		std::wstring input_placeholder;
		std::function<void(T)> input_callback;
		std::function<bool(T)> input_validator;
	public:
		Input(std::wstring placeholder, std::function<void(T)> callback, std::function<bool(T)> validator = nullptr) : input_placeholder(std::move(placeholder)),
			input_callback(std::move(callback)), input_validator(std::move(validator))
		{}

		void operator()()
		{
			std::wcout << input_placeholder << ":\n";
			T in;
			std::wcin >> in;
			if (std::wcin.eof())
			{
				std::wcerr << winput_cancel << std::endl;
				std::wcin.clear();
				return;
			}
			if (std::wcin.fail())
			{
				std::wcerr << winput_wrong_type_abort << std::endl;
				std::wcin.clear();
				std::wstring clearwcin;
				std::getline(std::wcin, clearwcin);
				return;
			}
			if (input_validator != nullptr && !input_validator(in))
			{
				std::wcerr << winput_invaild_abort << std::endl;
				return;
			}
			input_callback(in);
		}
	};
	template<> class Input<std::wstring>
	{
		std::wstring input_placeholder;
		std::function<void(std::wstring)> input_callback;
		std::function<bool(std::wstring)> input_validator;
	public:
		Input(std::wstring placeholder, std::function<void(std::wstring)> callback, std::function<bool(std::wstring)> validator = nullptr) : input_placeholder(std::move(placeholder)),
			input_callback(std::move(callback)), input_validator(std::move(validator))
		{}

		void operator()() const
		{
			std::wcout << input_placeholder << ":\n";
			std::wstring in;
			while (std::wcin.peek() == '\n')std::wcin.get();
			std::getline(std::wcin, in);
			if (std::wcin.eof())
			{
				std::wcerr << winput_cancel << std::endl;
				std::wcin.clear();
				return;
			}
			if (input_validator != nullptr && !input_validator(in))
			{
				std::wcerr << winput_invaild_abort << std::endl;
				return;
			}
			input_callback(in);
		}
	};
	template<> class Input<std::string>
	{
		std::string input_placeholder;
		std::function<void(std::string)> input_callback;
		std::function<bool(std::string)> input_validator;
	public:
		Input(std::string placeholder, std::function<void(std::string)> callback, std::function<bool(std::string)> validator = nullptr) : input_placeholder(std::move(placeholder)),
			input_callback(std::move(callback)), input_validator(std::move(validator))
		{}

		void operator()() const
		{
			std::cout << input_placeholder << ":\n";
			std::string in;
			while (std::cin.peek() == '\n')std::cin.get();
			std::getline(std::cin, in);
			if (std::cin.eof())
			{
				std::cerr << input_cancel << std::endl;
				std::cin.clear();
				return;
			}
			if (input_validator != nullptr && !input_validator(in))
			{
				std::cerr << input_invaild_abort << std::endl;
				return;
			}
			input_callback(in);
		}
	};
	template<typename T> class Form
	{
		decltype(T::field)& field = T::field;
		T data = T();
		std::function<void(const T&)> callback;
	public:
		explicit Form(std::function<void(T)> callback_) :callback(callback_) {}
		void operator()()
		{
			for (auto& f : field)
			{
				std::wcout << f << ": ";
				std::wcin >> data[f];
				if (std::wcin.eof())
				{
					std::wcerr << L"本次输入已取消\n";
					std::wcin.clear();
					return;
				}
				if (std::wcin.fail())
				{
					std::wcerr << L"错误的输入，本次输入已被终止\n";
					std::wcin.clear();
					std::wcin.get();
					return;
				}
			}
			callback(data);
		}
	};
	class Output
	{
		std::wstring out;
	public:
		template<typename T, typename...Args >
		explicit Output(std::wstring msg, T arg, Args...args)
		{
			const auto x = msg.find_first_of('#');
			msg = msg.substr(0, x) + std::to_wstring(arg) + msg.substr(x + 1);
			Output(msg, args...);
		}
		template<typename T>
		explicit Output(const std::wstring& msg, T arg)
		{
			const auto x = msg.find_first_of('#');
			out = msg.substr(0, x) + std::to_wstring(arg) + msg.substr(x + 1);
		}
		explicit Output(std::wstring msg) :out(std::move(msg)) {}
		void operator()() const
		{
			std::wcout << out << std::endl;
		}
	};
	template<typename T> class Table
	{
		const T& data;
	public:
		explicit Table(const T& data_) :data(data_) {}
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