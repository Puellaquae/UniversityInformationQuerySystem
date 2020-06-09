#pragma once
#include <functional>
#include <iostream>
#include <map>
#include <utility>

namespace 交互 {
	inline void 设置中文输入输出环境()
	{
		std::locale::global(std::locale(""));
	}

	const std::wstring winput_cancel = L"本次输入已取消";
	const std::wstring winput_wrong_type_abort = L"错误的输入，本次输入已终止";
	const std::wstring winput_invaild_abort = L"被拒绝的输入值，本次输入已终止";

	const std::string input_cancel = "本次输入已取消";
	const std::string input_wrong_type_abort = "错误的输入，本次输入已终止";
	const std::string input_invaild_abort = "被拒绝的输入值，本次输入已终止";

	class 欢迎界面
	{
		std::wstring welcome_title;
	public:
		explicit 欢迎界面(std::wstring 标题) :welcome_title(std::move(标题)) {}
		void operator()() const
		{
			std::wcout << "  +" << std::wstring(welcome_title.size() * 4, '-') << "+\n"
				<< "  |" << std::wstring(welcome_title.size(), ' ') << welcome_title << std::wstring(welcome_title.size(), ' ') << "|\n"
				<< "  +" << std::wstring(welcome_title.size() * 4, '-') << "+\n";
		}
	};
	class 菜单
	{
	public:
		struct 项目
		{
			int item_id{};
			std::wstring item_name;
			std::function<void()> item_procdure = nullptr;
			项目(const int 编号, std::wstring 名称, std::function<void()> 动作) :
			item_id(编号), item_name(std::move(名称)), item_procdure(std::move(动作)) {}
			项目(const int 编号, std::wstring 名称) :item_id(编号), item_name(std::move(名称)) {}
			项目() = default;
		};
	private:
		std::wstring menu_title;
		std::map<int, 项目> menu_items;
		void show()
		{
			std::wcout << "  " << std::wstring(7, '-') << menu_title << std::wstring(7, '-') << std::endl;
			for (const auto& item : menu_items)
			{
				std::wcout << item.first << "." << item.second.item_name << std::endl;
			}
		}
	public:
		菜单(std::wstring 标题, std::initializer_list<项目> 项目) :menu_title(std::move(标题))
		{
			for (const auto& item : 项目)
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
	template<typename T> class 输入
	{
		std::wstring input_placeholder;
		std::function<void(T)> input_callback;
		std::function<bool(T)> input_validator;
	public:
		输入(std::wstring 提示, std::function<void(T)> 回调, std::function<bool(T)> 校验 = nullptr) : input_placeholder(std::move(提示)),
			input_callback(std::move(回调)), input_validator(std::move(校验))
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
	template<> class 输入<std::wstring>
	{
		std::wstring input_placeholder;
		std::function<void(std::wstring)> input_callback;
		std::function<bool(std::wstring)> input_validator;
	public:
		输入(std::wstring 提示, std::function<void(std::wstring)> 回调, std::function<bool(std::wstring)> 校验 = nullptr) : input_placeholder(std::move(提示)),
			input_callback(std::move(回调)), input_validator(std::move(校验))
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
	template<> class 输入<std::string>
	{
		std::string input_placeholder;
		std::function<void(std::string)> input_callback;
		std::function<bool(std::string)> input_validator;
	public:
		输入(std::string 提示, std::function<void(std::string)> 回调, std::function<bool(std::string)> 校验 = nullptr) : input_placeholder(std::move(提示)),
			input_callback(std::move(回调)), input_validator(std::move(校验))
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
	template<typename T> class 表单输入
	{
		decltype(T::field)& field = T::field;
		T data = T();
		std::function<void(const T&)> callback;
	public:
		explicit 表单输入(std::function<void(T)> 回调) :callback(回调) {}
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
	class 输出
	{
		std::wstring out;
	public:
		template<typename T, typename...Args >
		explicit 输出(std::wstring msg, T arg, Args...args)
		{
			const auto x = msg.find_first_of('#');
			msg = msg.substr(0, x) + std::to_wstring(arg) + msg.substr(x + 1);
			输出(msg, args...);
		}
		template<typename T>
		explicit 输出(const std::wstring& msg, T arg)
		{
			const auto x = msg.find_first_of('#');
			out = msg.substr(0, x) + std::to_wstring(arg) + msg.substr(x + 1);
		}
		explicit 输出(std::wstring msg) :out(std::move(msg)) {}
		void operator()() const
		{
			std::wcout << out << std::endl;
		}
	};
	template<typename T> class 表单输出
	{
		const T& data;
	public:
		explicit 表单输出(const T& 数据) :data(数据) {}
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