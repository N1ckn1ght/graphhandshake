#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <string>
#include <queue>
#include <map>

using namespace std;
typedef unsigned int unt;

const string inf = "256497288.edges";
const string ouf = "./output.txt";
//const string inf = "example_input.txt";
//const string ouf = "example_output.txt";

struct node
{
	unt n;	// node id
	unt d;	// distance
};

vector <vector <unt>> graph;
map <string, unt> nodes;
map <unt, string> names;
vector <bool> used;
queue <node> q;

unt cnt;
vector <unt> linker;
// 0 is for NO LINK.
// 13 is for ANY NUMBER > 12.

void bfs()
{
	while (q.size() != 0)
	{
		node cur = q.front();
		q.pop();

		if (cur.d > 0)
		{
			cnt++;
			if (cur.d > 12)
			{
				linker[13]++;
			}
			linker[cur.d]++;
		}

		for (unt next : graph[cur.n])
		{
			if (!used[next])
			{
				used[next] = true;
				q.push({ next, cur.d + 1 });
			}
		}
	}
}

bool read(vector <vector <unt>>& g, map <string, unt> toNode, map <unt, string>& toName, string path)
{
	ifstream in(path);

	if (!in.is_open())
	{
		return false;
	}

	string first, second;
	unt cur_size = 0;

	while (in >> first)
	{
		in >> second;

		if (nodes.find(first) == nodes.end())
		{
			names[cur_size] = first;
			nodes[first] = cur_size++;
		}
		if (nodes.find(second) == nodes.end())
		{
			names[cur_size] = second;
			nodes[second] = cur_size++;
		}

		g.resize(cur_size);
		g[nodes[first]].push_back(nodes[second]);
		g[nodes[second]].push_back(nodes[first]);
	}
	in.close();

	return true;
}

void getresult(string &result)
{
	result += "Amount of vertices:\t" + to_string(graph.size()) + "\n";
	result += "NO LINK:\t\t" + to_string(linker[0] / 2) + "\n";
	result += "1 handshake:\t\t" + to_string(linker[1] / 2) + "\n";
	for (int i = 2; i < 13; i++)
	{
		result += to_string(i) + " handshakes:\t\t" + to_string(linker[i] / 2) + "\n";
	}
	result += "Too many handshakes:\t" + to_string(linker[13] / 2) + "\n";
}

int main()
{
	bool r = read(graph, nodes, names, inf);
	if (!r)
	{
		cout << "Kernel panic!\nFile " << inf << " does not exist!\n";
		return 0;
	}

	linker.resize(14, 0);
	for (unt i = 0; i < graph.size(); i++)
	{
		cnt = 0;
		used.clear();
		used.resize(graph.size(), false);

		used[i] = true;
		q.push({ i, 0 });
		bfs();

		linker[0] += (graph.size() - 1 - cnt);
	}

	string result;
	getresult(result);
	ofstream out(ouf);
	out << result;
	cout << result;
	out.close();
}