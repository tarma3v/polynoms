#ifndef CALCULATOR_H_INCLUDED
#define CALCULATOR_H_INCLUDED

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <utility>

using namespace std;

typedef long long ll;

const ll INF = -1e15;

bool is_digit(char symbol) {
	if (('0' <= symbol) && (symbol <= '9')) {
		return true;
	}
	else {
		return false;
	}
}

bool is_letter(char symbol) {
	if ((('a' <= symbol) && (symbol <= 'z')) || ('A' <= symbol) && (symbol <= 'Z')) {
		return true;
	}
	return false;
}

bool is_sign(char symbol) {
	if ((symbol == '+') || (symbol == '-') || (symbol == '^')) {
		return true;
	}
	else {
		return false;
	}
}

// is_repeat checks how
short is_repeat(string str) {
	bool check = false;
	string name = "", temp_name = "";
	str += ' ';
	for (int i = 0; i < str.size(); ++i) {
		if (is_letter(str[i])) {
			if (check) {
				temp_name += str[i];
			}
			else {
				temp_name = str[i];
				check = true;
			}
		}
		else { // name of variable has ended
			if (check) {
				if (name == "") {
					name = temp_name;
				}
				else if (temp_name != name) {
					return 2; // more than 2 variables
				}
				check = false;
			}
		}
	}
	if (name.size() > 1) {
		return 1; // only one-length names of variables are allowed
	}
	return 0; // ok
}

bool is_incorrect_symbols(string str) {
	for (int i = 0; i < str.size(); ++i) {
		if (!(is_digit(str[i]) || is_sign(str[i]) || is_letter(str[i]) || (str[i] == ' '))) {
			return true;
		}
	}
	return false;
}

void clean_spaces(string& str) { // delete spaces before and after reasonable part of input
	int i = 0, j = 0;
	for (i = 0; i < str.size(); ++i) {
		if (str[i] != ' ') {
			break;
		}
	}
	for (int j = str.size() - 1; j >= 0; --j) {
		if (str[i] != ' ') {
			break;
		}
	}
	if (j < i) {
		cout << "Input only consists of spaces\n";
	}
	else {
		str = str.substr(i, str.size() - j + i + 1);
	}
}

// quite hard to write
// as we can not just delete all spaces, we need to check whether there are some mistakes between them

int is_mistake(string str) { // this function tries to find mistakes between spaces
	if (is_sign(str[str.size() - 1])) {
		cout << "Invalid sign\n";
		return 10;
	}
	vector <pair <char, char> > v;
	// there is types of variables before and after spaces
	// we use it to analyze situations
	// for example {+; 1} is correct, {*; ^} is incorrect;
	clean_spaces(str);
	bool space_before = false;
	char memory;
	for (int i = 0; i < str.size(); ++i) {
		if (str[i] == ' ' && space_before == false) {
			memory = str[i - 1];
			space_before = true;
		}
		if (str[i] != ' ' && space_before == true) {
			v.push_back(make_pair(memory, str[i]));
			space_before = false;
		}
	}

	for (int i = 0; i < v.size(); ++i) {
		char a = v[i].first;
		char b = v[i].second;
		try {
			if (is_sign(a) && is_sign(b)) {
				if (a == '^' && b == '-') {
					// x ^ -3
					// + -
				}
				else {
					throw 1;
				}
			}
			else if (a == '^' && is_letter(b)) {
				throw 2; // x ^ x and 14 ^ x can't be part of Poly
			}
			else if (is_letter(a) && is_letter(b)) {
				throw 3; // x x (no sign between parts of sum)
			}
			else if (is_letter(a) && is_digit(b)) {
				throw 4; // x 1 (no sign between parts of sum)
			}
			else if (is_digit(a) && b == '^') {
				throw 5; // 4 ^ x
			}
			else if (is_digit(a) && is_digit(b)) {
				throw 6; // 4 4
			}
			// 4 x turns into 4x
		}
		catch (int a)
		{
			// Любые исключения типа int, сгенерированные в блоке try выше, обрабатываются здесь
			std::cerr << "We caught an int exception with value: " << a << '\n';
			return a;
		}
	}
	return 0;
}

