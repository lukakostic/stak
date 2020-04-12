#include <stdio.h> //not required
#include <string>
#include <vector>
#include <iostream> //cout and cin
#include <sstream> //<< and >> (cout,cin and string to number conversion)
#include <fstream> //open the text file
#include <chrono> //thread sleep
#include <thread> //thread sleep

using namespace std;

#define n if(stack->stack.empty()||(stack->stack.back()).length()!=0){ string z;stack->stack.push_back(z); if(dbg)cout<<"NEW"<<endl;} // add new string to back of stack->stack unless previous string is empty
#define g if(stack->stack.empty())n; string* s = &stack->stack.back(); // get string from back of stack->stack
#define l(X) stack->text[stack->index]==X //current character == X ?
#define e(X) s[0]==X //first stack->stack element == X ?
#define clr stack->stack.clear();
#define p ((stack->stack.size()>0)? stack->stack.pop_back():mainStack.stack.pop_back())
#define st(X) ((((int)stack->stack.size() - X)<0)?(mainStack.stack[(int)(mainStack.stack.size() + stack->stack.size()) - X]):(stack->stack[stack->stack.size() - X])) // get value from back of stack->stack offset by X
#define numOp double i1 = strToNum(&getStr(st(3),true,stack));double i2 = strToNum(&getStr(st(2),true,stack)); //Get two bottom strings of stack->stack and turn them to numbers
#define pline cout<<"-------------------------------------------"<<endl; // just print a line, used for #
#define jmp(X) stack->text.find(";"+X)+X.length()+1 // if X = "test" then find ";test" and move index to end of it
#define inj(X) stack->text.insert(stack->index, " \n" + X);stack->index--;


const bool dbg = false; //display debug stuff



struct variable {
	string identifier; //name
	int type = 0; //0 string, 1 number
	//data of this variable:
	string text = "";
	double number = 0;
};

struct stackInstance {
	string text;
	int index;
	int quotes = false; //used to know if inside string literal
	bool waitForNewline = false; //are we in a comment? If so wait till newline to continue executing

	vector<string> stack;
	vector<variable> variables;
};


stackInstance mainStack;

void Interpret(string t, bool main, stackInstance *caller); //early declaration because issues otherwise


string stripQuotes(string tx) {
	string name = string(tx);
	name.pop_back();
	name.erase(name.begin());
	return name;
}

double strToNum(string* str) {
	stringstream s;
	double i;
	s << str[0];
	s >> i;
	return i;
}

variable* getVar(string name, stackInstance *stack) {
	for (size_t i = 0; i < stack->variables.size(); i++)
	{
		if (stack->variables[i].identifier == name)return &stack->variables[i];
	}
	for (size_t i = 0; i < mainStack.variables.size(); i++)
	{
		if (mainStack.variables[i].identifier == name)return &mainStack.variables[i];
	}
	return nullptr;
}


string getStr(string name, bool stripQuote, stackInstance *stack) {
	//if name is a number
	if (isdigit(name[0]) || name[0] == '-') {

		stringstream ss;
		ss << name;
		double i;
		ss >> i;
		return to_string(i);

	}
	//if name is a string literal
	else if (name[0] == '{') {
		//strip the quotes from front and back
		if (stripQuote) name = stripQuotes(name);

		return name;
	}
	//lastly see if its a variable name and then get the text data of variable
	else
	{
		variable *v = getVar(name, stack);
		return v->type == 0 ? v->text : to_string(v->number);
	}
}

