#pragma once
#include <iostream>
template <typename T>

class CList
{
public:
	struct Node
	{
		T _data;
		Node* _Prev;
		Node* _Next;
	};

	class iterator
	{
	private:
		 Node* _node;
		 friend class CList;
	public:
		iterator(Node* node = nullptr) : _node(node) {};

		iterator operator ++(int)
		{
			//현재 노드를 다음 노드로 이동

			iterator temp(_node);
			_node = _node->_Next;

			return temp;
		}

		iterator& operator++()
		{
			_node = _node->_Next;
			return *this;
		}

		iterator operator --(int)
		{
			iterator temp(_node);
			_node = _node->_Prev;

			return temp;
		}

		iterator& operator--()
		{
			_node = _node->_Prev;

			return *this;
		}

		T& operator *()
		{
			return _node->_data;
		}

		bool operator ==(const iterator& other)
		{
			return _node == other._node;
		}
		bool operator !=(const iterator& other)
		{
			return _node != other._node;
		}

	};

public:
	CList()
	{
		_head._Prev = NULL;
		_head._Next = &_tail;

		_tail._Prev = &_head;
		_tail._Next = NULL;
	};

	~CList()
	{

	};

	T _data;
	Node* _Prev;
	Node* _Next;
	
	/*	첫번째 데이터 노드를 가리키는 이터레이터 리턴*/
	iterator begin()
	{
		return iterator(_head._Next);
	};


	/*Tail 노드를 가리키는(데이터가 없는 진짜 더미 끝 노드) 이터레이터를 리턴
	또는 끝으로 인지할 수 있는 이터레이터를 리턴*/
	iterator end()
	{
		return iterator(&_tail);
	};


	void push_front(T data)
	{
		Node* newNode = new Node;
		newNode->_data = data;
		newNode->_Prev = &_head;
		newNode->_Next = _head._Next;
		newNode->_Next->_Prev = newNode;
		_head._Next = newNode;

		++_size;
	};

	void push_back(T data)
	{
		Node* newNode = new Node;
		newNode->_data = data;
		newNode->_Next = &_tail;
		newNode->_Prev = _tail._Prev;

		_tail._Prev->_Next = newNode;
		_tail._Prev = newNode;

		++_size;
	}

	T pop_front()
	{
		return _head._Next._data;
	};

	T pop_back()
	{
		return _tail._Prev._data;
	};

	void clear()
	{
		CList<int>::iterator iter;
		for (iter = begin(); iter != end();)
		{
			iter = erase(iter);
		}
	};

	int size() { return _size; };

	bool empty() { return _size == 0; };

	iterator erase(iterator itor)
	{
		//- 이터레이터의 그 노드를 지움.
		//- 그리고 지운 노드의 다음 노드를 카리키는 이터레이터 리턴

		iterator temp(itor._node->_Next);

		itor._node->_Next->_Prev = itor._node->_Prev;
		itor._node->_Prev->_Next = itor._node->_Next;

		delete itor._node;

		--_size;

		return temp;
	};


	void remove(T Data)
	{
		CList<int>::iterator iter;
		for (iter = begin(); iter != end();)
		{
			if (*iter == Data)
				iter = erase(iter);
			else
				++iter;
		}
	}

private:

	int _size = 0;
	Node _head;
	Node _tail;
};