// now we can delete all spaces
string clean_all_spaces(string str) {
	string res;
	for (int i = 0; i < str.size(); ++i) {
		if (str[i] != ' ') {
			res += str[i];
		}
	}
	return res;
}

// fixes some problems
bool is_mistake_second(string str) {
	for (int i = 0; i < str.size() - 1; ++i) {
		if (str[i] == '^' && is_letter(str[i + 1])) {
			return true;
		}
	}
	return false;
}

struct Polynom {
	int deg;
	int coef;
	Polynom* next;
	// Polynom * prev;
};

void push_back_in_poly(Polynom*& head, int deg, int coef) {
	Polynom* p;
	p = new Polynom;
	p->deg = deg;
	p->coef = coef;
	p->next = nullptr;
	if (head == nullptr) {
		p->next = nullptr;
		head = p;
	}
	else if (head->next == nullptr) {
		if ((head->deg) > deg) {
			Polynom* q;
			q = head;
			head = p;
			head->next = q;
		}
		else if (head->deg < deg) {//there are some troubles
			head->next = p;
			p->next = nullptr;
		}
		else {
			head->coef += coef;
		}
	}
	else {
		Polynom* q = head;
		Polynom* pred = head;
		while (q != nullptr && deg > q->deg) {
			pred = q;
			q = q->next;
		}
		if (q == nullptr) { // p is max elem
			pred->next = p;
			p->next = nullptr;
		}
		else if (deg == q->deg) { // exists element with the same degree
			q->coef += coef;
		}
		else if (deg < q->deg) {
			if (pred == q) { // p is min elem
				p->next = q;
				head = p;
			}
			else { // p is between pred and q
				pred->next = p;
				p->next = q;
			}
		}
	}
}

Polynom* push_back_in_poly_2(Polynom* head, int deg, int coef) {
	Polynom* p;
	p = new Polynom;
	p->deg = deg;
	p->coef = coef;

	if (head == nullptr) {
		p->next = nullptr;
		head = p;
	}
	else if (head->next == nullptr) {
		if ((head->deg) > deg) {
			Polynom* q;
			q = head;
			head = p;
			head->next = q;
		}
		else if (head->deg < deg) {//there are some troubles
			head->next = p;
			p->next = nullptr;
		}
		else {
			head->coef += coef;
		}
	}
	else {
		Polynom* q = head;
		Polynom* pred = head;
		while (q != nullptr && deg > q->deg) {
			pred = q;
			q = q->next;
		}
		if (q == nullptr) { // p is max elem
			pred->next = p;
			p->next = nullptr;
		}
		else if (deg == q->deg) { // exists element with the same degree
			q->coef += coef;
		}
		else if (deg < q->deg) {
			if (pred == q) { // p is min elem
				p->next = q;
				head = p;
			}
			else { // p is between pred and q
				pred->next = p;
				p->next = q;
			}
		}
	}
	return head;
}

struct Polynom_database {
	int index;
	Polynom* pol;
	Polynom_database* next;
};

int size_of_base(Polynom_database* base_head) {
	int cnt = 0;
	while (base_head != nullptr) {
		base_head = base_head->next;
		++cnt;
	}
	return cnt;
}

void push_back_in_base(Polynom* P, Polynom_database*& base_head) {
	Polynom_database* L;
	L = new Polynom_database;
	L->pol = P;
	if (base_head == nullptr) {
		L->index = 0;
		L->next = nullptr;
		base_head = L;
	}
	else {
		L->index = (base_head->index) + 1;
		L->next = base_head;
		base_head = L;
	}
}

