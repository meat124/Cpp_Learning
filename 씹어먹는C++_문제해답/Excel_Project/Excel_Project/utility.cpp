#include "utils.h"

namespace MyExcel
{
	Vector::Vector(int n) : data(new string[n]), capacity(n), length(0) {} // ����Ʈ ���� ��� x

	void Vector::push_back(string s)
	{
		if (capacity <= length) // ���� ���� ���̰� �뷮���� ũ�ٸ�
		{
			string* temp = new string[capacity * 2]; // �ӽ÷� �����͸� ������ ���ڿ� temp �Ҵ�(�뷮�� �� ���)
			for (int i = 0; i < length; i++)
				temp[i] = data[i]; // �ӽ� ���ڿ� �迭�� ���� ������ ����
			delete[] data; // ���� data �޸� �Ҵ� ����
			data = temp; // string Ŭ������ = �������� �ӽ� ���ڿ� �迭�� temp �� �����͸� data �� �ٽ� ����
			capacity *= 2; // �뷮�� �� ��� �ø�
			delete[] temp;
		}
		data[length++] = s; // ���ڷ� ���޹��� ���ڿ� s�� �ֻ�ܿ� ����
	}

	string Vector::operator[](int i) { return data[i]; }

	void Vector::remove(int x)
	{
		for (int i = x + 1; i < length; i++)
			data[i - 1] = data[i]; // data[x] �� �����ϰ� ���� �����͵��� ������ ���
		length--; // ���Ҹ� �ϳ� ���������Ƿ� ���� ���̵� �ϳ� ����
	}

	int Vector::size() { return length; }

	Vector::~Vector()
	{
		if (data) // ���� data �� false �� ��ȯ�ϸ�
			delete[] data; // �Ҵ�� �޸� ����
	}

	Stack::Stack() : start(NULL, "") { current = &start; }
	void Stack::push(string s)
	{
		Node* n = new Node(current, s); // ���ο� ��带 �����ؼ� ���� ��带 ����Ű�� �����Ϳ� ���ڷ� ���޹��� ���ڿ� s �� ���ڷ� ����
		current = n; // �ֻ�� �����͸� ���Ӱ� ���� ���ڷ� �ʱ�ȭ
	}
	string Stack::pop()
	{
		if (current == &start)
			return ""; // ���� �ֻ�� �����Ͱ� ������ �����Ϳ� ���ٸ� �ƹ��͵� ��ȯ���� �ʴ´�.

		string s = current->s; // ���ο� ���ڿ� ��ü s �� �����ؼ� ���� ��带 ����Ű�� �������� current �� ���ڿ������͸� ����
		Node* prev = current; // Node Ŭ������ ����Ű�� ������ prev�� �����ϰ� ���� ��带 ����Ű�� ������ current �� �ʱ�ȭ
		current = current->prev; // ���� ��带 ����Ű�� ������ current �� ���� �������� prev �� �ʱ�ȭ

		// ���� �Ϸ�� ��带 �����Ѵ�.
		delete prev;
		return s; // �ֻ�ܿ��� ���� �����͸� ����
	}
	string Stack::peek() { return current->s; }
	bool Stack::is_empty()
	{
		if (current == &start) // ���� ��带 ����Ű�� �����Ͱ� ������ �����Ϳ� ���ٸ� true �� ��ȯ�Ѵ�.
			return true;
		return false;
	}
	Stack::~Stack()
	{
		while (current != &start)
		{
			Node* prev = current;
			current = current->prev;
			delete prev;
		}
	}

	NumStack::NumStack() : start(NULL, 0) { current = &start; }
	void NumStack::push(double s)
	{
		Node* n = new Node(current, s);
		current = n;
	}
	double NumStack::pop()
	{
		if (current == &start)
			return 0;

		double s = current->s;
		Node* prev = current;
		current = current->prev;

		// Delete popped node
		delete prev;
		return s;
	}
	double NumStack::peek() { return current->s; }
	bool NumStack::is_empty()
	{
		if (current == &start)
			return true;
		return false;
	}
	NumStack::~NumStack()
	{
		while (current != &start)
		{
			Node* prev = current;
			current = current->prev;
			delete prev;
		}
	}
}