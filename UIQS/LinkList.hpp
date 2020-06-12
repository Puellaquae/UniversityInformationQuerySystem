#pragma once
#include <utility>
#include <vector>
#include <functional>

template <typename T>
class LinkList
{
	struct Node
	{
		Node* prev = nullptr;
		Node* next = nullptr;
		T value;

		explicit Node(T val = T()) : value(std::move(val))
		{}

		bool operator==(const Node& node)
		{
			return next == node.next && prev == node.prev && value == node.value;
		}
	};

	Node void_node;
	Node* head;
	int _size = 0;
public:

	using value_type = T;
	using pointer = T*;
	using const_pointer = const T*;
	using reference = T&;
	using const_reference = const T&;

	LinkList()
	{
		void_node.prev = void_node.next = &void_node;
		head = &void_node;
	}

	LinkList(const LinkList& list) : LinkList()
	{
		for (auto it = list.begin(); it != list.end(); ++it)
		{
			push_back(*it);
		}
	}

	LinkList(LinkList&& list) noexcept : head(list.head), _size(list._size)
	{
		void_node.next = &void_node;
		head->prev = &void_node;
		list.void_node.prev->next = &void_node;
		void_node.prev = list.void_node.prev;
		list.head = &list.void_node;
		list.void_node.prev = &list.void_node;
	}

	LinkList(std::initializer_list<T> list) : LinkList()
	{
		for (auto x : list)
		{
			push_back(x);
		}
	}

	LinkList& operator=(const LinkList& list)
	{
		if (this == &list) { return *this; }
		int pos = 0;
		auto thit = begin();
		auto liit = list.begin();
		for (; pos < _size && pos < list.size(); ++pos, ++thit, ++liit)
		{
			thit->value = *liit;
		}
		for (; pos < list.size(); ++pos, ++liit)
		{
			push_back(*liit);
		}
		return *this;
	}

	LinkList& operator=(LinkList&& list) noexcept
	{
		if (this == &list) { return *this; }
		Node* pnode = void_node.prev;
		while (pnode != &void_node)
		{
			Node* prev = pnode->prev;
			delete pnode;
			pnode = prev;
		}
		head = list.head;
		_size = list._size;
		void_node.next = &void_node;
		head->prev = &void_node;
		list.void_node.prev->next = &void_node;
		void_node.prev = list.void_node.prev;
		list.head = &list.void_node;
		list.void_node.prev = &list.void_node;
		return *this;
	}

	~LinkList()
	{
		Node* pnode = void_node.prev;
		while (pnode != &void_node)
		{
			Node* prev = pnode->prev;
			delete pnode;
			pnode = prev;
		}
	}

	class Itor
	{
		Node* node = nullptr;

		explicit Itor(Node* pnode) : node(pnode)
		{}

		Node* operator->()
		{
			return node;
		}

	public:

		using value_type = T;
		using reference = T&;
		using pointer = T*;
		using const_reference = const T&;
		using const_pointer = const T*;

		T& operator*()
		{
			return node->value;
		}

		bool operator++()
		{
			if (node != nullptr && node->next != nullptr)
			{
				node = node->next;
				return true;
			}
			return false;
		}

		bool operator--()
		{
			if (node != nullptr && node->prev != nullptr)
			{
				node = node->prev;
				return true;
			}
			return false;
		}

		bool operator==(Itor it)
		{
			return it.node == node;
		}

		bool operator!=(Itor it)
		{
			return it.node != node;
		}

		friend class LinkList;
	};

	class ConstItor
	{
		const Node* node = nullptr;
	public:

		using value_type = T;
		using const_reference = const T&;
		using const_pointer = const T*;

		explicit ConstItor(const Node* pnode) : node(pnode)
		{}

		T operator*()
		{
			return node->value;
		}

		bool operator++()
		{
			if (node != nullptr && node->next != nullptr)
			{
				node = node->next;
				return true;
			}
			return false;
		}

		bool operator--()
		{
			if (node != nullptr && node->prev != nullptr)
			{
				node = node->prev;
				return true;
			}
			return false;
		}

