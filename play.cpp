#include <bits/stdc++.h>
using namespace std;

const int OUTPUT_WIDTH = 3;

#define ASSERT_EXCEPTION(cond, msg) do { if (!(cond)) throw msg; } while(0)

typedef bool T;
function<T(vector<T>)> f_equiv = [](vector<T> input) {
  return input[0] == input[1];
};
function<T(vector<T>)> f_implies = [](vector<T> input) {
  return !input[0] || input[1];
};
function<T(vector<T>)> f_or = [](vector<T> input) {
  return input[0] || input[1];
};
function<T(vector<T>)> f_and = [](vector<T> input) {
  return input[0] && input[1];
};
function<T(vector<T>)> f_not = [](vector<T> input) {
  return !input[0];
};
function<T(vector<T>)> f_id = [](vector<T> input) {
  return input[0];
};

struct Operator {
  string name;
  int n_operands;
  function<T(vector<T>)> calculate;
  Operator(string name, int n_operands, function<T(vector<T>)> calc)
    : name(name), n_operands(n_operands), calculate(calc) {}
};
bool operator == (const Operator &A, const Operator &B) {
  return A.name == B.name;
}
const Operator o_equiv("=", 2, f_equiv);
const Operator o_implies(">", 2, f_implies);
const Operator o_or("v", 2, f_or);
const Operator o_and("^", 2, f_and);
const Operator o_not("!", 1, f_not);
const Operator o_id("?", 1, f_id);
const vector<Operator> operators = {o_equiv, o_implies, o_or, o_and, o_not};

const vector<T> value_set = {0, 1};
vector<string> symbol_set;
map<string, T> value_of_symbols;

struct Node {
  vector<Node*> chs;
  Operator op;
  string symbol;
  Node(Operator op) : op(op), symbol("") {}
  Node(string symbol) : op(o_id), symbol(symbol) {}
  T value_cache;
  T evaluate() {
    if (op == o_id) {
      return value_cache = value_of_symbols[symbol];;
    } else {
      vector<T> operands(chs.size());
      for (size_t i = 0; i < chs.size(); ++i) operands[i] = chs[i]->evaluate();
      return value_cache = op.calculate(operands);
    }
  }
  void print_heading() {
    if (op == o_id) {
      cout << setw(OUTPUT_WIDTH) << symbol;
    } else {
      cout << setw(OUTPUT_WIDTH) << op.name;
      for (auto ch : chs) ch->print_heading();
    }
  }
  void print_content() {
    if (op == o_id) {
      cout << setw(OUTPUT_WIDTH) << value_cache;
    } else {
      cout << setw(OUTPUT_WIDTH) << value_cache;
      for (auto ch : chs) ch->print_content();
    }
  }
};
int locate(string s, string op) {
  int dep = 0;
  for (int i = 0; i < (int)s.size(); ++i) {
    if (s[i] == '(') dep++;
    if (s[i] == ')') dep--;
    if (dep == 0 && s.substr(i, op.size()) == op)
      return i;
  }
  return -1;
}
Node* resolve(string s) {
  // find an operator
  if (s.size() == 0) return nullptr;
  
  for (auto op : operators) {
    int pos = locate(s, op.name);
    if (pos == -1) continue;
    Node* u = new Node(op); // an operator
    // cerr << op.name << endl;
    Node *L = resolve(s.substr(0, pos));
    Node *R = resolve(s.substr(pos + op.name.size()));
    if (L) u->chs.push_back(L);
    if (R) u->chs.push_back(R);

    if (op.n_operands == 2) ASSERT_EXCEPTION(L && R,
      "An occurrence of " + op.name + "got wrong numbers of input: "
      + (L ? "" : "L missed ") + (R ? "" : "R missed"));
    if (op.n_operands == 1) ASSERT_EXCEPTION(!L && R,
      "An occurrence of " + op.name + "got wrong input: "
      + (L ? "L shouldnt be there" : "") + (R ? "" : "R missed"));
    return u;
  }
  // no operands
  if (s[0] == '(' && s.back() == ')') {
    return resolve(s.substr(1, s.size() - 2));
  }
  // a symbol
  Node *u = new Node(s);
  // cerr << "symbol" << endl;
  symbol_set.push_back(s);
  return u;
}

void trimspaces(string &s) // from Internet
{
	int index = 0;
	if(!s.empty())
	{
		while( (index = s.find(' ',index)) != string::npos)
		{
			s.erase(index,1);
		}
	}
}

bool checkBracesMatch(string s) {
  int dep = 0;
  for (size_t i = 0; i < s.size(); ++i) {
    if (s[i] == '(') dep++;
    if (s[i] == ')') dep--;
    if (s[i] < 0) return false;
  }
  return dep == 0;
}

bool checkAlphabet(string s) {
  set<char> alphabet;
  for (char c = 'a'; c <= 'z'; ++c) alphabet.insert(c);
  for (char c = 'A'; c <= 'Z'; ++c) alphabet.insert(c);
  for (auto op : operators) for (char c : op.name) alphabet.insert(c);
  alphabet.insert('(');
  alphabet.insert(')');
  alphabet.insert(' ');
  for (char c : s) if (!alphabet.count(c)) return false;
  return true;
}

Node *root;
void print_heading() {
  cout << "format: symbols | prefix notation" << endl;
  for (string s : symbol_set) {
    cout << setw(OUTPUT_WIDTH) << s;
  }
  cout << setw(OUTPUT_WIDTH) << "|";
  root->print_heading();
  cout << endl;
}

void print_content() {
  for (string s : symbol_set) {
    cout << setw(OUTPUT_WIDTH) << value_of_symbols[s];
  }
  cout << setw(OUTPUT_WIDTH) << "|";
  root->print_content();
  cout << endl;
}

void dfs(int depth) {
  if (depth == symbol_set.size()) {
    root->evaluate();
    print_content();
  } else {
    for (auto val : value_set) {
      value_of_symbols[symbol_set[depth]] = val;
      dfs(depth + 1);
    }
  }
}

int onecall() {
  cout << "Please input a logical expression, with =>^v! as five possible operators(in ascending priority)," << endl;
  cout << "any combination of uppercase/lowercase string(except for 'v') as symbols." << endl;
  string s;
  getline(cin, s);
  // if (s == "STOP") return -1;
  if (!checkAlphabet(s)) {
    cerr << "Fail: Bad alphabet" << endl;
    cerr << "Should be a~z, A~Z, space, (), or the following operators:" << endl;
    for (auto op : operators) cerr << op.name << " ";
    cerr << endl;
    return 1;
  }
  if (!checkBracesMatch(s)) {
    cerr << "Fail: Braces not matching." << endl;
    return 2;
  }

  trimspaces(s);
  symbol_set.clear();
  value_of_symbols.clear();
  try {
    root = resolve(s);
  } catch (string e) {
    cerr << "Error: " << e << endl;
    cerr << "Anyway your expression is flawed. Bye." << endl;
    return 3;
  }
  cerr << "Expression resolved" << endl;
  // with the expression tree, lets make it happen
  sort(symbol_set.begin(), symbol_set.end());
  symbol_set.erase(unique(symbol_set.begin(), symbol_set.end()), symbol_set.end());
  print_heading();
  dfs(0);
  return 0;
}

int main() {
  int res = 99;
  while (res) {
    res = onecall();
  }
  return 0;
}
