#include<iostream>

template <typename T>
struct Compare
{
	bool operator()(const T& a, const T& b) const { return a < b; }
};

template <typename T, typename Comp = Compare<T>>
T Min(T a, T b)
{
	Comp comp; // Comp 클래스의 인스턴스화
	if (comp(a, b)) // a < b 이면 1이 리턴되어서 if문이 실행된다.
		return a;
	return b;
}

int main()
{
	int a = 3, b = 5;
	std::cout << "Min " << a << " , " << b << " :: " << Min(a, b) << std::endl;

	std::string s1 = "abc", s2 = "def";
	std::cout << "Min " << s1 << " , " << s2 << " :: " << Min(s1, s2) << std::endl;
}