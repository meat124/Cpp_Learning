#ifndef UTILS_H
#define UTILS_H

#include<string>
using std::string;

namespace MyExcel
{
	class Vector
	{
		string* data; // 데이터를 보관하기 위한 문자열 배열
		int capacity; // 현재 할당된 크기
		int length; // 실제로 사용하는 양

	public:
		// 생성자
		Vector(int n = 1); // 만약 사용자가 인자를 지정하지 않으면 알아서 n = 1 로 지정된다. 이를 디폴트 인자라고 한다.

		// 맨 뒤에 새로운 원소를 추가한다.
		void push_back(string s);

		// 임의의 위치의 원소에 접근한다.
		string operator[](int i);

		// x 번째 위치한 원소를 제거한다.
		void remove(int x);

		// 현재 벡터의 크기를 구한다.
		int size();

		// 디폴트 소멸자
		~Vector();
	};

	class Stack
	{
		struct Node
		{
			Node* prev; // 하위 노드의 포인터를 가리키는 포인터
			string s; // 데이터가 저장되는 문자열

			// 생성자는 인자로 하위 노드의 포인터와 문자열 데이터를 받는다.
			Node(Node* prev, string s) : prev(prev), s(s) {}
		};

		Node* current; // 최상위 노드를 가리키는 포인터
		Node start; // 최하위(밑바닥) 노드를 가리키는 포인터

	public:
		Stack();

		// 최상단에 새로운 원소를 추가한다.
		void push(string s);

		// 최상단의 원소를 제거하고 반환한다.
		string pop();

		// 최상단의 원소를 반환한다.(제거 안함)
		string peek();

		// 스택이 비어있는지의 유무를 반환한다.
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