void delete_from_base(int index, Polynom_database*& base_head) {
	if (base_head == nullptr) {
		cout << "Base is empty!\n";
		return;
	}
	else if (base_head->next == nullptr) {
		Polynom_database* q = base_head;
		base_head = nullptr;
		delete q;
		return;
	}
	else if (index >= size_of_base(base_head)) {
		cout << "Entered value is too big\n";
		return;
	}
	else if (index == size_of_base(base_head) - 1) {
		Polynom_database* q;
		q = base_head;
		base_head = base_head->next;
		delete q;
		return;
	}
	Polynom_database* base_iter = base_head;
	while (base_iter->index != index + 1) {
		base_iter = base_iter->next;
	}
	Polynom_database* q = base_iter->next;
	Polynom_database* w = q->next;

	base_iter->next = w;
	delete q;
	// work even if w is nullptr
	base_iter = base_head;
	int cnt = size_of_base(base_head);
	while (base_iter != nullptr) {
		base_iter->index = --cnt;
		base_iter = base_iter->next;
	}
}

Polynom* derivative(int pos, Polynom_database*& base_head) {
	Polynom* result = nullptr;
	Polynom* head = nullptr;
	if (pos >= size_of_base(base_head)) {
		cout << "Entered value is too big\n";
	}
	else {
		Polynom_database* base_iter = base_head;
		while (base_iter->index != pos) {
			base_iter = base_iter->next;
		}
		Polynom* poly = base_iter->pol;
		while (poly != nullptr) {
            if(poly->deg != 0)
            {
                Polynom* q;
                q = new Polynom;
                q->deg = (poly->deg) - 1;
                q->coef = (poly->coef) * (poly->deg);
                q->next = nullptr;
                if (head == nullptr) {
                    head = q;
                    result = q;
                }
                else {
                    result->next = q;
                    result = q;
                }
            }
			poly = poly->next;
		}
	}
    if(head != nullptr)
        push_back_in_base(head, base_head);
	return head;
}

Polynom* polynom_merge(Polynom* first, Polynom* second) {
	Polynom* tail = nullptr;
	Polynom* head = nullptr;
	while (first != nullptr && second != nullptr) {
		if (first->deg == second->deg) {
			if (first->coef + second->coef != 0) {
				Polynom* q;
				q = new Polynom;
				q->coef = first->coef + second->coef;
				q->deg = first->deg;
				q->next = nullptr;
				if (tail == nullptr) {
					tail = q;
				}
				else {
					tail->next = q;
					tail = q;
				}
			}
			first = first->next;
			second = second->next;
		}
		else if (first->deg < second->deg) {
			Polynom* q;
			q = new Polynom;
			q->deg = first->deg;
			q->coef = first->coef;
			q->next = nullptr;
			first = first->next;
			if (tail == nullptr) {
				tail = q;
			}
			else {
				tail->next = q;
				tail = q;
			}
		}
		else {
			Polynom* q;
			q = new Polynom;
			q->deg = second->deg;
			q->coef = second->coef;
			q->next = nullptr;
			second = second->next;
			if (tail == nullptr) {
				tail = q;
			}
			else {
				tail->next = q;
				tail = q;
			}
		}
		if (head == nullptr) {
			head = tail;
		}
	}
	if (first == nullptr) {
		while (second != nullptr) {
			Polynom* q;
			q = new Polynom;
			q->deg = second->deg;
			q->coef = second->coef;
			q->next = nullptr;
			tail->next = q;
			tail = q;
			second = second->next;
		}
	}
	else {
		while (first != nullptr) {
			Polynom* q;
			q = new Polynom;
			q->deg = first->deg;
			q->coef = first->coef;
			q->next = nullptr;
			tail->next = q;
			tail = q;
			first = first->next;
		}
	}
	return head;
}


Polynom* polynom_sum(int i, int j, Polynom_database*& base_head) {
	Polynom* result = nullptr;
	Polynom* first = nullptr, * second = nullptr;
	Polynom_database* base_iter = base_head;
	while (base_iter != nullptr) {
		if ((base_iter->index) == i) {
			first = base_iter->pol;
			break;
		}
		base_iter = base_iter->next;
	}
	base_iter = base_head;
	while (base_iter != nullptr) {
		if ((base_iter->index) == j) {
			second = base_iter->pol;
			break;
		}
		base_iter = base_iter->next;
	}
	Polynom* answer = polynom_merge(first, second);
    push_back_in_base(answer, base_head);
	return answer;
}