//see if a function is at bottom of stack->stack and execute if it is
void eval(stackInstance *stack) {
	if (stack->stack.empty())return;

	bool again = true;

	g;

	if (e("print")) {
		cout << getStr(st(2), true, stack);

		p; p;
	}
	if (e("end")) {
		stack->index = stack->text.size();
		p;
	}
	else if (e("printl")) {

		cout << getStr(st(2), true, stack) << endl;

		p; p;
	}
	else if (e("=")) {
		string str = st(3);
		string val = string(getStr(st(2), false, stack));


		variable* v = nullptr;

		for (size_t i = 0; i < stack->variables.size(); i++)
		{
			if (stack->variables[i].identifier == str) {
				v = &stack->variables[i];
				break;
			}
		}

		if (v == nullptr) {
			variable newVar;
			v = &newVar;
		}

		v[0].identifier = str;

		if (isdigit(val[0])) {
			stringstream ss;
			ss << val;
			ss >> v[0].number;
			v[0].type = 1;
		}
		else
		{
			v[0].text = stripQuotes(val);
			v[0].type = 0;
		}
		stack->variables.push_back(v[0]);

		//cout << "start: " << v[0].text << " :end" << endl;

		p; p; p;
	}
	else if (e("!")) {
		string res = strToNum(&getStr(st(2), true, stack)) > 0 ? "0" : "1";

		p; p;

		stack->stack.push_back(res);

	}
	else if (e("+")) {
		numOp;
		string res = to_string(i1 + i2);
		p; p; p;
		stack->stack.push_back(res);
	}
	else if (e("-")) {
		numOp;
		string res = to_string(i1 - i2);
		p; p; p;
		stack->stack.push_back(res);
	}
	else if (e("*")) {
		numOp;
		string res = to_string(i1 * i2);
		p; p; p;
		stack->stack.push_back(res);
	}
	else if (e("/")) {
		numOp;
		string res = to_string(i1 / i2);
		p; p; p;
		stack->stack.push_back(res);
	}
	else if (e("%")) {
		numOp;
		string res = to_string(fmod(i1, i2));
		p; p; p;
		stack->stack.push_back(res);
	}
	else if (e(">")) {
		numOp;
		string res = i1 > i2 ? "1" : "0";
		p; p; p;
		stack->stack.push_back(res);
	}
	else if (e("<")) {
		numOp;
		string res = i1 < i2 ? "1" : "0";
		p; p; p;
		stack->stack.push_back(res);
	}
	else if (e("==")) {
		string res = getStr(st(3), true, stack) == getStr(st(2), true, stack) ? "1" : "0";
		p; p; p;
		stack->stack.push_back(res);
	}
	else if (e("cnt")) {

	}
	else if (e("###")) {
		p;
		pline;
		cout << "vars size: " << stack->variables.size() << endl;
		cout << "stack->stack size: " << stack->stack.size() << endl;
		for (size_t i = 0; i < stack->stack.size(); i++)
		{
			cout << i << ": " << stack->stack[i] << endl;
		}
		pline;
	}
	else if (e("empty")) {
		clr;
	}
	else if (e("pop")) {
		p; p;
	}
	else if (e("beep")) {
		cout << '\a';
		p;
	}
	else if (e("dup")) {
		p; stack->stack.push_back(stack->stack.back());
	}
	else if (e("reverse")) {
		vector<string> s = stack->stack;
		for (size_t i = 0; i < s.size(); i++)
		{
			stack->stack[i] = s[s.size() - (i + 1)];
		}
	}
	else if (e("stop")) {
		while (1) {}
	}
	else if (e("ifjmp")) {
		string ind = "1";
		string str = getStr(st(3), true, stack);
		if (str == "0.000000") {
			ind = "0";
		}
		if (ind != "0") {
			stack->index = jmp(st(2));
		}

		p; p; p;
	}
	else if (e("ifjmpStr")) {
		string ind = "1";
		string str = getStr(st(3), true, stack);
		if (str == "0.000000") {
			ind = "0";
		}
		if (ind != "0") {
			stack->index = jmp(getStr(st(2), true, stack));
		}

		p; p; p;
	}
	else if (e("ifput")) {
		string ind = "1";
		string str = getStr(st(3), true, stack);
		if (str == "0.000000") {
			ind = "0";
		}
		if (ind != "0") {
			ind = st(2);
		}
		else {
			ind = "";
		}

		p; p; p;

		if (ind != "")stack->stack.push_back(ind);
	}
	else if (e("if")) {
		string ind = "1";
		string str = getStr(st(3), true, stack);
		if (str == "0.000000") {
			ind = "0";
		}
		bool cnd = false;
		if (ind != "0") {
			cnd = true;
			ind = getStr(st(2), true, stack);
		}

		p; p; p;

		if (cnd)
			Interpret(ind, false, stack);
	}
	else if (e("jmpStr")) {
		stack->index = jmp(getStr(st(2), true, stack));
		p; p;
	}
	else if (e("jmp")) {
		stack->index = jmp(st(2));
		p; p;
	}
	else if (e("input")) {
		p;
		string input;
		getline(cin, input);
		input = '{' + input + '}';
		stack->stack.push_back(input);
	}
	else if (e("inputraw")) {
		p;
		string input;
		getline(cin, input);
		stack->stack.push_back(input);
	}
	else if (e("wait")) {
		this_thread::sleep_for(chrono::milliseconds((int)(strToNum(&st(2)) * 1000)));
		p; p;
	}
	else if (e("inject")) {
		p;
		string input;
		getline(cin, input);
		inj(input);
	}
	else if (e("injectStr")) {
		p;
		inj(getStr(st(1), true, stack));
		p;
	}
	else if (e("eval")) {
		string str = getStr(st(2), true, stack);
		p; p;
		Interpret(str, false, stack);
	}
	else {
		again = false;
	}

	if (again)eval(stack);
}

