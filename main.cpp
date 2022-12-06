#include <iostream>
#include <stack>
#include <string>

using namespace std;

struct Priority												// �������� ��������� val � ����� ��������� sign
{
	char sign = ' ';
	int val = 0;
};

// ���������� �������
void token(string, Priority);								// ������������� ����� 	� ����������	
void parse(char&, stack <char>&, stack <int>&, Priority);	// ����������� ������� ���������� � �����
void sort(stack <char>&, stack <int>&);						// ���������� ������ � ������, ��������� �� �����
int op_priority(char&, Priority);							// ����������� ���������� � ����������� �� ���������
int calc(char&, int, int);									// ���������� �������������� ��������

int op_priority(char& op, Priority pr)
{
	pr.sign = op;
	if ((op == '+') || (op == '-')) { pr.val = 1; }
	else if ((op == '*') || (op == '/')) { pr.val = 2; }
	else { pr.val = 0; }									// �.�. "(" � ")" ���� ����� ������� ���������
	return pr.val;
}

int calc(char& op, int a, int b)
{
	switch (op)
	{
	case '*':
		return b * a;
		break;
	case '/':
		return b / a;
		break;
	case '+':
		return b + a;
		break;
	case '-':
		return b - a;
		break;
	case '.':
		cout << "ERROR::Must be only INT numbers!" << endl;
		return 1;
	default:
		cout << "ERROR::Unknown symbol!" << endl;
		return 1;
	}
}

void sort(stack <char>& operations, stack <int>& numbers)
{
	int a(0), b(0), res(0);
	char c = ' ';
	a = numbers.top();
	numbers.pop();
	c = operations.top();
	operations.pop();
	if (numbers.empty())									// ��������� ������� ������ ������ �� ������� �����
	{
		res = calc(c, a, 0);
		//cout << '0' << c << a << "=" << res << endl;
		numbers.push(res);
	}
	else
	{
		b = numbers.top();
		numbers.pop();
		res = calc(c, a, b);
		//cout << b << c << a << "=" << res << endl;
		numbers.push(res);
	}
}

void parse(char& op, stack <char>& operations, stack <int>& numbers, Priority pr)
{
	if (!operations.empty())								
	{
		if (op != '(')										// "(" ����� ������������ � ����
		{
			if (op_priority(op, pr) <= op_priority(operations.top(), pr))
			{
				if (op == ')')
				{
					while (operations.top() != '(')			// ���������� ��������� � ������� �� "("
					{
						sort(operations, numbers);
					}
					operations.pop();						// ")" ���������� "(", ������� "(" ��������� �� �����
				}
				else
				{	
					sort(operations, numbers);
				}
			}
		}
	}
	if (op != ')')											// �������� ��������� � ����  ( ")" - ��������� ������ ��� ���������� )
	{
		operations.push(op);
	}
}

void token(string expr, Priority pr)		
{
	stack <int> numbers;									// ���� ����� � ��������� ������
	stack <char> operations;								// ���� ���������� � ��������� ������

	int num(0);												// ������� ����� �����
	char op = ' ';											// ������� ����� ���������
	bool flag(false);										// ���� ��� ����������� �������������� ����� (���� ��� �� ���������)

	int a(0), b(0), res(0);
	char c = ' ';

	for (size_t i = 0; i < expr.length(); i++)
	{
		if (isdigit(expr[i]))
		{
			num = num * 10 + expr[i] - '0';
			if (!isdigit(expr[i + 1]))
			{
				numbers.push(num);
				if ((flag == true) && (operations.top() != '('))
				{
					a = numbers.top();
					numbers.pop();
					c = operations.top();
					operations.pop();
					res = calc(c, a, 0);
					//cout << '0' << c << a << "=" << res << endl;
					numbers.push(res);
				}
				flag = false;
			}
		}
		else
		{
			num = 0;
			op = expr[i];
			if (op == '(') { flag = true; }
			parse(op, operations, numbers, pr);
		}
	}
	while (!operations.empty())
	{
		sort(operations, numbers);
	}
	
	res = numbers.top();
	numbers.pop();
	if (!numbers.empty()) { cout << "ERROR::Envaly entry!" << endl; }
	else { cout << "Answer: " << res << endl; }
}

int main()
{
	Priority pr;
	string expr;											// �������� � ������� ���������
	do
	{
		cout << "Enter the expression: ";
		getline(cin, expr);
		expr.erase(std::remove(expr.begin(), expr.end(), ' '), expr.end());		// �������� ���� �������� � �����
		token(expr, pr);
	} while (!expr.empty());

	return 0;
};