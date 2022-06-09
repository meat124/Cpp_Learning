#ifndef CELL_TABLE_H
#define CELL_TABLE_H

#include<iostream>
#include<string>
#include<fstream>
#include "Cell_Table.h"

using namespace std;

namespace MyExcel
{
	class Table;
	class Cell // 큰 테이블에서 한 칸을 의미하는 객체
	{
	protected:
		int x, y; // 위치
		Table* table; // 어느 테이블 소속인지

	public:
		virtual string stringify() = 0; // 가상 함수로 해당 셀 값을 문자열로 변환
		virtual int to_numeric() = 0; // 가상 함수로 해당 셀 값을 정수 데이터로 변환

		Cell(int x, int y, Table* table);
	};
	Cell::Cell(int x, int y, Table* table) : x(x), y(y), table(table) {}

	// 문자열 정보를 보관하는 클래스
	class StringCell : public Cell
	{
		string data;

	public:
		string stringify();
		int to_numeric();

		StringCell(string data, int x, int y, Table* t);
	};
	StringCell::StringCell(string data, int x, int y, Table* t) : data(data), Cell(x, y, t) {}
	string StringCell::stringify() { return data; }
	int StringCell::to_numeric() { return 0; }

	// 정수 정보를 보관하는 클래스
	class NumberCell : public Cell
	{
		int data;

	public:
		string stringify();
		int to_numeric();

		NumberCell(int data, int x, int y, Table* t);
	};
	NumberCell::NumberCell(int data, int x, int y, Table* t) : data(data), Cell(x, y, t) {}
	string NumberCell::stringify() { return to_string(data); }
	int NumberCell::to_numeric() { return data; }

	// 시간 정보를 보관하는 클래스
	class DateCell : public Cell
	{
		time_t data;

	public:
		string stringify();
		int to_numeric();

		DateCell(string s, int x, int y, Table* t);
	};
	DateCell::DateCell(string s, int x, int y, Table* t) : Cell(x, y, t)
	{
		// 입력받는 형식은 항상 yyyy-mm-dd 꼴이라 가정한다.
		int year = atoi(s.c_str()); // 첫 번째 주소는 yyyy 까지만 들어가므로 주소값 설정할 필요 x
		int month = atoi(s.c_str() + 5); // 첫 번째 문자의 주소값에 5를 더하면 mm 의 첫 번째 문자로 이동
		int day = atoi(s.c_str() + 8); // 첫 번째 문자의 주소값에 8을 더하면 dd 의 첫 번째 문자로 이동

		tm timeinfo; // tm 클래스의 객체 timeinfo 선언, tm 클래스는 일월년 시분초 단위로 데이터를 보관하는 클래스이다.

		timeinfo.tm_year = year - 1900;
		timeinfo.tm_mon = month - 1;
		timeinfo.tm_mday = day;
		timeinfo.tm_hour = 0;
		timeinfo.tm_min = 0;
		timeinfo.tm_sec = 0;

		data = mktime(&timeinfo); // mktime 함수는 tm 구조체를 time_t 형식으로 변환해준다.
	}
	string DateCell::stringify()
	{
		char buf[50];
		tm temp;

		// temp 는 tm 구조체, data 는 time_t 타입이다.
		// localtime_s 함수는 time_t 타입의 변수를 tm 구조체로 변환하여 저장한다.
		// 즉 temp 에 time_t 타입의 변수인 data 가 tm 구조체로 변환되어 저장된다.
		localtime_s(&temp, &data);

		// strftime 함수는 시간 타입을 스트링으로 변환하여 저장
		// 즉 tm 구조체 타입인 temp 를 buf 배열에 문자열로 저장
		strftime(buf, 50, "%F", &temp);

		return string(buf); // 문자열을 리턴
	}
	int DateCell::to_numeric() { return static_cast<int>(data); }

	class Table
	{
	protected:
		// 행 및 열의 최대 크기
		int max_row_size, max_col_size;

