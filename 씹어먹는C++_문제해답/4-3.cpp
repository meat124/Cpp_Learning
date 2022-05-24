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
    ~string(); //�Ҹ���

    void add_string(const string &s);  // str�ڿ� s�� ���δ�.
    void copy_string(const string &s); // str�� s�� �����Ѵ�.
    void show_string();                // str�� ����Ѵ�.
    int strlen();                      //���ڿ� ���� ���� �Լ�
};
string::string(char c, int n)
{
    std::cout << "������ ȣ�� ! " << std::endl;
    len = n;
    str = new char[len + 1];
    for (int i = 0; i < len; i++)
        str[i] = c;
    str[len] = 0; //���ڿ����� Ȯ���ϴ� ������ 0�� �������� ������ ������ ���� ��µ� �� �ִ�.
}
string::string(const char *s)
{
    std::cout << "���� ������ ȣ�� ! " << std::endl;
    len = 0;
    while (*s)
    {
        len++;
        s++;
    }
    s -= len;
    str = new char[len + 1]; // new�����ڸ� Ȱ���ؼ� �޸𸮸� ���� �Ҵ�
    strcpy(str, s);
    str[len] = 0; // ���ڿ����� Ȯ���ϱ� ���� �� �κп� 0�� ����
}
string::string(const string &s)
{
    std::cout << "���� ������ ȣ�� ! " << std::endl;
    len = s.len;
    str = new char[len + 1];
    strcpy(str, s.str);
}
string::~string() // �Ҹ� ������
{
    std::cout << "�Ҹ� ������ ȣ�� ! " << std::endl;
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
    char *temp = new char[total_len]; // ���ο� �޸𸮸� ���� �Ҵ�
    strcpy(temp, str);
    strcpy(temp + len, s.str); // ������ str�� �ִ� ���̸�ŭ �ڷ� �а� ���ο� ���ڿ��� ����
    delete[] str;              // ������ ���ڿ��� �Ҵ�Ǿ��� �޸𸮸� ����
    len = len + s.len;         // ������ len�� ���Ӱ� �߰��� ���ڿ��� ���̱��� �����ؼ� ����
    temp[len] = 0;             // ���Ӱ� �Ҵ�� ���ڿ��� ���Ḧ ǥ���ϱ� ���� 0�� �־���
    str = temp;                // �ӽ� �������� ���� temp�� �ٽ� str�� �Ű��ָ鼭 ����
}
void string::copy_string(const string &s)
{
    delete[] str;
    str = new char[s.len + 1];
    len = s.len;
    strcpy(str, s.str); // ������ str�޸𸮿� ���Ӱ� �Էµ� s�� ���ڿ��� ����
}
void string::show_string()
{
    std::cout << "���ڿ��� ����մϴ�.\n"
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

    std::cout << "str1�� str2�� �߰� ! " << std::endl;
    str1.add_string(str2);
    std::cout << "str2�� str3�� �߰� ! " << std::endl;
    str2.add_string(str3);

    str1.show_string();
    str2.show_string();
    str3.show_string();

    std::cout << "str2 �� str1�� ���ڿ� ���� ! " << std::endl;
    str2.copy_string(str1);

    str1.show_string();
    str2.show_string();
    str3.show_string();

    return 0;
}