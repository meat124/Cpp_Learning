#include <iostream>
#include <string.h>

class string
{
    char *str;
    int len = 0;

public:
    string(char c, int n);
    string(const char *s);
    string(const string &s);
    ~string(); //소멸자

    void add_string(const string &s);  // str뒤에 s를 붙인다.
    void copy_string(const string &s); // str에 s를 복사한다.
    void show_string();                // str을 출력한다.
    int strlen();                      //문자열 길이 리턴 함수
};
string::string(char c, int n)
{
    std::cout << "생성자 호출 ! " << std::endl;
    len = n;
    str = new char[len + 1];
    for (int i = 0; i < len; i++)
        str[i] = c;
    str[len] = 0; //문자열인지 확인하는 문자인 0을 대입하지 않으면 쓰레기 값이 출력될 수 있다.
}
string::string(const char *s)
{
    std::cout << "복사 생성자 호출 ! " << std::endl;
    len = 0;
    while (*s)
    {
        len++;
        s++;
    }
    s -= len;
    str = new char[len + 1]; // new연산자를 활용해서 메모리를 동적 할당
    strcpy(str, s);
    str[len] = 0; // 문자열임을 확인하기 위해 끝 부분에 0을 대입
}
string::string(const string &s)
{
    std::cout << "복사 생성자 호출 ! " << std::endl;
    len = s.len;
    str = new char[len + 1];
    strcpy(str, s.str);
}
string::~string() // 소멸 연산자
{
    std::cout << "소멸 연산자 호출 ! " << std::endl;
    if (str)
        delete[] str;
}
int string::strlen()
{
    return len;
}
void string::add_string(const string &s)
{
    int total_len = len + s.len + 1;
    char *temp = new char[total_len]; // 새로운 메모리를 동적 할당
    strcpy(temp, str);
    strcpy(temp + len, s.str); // 기존에 str이 있던 길이만큼 뒤로 밀고 새로운 문자열을 복사
    delete[] str;              // 기존의 문자열이 할당되었던 메모리를 해제
    len = len + s.len;         // 기존의 len을 새롭게 추가된 문자열의 길이까지 포함해서 연장
    temp[len] = 0;             // 새롭게 할당된 문자열의 종료를 표시하기 위해 0을 넣어줌
    str = temp;                // 임시 공간으로 쓰던 temp를 다시 str로 옮겨주면서 종료
}
void string::copy_string(const string &s)
{
    delete[] str;
    str = new char[s.len + 1];
    len = s.len;
    strcpy(str, s.str); // 기존의 str메모리에 새롭게 입력된 s의 문자열을 복사
}
void string::show_string()
{
    std::cout << "문자열을 출력합니다.\n"
              << str << std::endl;
}

int main()
{
    string str1('a', 5);
    string str2('b', 5);
    string str3(str1);

    str1.show_string();
    str2.show_string();
    str3.show_string();

    std::cout << "str1에 str2를 추가 ! " << std::endl;
    str1.add_string(str2);
    std::cout << "str2에 str3를 추가 ! " << std::endl;
    str2.add_string(str3);

    str1.show_string();
    str2.show_string();
    str3.show_string();

    std::cout << "str2 에 str1의 문자열 복사 ! " << std::endl;
    str2.copy_string(str1);

    str1.show_string();
    str2.show_string();
    str3.show_string();

    return 0;
}