		// 데이터를 보관하는 테이블
		// Cell* 을 보관하는 2차원 배열이라 생각
		Cell*** data_table;

	public:
		Table(int max_row_size, int max_col_size);

		~Table();

		// 새로운 셀을 row 행 col 열에 등록한다.
		void reg_cell(Cell* c, int row, int col);

		// 해당 셀의 정수값을 반환한다.
		// s : 셀 이름 ( Ex. A3, B6 과 같이)
		int to_numeric(const string& s);

		// 행 및 열 번호로 셀을 호출한다.
		int to_numeric(int row, int col);

		// 해당 셀의 문자열을 반환한다.
		string stringify(const string& s);
		string stringify(int row, int col);

		virtual string print_table() = 0;
	};
	Table::Table(int max_row_size, int max_col_size)
		: max_row_size(max_row_size), max_col_size(max_col_size)
	{
		data_table = new Cell * *[max_row_size]; // 이중 포인터로 행의 개수 만큼의 요소를 가리킨다.
		for (int i = 0; i < max_row_size; i++)
		{
			data_table[i] = new Cell * [max_col_size]; // 포인터로 열의 개수 만큼의 요소를 가리킨다.
			for (int j = 0; j < max_col_size; j++)
			{
				data_table[i][j] = NULL; // 데이터 테이블의 요소를 NULL로 초기화한다.
			}
		}
	}
	Table::~Table()
	{
		for (int i = 0; i < max_row_size; i++)
		{
			for (int j = 0; j < max_col_size; j++)
			{
				if (data_table[i][j]) // 만약 data_table 의 요소가 NULL 이 아니면 실행
					delete data_table[i][j]; // 메모리 해제
			}
		}
		for (int i = 0; i < max_row_size; i++)
			delete[] data_table[i]; // 행의 개수 만큼 data_table 해제
		delete[] data_table; // 전체 data_table 해제
	}
	void Table::reg_cell(Cell* c, int row, int col)
	{
		// 만약 행과 열이 최대값보다 하나라도 큰 것이 있다면 종료
		if (!(row < max_row_size && col < max_col_size))
			return;

		// 만약 등록하고자 하는 위치에 이미 다른 셀 객체가 등록되어 있다면
		// 해당 객체를 delete 해서 메모리 해제하고 다시 등록하면 된다.
		if (data_table[row][col]) // 만약 셀 객체가 등록되어 있으면 0이 아닌 값이 리턴된다. 
			delete data_table[row][col];
		// 이제 빈 상태
		data_table[row][col] = c; // 인자로 전달받은 c를 테이블에 등록
	}
	int Table::to_numeric(const string& s) // 셀 이름을 받아서 위치의 값을 리턴(A1, B2 이런 식으로 인자 받음)
	{
		// Cell 이름으로 받는다.
		int col = s[0] - 'A'; // 만약 B 행이라면 1을 저장, A행이라면 0을 저장

		// s.c_str() 으로 문자열 s의 첫번째 문자의 주소값을 반환하고 거기에 1을 더하여 두 번째 문자의 주소값을 얻는다.
		// 그 주소값은 문자열 형식의 숫자이므로, atoi 함수를 이용해서 int 타입으로 변환한다.
		// atoi() >> atoi(주소값) >> 주소값에 있는 문자를 int 타입으로 변환 >> 따라서 A1 같은 위치에서 숫자 반환 가능
		// 마지막으로 1을 빼주어서 실제 인덱스와 동일하게 만든다. 만약 A1 이라면 0 행 이므로
		int row = atoi(s.c_str() + 1) - 1;

		if (row < max_row_size && col < max_col_size) // 만약 행과 열이 모두 최대치보다 작다면 실행
		{
			if (data_table[row][col]) // 만약 인자로 받은 위치에 셀 객체가 등록되어 있다면 그냥 그 값을 리턴한다.
				return data_table[row][col]->to_numeric();
		}
		// 만약 인자로 받은 위치에 셀 객체가 등록되어 있지 않다면 0을 리턴한다.
		return 0;
	}
	int Table::to_numeric(int row, int col) // 셀의 위치를 행과 열로 받아서 위치의 값을 리턴하는 함수
	{
		// 행과 열이 최대값보다 작고, 인자로 받은 위치에 셀 객체가 등록되어 있는 경우 if문 실행
		if (row < max_row_size && col < max_col_size && data_table[row][col])
			return data_table[row][col]->to_numeric(); // 등록되어 있는 위치에 값을 반환
		// 셀 객체가 등록되어 있지 않다면 0을 반환
		return 0;
	}
	string Table::stringify(const string& s)
	{
		// Cell 이름으로 받는다.
		int col = s[0] - 'A'; // 문자열의 첫 부분인 문자에서 A을 빼면 A라면 0이 리턴되고, B라면 1이 리턴된다.

		// s.c_str() 는 s 문자열의 시작 주소값을 반환한다.
		// 시작 주소값에 1을 더하면 두 번째 문자의 주소값이 된다.
		// 그 주소값에 atoi 함수를 사용하면 그 주소에 위치한 문자열의 값이 int 타입으로 변환된다.
		// int 정수 값에 1을 빼주면 실제 인덱스 값이 완성된다.
		int row = atoi(s.c_str() + 1) - 1;

		if (row < max_row_size && col < max_col_size)
		{
			if (data_table[row][col])
				return data_table[row][col]->stringify();
		}
		return ""; // 문자열 string 을 리턴해야 하므로 아무것도 리턴하지 않는다는 문자열인 "" 를 리턴한다.
	}
	string Table::stringify(int row, int col)
	{
		// 만약 행과 열이 최대치보다 작다면, 그리고 테이블이 비어있지 않다면
		if (row < max_row_size && col < max_col_size && data_table[row][col])
			return data_table[row][col]->stringify();

		return ""; // 만약 차 있으면 아무것도 반환하지 않는다.
	}
	std::ostream& operator<<(std::ostream& o, Table& table)
	{
		// ostream 클래스의 << 연산자를 오버로딩하는 함수
		o << table.print_table(); // ostream 의 객체 레퍼런스인 o 의 내용을 리턴한다.
		return o;
	}

