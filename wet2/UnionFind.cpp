#include "UnionFind.h"
#include <iostream>

using namespace std;


UnionFind::UnionFind(int k) : k(k)
{
	parent = new int[k];
	size = new int[k];
	for (int i = 0; i < k; i++) {
		parent[i] = -1;
		size[i] = 1;
	}
}

UnionFind::~UnionFind()
{
	delete[] parent;
	delete[] size;
}

int UnionFind::find(int group)
{
	int i = group - 1;
	while (parent[i] != -1) {
		i = parent[i];
	}
	reducePath(group, i);
	return i + 1;
}

void UnionFind::reducePath(int group, int root)
{
	int temp = group - 1;
	while (temp != root && temp != -1) {
		int pastParent = parent[temp];
		parent[temp] = root;
		temp = pastParent;
	}
}

int UnionFind::uniongroups(int group1, int group2)
{
	int parent1 = find(group1) - 1;
	int parent2 = find(group2) - 1;
	if (parent1 == parent2) {
		return parent1 + 1;
	}
	if (size[parent1] < size[parent2]) {
		parent[parent1] = parent2;
		size[parent2] += size[parent1];
		size[parent1] = -1;
		return parent2 + 1;
	} else {
		parent[parent2] = parent1;
		size[parent1] += size[parent2];
		size[parent2] = -1;
	}
	return parent1 + 1;
}
