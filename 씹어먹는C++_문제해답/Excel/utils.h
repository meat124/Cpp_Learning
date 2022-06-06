#ifndef UTILS_H
#define UTILS_H

#include<string>
using std::string;

namespace MyExcel
{
	class Vector
	{
		string* data; // �����͸� �����ϱ� ���� ���ڿ� �迭
		int capacity; // ���� �Ҵ�� ũ��
		int length; // ������ ����ϴ� ��

	public:
		// ������
		Vector(int n = 1); // ���� ����ڰ� ���ڸ� �������� ������ �˾Ƽ� n = 1 �� �����ȴ�. �̸� ����Ʈ ���ڶ�� �Ѵ�.

		// �� �ڿ� ���ο� ���Ҹ� �߰��Ѵ�.
		void push_back(string s);

		// ������ ��ġ�� ���ҿ� �����Ѵ�.
		string operator[](int i);

		// x ��° ��ġ�� ���Ҹ� �����Ѵ�.
		void remove(int x);

		// ���� ������ ũ�⸦ ���Ѵ�.
		int size();

		// ����Ʈ �Ҹ���
		~Vector();
	};

	class Stack
	{
		struct Node
		{
			Node* prev; // ���� ����� �����͸� ����Ű�� ������
			string s; // �����Ͱ� ����Ǵ� ���ڿ�

			// �����ڴ� ���ڷ� ���� ����� �����Ϳ� ���ڿ� �����͸� �޴´�.
			Node(Node* prev, string s) : prev(prev), s(s) {}
		};

		Node* current; // �ֻ��� ��带 ����Ű�� ������
		Node start; // ������(�عٴ�) ��带 ����Ű�� ������

	public:
		Stack();

		// �ֻ�ܿ� ���ο� ���Ҹ� �߰��Ѵ�.
		void push(string s);

		// �ֻ���� ���Ҹ� �����ϰ� ��ȯ�Ѵ�.
		string pop();

		// �ֻ���� ���Ҹ� ��ȯ�Ѵ�.(���� ����)
		string peek();

		// ������ ����ִ����� ������ ��ȯ�Ѵ�.
		bool is_empty();

		~Stack();
	};

	class NumStack
	{
		struct Node
		{
			Node* prev;
			double s;

			Node(Node* prev, double s) : prev(prev), s(s) {}
		};

		Node* current;
		Node start;

	public:
		NumStack();
		void push(double s);
		double pop();
		double peek();
		bool is_empty();

		~NumStack();
	};
}
#endif