void Interpret(string t, bool main, stackInstance *caller) {

	stackInstance* stack = &mainStack;

	if (main == false) {
		stackInstance stackI = stackInstance();

		stackI.index = 0;
		stackI.stack = vector<string>();
		stackI.variables = vector<variable>();
		//stackI.text = string(t);
		stackI.quotes = 0;
		stackI.waitForNewline = false;
		stack = &stackI;

		//duzina od stack.text je 10
	}

	//duzina od stack.text je 0

	stack->text = string(t);



	try {


		for (stack->index = 0; stack->index < (int)stack->text.length(); stack->index++)
		{

			if (l(';'))stack->waitForNewline = true;

			if (stack->waitForNewline == true) {
				if (l('\n')) {
					stack->waitForNewline = false;
				}
				continue;
				//dont execute if in a comment until newline is encountered, then continue
			}
			bool quote = false;
			//in a string literal?
			if (l('{')) {
				if (dbg)cout << "QUOTES OPEN" << endl;
				//if (stack->stack.empty() == false && (stack->stack.back()).length() == 0)p; // If we have an empty stack->stack element before this one, remove it // now n(); does that
				stack->quotes += 1;

				if (stack->quotes == 1)
				{
					if (dbg)cout << "QUOTES == 1" << endl;
					//Encountered an opening quote!
					quote = true;
					n;
				}
			}
			else if (l('}'))
			{
				if (dbg)cout << "QUOTES CLOSED" << endl;
				//if (stack->stack.empty() == false && (stack->stack.back()).length() == 0)p; // If we have an empty stack->stack element before this one, remove it // now n(); does that
				stack->quotes -= 1;
				if (stack->quotes == 0)
				{
					if (dbg)cout << "QUOTES == 0" << endl;
					//Encountered a closing quote!
					quote = true;
					g;
					s[0] = '{' + s[0] + '}'; //wrap our string in some quotes to make it a string literal
					n;
				}
			}
			

			if (!quote)
			{
				if (stack->quotes > 0) {

					if (dbg)cout << "q: " << stack->text[stack->index] << endl;

					g;
					s[0] += stack->text[stack->index]; //add chars to current string literal
				}
				else {
					//if space or newline seen, check the stack->stack if there is a function name at bottom string to be executed (eval() does this)
					if (l(' ') || l('\n')) {
						//if (dbg)cout << "SPACE OR NEWLINE, EVALING" << endl;//////////////
						eval(stack);
						n;
					}
					//else keep adding chars to current string on bottom of stack->stack
					else {
						g;
						if (dbg)cout << stack->text[stack->index] << endl;//////////////
						s[0] += stack->text[stack->index];
					}
				}
			}

			if (stack->index == stack->text.length() - 1)eval(stack); // if we are at the end, eval one last time
		}
		if (!main) {

			for (size_t i = 0; i < stack->stack.size(); i++)
			{
				caller->stack.push_back(stack->stack[i]);

			}
		}
	}
	catch (int e) {//If error, display stack->stack info
		if (e != 0) cout << "Error! " << e << endl << endl;
		cout << endl << endl << "stack->stack size: " << stack->stack.size() << endl;
		cout << endl << "stack->stack: " << endl;
		for (size_t i = 0; i < stack->stack.size(); i++)
		{
			cout << i << ": " << stack->stack[i] << "	lenght:" << stack->stack[i].length() << endl;
		}
	}
}



int main(int argc, char** args) {

	if (argc == 1) return 1;

	string text = "";
	//read text contents of file
	ifstream myReadFile; //opened file
	myReadFile.open(args[1]);
	if (myReadFile.is_open()) {
		string line; //current line
		while (!myReadFile.eof()) {
			while (getline(myReadFile, line)) {
				line.append("\n");
				text.append(line);
			}
		}
	}
	myReadFile.close();
	//ok done reading, now go thru text and interpret stuff

	mainStack.index = 0;
	mainStack.text = text;
	mainStack.stack = vector<string>();
	mainStack.quotes = 0;
	mainStack.waitForNewline = false;

	Interpret(text, true, &mainStack);

}