	class TxtTable : public Table
	{
		// 
		string repeat_char(int n, char c);

		// 숫자로 된 열 번호를 A,B, ... , Z,AA,AB 이런 순으로 매겨준다.
		string col_num_to_str(int n);

	public:
		TxtTable(int row, int col); // 인자로 행과 열을 받는 생성자

		// 텍스트로 표를 깨끗하게 출력
		string print_table(); // 상속받은 Table 클래스에 존재하는 함수를 오버라이딩
	};
	TxtTable::TxtTable(int row, int col) : Table(row, col) {}

	string TxtTable::print_table()
	{
		string total_table;

		// 열의 최대값 만큼의 int 타입의 배열을 동적 할당
		// 각각의 열의 길이를 저장하게 된다.
		int* col_max_wide = new int[max_col_size];

		for (int i = 0; i < max_col_size; i++)
		{
			unsigned int max_wide = 2;
			for (int j = 0; j < max_row_size; j++)
			{
				// data_table[i][j] 가 값이 존재하고
				// data_table[i][j] 에 존재하는 문자열을 stringify 로 문자열을 반환하고
				// 그 문자열의 길이를 length() 함수로 구한 뒤
				// 그 길이가 max_wide 값보다 크다면
				// 그 길이를 max_wide 에 저장
				if (data_table[i][j] && data_table[i][j]->stringify().length() > max_wide)
					max_wide = data_table[i][j]->stringify().length();
			}
			col_max_wide[i] = max_wide;
		}
		// 맨 상단에 열 정보 표시
		total_table += "    ";
		int total_wide = 4;
		for (int i = 0; i < max_col_size; i++)
		{
			if (col_max_wide[i]) // 만약 col_max_wide[i] 가 0이 아니라면 실행
			{
				// 각 열에서 가장 긴 문자열 만큼 길이가 할당 되어야 하므로 max_len 을 설정한다.
				int max_len = max(2, col_max_wide[i]); // max_len 을 2와 col_max_wide[i] 중에 큰 것으로 저장
				total_table += " | " + col_num_to_str(i); // i의 값을 A,B,C... 문자로 변환해서 열의 위치를 알려줌
				total_table += repeat_char(max_len - col_num_to_str(i).length(), ' ');

				total_wide += (max_len + 3);
			}
		}

		total_table += "\n";
		// 기본적으로 최대 9999 번째 행 까지 지원
		for (int i = 0; i < max_row_size; i++)
		{
			total_table += repeat_char(total_wide, '-'); // total_wide 값 만큼 - 를 반복해서 붙임
			total_table += "\n" + to_string(i + 1); // 개행을 한 뒤, i 인덱스 값에 1을 더해서 실제 행의 값을 표시
			total_table += repeat_char(4 - to_string(i + 1).length(), ' ');

			for (int j = 0; j < max_col_size; j++)
			{
				if (col_max_wide[j]) // 행의 최대 길이가 0이 아니라면 실행
				{
					int max_len = max(2, col_max_wide[j]);

					string s = "";
					if (data_table[i][j])
						s = data_table[i][j]->stringify(); // 각 셀의 값을 문자열로 리턴해서 string s 에 저장
					total_table += " | " + s; // | 로 구분한 뒤에 s 를 넣어서 셀 출력
					total_table += repeat_char(max_len - s.length(), ' ');
				}
			}
			total_table += "\n";
		}
		// 모든 셀은 한 줄로만 출력된다.
		return total_table;
	}
	string TxtTable::repeat_char(int n, char c)
	{
		string s = "";
		for (int i = 0; i < n; i++)
			s.push_back(c); // s 문자열에 char c를 n 번 만큼 덧붙임

		return s;
	}
	// 숫자로 된 열 번호를 A,B,C, ... , Z,AA,AB 이런 식으로 변환해준다.
	string TxtTable::col_num_to_str(int n)
	{
		string s = "";
		if (n < 26) // 만약 n 의 값이 알파벳의 개수보다 작다면
			s.push_back('A' + n); // 만약 n = 0 이면 A 리턴, n = 2 이면 C 리턴 ... n = 25 이면 Z 리턴
		else
		{
			char first = 'A' + n / 26 - 1;
			char second = 'A' + n % 26;

			s.push_back(first);
			s.push_back(second);
		}

		return s;
	}

