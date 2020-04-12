#include <stdio.h> //not required
#include <string>
#include <vector>
#include <iostream> //cout and cin
#include <sstream> //<< and >> (cout,cin and string to number conversion)
#include <fstream> //open the text file
#include <chrono> //thread sleep
#include <thread> //thread sleep

using namespace std;

#define n if(stack.empty()||(stack.back()).length()!=0){ string z;stack.push_back(z); if(dbg)cout<<"NEW"<<endl;} // add new string to back of stack unless previous string is empty
#define g if(stack.empty())n; string* s = &stack.back(); // get string from back of stack
#define l(X) text[index]==X //current character == X ?
#define e(X) s[0]==X //first stack element == X ?
#define clr stack.clear();
#define p stack.pop_back();
#define st(X) stack[stack.size() - X] // get value from back of stack offset by X
#define numOp double i1 = strToNum(&getStr(st(3)));double i2 = strToNum(&getStr(st(2))); //Get two bottom strings of stack and turn them to numbers
#define pline cout<<"-------------------------------------------"<<endl; // just print a line, used for #
#define jmp(X) text.find(";"+X)+X.length()+1 // if X = "test" then find ";test" and move index to end of it
#define inj(X) text.insert(index, " \n" + X);index--;

const bool dbg = false; //display debug stuff

string text = ""; // whole text
int index = 0; // current character we are looking at from text

bool quotes = false; //used to know if inside string literal


struct variable {
	string identifier; //name
	int type = 0; //0 string, 1 number
	//data of this variable:
	string text = "";
	double number = 0;
};

vector<string> stack;
vector<variable> variables;


double strToNum(string* str) {
	stringstream s;
	double i;
	s << str[0];
	s >> i;
	return i;
}

int getVar(string name) {
	for (size_t i = 0; i < variables.size(); i++)
	{
		if (variables[i].identifier == name)return i;
	}
	return -1;
}


string getStr(string name) {
	//if name is a number
	if (isdigit(name[0]) || name[0] == '-') {

		stringstream ss;
		ss << name;
		double i;
		ss >> i;
		return to_string(i);

	}
	//if name is a string literal
	else if (name[0] == '"') {
		//strip the quotes from front and back
		name.pop_back();
		name.erase(name.begin());

		return name;
	}
	//lastly see if its a variable name and then get the text data of variable
	else
	{
		int i = getVar(name);
		return variables[i].type == 0 ? variables[i].text : to_string(variables[i].number);
	}
}

