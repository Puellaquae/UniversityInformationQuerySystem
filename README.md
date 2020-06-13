# ZJUT 大一下 C++ 2019/2020(2) 课程设计

## 高校信息查询系统

### 基本要求：

以高校为基本单位进行信息存放，高校信息包括：编号、学校名称、地址、省份、建校时间、网址等。

### 功能要求：

1、设计菜单实现功能选择；

2、能够对高校信息进行维护：新建、修改、删除操作；

3、按照高校任意信息进行查询；

4、按照省份、建校时间等信息项进行排序整理；

5、以文件形式保存相关信息，可以读取默认文件中的信息进行查询等操作。

## 如何修改此代码以完成其他题目

**请使用支持 C++ 11 及以上的编译器编译**

以**学生成绩管理系统**为例

主要需要修改 ```University``` （可依题目改名字）和 ```main``` 函数

```University``` 要求

1.实现静态成员 ```std::vector<std::wstring> field```

2.实现成员函数 ```std::wstring& operator[](std::wstring)``` , ```std::wstring operator[](std::wstring) const``` , ```std::wstring operator std::wstring() const```

3.要求 无参数构造函数 和 参数为 ```std::wstring``` 的构造函数

```cpp
struct Student
{
	std::wstring id, name, gender, classes, chinese, math, english, programming;

	explicit Student(const std::wstring& rawdata)
	{
		std::wstringstream wss(rawdata);
		wss >> id >> name >> gender >> classes >> chinese >> math >> english >> programming;
	}

	explicit operator std::wstring() const
	{
		std::wstringstream wss;
		wss << id << '\t' << name << '\t' << gender << '\t' << classes << '\t' << chinese << '\t' << math <<'\t' << english <<'\t' << programming;
		return wss.str();
	}

	Student() = default;

	std::wstring& operator[](const std::wstring& index)
	{
		if (index == L"学号")return id;
		if (index == L"名称")return name;
		if (index == L"性别")return gender;
		if (index == L"班级")return classes;
        //...省略
        
		throw;
	}

	std::wstring operator[](const std::wstring& index) const
	{
		if (index == L"学号")return id;
		if (index == L"名称")return name;
		if (index == L"性别")return gender;
		if (index == L"班级")return classes;
        //...如上省略
        if (index == L"平均成绩"){
            std::wstringstream wss;
            wss << chinese << '\t' << math <<'\t' << english <<'\t' << programming;
            int c,m,e,p;
            double sum;
            wss >> c >> m >> e >> p;
            sum = (c + m + e + p)/4.0;
            return std::to_wstring(sum);    
        }
        //...省略
		throw;
	}

	static bool contain(const std::wstring& item)
	{
        //注意平均成绩和总分的特殊性
		return item==L"平均成绩"||item==L"总成绩"||std::find(field.begin(), field.end(), item) != field.end();
	}

	static const std::vector<std::wstring> field;
	static const std::map<std::wstring, Validator::Validator> validators;//此非强制实现
};

const std::vector<std::wstring> University::field = { L"姓名", L"学号", L"性别", /*...略...*/ };
const std::map<std::wstring, Validator::Validator> University::validators = {
	{L"学号", Validator::int_Validator},
    //...略...
    //不需要值校验器的项无需写
};
```

```main``` 可以参照源代码的使用方法（或[ localization 分支中的汉化版本](https://github.com/Puellaquae/ClassDesign/blob/localization/UIQS/%E4%B8%BB%E7%A8%8B%E5%BA%8F.cpp)）

```cpp
    dataBase.always_save = true;//题目特殊要求
    using namespace Interact;
	SetChineseEnvironment();
	typedef Menu::Item item;
	Welcome(L"学生成绩管理系统")();

	const auto x = Reduce(University::field.begin(), University::field.end(),
		[&](const std::wstring& a, const std::wstring& b) {return a + L"|" + b; });

	Menu(L"开始", {
		item(1, L"信息查询", [=] {
			Menu(L"信息查询", {
				item(1, L"通过学生[" + x + L"]查询",[] {
					Input<std::wstring>(L"请输入要用于查找的项目",[&](const std::wstring& item) {
						if (!University::contain(item))
						{
							Output(L"此项不存在")();
							return;
						}
						Input<std::wstring>(L"请输入查找的值", [&](const std::wstring& val) {
							Table<DataBase<University>>(dataBase.query(L"选择 " + item + L" 包含 " + val))();
						})();
					})();
				}),
				item(2, L"高级查询",[]() {
					Output(L"查询命令说明（不要省略空格）:\n\t匹配:\"选择 项目名 等于|不等于|包含|不包含|大于|小于|不小于|不大于 值\"\n\t排序:\"根据 项目名 升序|降序\"")();
					Input<std::wstring>(L"请使用命令查询",[&](const std::wstring& cmd) {
						Table<DataBase<University>>(dataBase.query(cmd))();
					})();
				}),
				item(3, L"返回上级菜单")
			})();
		}),
		item(2, L"信息维护", [] {
			Menu(L"信息维护", {
				item(1, L"删除信息",[] {
					Input<std::wstring>(L"请输入要删除的学生学号",[&](const std::wstring& index) {
						const int res = dataBase.remove([&](const University& u) {return u[L"学号"] == index; });
						Output(L"更新共影响#条信息",res)();
					})();
				}),
				item(2, L"修改信息",[] {
					Input<std::wstring>(L"请输入要修改的学生学号",[](const std::wstring& index) {
						Input<std::wstring>(L"请输入要修改的项目",[&](const std::wstring& item) {
							if (!University::contain(item))
							{
								Output(L"此项不存在")();
								return;
							}
							Input<std::wstring>(L"请输入新值",[&](const std::wstring& newval) {
								const int res = dataBase.update(item,newval,[&](const University& u) {
									return u[L"学号"] == index;
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
```

例子未完成 **统计学生成绩，按照班级和科目计算平均分** ，可以利用 **FP.hpp** 中提供的 **MapAndSum** 函数自行完成。