#ifndef UNIONFIND_H_
#define UNIONFIND_H_
#include <stdio.h>
#include <iostream>
#include <math.h>

class UnionFind
{
    public:
    int* parent;
    int* size;
    int k;
    UnionFind(int k);
    int find(int group);
    void reducePath(int group,int root);
    int uniongroups(int group1,int group2);
    ~UnionFind();  
};


#endif