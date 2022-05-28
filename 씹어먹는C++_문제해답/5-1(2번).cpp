#include <iostream>

class Complex
{
private:
    double real, img; // �Ǽ�, ��� �κ�

    double get_number(const char *str, int from, int to) const;

    // atof �Լ��� �̿��Ͽ� get_number �Լ��� �����Ѵ�.
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
    // �Է� ���� ���ڿ��� �м��Ͽ� real �κа� img �κ��� ã�´�.
    // ���ڿ��� ���� ������ ����. "[��ȣ] (�Ǽ���) (��ȣ) i (�����)"
    // �� �� �� ���� ��ȣ�� ���� �����ϴ�. (������ + �� ����)

    int begin = 0, end = strlen(str);
    img = 0.0;
    real = 0.0;

    // ���� ���� ������ �Ǵ� i�� ��ġ�� ã�´�.
    int pos_i = -1; // -1�� �ʱ�ȭ ���Ѽ� ó������ ���� ������ ����Ű�� ���ϰ� ���´�.
    for (int i = 0; i != end; i++)
    {
        if (str[i] == 'i')
        {
            pos_i = i; // i�� �ε����� ���� �ʱ�ȭ ��Ų��.
            break;
        }
    }

    // ���� i �� ���ٸ� �� ���Ҽ��� �Ǽ��̴�.
    if (pos_i == -1)
    {
        real = get_number_atof(str, begin, end - 1);
        return;
    }

    // ���� i �� �ִٸ� �Ǽ��ο� ����θ� ����� ó���ϸ� �ȴ�.
    real = get_number_atof(str, begin, pos_i - 1);
    img = get_number_atof(str, pos_i + 1, end - 1); // pos_i�� ��ġ�� i�̹Ƿ� �װź��� �ϳ� �ø� �� ���� �����Ѵ�.

    if (pos_i >= 1 && str[pos_i - 1] == '-')
        img *= -1.0; // ����κ��� �����ϰ� i�� ���� ��ȣ�� -��� -1�� ���ؼ� �����Ѵ�.
}
double Complex::get_number(const char *str, int from, int to) const // ������ get_number �Լ�
{
    bool minus = false; // �⺻������ ������ ����
    if (from > to)      // �������� ���������� ũ�ٸ� 0�� ��ȯ�Ѵ�.
        return 0;
    if (str[from] == '-') // �������� ���ڰ� - ��� �����̹Ƿ� minus�� true�� �ٲ۴�.
        minus = true;
    if (str[from] == '-' || str[from] == '+') // �������� - �ų� +�̸� ��ȣ�� ���� �Ǵ��� �ʿ� �����Ƿ� ��ĭ �δ�.
        from++;

    double num = 0.0;
    double decimal = 1.0;

    bool integer_part = true;        // �⺻������ ������� ����
    for (int i = from; i <= to; i++) // from ����  to ���� �˻�
    {
        // isdigit �Լ��� �������� �ƴ��� �Ǵ�(������ 1, �ƴϸ� 0 ��ȯ)
        if (isdigit(str[i]) && integer_part) // ���� �����̰�, ������� ����
        {
            num *= 10.0;           // �ڸ��� �ø���
            num += (str[i] - '0'); // ���� �ڸ��� ���� str[i] - '0'
        }
        else if (str[i] == '.') // ���� �Ҽ���� ���� true �� false�� ����
            integer_part = false;
        else if (isdigit(str[i]) && !integer_part) // ���� �Ҽ��̰� str[i] �� �������
        {
            decimal /= 10.0;                   // �ڸ��� ���� �ϳ� ������
            num += ((str[i] - '0') * decimal); // ������ decimal�� ���ؼ� ���Ѵ�.
        }
        else
            break;
    }

    if (minus) // minus �� true�̸� -1 �� ���ؼ� ������ ��ȯ�Ѵ�.
        num *= -1.0;

    return num;
}
double Complex::get_number_atof(const char *str, int from, int to) const // atof �� �̿��ؼ� ������ get_number �Լ�
{
    if (from > to)
        return 0; // �������� ���������� ũ�ٸ� 0�� ��ȯ�Ѵ�.

    char temp[50] = {0};
    strncpy(temp, str + from, to - from + 1); // �ι�° ������ ��� ���ڿ��� char* �����̹Ƿ� �ε����� ǥ���ϸ� �ȵǰ� from�� ���� ���·� ǥ���Ѵ�.

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
    // �Է¹��� ���ڿ��� �м��Ͽ� real �κа� img �κ��� ã�´�.
    // ���ڿ��� ���� ������ ����. "[��ȣ] (�Ǽ���)(��ȣ)i(�����)"
    // �� �� �� ���� ��ȣ�� ���� �����ϴ�. (������ +�� ����)

    int begin = 0, end = strlen(str);
    double str_real = 0.0, str_img = 0.0;

    // ���� ���� ���� �Ǽ��ο� ����θ� �����ϴ� ������ �Ǵ� i�� ���� ã�´�.
    int pos_i = -1;
    for (int i = 0; i != end; i++)
    {
        if (str[i] == 'i')
        {
            pos_i = i;
            break;
        }
    }

    // ���� i�� �������� ������ �� ���� �Ǽ��� �����ϴ� ���Ҽ��̴�.
    if (pos_i == -1)
    {
        str_real = get_number_atof(str, begin, end - 1);

        Complex temp(str_real, str_img);
        return (*this) + temp;
    }

    // ���� i�� �����ϸ�, �Ǽ��ο� ����θ� ����� ó���ϸ� �ȴ�.
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
    Complex temp(str);     // �Ͻ������� Complex �ν��Ͻ� �������� ���ڿ� str ����
    return (*this) - temp; // �ν��Ͻ� �ڽſ��Լ� ���ڷ� �޾ƿ� temp�� ���� �������� ����
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
    return *this; // ��ü �ڽ��� ���۷����� �����Ѵ�.
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