long double polynom_value(int index, long double value, Polynom_database* base_head) {
	long double result = 0;
	if (index > size_of_base(base_head)) {
		cout << "Entered indexes are too big\n";
		return INF;
	}
	else {
		Polynom* poly = nullptr;
		Polynom_database* base_iter = base_head;
		while (base_iter != nullptr) {
			if ((base_iter->index) == index) {
				poly = base_iter->pol;
				break;
			}
			base_iter = base_iter->next;
		}
		if (value == 0) {
			while (poly != nullptr) {
				if (poly->deg == 0) {
					result += poly->coef;
				}
				poly = poly->next;
			}
		}
		else {
			while (poly != nullptr) {
				result += (poly->coef) * pow(value, poly->deg);
				poly = poly->next;
			}
		}
	}
	return result;
}

void show(Polynom_database* base_head, char name) { // pls
	Polynom_database* base_iter = base_head;
	Polynom* poly_iter;
	while (base_iter != nullptr) {
		poly_iter = base_iter->pol;
		cout << base_iter->index << " : ";
		while (poly_iter != nullptr) {
			cout << poly_iter->coef << name << '^' << poly_iter->deg;
			if (poly_iter->next != nullptr) {
				cout << " + ";
			}
			poly_iter = poly_iter->next;
		}
		cout << endl;
		base_iter = base_iter->next;
	}
}

Polynom* polynom_mul(int i, int j, Polynom_database*& base_head) {
	Polynom* first, * second;
	Polynom* head = nullptr;
	Polynom_database* base_iter = base_head;

	while (base_iter->index != i) {
		base_iter = base_iter->next;
	}
	first = base_iter->pol;
	base_iter = base_head;
	while (base_iter->index != j) {
		base_iter = base_iter->next;
	}
	second = base_iter->pol;

	Polynom* first_iter = first, * second_iter = second;
	int dg, cf;

	while (first_iter != nullptr) {
		second_iter = second;
		dg = first_iter->deg;
		cf = first_iter->coef;
		while (second_iter != nullptr) {
			push_back_in_poly(head, dg + (second_iter->deg), second_iter->coef * cf);
			second_iter = second_iter->next;
		}
		first_iter = first_iter->next;
	}

		push_back_in_base(head, base_head);
    return head;

}

void make_polynom(string str, Polynom_database*& base_head) {
	Polynom* head = nullptr;
	int coef = 0; int deg = 0;
	str = '+' + str + '+';
	int state = 0; // 0 - coefficient, 1 - degree
	bool positive_coef = true, positive_deg = true;
	for (int i = 1; i < str.size(); ++i) {
		if ((str[i] == '+' || str[i] == '-') && (str[i - 1] != '^')) {
			if (is_letter(str[i - 1])) {
				deg = 1;
				positive_deg = true;
			}
			if (coef != 0) {
				if (!positive_coef) {
					coef *= -1;
				}
				if (!positive_deg) {
					deg *= -1;
				}
				push_back_in_poly(head, deg, coef);
			}
			deg = 0;
			coef = 0;
			state = 0;
			if (str[i] == '-') {
				positive_coef = false;
			}
			else {
				positive_coef = true;
			}
		}
		else if (is_letter(str[i])) {
			if (is_sign(str[i - 1])) {
				coef = 1;
			} // + x
			state = 1;
		}
		else if (is_digit(str[i])) {
			if (state == 0) {
				coef = coef * 10 + (str[i] - '0');
			}
			else {
				deg = deg * 10 + (str[i] - '0');
			}
		}
		else if (str[i] == '^') {
			state = 1;
			deg = 0;
			positive_deg = true;
		}
		else if (str[i] == '-' && str[i - 1] == '^') {
			positive_deg = false;
		}
	}
	push_back_in_base(head, base_head);
}

