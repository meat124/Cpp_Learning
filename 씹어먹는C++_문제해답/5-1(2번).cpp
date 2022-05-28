#include <iostream>

class Complex
{
private:
    double real, img; // 실수, 허수 부분

    double get_number(const char *str, int from, int to) const;

    // atof 함수를 이용하여 get_number 함수를 변형한다.
    double get_number_atof(const char *str, int from, int to) const;

public:
    Complex(double real, double img)
        : real(real), img(img) {}
    Complex(const Complex &c)
    {
        real = c.real;
        img = c.img;
    }
    Complex(const char *str);

    Complex operator+(const Complex &c) const;
    Complex operator+(const char *str);

    Complex operator-(const Complex &c) const;
    Complex operator-(const char *str) const;
    Complex operator*(const Complex &c) const;
    Complex operator*(const char *str) const;
    Complex operator/(const Complex &c) const;
    Complex operator/(const char *str) const;

    Complex &operator=(const Complex &c);

    Complex &operator+=(const Complex &c);
    Complex &operator-=(const Complex &c);
    Complex &operator*=(const Complex &c);
    Complex &operator/=(const Complex &c);

    void println()
    {
        std::cout << "( " << real << " , " << img << " ) " << std::endl;
    }
};
Complex::Complex(const char *str)
{
    // 입력 받은 문자열을 분석하여 real 부분과 img 부분을 찾는다.
    // 문자열의 끝은 다음과 같다. "[부호] (실수부) (부호) i (허수부)"
    // 이 때 맨 앞의 부호는 생략 가능하다. (생략시 + 라 가정)

    int begin = 0, end = strlen(str);
    img = 0.0;
    real = 0.0;

    // 먼저 가장 기준이 되는 i의 위치를 찾는다.
    int pos_i = -1; // -1로 초기화 시켜서 처음에는 양의 정수를 가리키지 못하게 막는다.
    for (int i = 0; i != end; i++)
    {
        if (str[i] == 'i')
        {
            pos_i = i; // i의 인덱스로 값을 초기화 시킨다.
            break;
        }
    }

    // 만약 i 가 없다면 그 복소수는 실수이다.
    if (pos_i == -1)
    {
        real = get_number_atof(str, begin, end - 1);
        return;
    }

    // 만약 i 가 있다면 실수부와 허수부를 나누어서 처리하면 된다.
    real = get_number_atof(str, begin, pos_i - 1);
    img = get_number_atof(str, pos_i + 1, end - 1); // pos_i의 위치는 i이므로 그거보다 하나 늘린 곳 부터 시작한다.

    if (pos_i >= 1 && str[pos_i - 1] == '-')
        img *= -1.0; // 허수부분이 존재하고 i의 앞의 부호가 -라면 -1을 곱해서 변경한다.
}
double Complex::get_number(const char *str, int from, int to) const // 기존의 get_number 함수
{
    bool minus = false; // 기본적으로 양수라고 가정
    if (from > to)      // 시작점이 종료점보다 크다면 0을 반환한다.
        return 0;
    if (str[from] == '-') // 시작점의 문자가 - 라면 음수이므로 minus를 true로 바꾼다.
        minus = true;
    if (str[from] == '-' || str[from] == '+') // 시작점이 - 거나 +이면 부호는 이제 판단할 필요 없으므로 한칸 민다.
        from++;

    double num = 0.0;
    double decimal = 1.0;

    bool integer_part = true;        // 기본적으로 정수라고 가정
    for (int i = from; i <= to; i++) // from 부터  to 까지 검사
    {
        // isdigit 함수는 숫자인지 아닌지 판단(맞으면 1, 아니면 0 반환)
        if (isdigit(str[i]) && integer_part) // 만약 숫자이고, 정수라면 실행
        {
            num *= 10.0;           // 자릿수 올리고
            num += (str[i] - '0'); // 일의 자리에 대입 str[i] - '0'
        }
        else if (str[i] == '.') // 만약 소수라면 정수 true 를 false로 변경
            integer_part = false;
        else if (isdigit(str[i]) && !integer_part) // 만약 소수이고 str[i] 가 정수라면
        {
            decimal /= 10.0;                   // 자릿수 곱을 하나 내리고
            num += ((str[i] - '0') * decimal); // 정수를 decimal을 곱해서 더한다.
        }
        else
            break;
    }

    if (minus) // minus 가 true이면 -1 을 곱해서 음수로 변환한다.
        num *= -1.0;

    return num;
}
double Complex::get_number_atof(const char *str, int from, int to) const // atof 를 이용해서 구현한 get_number 함수
{
    if (from > to)
        return 0; // 시작점이 종료점보다 크다면 0을 반환한다.

    char temp[50] = {0};
    strncpy(temp, str + from, to - from + 1); // 두번째 인자인 출발 문자열은 char* 형태이므로 인덱스로 표시하면 안되고 from을 더한 형태로 표현한다.

    double num = atof(temp);
    return num;
}

