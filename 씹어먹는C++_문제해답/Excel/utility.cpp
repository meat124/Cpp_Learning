#include "utils.h"

namespace MyExcel
{
	Vector::Vector(int n) : data(new string[n]), capacity(n), length(0) {} // 디폴트 인자 명시 x

	void Vector::push_back(string s)
	{
		if (capacity <= length) // 만약 현재 길이가 용량보다 크다면
		{
			string* temp = new string[capacity * 2]; // 임시로 데이터를 저장할 문자열 temp 할당(용량은 두 배로)
			for (int i = 0; i < length; i++)
				temp[i] = data[i]; // 임시 문자열 배열에 기존 데이터 복사
			delete[] data; // 기존 data 메모리 할당 해제
			data = temp; // string 클래스의 = 연산으로 임시 문자열 배열인 temp 의 데이터를 data 로 다시 복사
			capacity *= 2; // 용량을 두 배로 늘림
			delete[] temp;
		}
		data[length++] = s; // 인자로 전달받은 문자열 s를 최상단에 저장
	}

	string Vector::operator[](int i) { return data[i]; }

	void Vector::remove(int x)
	{
		for (int i = x + 1; i < length; i++)
			data[i - 1] = data[i]; // data[x] 를 삭제하고 뒤의 데이터들을 앞으로 당김
		length--; // 원소를 하나 삭제했으므로 현재 길이도 하나 줄임
	}

	int Vector::size() { return length; }

	Vector::~Vector()
	{
		if (data) // 만약 data 가 false 를 반환하면
			delete[] data; // 할당된 메모리 해제
	}

	Stack::Stack() : start(NULL, "") { current = &start; }
	void Stack::push(string s)
	{
		Node* n = new Node(current, s); // 새로운 노드를 생성해서 현재 노드를 가리키는 포인터와 인자로 전달받은 문자열 s 를 인자로 전달
		current = n; // 최상단 포인터를 새롭게 만든 인자로 초기화
	}
	string Stack::pop()
	{
		if (current == &start)
			return ""; // 만약 최상단 포인터가 최하위 포인터와 같다면 아무것도 반환하지 않는다.

		string s = current->s; // 새로운 문자열 객체 s 를 생성해서 현재 노드를 가리키는 포인터인 current 의 문자열데이터를 복사
		Node* prev = current; // Node 클래스를 가리키는 포인터 prev를 선언하고 현재 노드를 가리키는 포인터 current 로 초기화
		current = current->prev; // 현재 노드를 가리키는 포인터 current 를 하위 포인터인 prev 로 초기화

		// 팝이 완료된 노드를 삭제한다.
		delete prev;
		return s; // 최상단에서 꺼낸 데이터를 리턴
	}
	string Stack::peek() { return current->s; }
	bool Stack::is_empty()
	{
		if (current == &start) // 현재 노드를 가리키는 포인터가 최하위 포인터와 같다면 true 를 반환한다.
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