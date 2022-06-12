#include <iostream>
#include <string>
// 9-2 강 문제 1
using namespace std;

template <int N>
struct fib
{
    static const int result = fib<N - 1>::result + fib<N - 2>::result;
};

// 1 번 째 피보나치 수에 대한 베이스 케이스
template <>
struct fib<1>
{
    static const int result = 1;
};

// 2 번 째 피보나치 수에 대한 베이스 케이스
template <>
struct fib<2>
{
    static const int result = 1;
};

int main()
{
    for (int i = 1; i <= 10; i++)
    {
        cout << "5 번 째 피보나치 수 :: " << fib<5>::result << endl;
    }
}