void compare(int i, int j, Polynom_database* base_head) {
	Polynom* first, * second;
	if (max(i, j) >= size_of_base(base_head)) {
		cout << "Entered value is too big\n";
		return;
	}
	if (i == j) {
		cout << "Request to same polynom\n";
		return;
	}
	Polynom_database* base_iter = base_head;
	while (base_iter->index != i) {
		base_iter = base_iter->next;
	}
	first = base_iter->pol;
	base_iter = base_head;
	while (base_iter->index != j) {
		base_iter = base_iter->next;
	}
	second = base_iter->pol;
	while (first != nullptr && second != nullptr) {
		if (first->coef != second->coef ||
			first->deg != second->deg) {
			cout << "Polynoms aren't same\n";
			return;
		}
		first = first->next;
		second = second->next;
	}
	if (first != nullptr || second != nullptr) {
		cout << "Polynoms aren't same\n";
	}
	else {
		cout << "Polynoms are same\n";
	}
}

void help() {
	cout << "<Help> \n";
	cout << "type h if you want to get help \n";
	cout << "type s if you want to count sum of two polynoms \n";
	cout << "type v if you want to count value of polynom \n";
	cout << "type p if you want to enter a new polynom \n";
	cout << "type d if you want to delete a polynom from base \n";
	cout << "type q if you want to count a derivative of a poly \n";
	cout << "type w if you want to watch all polynoms \n";
	cout << "type e if you want to exit \n";
	cout << "type c if you want to compare two polynoms\n";
	cout << "type l if you want to dot two polynoms\n";
	cout << "press f to find a solution of polynom\n";
}

long double polynom_value_2(Polynom* head, long double value) {
	long double result = 0;

	Polynom* poly = head;
	if (value == 0) {
		while (poly != nullptr) {
			if (poly->deg == 0) {
				result += poly->coef;
			}
			poly = poly->next;
		}
	}
	else {
		while (poly != nullptr) {
			result += (poly->coef) * pow(value, poly->deg);
			poly = poly->next;
		}
	}

	return result;
}

vector <int> divisors(int num) { // num > 0
	vector <int> result;
	int i;
	for (i = 1; i * i <= num; ++i) {
		if (num % i == 0) {
			result.push_back(i);
			if (i != num / i)
				result.push_back(num / i);
		}
	}
	return result;
}

void find_solution(int ind, Polynom_database* base_head) {
	if (ind >= size_of_base(base_head)) {
		cout << "Entered value is too big\n";
		return;
	}
	Polynom_database* base_iter = base_head;
	while (base_iter->index != ind) {
		base_iter = base_iter->next;
	}
	Polynom* head = base_iter->pol;
	vector <int> ans;
	vector <int> div;
	if (head == nullptr) {
		cout << "Polynom is empty!\n";
	}
	if (head->deg > 0) { // 0 is solution
		div = divisors(abs(head->coef));
		for (int i = 0; i < div.size(); ++i) {
			if (abs(polynom_value_2(head, div[i])) < 0.01) {
				ans.push_back(div[i]);
			}
			if (abs(polynom_value_2(head, -div[i])) < 0.01) {
				ans.push_back(-div[i]);
			}
		}
		ans.push_back(0);
	}
	else { // 0 is not a solution
		if (head->coef != 0) {
			div = divisors(abs(head->coef));
			for (int i = 0; i < div.size(); ++i) {
				if (abs(polynom_value_2(head, div[i])) < 0.01) {
					ans.push_back(div[i]);
				}
				if (abs(polynom_value_2(head, -div[i])) < 0.01) {
					ans.push_back(-div[i]);
				}
			}
		}
	}
	cout << '{';
	for (int j = 0; j < ans.size(); ++j) {
		cout << ans[j];
		if (j != ans.size() - 1)
			cout << "; ";
	}
	cout << "}\n";
}

char find_name(string str) {
	for (int i = 0; i < str.size(); ++i) {
		if (is_letter(str[i])) {
			return str[i];
		}
	}
	return ' ';
}

bool separate(char a, char b) {
	if (is_digit(a) && is_digit(b)) {
		return false;
	}
	else if (is_letter(a) && is_letter(b)) {
		return false;
	}
	else if (is_sign(a) && is_sign(b)) {
		return true;
	}
	else {
		return true;
	}
}