Complex Complex::operator+(const Complex &c) const
{
    Complex temp(real + c.real, img + c.img);
    return temp;
}
Complex Complex::operator+(const char *str)
{
    // 입력받은 문자열을 분석하여 real 부분과 img 부분을 찾는다.
    // 문자열의 꼴은 다음과 같다. "[부호] (실수부)(부호)i(허수부)"
    // 이 때 맨 앞의 부호는 생략 가능하다. (생략시 +라 가정)

    int begin = 0, end = strlen(str);
    double str_real = 0.0, str_img = 0.0;

    // 가장 먼저 수를 실수부와 허수부를 구분하는 기준이 되는 i를 먼저 찾는다.
    int pos_i = -1;
    for (int i = 0; i != end; i++)
    {
        if (str[i] == 'i')
        {
            pos_i = i;
            break;
        }
    }

    // 만약 i가 존재하지 않으면 이 수는 실수만 존재하는 복소수이다.
    if (pos_i == -1)
    {
        str_real = get_number_atof(str, begin, end - 1);

        Complex temp(str_real, str_img);
        return (*this) + temp;
    }

    // 만약 i가 존재하면, 실수부와 허수부를 나누어서 처리하면 된다.
    str_real = get_number_atof(str, begin, pos_i - 1);
    str_img = get_number_atof(str, pos_i + 1, end - 1);

    if (pos_i >= 1 && str[pos_i - 1] == '-')
        str_img *= -1.0;

    Complex temp(str_real, str_img);
    return (*this) + temp;
}
Complex Complex::operator-(const Complex &c) const
{
    Complex temp(real - c.real, img - c.img);
    return temp;
}
Complex Complex::operator-(const char *str) const
{
    Complex temp(str);     // 일시적으로 Complex 인스턴스 생성으로 문자열 str 저장
    return (*this) - temp; // 인스턴스 자신에게서 인자로 받아온 temp를 뺄셈 연산으로 제거
}
Complex Complex::operator*(const Complex &c) const
{
    Complex temp(real * c.real - img * c.img, real * c.img + img * c.real);
    return temp;
}
Complex Complex::operator*(const char *str) const
{
    Complex temp(str);
    return (*this) * temp;
}
Complex Complex::operator/(const Complex &c) const
{
    Complex temp(
        (real * c.real + img * c.img) / (c.real * c.real + c.img * c.img),
        (img * c.real - real * c.img) / (c.real * c.real + c.img * c.img));
    return temp;
}
Complex Complex::operator/(const char *str) const
{
    Complex temp(str);
    return (*this) / temp;
}
Complex &Complex::operator=(const Complex &c)
{
    real = c.real;
    img = c.img;
    return *this; // 객체 자신의 레퍼런스를 리턴한다.
}
Complex &Complex::operator+=(const Complex &c)
{
    (*this) = (*this) + c;
    return (*this);
}
Complex &Complex::operator-=(const Complex &c)
{
    (*this) = (*this) - c;
    return (*this);
}
Complex &Complex::operator*=(const Complex &c)
{
    (*this) = (*this) * c;
    return (*this);
}
Complex &Complex::operator/=(const Complex &c)
{
    (*this) = (*this) / c;
    return (*this);
}

int main()
{
    Complex a(0, 0);
    a = a + "-1.1+i3.923";
    a.println();
    a = a - "1.2-i1.823";
    a.println();
    a = a * "2.3+i22";
    a.println();
    a = a / "-12+i55";
    a.println();
}