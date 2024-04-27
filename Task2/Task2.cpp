#include <iostream>
#include <cassert>
#include <string> 
using namespace std;

struct Transformer;
struct Number;
struct BinaryOperation;
struct FunctionCall;
struct Variable;

struct Expression //базовая абстрактная структура
{
	virtual ~Expression() { } //виртуальный деструктор
	virtual double evaluate() const = 0; //абстрактный метод «вычислить»
	virtual Expression* transform(Transformer* tr) const = 0;
	virtual string print() const = 0;//абстрактный метод печать
};

struct Transformer //pattern Visitor
{
	virtual ~Transformer() { }
	virtual Expression* transformNumber(Number const*) = 0;
	virtual Expression* transformBinaryOperation(BinaryOperation const*) = 0;
	virtual Expression* transformFunctionCall(FunctionCall const*) = 0;
	virtual Expression* transformVariable(Variable const*) = 0;
};

struct Number : Expression // стуктура «Число»
{
	Number(double value) : value_(value) {} //конструктор
	double value() const { return value_; }; // метод чтения значения числа
	double evaluate() const { return value_; } // реализация виртуального метода «вычислить»
	~Number() {}//деструктор, тоже виртуальный
	string print() const { return to_string(this->value_); }
	Expression* transform(Transformer* tr) const { return tr->transformNumber(this); }
private:
	double value_; // само вещественное число
};

struct BinaryOperation : Expression // «Бинарная операция»
{
	enum { // перечислим константы, которыми зашифруем символы операций 
		PLUS = '+',
		MINUS = '-',
		DIV = '/',
		MUL = '*'
	};
	// в конструкторе надо указать 2 операнда — левый и правый, а также сам символ операции
	BinaryOperation(Expression const* left, int op, Expression const* right) : left_(left), op_(op), right_(right)
	{
		assert(left_ && right_);
	}
	~BinaryOperation() //в деструкторе освободим занятую память
	{
		delete left_; delete right_;
	}
	Expression const* left() const { return left_; } // чтение левого операнда
	Expression const* right() const { return right_; } // чтение правого операнда
	int operation() const { return op_; } // чтение символа операции
	double evaluate() const // реализация виртуального метода «вычислить»
	{
		double left = left_->evaluate(); // вычисляем левую часть
		double right = right_->evaluate(); // вычисляем правую часть
		switch (op_) // в зависимости от вида операции, складываем, вычитаем, умножаем или делим левую и правую части
		{
		case PLUS: return left + right;
		case MINUS: return left - right;
		case DIV: return left / right;
		case MUL: return left * right;
		}
	}
	Expression* transform(Transformer* tr) const
	{
		return tr->transformBinaryOperation(this);
	}
	std::string print() const {
		return this->left_->print() + std::string(1, this->op_) + this->right_->print();
	}
private:
	Expression const* left_; // указатель на левый операнд
	Expression const* right_; // указатель на правый операнд
	int op_; // символ операции
};

struct FunctionCall : Expression // структура «Вызов функции»
{
	// в конструкторе надо учесть имя функции и ее аргумент
	FunctionCall(std::string const& name, Expression const* arg) : name_(name), arg_(arg)
	{
		assert(arg_);
		assert(name_ == "sqrt" || name_ == "abs"); // разрешены только вызов sqrt и abs
	}
	std::string const& name() const
	{
		return name_;
	}
	Expression const* arg() const // чтение аргумента функции
	{
		return arg_;
	}
	~FunctionCall() { delete arg_; } // освобождаем память в деструкторе
	virtual double evaluate() const { // реализация виртуального метода «вычислить»
		if (name_ == "sqrt")
			return sqrt(arg_->evaluate()); // либо вычисляем корень квадратный
		else return fabs(arg_->evaluate()); // либо модуль — остальные функции запрещены
	}
	std::string print() const {
		return this->name_ + "(" + this->arg_->print() + ")";
	}
	Expression* transform(Transformer* tr) const
	{ return tr->transformFunctionCall(this); }
private:
	std::string const name_; // имя функции
	Expression const* arg_; // указатель на ее аргумент
};

struct Variable : Expression // структура «Переменная»
{
	Variable(std::string const& name) : name_(name) { } //в конструкторе надо указать ее имя
	std::string const& name() const { return name_; } // чтение имени переменной
	double evaluate() const // реализация виртуального метода «вычислить»
	{ return 0.0; }
	std::string print() const { return this->name_; }
	Expression* transform(Transformer* tr) const
	{ return tr->transformVariable(this); }
private:
	std::string const name_; // имя переменной
};

struct FoldConstants : Transformer
{
	Expression* transformNumber(Number const* number)
	{
		Expression* exp = new Number(number->value());
		return exp;
		// числа не сворачиваются, поэтому просто возвращаем копию
	}
	Expression* transformBinaryOperation(BinaryOperation const* binop)
	{
		// Создаем указатели на левое и правое выражение
		Expression* nleft = (binop->left())->transform(this); // переход в левый операнд, чтобы свернуть
		Expression* nright = (binop->right())->transform(this); // переход в правый операнд, чтобы свернуть
		int noperation = binop->operation();

		// Создаем новый объект типа BinaryOperation с новыми указателями
		BinaryOperation* nbinop = new BinaryOperation(nleft, noperation, nright);
		// Проверка на приводимость указателей к типу Number
		Number* nleft_is_number = dynamic_cast<Number*>(nleft);
		Number* nright_is_number = dynamic_cast<Number*>(nright);
		if (nleft_is_number && nright_is_number) {
			// Вычисляем значение выражения
			Expression* result = new Number(binop->evaluate());
			delete nbinop;
			return result;
		}
		else return nbinop;

	}
	Expression* transformFunctionCall(FunctionCall const* fcall)
	{
		// Указатель на аргумент
		Expression* arg = (fcall->arg())->transform(this);// рекурсивно сворачиваем аргумент
		std::string const& nname = fcall->name();

		// Объект типа FunctionCall с новым указателем
		FunctionCall* nfcall = new FunctionCall(nname, arg);

		// Проверка на приводимость указателя к типу Number
		Number* arg_is_number = dynamic_cast<Number*>(arg);
		if (arg_is_number) { // если аргумент — число вычисляем значение выражения
			Expression* result = new Number(fcall->evaluate());
			delete nfcall;
			return result;
		}
		else return nfcall;
	}
	Expression* transformVariable(Variable const* var)
	{
		Expression* exp = new Variable(var->name());
		return exp;
		// переменные не сворачиваем, поэтому просто возвращаем копию
	}
	~FoldConstants() { };
};

int main()
{
	Number* n32 = new Number(32.0);
	Number* n16 = new Number(16.0);
	BinaryOperation* minus = new BinaryOperation(n32, BinaryOperation::MINUS, n16);
	FunctionCall* callSqrt = new FunctionCall("sqrt", minus);
	Variable* var = new Variable("var");
	BinaryOperation* mult = new BinaryOperation(var, BinaryOperation::MUL, callSqrt);
	FunctionCall* callAbs = new FunctionCall("abs", mult);
	FoldConstants FC;
	Expression* newExpr = callAbs->transform(&FC);
}