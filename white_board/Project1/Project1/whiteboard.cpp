#include<iostream>
#include<string>

// 템플릿 첫 활용
template <typename T>
class Vector
{
	T* data;
	int capacity;
	int length;

public:
	typedef T value_type;

	Vector(int n = 1) : data(new T[n]), capacity(n), length(0) {}

	void push_back(T s)
	{
		if (capacity <= length)
		{
			T* temp = new T[capacity * 2];
			for (int i = 0; i < length; i++)
				temp[i] = data[i];

			delete[] data;
			data = temp;
			capacity *= 2;
		}

		data[length] = s;
		length++;
	}

	T operator[](int i) { return data[i]; }

	void remove(int x)
	{
		for (int i = x + 1; i < length; i++)
			data[i - 1] = data[i];

		length--;
	}

	int size() { return length; }

	~Vector()
	{
		if (data)
			delete[] data;
	}
};

template<>
class Vector<bool>
{
	unsigned int* data;
	int capacity;
	int length;

public:
	typedef bool value_type;

	Vector(int n = 1)
		: data(new unsigned int[n / 32 + 1]), capacity(n / 32 + 1), length(0)
	{
		for (int i = 0; i < capacity; i++)
			data[i] = 0;
	}

	void push_back(bool s)
	{
		// 만약 현재 용량보다 길이가 더 길거나 같다면, 새롭게 메모리를 할당한다.(2배로)
		if (capacity * 32 <= length)
		{
			unsigned int* temp = new unsigned int[capacity * 2];
			for (int i = 0; i < capacity; i++)
				temp[i] = data[i];
			for (int i = capacity; i < 2 * capacity; i++)
				temp[i] = 0;

			delete[] data;
			data = temp;
			capacity *= 2;
		}
		
		// 대입하는 과정
		if (s) // 만약 true 이면 실행
		{
			// data[length / 32] 의 값을 비트로 나타낸 값과
			// ex. 만약 값이 4 라면 0000 0100
			// 오른쪽 항은 1비트, 즉 0000 0001 을 왼쪽으로 (length % 32) 칸 만큼 이동한다.
			// 최종적으로 좌변과 우변을 비교연산을 하여서 각 비트에서 하나라도 1이 존재하면 1로 대입한다.
			// 즉 0101 0101 |= 1010 1010; 은 1111 1111 이 된다.
			// 최종적으로 나온 바이트값을 좌변에 할당한다.
			data[length / 32] |= (1 << (length % 32));
			// A | B 해서 나온 값을 A 에 할당하는 식이 A |= B 이다.
			// 즉, length / 32 는 현재 data 에 쌓아가면서 최전방에 존재하는 요소이므로
			// 1 을 왼쪽으로 시프트 연산을 통해 이동 시킨 뒤, 할당한다.
		}

		length++;
	}

	// i 인덱스의 원소에 접근한다.
	bool operator[](int i) { return (data[i / 32] & (1 << (i % 32))) != 0; }

	// x 번째 위치한 원소를 제거한다.
	void remove(int x)
	{
		for (int i = x; i < length; i++)
		{
			int prev = i - 1; // 이전 위치
			int curr = i; // 현재 위치

			// 만약 curr 위치에 있는 비트가 1 이라면
			// prev 위치에 있는 비트를 1로 만든다.
			if (data[curr / 32] & (1 << (curr % 32))) // data[i] 가 1이고 
			{
				data[prev / 32] |= (1 << (prev % 32));
			}
			// 아니면 prev 위치에 있는 비트를 0 으로 지운다.
			else
			{
				// unsigned int 타입으로 선언한 변수에 16진수 대입
				// 이 16 진수는 4 바이트를 모두 1로 채우는 값이다.
				// 즉, 16 진수는 2 진수를 4바이트로 변환하는 규칙이 존재한다!
				unsigned int all_ones_except_prev = 0xFFFFFFFF; // 16 진수로 1 바이트에서 표현 가능한 가장 큰 수
				// ^ 연산은 다르면 1을, 같으면 0을 반환한다.
				// 따라서 1 << (prev % 32) 에서 0인 부분이 
				all_ones_except_prev ^= (1 << (prev % 32));
				data[prev / 32] &= all_ones_except_prev;
			}
		}
		length--;
	}

	// 현재 벡터의 크기를 구한다.
	int size() { return length; }
	~Vector()
	{
		if (data)
			delete[] data;
	}
};

int main()
{
	// int 를 보관하는 벡터를 만든다.
	Vector<int> int_vec; // 먼저 클래스를 적고 <> 안에 우리가 원하는 타입을 적는다.
	int_vec.push_back(3);
	int_vec.push_back(2);

	std::cout << "------- int vector -------" << std::endl;
	std::cout << "첫 번째 원소 : " << int_vec[0] << std::endl;
	std::cout << "두 번째 원소 : " << int_vec[1] << std::endl;

	Vector<std::string> str_vec; // string 을 보관하는 벡터 생성
	str_vec.push_back("hello");
	str_vec.push_back("world");
	
	std::cout << "------- std::string vector -------" << std::endl;
	std::cout << "첫 번째 원소 : " << str_vec[0] << std::endl;
	std::cout << "두 번째 원소 : " << str_vec[1] << std::endl;

	Vector<bool> bool_vec;
	bool_vec.push_back(true);
	bool_vec.push_back(false);
	bool_vec.push_back(true);
	bool_vec.push_back(true);
	bool_vec.push_back(false);
	bool_vec.push_back(true);
	bool_vec.push_back(true);
	bool_vec.push_back(false);
	bool_vec.push_back(true);
	bool_vec.push_back(false);
	bool_vec.push_back(false);
	bool_vec.push_back(false);
	bool_vec.push_back(true);
	bool_vec.push_back(true);
	bool_vec.push_back(false);
	bool_vec.push_back(true);
	bool_vec.push_back(true);
	bool_vec.push_back(false);
	bool_vec.push_back(true);
	bool_vec.push_back(false);
	bool_vec.push_back(true);
	bool_vec.push_back(false);
	bool_vec.push_back(true);

	std::cout << "-------- bool vector --------" << std::endl;
	for (int i = 0; i < bool_vec.size(); i++)
	{
		std::cout << bool_vec[i];
	}
	std::cout << std::endl;
}