//see if a function is at bottom of stack and execute if it is
void eval() {
	if (stack.empty())return;

	bool again = true;

	g;

	if (e("print")) {
		cout << getStr(st(2));

		p; p;
	}
	else if (e("printl")) {
		cout << getStr(st(2)) << endl;

		p; p;
	}
	else if (e("=")) {
		string str = st(3);
		string val = string(getStr(st(2)));

		variable* v = nullptr;

		for (size_t i = 0; i < variables.size(); i++)
		{
			if (variables[i].identifier == str) {
				v = &variables[i];
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
			v[0].text = val;
			v[0].type = 0;
		}
		variables.push_back(v[0]);

		p; p; p;
	}
	else if (e("!")) {
		string res = strToNum(&getStr(st(2))) > 0 ? "0" : "1";

		p; p;

		stack.push_back(res);

	}
	else if (e("+")) {
		numOp;
		string res = to_string(i1 + i2);
		p; p; p;
		stack.push_back(res);
	}
	else if (e("-")) {
		numOp;
		string res = to_string(i1 - i2);
		p; p; p;
		stack.push_back(res);
	}
	else if (e("*")) {
		numOp;
		string res = to_string(i1 * i2);
		p; p; p;
		stack.push_back(res);
	}
	else if (e("/")) {
		numOp;
		string res = to_string(i1 / i2);
		p; p; p;
		stack.push_back(res);
	}
	else if (e("%")) {
		numOp;
		string res = to_string(fmod(i1, i2));
		p; p; p;
		stack.push_back(res);
	}
	else if (e(">")) {
		numOp;
		string res = i1 > i2 ? "1" : "0";
		p; p; p;
		stack.push_back(res);
	}
	else if (e("<")) {
		numOp;
		string res = i1 < i2 ? "1" : "0";
		p; p; p;
		stack.push_back(res);
	}
	else if (e("==")) {
		string res = getStr(st(3)) == getStr(st(2)) ? "1" : "0";
		p; p; p;
		stack.push_back(res);
	}
	else if (e("###")) {
		p;
		pline;
		cout << "vars size: " << variables.size() << endl;
		cout << "stack size: " << stack.size() << endl;
		cout << endl << "stack: " << endl;
		for (size_t i = 0; i < stack.size(); i++)
		{
			cout << i << ": " << stack[i] << endl;
		}
		pline;
	}
	else if (e("empty")) {
		clr;
	}
	else if (e("pop")) {
		p; p;
	}
	else if (e("dup")) {
		p; stack.push_back(stack.back());
	}
	else if (e("stop")) {
		while (1) {}
	}
	else if (e("ifjmp")) {
		string ind;
		int i = getVar(st(3));
		if (i == -1) {
			stringstream ss1;
			ss1 << st(3);
			ss1 >> i;
			ind = to_string(i);
		}
		else
		{
			ind = to_string((int)(variables[i].number + 0.5));
		}
		if (ind != "0") {
			index = jmp(st(2));
		}

		p; p; p;
	}
	else if (e("ifput")) {
		string ind;
		int i = getVar(st(3));
		if (i == -1) {
			stringstream ss1;
			ss1 << st(3);
			ss1 >> i;
			ind = to_string(i);
		}
		else
		{
			ind = to_string((int)(variables[i].number + 0.5));
		}
		if (ind != "0") {
			ind = st(2);
		}
		else {
			ind = "";
		}

		p; p; p;

		if (ind != "")stack.push_back(ind);
	}
	else if (e("if")) {
		string ind;
		int i = getVar(st(3));
		if (i == -1) {
			stringstream ss1;
			ss1 << st(3);
			ss1 >> i;
			ind = to_string(i);
		}
		else
		{
			ind = to_string((int)(variables[i].number + 0.5));
		}

		if (ind != "0") {
			inj(getStr(st(2)));
		}

		p; p; p;
	}
	else if (e("jmp")) {
		index = jmp(st(2));
		p; p;
	}
	else if (e("input")) {
		p;
		string input;
		getline(cin, input);
		input = '"' + input + '"';
		stack.push_back(input);
	}
	else if (e("inputraw")) {
		p;
		string input;
		getline(cin, input);
		stack.push_back(input);
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
	else {
		again = false;
	}

	if (again)eval();
}

int main(int argc, char** args) {

	if (argc == 1) return 1;

	try {

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

		bool waitForNewline = false; //are we in a comment? If so wait till newline to continue executing

		for (index = 0; index < (int)text.length(); index++)
		{
			if (l(';'))waitForNewline = true;

			if (waitForNewline == true) {
				if (l('\n')) {
					waitForNewline = false;
				}
				continue;
				//dont execute if in a comment until newline is encountered, then continue
			}

			//in a string literal?
			if (l('"')) {
				if (dbg)cout << "QUOTES" << endl;
				//if (stack.empty() == false && (stack.back()).length() == 0)p; // If we have an empty stack element before this one, remove it // now n(); does that
				quotes = !quotes;
				if (quotes == true) {
					//Encountered an opening quote!
					n;
				}
				else
				{
					//Encountered a closing quote!
					g;
					s[0] = '"' + s[0] + '"'; //wrap our string in some quotes to make it a string literal
					n;
				}
			}
			else {
				if (quotes == true) {
					if (dbg)cout << "q: " << text[index] << endl;

					g;
					s[0] += text[index]; //add chars to current string literal
				}
				else {
					//if space or newline seen, check the stack if there is a function name at bottom string to be executed (eval() does this)
					if (l(' ') || l('\n')) {
						//if (dbg)cout << "SPACE OR NEWLINE, EVALING" << endl;//////////////
						eval();
						n;
					}
					//else keep adding chars to current string on bottom of stack
					else {
						g;
						if (dbg)cout << text[index] << endl;//////////////
						s[0] += text[index];
					}
				}
			}
			if (index == text.length() - 1)eval(); // if we are at the end, eval one last time
		}
	}
	catch (int e) {//If error, display stack info
		if (e != 0) cout << "Error! " << e << endl << endl;
		cout << endl << endl << "stack size: " << stack.size() << endl;
		cout << endl << "stack: " << endl;
		for (size_t i = 0; i < stack.size(); i++)
		{
			cout << i << ": " << stack[i] << "	lenght:" << stack[i].length() << endl;
		}
	}

}