		bool operator==(ConstItor it)
		{
			return it.node == node;
		}

		bool operator!=(ConstItor it)
		{
			return it.node != node;
		}
	};

	using iterator = Itor;
	using const_iterator = ConstItor;

	void push_front(const T& val)
	{
		Node* newhead = new Node(val);
		_size++;
		newhead->prev = &void_node;
		newhead->next = head;
		head->prev = newhead;
		head = newhead;
	}

	void pop_front()
	{
		if (!empty())
		{
			Node* headnxt = head->next;
			delete head;
			_size--;
			head = headnxt;
			head->prev = &void_node;
		}
#ifdef _DEBUG
		else { throw "Do Pop On A Empty List"; }
#endif
	}

	void push_back(const T& val)
	{
		if (!empty())
		{
			Node* newend = new Node(val);
			_size++;
			newend->next = &void_node;
			newend->prev = void_node.prev;
			void_node.prev->next = newend;
			void_node.prev = newend;
		}
		else
		{
			push_front(val);
		}
	}

	void pop_back()
	{
		if (!empty())
		{
			Node* newprev = void_node.prev->prev;
			newprev->next = &void_node;
			delete void_node.prev;
			_size--;
			void_node.prev = newprev;
		}
#ifdef _DEBUG
		else { throw "Do Pop On A Empty List"; }
#endif
	}

	bool empty() const
	{
		return head == &void_node || _size == 0;
	}

	T& front()
	{
		return head->value;
	}

	T& back()
	{
		return void_node.prev->value;
	}

	T front() const
	{
		return head->value;
	}

	T back() const
	{
		return void_node.prev->value;
	}

	Itor insert_after(Itor it, const T& val)
	{
		if (it->next != &void_node)
		{
			Node* itnxt = it->next;
			it->next = new Node(val);
			_size++;
			it->next->next = itnxt;
			return Itor(it->next);
		}
		return it;
	}

	Itor erase(Itor it)
	{
		if (empty())
		{
			return it;
		}
		if (it.node == head)
		{
			Node* next = head->next;
			delete head;
			_size--;
			head = next;
			head->prev = &void_node;
			return Itor(head);
		}
		Node* prev = it->prev;
		Node* next = it->next;
		delete it.node;
		_size--;
		prev->next = next;
		next->prev = prev;
		return Itor(prev);
	}

	Itor begin()
	{
		return Itor(head);
	}

	Itor end()
	{
		return Itor(&void_node);
	}

	ConstItor begin() const
	{
		return ConstItor(head);
	}

	ConstItor end() const
	{
		return ConstItor(&void_node);
	}

	int erase(std::function<bool(const T&)> pred)
	{
		int ans = 0;
		for (auto it = begin(); it != end(); ++it)
		{
			if (pred(it->value))
			{
				it = erase(it);
				ans++;
			}
		}
		return ans;
	}

	int erase(const T& val)
	{
		int ans = 0;
		for (auto it = begin(); it != end(); ++it)
		{
			if (it->value == val)
			{
				it = erase(it);
				ans++;
			}
		}
		return ans;
	}

	int size() const
	{
		return _size;
	}

	void swap(Itor ia, Itor ib)
	{
		std::swap(ia->value, ib->value);
	}

	void sort(std::function<bool(const T&, const T&)> cmp = [](const T& a, const T& b) { return a > b; })
	{
		for (Itor i = begin(); i != end(); ++i)
		{
			Itor maxi = i;
			for (Itor j = i; j != end(); ++j)
			{
				if (cmp(maxi->value, j->value))
				{
					maxi = j;
				}
			}
			swap(maxi, i);
		}
	}

	template <typename Tb>
	bool operator==(const LinkList<Tb> list) const
	{
		if (std::is_same<Tb, T>::value)
		{
			if (list.size() != size())
			{
				return false;
			}
			auto it = begin();
			auto lit = list.begin();
			for (; it != end() && lit != list.end(); ++it, ++lit)
			{
				if (*it != *lit)
				{
					return false;
				}
			}
			return true;
		}
		return false;
	}
};
