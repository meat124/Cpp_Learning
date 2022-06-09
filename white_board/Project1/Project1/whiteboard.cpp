#include<iostream>
#include<string>

// ���ø� ù Ȱ��
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
		// ���� ���� �뷮���� ���̰� �� ��ų� ���ٸ�, ���Ӱ� �޸𸮸� �Ҵ��Ѵ�.(2���)
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
		
		// �����ϴ� ����
		if (s) // ���� true �̸� ����
		{
			// data[length / 32] �� ���� ��Ʈ�� ��Ÿ�� ����
			// ex. ���� ���� 4 ��� 0000 0100
			// ������ ���� 1��Ʈ, �� 0000 0001 �� �������� (length % 32) ĭ ��ŭ �̵��Ѵ�.
			// ���������� �º��� �캯�� �񱳿����� �Ͽ��� �� ��Ʈ���� �ϳ��� 1�� �����ϸ� 1�� �����Ѵ�.
			// �� 0101 0101 |= 1010 1010; �� 1111 1111 �� �ȴ�.
			// ���������� ���� ����Ʈ���� �º��� �Ҵ��Ѵ�.
			data[length / 32] |= (1 << (length % 32));
			// A | B �ؼ� ���� ���� A �� �Ҵ��ϴ� ���� A |= B �̴�.
			// ��, length / 32 �� ���� data �� �׾ư��鼭 �����濡 �����ϴ� ����̹Ƿ�
			// 1 �� �������� ����Ʈ ������ ���� �̵� ��Ų ��, �Ҵ��Ѵ�.
		}

		length++;
	}

	// i �ε����� ���ҿ� �����Ѵ�.
	bool operator[](int i) { return (data[i / 32] & (1 << (i % 32))) != 0; }

	// x ��° ��ġ�� ���Ҹ� �����Ѵ�.
	void remove(int x)
	{
		for (int i = x; i < length; i++)
		{
			int prev = i - 1; // ���� ��ġ
			int curr = i; // ���� ��ġ

			// ���� curr ��ġ�� �ִ� ��Ʈ�� 1 �̶��
			// prev ��ġ�� �ִ� ��Ʈ�� 1�� �����.
			if (data[curr / 32] & (1 << (curr % 32))) // data[i] �� 1�̰� 
			{
				data[prev / 32] |= (1 << (prev % 32));
			}
			// �ƴϸ� prev ��ġ�� �ִ� ��Ʈ�� 0 ���� �����.
			else
			{
				// unsigned int Ÿ������ ������ ������ 16���� ����
				// �� 16 ������ 4 ����Ʈ�� ��� 1�� ä��� ���̴�.
				// ��, 16 ������ 2 ������ 4����Ʈ�� ��ȯ�ϴ� ��Ģ�� �����Ѵ�!
				unsigned int all_ones_except_prev = 0xFFFFFFFF; // 16 ������ 1 ����Ʈ���� ǥ�� ������ ���� ū ��
				// ^ ������ �ٸ��� 1��, ������ 0�� ��ȯ�Ѵ�.
				// ���� 1 << (prev % 32) ���� 0�� �κ��� 
				all_ones_except_prev ^= (1 << (prev % 32));
				data[prev / 32] &= all_ones_except_prev;
			}
		}
		length--;
	}

	// ���� ������ ũ�⸦ ���Ѵ�.
	int size() { return length; }
	~Vector()
	{
		if (data)
			delete[] data;
	}
};

int main()
{
	// int �� �����ϴ� ���͸� �����.
	Vector<int> int_vec; // ���� Ŭ������ ���� <> �ȿ� �츮�� ���ϴ� Ÿ���� ���´�.
	int_vec.push_back(3);
	int_vec.push_back(2);

	std::cout << "------- int vector -------" << std::endl;
	std::cout << "ù ��° ���� : " << int_vec[0] << std::endl;
	std::cout << "�� ��° ���� : " << int_vec[1] << std::endl;

	Vector<std::string> str_vec; // string �� �����ϴ� ���� ����
	str_vec.push_back("hello");
	str_vec.push_back("world");
	
	std::cout << "------- std::string vector -------" << std::endl;
	std::cout << "ù ��° ���� : " << str_vec[0] << std::endl;
	std::cout << "�� ��° ���� : " << str_vec[1] << std::endl;

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