string change(string str) {
	string res;
	for (int i = 0; i < str.size() - 1; ++i) {
		if (separate(str[i], str[i + 1])) {
			res = res + str[i] + ' ';
		}
		else {
			res = res + str[i];
		}
	}
	res += str[str.size() - 1];
	return res;
}
/*
int main() {
	cout << fixed << setprecision(3);
	Polynom_database* base_head = nullptr;
	help();
	string s;
	char c;
	char name = ' ';
	char name_tmp = ' ';
	while (1 > 0) {
		cin >> s;
		if (s.size() > 1) {
			cout << "Name of function is very long\n";
			continue;
		}
		c = s[0];
		if (c == 'e') {
			exit(0);
		}
		else if (c == 'h') {
			help();
		}
		else if (c == 's') {
			int first_iter, second_iter;
			cin >> first_iter >> second_iter;
			polynom_sum(first_iter, second_iter, base_head);
		}
		else if (c == 'v') {
			long double value;
			int index;
			cout << "index : ";
			cin >> index;
			cout << "value : ";
			cin >> value;
			cout << "Value in " << value << " is equal to " << polynom_value(index, value, base_head);
		}
		else if (c == 'p') {
			Polynom* head = nullptr;
			cout << "Enter your poly\n";
			string str, sr;
			getline(cin, sr);
			getline(cin, str);
			str = change(str);
			name_tmp = find_name(str);
			if (name != name_tmp && (name != ' ') && (name_tmp != ' ')) {
				cout << "another variable\n";
				continue;
			}
			else if (name == ' ' && name_tmp != ' ') {
				name = name_tmp;
			}
			if (is_repeat(str) == 2) {
				cout << "Input is incorrect, error: two or more variables\n";
			}
			else if (is_repeat(str) == 1) {
				cout << "Length of name of variable is more than 1\n";
			}
			else if (is_incorrect_symbols(str)) {
				cout << "Input consists incorrect symbols\n";
			}
			else if (is_mistake(str) != 0) {
				cout << "Mistake in symbols\n";
			}
			else if (is_mistake_second(str)) {
				cout << "It's not a poly\n";
			}
			else {
				str = clean_all_spaces(str);
				make_polynom(str, base_head);
				show(base_head, name);
			}
		}
		else if (c == 'd') {
			int index;
			cin >> index;
			delete_from_base(index, base_head);
		}
		else if (c == 'q') {
			int index;
			cin >> index;
			derivative(index, base_head);
		}
		else if (c == 'w') {
			show(base_head, name);
		}
		else if (c == 'c') {
			int ind1, ind2;
			cin >> ind1 >> ind2;
			compare(ind1, ind2, base_head);
		}
		else if (c == 'l') {
			int ind1, ind2;
			cin >> ind1 >> ind2;
			dot(ind1, ind2, base_head);
		}
		else if (c == 'f') {
			int ind;
			cin >> ind;
			find_solution(ind, base_head);
		}
		else {
			cout << "This function doesn't exist, try another\n";
		}
	}
	// cout << "kdjfkfjfkjgjfg";

}
*/

bool is_correct(string str) {
	str = change(str);
	if (is_repeat(str) == 2) {
		cout << "Input is incorrect, error: two or more variables\n";
	}
	else if (is_repeat(str) == 1) {
		cout << "Length of name of variable is more than 1\n";
	}
	else if (is_incorrect_symbols(str)) {
		cout << "Input consists incorrect symbols\n";
	}
	else if (is_mistake(str) != 0) {
		cout << "Mistake in symbols\n";
	}
	else if (is_mistake_second(str)) {
		cout << "It's not a poly\n";
	}
	else {
		str = clean_all_spaces(str);
		return true;
	}
	return false;
}

string from_int_to_string(int x) {
    if(x == 0)
        return "0";


	string ans;
	bool t = x < 0;
	if(t)
        x *= -1;
	while (x > 0) {
		ans += (char)(x % 10 + '0');
		x /= 10;
	}
	if(t)
        ans += '-';
	reverse(ans.begin(), ans.end());
	return ans;
}

string show_polynom(Polynom* head, char name) {
	string ans;
	while (head != nullptr) {

		ans = ans + from_int_to_string(head->coef) + name + '^' + from_int_to_string(head->deg);
		if (head->next != nullptr) {
			ans += " + ";
		}
		head = head->next;
	}
	return ans;
}

