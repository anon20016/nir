#include <fstream>
#include <iostream>
#include <algorithm>
#include <vector>
#include <set>
#include <map>
#include <string>
#include <ctime>

typedef long long li;
using namespace std;

#define forn(i, n) for(int i = 0; i < int(n); ++i)
#define fore(i, l, r) for(int i = int(l); i < int(r); ++i)

li StrToInt(string test) {			// Конвертер строка в число
	li res = 0;
	for (auto i : test) {
		res = res * 10 + (i - '0');
	}
	return res;
}
string IntToStr(li test) {			// Конвертер число в строку
	string res = "";
	while (test) {
		res = (char)(test % 10 + '0') + res;
		test /= 10;
	}
	return res;
}

string CharGraph6ToMatrix(char q) {	// Перевод символа в строку бит
	int temp = q - 63;
	string res = "";
	for (int i = (1 << 5); i > 0; i = i >> 1) {
		if (temp & i) {
			res = res + '1';
		}
		else {
			res = res + '0';
		}
	}
	return res;
}
vector<vector<int> > Graph6ToMatrix(string q) {	// Перевод формата graph6 в матрицу смежности
	int n = q[0] - 'A' + 2;
	vector<vector<int> > res(n, vector<int>(n, 0));
	string p = "";
	for (int i = 1; i < q.size(); i++) {
		p = p + CharGraph6ToMatrix(q[i]);
	}
	int t = 0;
	for (int j = 1; j < n; j++) {
		forn(i, j) {
			res[i][j] = p[t] - '0';
			res[j][i] = p[t] - '0';
			t++;
		}
	}
	vector<vector<int> > a(n, vector<int>());
	for (int i = 0; i < n; i++) {
		for (int j = i + 1; j < n; j++) {
			if (res[i][j] == 1) {
				a[i].push_back(j);
				a[j].push_back(i);
			}
		}
	}
	return(a);
}

//const vector<li> C = { 0, 0, 1, 2, 4, 12, 56, 456, 6880, 191536, 9733056, 903753248 };

vector<int> used, tin, fup;
vector<vector<int> > g;

void dfsS(int v, int p, int & result, int & timer) {
	used[v] = true;
	tin[v] = fup[v] = timer++;
	int children = 0;
	for (size_t i = 0; i < g[v].size(); ++i) {
		int to = g[v][i];
		if (to == p)  continue;
		if (used[to])
			fup[v] = min(fup[v], tin[to]);
		else {
			dfsS(to, v, result, timer);
			fup[v] = min(fup[v], fup[to]);
			if (fup[to] >= tin[v] && p != -1) {
				result++;
			}
			++children;
		}
	}
	if (p == -1 && children > 1) {
		result++;
	}
}

void dfsM(int v, int p, int& result, int& timer) {
	used[v] = true;
	tin[v] = fup[v] = timer++;
	for (size_t i = 0; i < g[v].size(); ++i) {
		int to = g[v][i];
		if (to == p)  continue;
		if (used[to])
			fup[v] = min(fup[v], tin[to]);
		else {
			dfsM(to, v, result, timer);
			fup[v] = min(fup[v], fup[to]);
			if (fup[to] > tin[v]) {
				result++;
			}
		}
	}
}

void color(int v, vector<int> c, int t, int p, int & result) {
	if (result != 0) {
		return;
	}
	set<int> s;
	for (int i = 0; i < t; i++) {
		s.insert(i + 1);
	}
	for (int i = 0; i < g[v].size(); i++) {
		if (c[g[v][i]] != 0) {
			s.erase(c[g[v][i]]);
		}
	}
	if (p == c.size() - 1 && s.size() > 0) {
		result = t;
		return;
	}
	for (auto i : s) {
		c[v] = i;
		for (int i = 0; i < g.size(); i++) {
			if (c[i] == 0) {
				color(i, c, t, p + 1, result);
			}
		}
	}	
}

int main()
{
	ofstream out("output.txt");						 // Файл для вывода результатов
	fore(n, 2, 7) {
		unsigned int start_time = clock();			 // начальное время
		string path = "input" + IntToStr(n) + ".txt";
		ifstream in(path);		
		string s;
		map<pair<int, pair<int, int> >, int> m;
		while (getline(in, s)) {
			g = Graph6ToMatrix(s);
			int bridges = 0, timer1 = 0, timer2 = 0, points = 0, mincolor = 0;
			used.clear();
			tin.clear();
			fup.clear();
			used.resize(n, 0);
			tin.resize(n, 0);
			fup.resize(n, 0);

			for (int w = 0; w < n; w++) {
				if (used[w] == 0) {
					dfsS(0, -1, points, timer1);
				}
			}
			used.clear();
			tin.clear();
			fup.clear();
			used.resize(n, 0);
			tin.resize(n, 0);
			fup.resize(n, 0);
			for (int w = 0; w < n; w++) {
				if (used[w] == 0) {
					dfsM(0, -1, bridges, timer2);
				}
			}
			for (int j = 1; j <= n; j++) {
				vector<int> c(g.size(), 0);
				for (int w = 0; w < n; w++) {
					if (c[w] == 0) {
						color(w, c, j, 0, mincolor);
					}
				}
			}
			m[{bridges, { points, mincolor }}]++;
			if (mincolor == 0) {
				int r;
			}
		}
		out << "Ответ для n = " << n << endl;
	
		for (auto i : m) {
			out << i.first.second.first << " т/с, " << i.first.first << " м, " << i.first.second.second << " хром/ч - " << i.second << " граф" << endl;
		}
		out << endl;
	}
	return 0;
}
