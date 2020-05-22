#include <string>
#include <algorithm>
#include "Interact.hpp"
#include "DataBase.hpp"
#include "University.hpp"

DataBase<University> dataBase = DataBase<University>("data.txt");

int main()
{
	using namespace Interact;
	SetChineseEnvironment();
	typedef Menu::Item item;
	Welcome(L"高校信息查询系统")();

	Menu(L"开始", {
		item(1, L"信息查询", [] {
			Menu(L"信息查询", {
				item(1, L"显示所有高校信息",[] {[&] {
					Table<DataBase<University>>(dataBase.query(L""))();
					}();
				}),
				item(2, L"通过高校[编号|名称|地址|省份|建校时间|网址]查询",[] {
					Input<std::wstring>(L"请输入要用于查找的项目",[&](const std::wstring& item) {
						if (std::find(University::field.begin(),University::field.end(),item) == University::field.end())
						{
							Output(L"此项不存在")();
							return;
						}
						Input<std::wstring>(L"请输入查找的值", [&](const std::wstring& val) {
							Table<DataBase<University>>(dataBase.query(L"选择 " + item + L" 包含 " + val))();
						})();
					})();
				}),
				item(3, L"高级查询",[]() {
					Output(L"查询命令说明（不要省略空格）:\n\t匹配:\"选择 项目名 等于|不等于|包含|不包含|大于|小于|不小于|不大于 值\"\n\t排序:\"根据 项目名 升序|降序\"")();
					Input<std::wstring>(L"请使用命令查询",[&](const std::wstring& cmd) {
						Table<DataBase<University>>(dataBase.query(cmd))();
					})();
				}),
				item(4, L"返回上级菜单")
			})();
		}),
		item(2, L"信息维护", [] {
			Menu(L"信息维护", {
				item(1, L"删除信息",[] {
					Input<std::wstring>(L"请输入要删除的高校编号",[&](const std::wstring& index) {
						const int res = dataBase.remove([&](const University& u) {return u.id == index; });
						Output(L"更新共影响#条信息",res)();
					})();
				}),
				item(2, L"修改信息",[] {
					Input<std::wstring>(L"请输入要修改的高校编号",[](const std::wstring& index) {
						Input<std::wstring>(L"请输入要修改的项目",[&](const std::wstring& item) {
							if (std::find(University::field.begin(),University::field.end(),item) == University::field.end())
							{
								Output(L"此项不存在")();
								return;
							}
							Input<std::wstring>(L"请输入新值",[&](const std::wstring& newval) {
								const int res = dataBase.update(item,newval,[&](const University& u) {
									return u.id == index;
								});
								Output(L"更新共影响#条信息",res)();
							})();
						})();
					})();
				}),
				item(3, L"增加信息",[] {
					Form<University>([&](const University& val) {
						dataBase.insert(val);
						Output(L"信息已添加")();
					})();
				}),
				item(4, L"返回上级菜单")
			})();
		}),
		item(3, L"退出系统")
		})();
		return 0;
}