Polynom* make_polynom(string str) {
	str = change(str);
	str = clean_all_spaces(str);
	Polynom* head = nullptr;
	int coef = 0; int deg = 0;
	str = '+' + str + '+';
	int state = 0; // 0 - coefficient, 1 - degree
	bool positive_coef = true, positive_deg = true;
	for (int i = 1; i < str.size(); ++i) {
		if ((str[i] == '+' || str[i] == '-') && (str[i - 1] != '^')) {
			if (is_letter(str[i - 1])) {
				deg = 1;
				positive_deg = true;
			}
			if (coef != 0) {
				if (!positive_coef) {
					coef *= -1;
				}
				if (!positive_deg) {
					deg *= -1;
				}
				push_back_in_poly(head, deg, coef);
			}
			deg = 0;
			coef = 0;
			state = 0;
			if (str[i] == '-') {
				positive_coef = false;
			}
			else {
				positive_coef = true;
			}
		}
		else if (is_letter(str[i])) {
			if (is_sign(str[i - 1])) {
				coef = 1;
			} // + x
			state = 1;
		}
		else if (is_digit(str[i])) {
			if (state == 0) {
				coef = coef * 10 + (str[i] - '0');
			}
			else {
				deg = deg * 10 + (str[i] - '0');
			}
		}
		else if (str[i] == '^') {
			state = 1;
			deg = 0;
			positive_deg = true;
		}
		else if (str[i] == '-' && str[i - 1] == '^') {
			positive_deg = false;
		}
	}
	return head;
}


Polynom* polynom_merge_minus(Polynom* first, Polynom* second) {
	Polynom* tail = nullptr;
	Polynom* head = nullptr;
	while (first != nullptr && second != nullptr) {
		if (first->deg == second->deg) {
			if (first->coef != second->coef) {
				Polynom* q;
				q = new Polynom;
				q->coef = first->coef - second->coef;
				q->deg = first->deg;
				q->next = nullptr;
				if (tail == nullptr) {
					tail = q;
				}
				else {
					tail->next = q;
					tail = q;
				}
			}
			first = first->next;
			second = second->next;
		}
		else if (first->deg < second->deg) {
			Polynom* q;
			q = new Polynom;
			q->deg = first->deg;
			q->coef = first->coef;
			q->next = nullptr;
			first = first->next;
			if (tail == nullptr) {
				tail = q;
			}
			else {
				tail->next = q;
				tail = q;
			}
		}
		else {
			Polynom* q;
			q = new Polynom;
			q->deg = second->deg;
			q->coef = -second->coef;
			q->next = nullptr;
			second = second->next;
			if (tail == nullptr) {
				tail = q;
			}
			else {
				tail->next = q;
				tail = q;
			}
		}
		if (head == nullptr) {
			head = tail;
		}
	}
	if (first == nullptr) {
		while (second != nullptr) {
			Polynom* q;
			q = new Polynom;
			q->deg = second->deg;
			q->coef = -second->coef;
			q->next = nullptr;
			tail->next = q;
			tail = q;
			second = second->next;
		}
	}
	else {
		while (first != nullptr) {
			Polynom* q;
			q = new Polynom;
			q->deg = first->deg;
			q->coef = first->coef;
			q->next = nullptr;
			tail->next = q;
			tail = q;
			first = first->next;
		}
	}
	return head;
}

Polynom* polynom_minus(int i, int j, Polynom_database*& base_head) {
	Polynom* result = nullptr;
	Polynom* first = nullptr, * second = nullptr;
	Polynom_database* base_iter = base_head;
	while (base_iter != nullptr) {
		if ((base_iter->index) == i) {
			first = base_iter->pol;
			break;
		}
		base_iter = base_iter->next;
	}
	base_iter = base_head;
	while (base_iter != nullptr) {
		if ((base_iter->index) == j) {
			second = base_iter->pol;
			break;
		}
		base_iter = base_iter->next;
	}
	Polynom* answer = polynom_merge_minus(first, second);

		push_back_in_base(answer, base_head);
	return answer;
}


#endif // CALCULATOR_H_INCLUDED
