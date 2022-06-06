#include<iostream>
#include<string>

using namespace std;

class Table;
class Cell // 큰 테이블에서 한 칸을 의미하는 객체
{
protected:
	int x, y; // 위치
	Table* table; // 어느 테이블 소속인지

	string data; // 문자열 데이터

public:
	virtual string stringify(); // 가상 함수로 해당 셀 값을 문자열로 변환
	virtual int to_numeric(); // 가상 함수로 해당 셀 값을 정수 데이터로 변환

	Cell(string data, int x, int y, Table* table);
};
Cell::Cell(string data, int x, int y, Table* table)
	: data(data), x(x), y(y), table(table) {}

string Cell::stringify() { return data; }
int Cell::to_numeric() { return 0; }

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
	if (!(row < max_row_size && col < max_col_size))
		return;

	// 만약 등록하고자 하는 위치에 이미 다른 셀 객체가 등록되어 있다면
	// 해당 객체를 delete 해서 메모리 해제하고 다시 등록하면 된다.
	if (data_table[row][col])
		delete data_table[row][col];

	data_table[row][col] = c; // 인자로 전달받은 c를 테이블에 등록
}
int Table::to_numeric(const string& s) // 셀 이름을 받아서 위치의 값을 리턴(A1, B2 이런 식으로 인자 받음)
{
	// Cell 이름으로 받는다.
	int col = s[0] - 'A'; // 만약 B 행이라면 1을 저장, A행이라면 0을 저장

	// s.c_str() 으로 문자열 s의 첫번째 문자의 주소값을 반환하고 거기에 1을 더하여 두 번째 문자의 주소값을 얻는다.
	// 그 주소값은 문자열 형식의 숫자이므로, atoi 함수를 이용해서 int 타입으로 변환한다.
	// 마지막으로 1을 빼주어서 실제 인덱스와 동일하게 만든다.
	int row = atoi(s.c_str() + 1) - 1; 

	if (row < max_row_size && col < max_col_size) // 만약 행과 열이 모두 최대치보다 작다면 실행
	{
		if (data_table[row][col]) // 만약 등록하고자 하는 위치에 셀 객체가 등록되어 있다면 그냥 그 값을 리턴한다.
			return data_table[row][col]->to_numeric();
	}
	return 0;
}
int Table::to_numeric(int row, int col)
{
	if (row < max_row_size && col < max_col_size && data_table[row][col])
		return data_table[row][col]->to_numeric();
	return 0;
}
string Table::stringify(const string& s)
{
	// Cell 이름으로 받는다.
	int col = s[0] - 'A';
	int row = atoi(s.c_str() + 1) - 1;

	if (row < max_row_size && col < max_col_size)
	{
		if (data_table[row][col])
			return data_table[row][col]->stringify();
	}
	return 0;
}
string Table::stringify(int row, int col)
{
	// 만약 행과 열이 최대치보다 작다면, 그리고 테이블이 비어있지 않다면
	if (row < max_row_size && col < max_col_size && data_table[row][col])
		return data_table[row][col]->stringify();

	return ""; // 만약 차 있으면 아무것도 반환하지 않는다.
}