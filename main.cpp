#include <iostream>
#include <stack>
#include <string>

using namespace std;

struct Priority											// содержит приоритет val и знака оператора sign
{
	char sign = ' ';
	int val = 0;
};

// объявление функций
void token(string, Priority);									// распознавание чисел 	и операторов	
void parse(char&, stack <char>&, stack <int>&, Priority);					// определение порядка операторов в стеке
void sort(stack <char>&, stack <int>&);								// сортировка данных в стеках, замещение на новые
int op_priority(char&, Priority);								// определение приоритета в зависимости от оператора
int calc(char&, int, int);									// вычисление перетасовочных значений

int op_priority(char& op, Priority pr)
{
	pr.sign = op;
	if ((op == '+') || (op == '-')) { pr.val = 1; }
	else if ((op == '*') || (op == '/')) { pr.val = 2; }
	else { pr.val = 0; }									// т.е. "(" и ")" тоже имеют 0 приоритет
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
		if (a == 0) { cout << "ERROR::Can't divide by zero!" << endl; exit(1); }
		else { return b / a; }
		break;
	case '+':
		return b + a;
		break;
	case '-':
		return b - a;
		break;
	case '.':
		cout << "ERROR::Must be only INT numbers!" << endl;
		exit(1);;
	default:
		cout << "ERROR::Unknown symbol!" << endl;
		exit(1);;
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
	if (numbers.empty())									// избежание случаев взятия данных из пустого стека
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
		if (op != '(')									// "(" сразу направляется в стек
		{
			if (op_priority(op, pr) <= op_priority(operations.top(), pr))
			{
				if (op == ')')
				{
					while (operations.top() != '(')				// выполнение выражения в скобках до "("
					{
						sort(operations, numbers);
					}
					operations.pop();					// ")" замещается "(", поэтому "(" удаляется из стека
				}
				else
				{	
					sort(operations, numbers);
				}
			}
		}
	}
	if (op != ')')										// внесение оператора в стек  ( ")" - необходим только для вычислений )
	{
		operations.push(op);
	}
}

void token(string expr, Priority pr)		
{
	stack <int> numbers;									// стек чисел с командной строки
	stack <char> operations;								// стек операторов с командной строки

	int num(0);										// единица стека чисел
	char op = ' ';										// единица стека опеаторов
	bool flag(false);									// флаг для определения отрицательного числа (если это не вычитание)

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
	if (!numbers.empty()) { cout << "ERROR::Envaly entry!" << endl; exit(1); }
	else { cout << "Answer: " << res << endl; }
}

void done()
{
	Priority pr;
	string expr;										// вводимое с консоли выражение
	do
	{
		cout << "Enter the expression: ";
		getline(cin, expr);
		expr.erase(remove(expr.begin(), expr.end(), ' '), expr.end());		// удаление всех пробелов в вводе
		if (!expr.empty()) { token(expr, pr); }
	} while (!expr.empty());
}

int main()
{
	atexit(done);										// вызывается запрос на новое выражение после ошибки 
	done();

	return 0;
};
