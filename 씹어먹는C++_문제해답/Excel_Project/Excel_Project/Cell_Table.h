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
	class Cell // ū ���̺��� �� ĭ�� �ǹ��ϴ� ��ü
	{
	protected:
		int x, y; // ��ġ
		Table* table; // ��� ���̺� �Ҽ�����

	public:
		virtual string stringify() = 0; // ���� �Լ��� �ش� �� ���� ���ڿ��� ��ȯ
		virtual int to_numeric() = 0; // ���� �Լ��� �ش� �� ���� ���� �����ͷ� ��ȯ

		Cell(int x, int y, Table* table);
	};
	Cell::Cell(int x, int y, Table* table) : x(x), y(y), table(table) {}

	// ���ڿ� ������ �����ϴ� Ŭ����
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

	// ���� ������ �����ϴ� Ŭ����
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

	// �ð� ������ �����ϴ� Ŭ����
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
		// �Է¹޴� ������ �׻� yyyy-mm-dd ���̶� �����Ѵ�.
		int year = atoi(s.c_str()); // ù ��° �ּҴ� yyyy ������ ���Ƿ� �ּҰ� ������ �ʿ� x
		int month = atoi(s.c_str() + 5); // ù ��° ������ �ּҰ��� 5�� ���ϸ� mm �� ù ��° ���ڷ� �̵�
		int day = atoi(s.c_str() + 8); // ù ��° ������ �ּҰ��� 8�� ���ϸ� dd �� ù ��° ���ڷ� �̵�

		tm timeinfo; // tm Ŭ������ ��ü timeinfo ����, tm Ŭ������ �Ͽ��� �ú��� ������ �����͸� �����ϴ� Ŭ�����̴�.

		timeinfo.tm_year = year - 1900;
		timeinfo.tm_mon = month - 1;
		timeinfo.tm_mday = day;
		timeinfo.tm_hour = 0;
		timeinfo.tm_min = 0;
		timeinfo.tm_sec = 0;

		data = mktime(&timeinfo); // mktime �Լ��� tm ����ü�� time_t �������� ��ȯ���ش�.
	}
	string DateCell::stringify()
	{
		char buf[50];
		tm temp;

		// temp �� tm ����ü, data �� time_t Ÿ���̴�.
		// localtime_s �Լ��� time_t Ÿ���� ������ tm ����ü�� ��ȯ�Ͽ� �����Ѵ�.
		// �� temp �� time_t Ÿ���� ������ data �� tm ����ü�� ��ȯ�Ǿ� ����ȴ�.
		localtime_s(&temp, &data);

		// strftime �Լ��� �ð� Ÿ���� ��Ʈ������ ��ȯ�Ͽ� ����
		// �� tm ����ü Ÿ���� temp �� buf �迭�� ���ڿ��� ����
		strftime(buf, 50, "%F", &temp);

		return string(buf); // ���ڿ��� ����
	}
	int DateCell::to_numeric() { return static_cast<int>(data); }

	class Table
	{
	protected:
		// �� �� ���� �ִ� ũ��
		int max_row_size, max_col_size;

		// �����͸� �����ϴ� ���̺�
		// Cell* �� �����ϴ� 2���� �迭�̶� ����
		Cell*** data_table;

	public:
		Table(int max_row_size, int max_col_size);

		~Table();

		// ���ο� ���� row �� col ���� ����Ѵ�.
		void reg_cell(Cell* c, int row, int col);

		// �ش� ���� �������� ��ȯ�Ѵ�.
		// s : �� �̸� ( Ex. A3, B6 �� ����)
		int to_numeric(const string& s);

		// �� �� �� ��ȣ�� ���� ȣ���Ѵ�.
		int to_numeric(int row, int col);

		// �ش� ���� ���ڿ��� ��ȯ�Ѵ�.
		string stringify(const string& s);
		string stringify(int row, int col);

		virtual string print_table() = 0;
	};
	Table::Table(int max_row_size, int max_col_size)
		: max_row_size(max_row_size), max_col_size(max_col_size)
	{
		data_table = new Cell * *[max_row_size]; // ���� �����ͷ� ���� ���� ��ŭ�� ��Ҹ� ����Ų��.
		for (int i = 0; i < max_row_size; i++)
		{
			data_table[i] = new Cell * [max_col_size]; // �����ͷ� ���� ���� ��ŭ�� ��Ҹ� ����Ų��.
			for (int j = 0; j < max_col_size; j++)
			{
				data_table[i][j] = NULL; // ������ ���̺��� ��Ҹ� NULL�� �ʱ�ȭ�Ѵ�.
			}
		}
	}
	Table::~Table()
	{
		for (int i = 0; i < max_row_size; i++)
		{
			for (int j = 0; j < max_col_size; j++)
			{
				if (data_table[i][j]) // ���� data_table �� ��Ұ� NULL �� �ƴϸ� ����
					delete data_table[i][j]; // �޸� ����
			}
		}
		for (int i = 0; i < max_row_size; i++)
			delete[] data_table[i]; // ���� ���� ��ŭ data_table ����
		delete[] data_table; // ��ü data_table ����
	}
	void Table::reg_cell(Cell* c, int row, int col)
	{
		// ���� ��� ���� �ִ밪���� �ϳ��� ū ���� �ִٸ� ����
		if (!(row < max_row_size && col < max_col_size))
			return;

		// ���� ����ϰ��� �ϴ� ��ġ�� �̹� �ٸ� �� ��ü�� ��ϵǾ� �ִٸ�
		// �ش� ��ü�� delete �ؼ� �޸� �����ϰ� �ٽ� ����ϸ� �ȴ�.
		if (data_table[row][col]) // ���� �� ��ü�� ��ϵǾ� ������ 0�� �ƴ� ���� ���ϵȴ�. 
			delete data_table[row][col];
		// ���� �� ����
		data_table[row][col] = c; // ���ڷ� ���޹��� c�� ���̺� ���
	}
	int Table::to_numeric(const string& s) // �� �̸��� �޾Ƽ� ��ġ�� ���� ����(A1, B2 �̷� ������ ���� ����)
	{
		// Cell �̸����� �޴´�.
		int col = s[0] - 'A'; // ���� B ���̶�� 1�� ����, A���̶�� 0�� ����

		// s.c_str() ���� ���ڿ� s�� ù��° ������ �ּҰ��� ��ȯ�ϰ� �ű⿡ 1�� ���Ͽ� �� ��° ������ �ּҰ��� ��´�.
		// �� �ּҰ��� ���ڿ� ������ �����̹Ƿ�, atoi �Լ��� �̿��ؼ� int Ÿ������ ��ȯ�Ѵ�.
		// atoi() >> atoi(�ּҰ�) >> �ּҰ��� �ִ� ���ڸ� int Ÿ������ ��ȯ >> ���� A1 ���� ��ġ���� ���� ��ȯ ����
		// ���������� 1�� ���־ ���� �ε����� �����ϰ� �����. ���� A1 �̶�� 0 �� �̹Ƿ�
		int row = atoi(s.c_str() + 1) - 1;

		if (row < max_row_size && col < max_col_size) // ���� ��� ���� ��� �ִ�ġ���� �۴ٸ� ����
		{
			if (data_table[row][col]) // ���� ���ڷ� ���� ��ġ�� �� ��ü�� ��ϵǾ� �ִٸ� �׳� �� ���� �����Ѵ�.
				return data_table[row][col]->to_numeric();
		}
		// ���� ���ڷ� ���� ��ġ�� �� ��ü�� ��ϵǾ� ���� �ʴٸ� 0�� �����Ѵ�.
		return 0;
	}
	int Table::to_numeric(int row, int col) // ���� ��ġ�� ��� ���� �޾Ƽ� ��ġ�� ���� �����ϴ� �Լ�
	{
		// ��� ���� �ִ밪���� �۰�, ���ڷ� ���� ��ġ�� �� ��ü�� ��ϵǾ� �ִ� ��� if�� ����
		if (row < max_row_size && col < max_col_size && data_table[row][col])
			return data_table[row][col]->to_numeric(); // ��ϵǾ� �ִ� ��ġ�� ���� ��ȯ
		// �� ��ü�� ��ϵǾ� ���� �ʴٸ� 0�� ��ȯ
		return 0;
	}
	string Table::stringify(const string& s)
	{
		// Cell �̸����� �޴´�.
		int col = s[0] - 'A'; // ���ڿ��� ù �κ��� ���ڿ��� A�� ���� A��� 0�� ���ϵǰ�, B��� 1�� ���ϵȴ�.

		// s.c_str() �� s ���ڿ��� ���� �ּҰ��� ��ȯ�Ѵ�.
		// ���� �ּҰ��� 1�� ���ϸ� �� ��° ������ �ּҰ��� �ȴ�.
		// �� �ּҰ��� atoi �Լ��� ����ϸ� �� �ּҿ� ��ġ�� ���ڿ��� ���� int Ÿ������ ��ȯ�ȴ�.
		// int ���� ���� 1�� ���ָ� ���� �ε��� ���� �ϼ��ȴ�.
		int row = atoi(s.c_str() + 1) - 1;

		if (row < max_row_size && col < max_col_size)
		{
			if (data_table[row][col])
				return data_table[row][col]->stringify();
		}
		return ""; // ���ڿ� string �� �����ؾ� �ϹǷ� �ƹ��͵� �������� �ʴ´ٴ� ���ڿ��� "" �� �����Ѵ�.
	}
	string Table::stringify(int row, int col)
	{
		// ���� ��� ���� �ִ�ġ���� �۴ٸ�, �׸��� ���̺��� ������� �ʴٸ�
		if (row < max_row_size && col < max_col_size && data_table[row][col])
			return data_table[row][col]->stringify();

		return ""; // ���� �� ������ �ƹ��͵� ��ȯ���� �ʴ´�.
	}
	std::ostream& operator<<(std::ostream& o, Table& table)
	{
		// ostream Ŭ������ << �����ڸ� �����ε��ϴ� �Լ�
		o << table.print_table(); // ostream �� ��ü ���۷����� o �� ������ �����Ѵ�.
		return o;
	}

	class TxtTable : public Table
	{
		// 
		string repeat_char(int n, char c);

		// ���ڷ� �� �� ��ȣ�� A,B, ... , Z,AA,AB �̷� ������ �Ű��ش�.
		string col_num_to_str(int n);

	public:
		TxtTable(int row, int col); // ���ڷ� ��� ���� �޴� ������

		// �ؽ�Ʈ�� ǥ�� �����ϰ� ���
		string print_table(); // ��ӹ��� Table Ŭ������ �����ϴ� �Լ��� �������̵�
	};
	TxtTable::TxtTable(int row, int col) : Table(row, col) {}

	string TxtTable::print_table()
	{
		string total_table;

		// ���� �ִ밪 ��ŭ�� int Ÿ���� �迭�� ���� �Ҵ�
		// ������ ���� ���̸� �����ϰ� �ȴ�.
		int* col_max_wide = new int[max_col_size];

		for (int i = 0; i < max_col_size; i++)
		{
			unsigned int max_wide = 2;
			for (int j = 0; j < max_row_size; j++)
			{
				// data_table[i][j] �� ���� �����ϰ�
				// data_table[i][j] �� �����ϴ� ���ڿ��� stringify �� ���ڿ��� ��ȯ�ϰ�
				// �� ���ڿ��� ���̸� length() �Լ��� ���� ��
				// �� ���̰� max_wide ������ ũ�ٸ�
				// �� ���̸� max_wide �� ����
				if (data_table[i][j] && data_table[i][j]->stringify().length() > max_wide)
					max_wide = data_table[i][j]->stringify().length();
			}
			col_max_wide[i] = max_wide;
		}
		// �� ��ܿ� �� ���� ǥ��
		total_table += "    ";
		int total_wide = 4;
		for (int i = 0; i < max_col_size; i++)
		{
			if (col_max_wide[i]) // ���� col_max_wide[i] �� 0�� �ƴ϶�� ����
			{
				// �� ������ ���� �� ���ڿ� ��ŭ ���̰� �Ҵ� �Ǿ�� �ϹǷ� max_len �� �����Ѵ�.
				int max_len = max(2, col_max_wide[i]); // max_len �� 2�� col_max_wide[i] �߿� ū ������ ����
				total_table += " | " + col_num_to_str(i); // i�� ���� A,B,C... ���ڷ� ��ȯ�ؼ� ���� ��ġ�� �˷���
				total_table += repeat_char(max_len - col_num_to_str(i).length(), ' ');

				total_wide += (max_len + 3);
			}
		}

		total_table += "\n";
		// �⺻������ �ִ� 9999 ��° �� ���� ����
		for (int i = 0; i < max_row_size; i++)
		{
			total_table += repeat_char(total_wide, '-'); // total_wide �� ��ŭ - �� �ݺ��ؼ� ����
			total_table += "\n" + to_string(i + 1); // ������ �� ��, i �ε��� ���� 1�� ���ؼ� ���� ���� ���� ǥ��
			total_table += repeat_char(4 - to_string(i + 1).length(), ' ');

			for (int j = 0; j < max_col_size; j++)
			{
				if (col_max_wide[j]) // ���� �ִ� ���̰� 0�� �ƴ϶�� ����
				{
					int max_len = max(2, col_max_wide[j]);

					string s = "";
					if (data_table[i][j])
						s = data_table[i][j]->stringify(); // �� ���� ���� ���ڿ��� �����ؼ� string s �� ����
					total_table += " | " + s; // | �� ������ �ڿ� s �� �־ �� ���
					total_table += repeat_char(max_len - s.length(), ' ');
				}
			}
			total_table += "\n";
		}
		// ��� ���� �� �ٷθ� ��µȴ�.
		return total_table;
	}
	string TxtTable::repeat_char(int n, char c)
	{
		string s = "";
		for (int i = 0; i < n; i++)
			s.push_back(c); // s ���ڿ��� char c�� n �� ��ŭ ������

		return s;
	}
	// ���ڷ� �� �� ��ȣ�� A,B,C, ... , Z,AA,AB �̷� ������ ��ȯ���ش�.
	string TxtTable::col_num_to_str(int n)
	{
		string s = "";
		if (n < 26) // ���� n �� ���� ���ĺ��� �������� �۴ٸ�
			s.push_back('A' + n); // ���� n = 0 �̸� A ����, n = 2 �̸� C ���� ... n = 25 �̸� Z ����
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
