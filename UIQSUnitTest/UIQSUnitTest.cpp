#include "pch.h"
#include "CppUnitTest.h"
#include "../UIQS/双向链表.hpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UIQSUnitTest {
	TEST_CLASS(LinkLish)
	{
	public:
		TEST_METHOD(LinkListEqual)
		{
			双向链表<int> list1({ 1,2,3 });
			双向链表<int> list2({ 3,2,1 });
			Assert::IsFalse(list1 == list2);
			//自反性
			双向链表<int> listA({ 1,2,3 });
			Assert::IsTrue(listA == listA);
			//对称性
			const 双向链表<int> listB = listA;
			Assert::IsTrue(listA == listB);
			Assert::IsTrue(listB == listA);
			//传递性
			const 双向链表<int> listC = { 1,2,3 };
			Assert::IsTrue(listB == listC);
			Assert::IsTrue(listA == listC);
			//强类型
			const 双向链表<long> listLA({ 1,2,3 });
			Assert::IsFalse(listA == listLA);
			const 双向链表<double> listDA({ 1.0,2.0,3.0 });
			Assert::IsFalse(listA == listDA);
		}

		TEST_METHOD(LinkListPushBack)
		{
			双向链表<int> list;
			list.push_back(9);
			Assert::AreEqual(9, list.back());
			Assert::AreEqual(9, list.front());
			list.push_back(10);
			Assert::AreEqual(10, list.back());
			Assert::AreEqual(9, list.front());
		}

		TEST_METHOD(LinkListPopBack)
		{
			双向链表<int> list({ 9,10,11 });
			list.pop_back();
			Assert::AreEqual(10, list.back());
			Assert::AreEqual(9, list.front());
			list.pop_back();
			Assert::AreEqual(9, list.back());
			Assert::AreEqual(9, list.front());
		}

		TEST_METHOD(LinkListPushFront)
		{
			双向链表<int> list;
			list.push_front(9);
			Assert::AreEqual(9, list.back());
			Assert::AreEqual(9, list.front());
			list.push_front(10);
			Assert::AreEqual(9, list.back());
			Assert::AreEqual(10, list.front());
		}

		TEST_METHOD(LinkListPopFront)
		{
			双向链表<int> list({ 9,10,11 });
			list.pop_front();
			Assert::AreEqual(11, list.back());
			Assert::AreEqual(10, list.front());
			list.pop_front();
			Assert::AreEqual(11, list.back());
			Assert::AreEqual(11, list.front());
		}

		TEST_METHOD(LinkListEmpty)
		{
			双向链表<int> list;
			Assert::IsTrue(list.empty());
			list.push_back(12);
			Assert::IsFalse(list.empty());
			list.pop_back();
			Assert::IsTrue(list.empty());
		}

		TEST_METHOD(LinkListErase)
		{
			双向链表<int> list({ 1,2,3,4,5 });
			Assert::AreEqual(5, list.size());
			list.erase(1);
			Assert::AreEqual(4, list.size());
			auto it = list.begin();
			Assert::AreEqual(2, *it);
			++it;
			Assert::AreEqual(3, *it);
			list.erase(4);
			++it;
			Assert::AreEqual(5, *it);
			list.erase(it);
			Assert::AreEqual(3, list.back());
			双向链表<int> listB({ 1,2,3,4,5,6,7 });
			双向链表<int> listEB({ 2,4,6 });
			listB.erase([](const int x) {return x % 2 == 1; });
			Assert::IsTrue(listEB == listB);
		}
	};
}
