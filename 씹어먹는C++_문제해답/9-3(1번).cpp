#include <iostream>
#include <string>
// 9-2 �� ���� 1
using namespace std;

template <int N>
struct fib
{
    static const int result = fib<N - 1>::result + fib<N - 2>::result;
};

// 1 �� ° �Ǻ���ġ ���� ���� ���̽� ���̽�
template <>
struct fib<1>
{
    static const int result = 1;
};

// 2 �� ° �Ǻ���ġ ���� ���� ���̽� ���̽�
template <>
struct fib<2>
{
    static const int result = 1;
};

int main()
{
    for (int i = 1; i <= 10; i++)
    {
        cout << "5 �� ° �Ǻ���ġ �� :: " << fib<5>::result << endl;
    }
}