	class HtmlTable : public Table
	{
	public:
		HtmlTable(int row, int col) : Table(row, col) {}

		string print_table()
		{
			string s = "<table border = '1' cellpadding = '10'>";
			for (int i = 0; i < max_row_size; i++)
			{
				s += "<tr>";
				for (int j = 0; j < max_col_size; j++)
				{
					s += "<td>";
					if (data_table[i][j])
						s += data_table[i][j]->stringify();
					s += "</td>";
				}
				s += "</tr>";
			}
			s += "</table>";
			return s;
		}
	};

	class CSVTable : public Table
	{
	public:
		CSVTable(int row, int col) : Table(row, col) {}

		string print_table()
		{
			string s = "";
			for (int i = 0; i < max_row_size; i++)
			{
				for (int j = 0; j < max_col_size; j++)
				{
					if (j >= 1)
						s += ",";

					string temp;
					if (data_table[i][j])
						temp = data_table[i][j]->stringify();

					for (int k = 0; k < temp.length(); k++)
					{
						if (temp[k] == '"')
						{
							temp.insert(k, 1, '"');

							k++;
						}
					}
					temp = '"' + temp + '"';
					s += temp;
				}
				s += '\n';
			}
			return s;
		}
	};
}
#endif // !CELL_TABLE_H
