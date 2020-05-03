#pragma once
#include <functional>
#include <iostream>
#include <map>
#include <utility>

namespace Interact {
	inline void SetChineseEnvironment()
	{
		std::wcout.imbue(std::locale("zh_CN"));
		std::wcin.imbue(std::locale("zh_CN"));
	}

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
			while (std::cin >> cmd)
			{
				if (menu_items.find(cmd) == menu_items.end())
				{
					std::wcout << L"不存在此项" << std::endl;
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
	template<typename T>
	class Input
	{
		std::wstring input_placeholder;
		std::function<void(T)> input_callback;
	public:
		Input(std::wstring placeholder, std::function<void(T)> callback) : input_placeholder(std::move(placeholder)),
			input_callback(callback)
		{}

		void operator()()
		{
			std::wcout << input_placeholder << ":\n";
			T in;
			std::wcin >> in;
			input_callback(in);
		}
	};
	template<>
	class Input<std::wstring>
	{
		std::wstring input_placeholder;
		std::function<void(std::wstring)> input_callback;
	public:
		Input(std::wstring placeholder, std::function<void(std::wstring)> callback) : input_placeholder(std::move(placeholder)),
			input_callback(std::move(callback))
		{}

		void operator()() const
		{
			std::wcout << input_placeholder << ":\n";
			std::wstring in;
			while (std::wcin.peek() == '\n')std::wcin.get();
			std::getline(std::wcin, in);
			input_callback(in);
		}
	};
	template<typename T>
	class Form
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
			Output(msg, args);
		}
		template<typename T>
		explicit Output(const std::wstring msg, T arg)
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
			auto& field = decltype(*data.begin())::field;
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