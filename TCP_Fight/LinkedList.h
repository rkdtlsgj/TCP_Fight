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
			//���� ��带 ���� ���� �̵�

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
	
	/*	ù��° ������ ��带 ����Ű�� ���ͷ����� ����*/
	iterator begin()
	{
		return iterator(_head._Next);
	};


	/*Tail ��带 ����Ű��(�����Ͱ� ���� ��¥ ���� �� ���) ���ͷ����͸� ����
	�Ǵ� ������ ������ �� �ִ� ���ͷ����͸� ����*/
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
		//- ���ͷ������� �� ��带 ����.
		//- �׸��� ���� ����� ���� ��带 ī��Ű�� ���ͷ����� ����

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