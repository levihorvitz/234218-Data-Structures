#ifndef AVLTREE_H_
#define AVLTREE_H_
#include <stdio.h>
#include <iostream>
#include <math.h>

//node level
class Node
{
public:
    int level;
    int height;
    Node* left;
    Node* right;
    Node* parent;
    int average;
    int* scores;
    int* sumScores;
    Node():level(0), height(0) ,left(NULL), right(NULL), parent(NULL),average(0)
    {
        scores = new int [201];
        sumScores = new int [201];
        for(int i=0; i<201; i++)
        {
            scores[i]=0;
            sumScores[i]=0;
        }
    }
    Node(int level,Node*parent) : level(level), height(0) ,left(NULL), right(NULL), parent(parent),average(0)
    {
        scores = new int [201];
        sumScores = new int [201];
        for(int i=0; i<201; i++)
        {
            scores[i]=0;
            sumScores[i]=0;
        }
    }
    void operator=(const Node &t)
    {
        this->level=t.level;
        for (int i = 0; i < 201; i++)
        {
            this->scores[i]=t.scores[i];
        }
        
    }
    ~Node()
    {
        delete[] scores;
        delete[] sumScores;
    }
};

//avl level
class AVLTree
{
    public:
    Node* head;
    int size;
    //constructor
    AVLTree(): head(NULL),size(0){}
    //constructor for merge
    AVLTree(int height,int toDelete);
    //add and its help function
    void add(int addValue);
    // remove function
    void remove (int removeValue);
    //merge trees of pointers
    AVLTree* mergedAVL(AVLTree* dis,AVLTree* src);
    //isExist
    bool isExist(int level);
    //destructor
    ~AVLTree();
    void clearTree();
    /*
    //for debugging
    void printDetails();
    void printDetailsAux(Node* head);
    */
    //for debug
    void printPlayers(Node* head);
    //for debugging
    void printGroup(Node* head, int &size); 
    Node* binarySearch(Node* head, int addValue);
    void fillArray(Node array[],Node* head,int& index,int size);
    void inOrder(Node* head,int& size,int& index,int arr[]);
    
    //add help function

    void AbalanceTree(Node *current);
    int calculateBF(Node* node);
    int getHeight(Node* node);
    void balanceTree(Node *current, int curBF);
    void rotateRight(Node* damagedHead);
    int max(int a,int b)
    {
        return (a>b) ? a:b;
    }
    void rotateLeft(Node* damagedHead);
    void updateHead();
    //remove function and its help functions
    int getSonsNum(Node *head);
    void removeIfNoSons(Node* removeNode);
    void RbalanceTree(Node *current);
    void removeIfOneSon(Node *removeNode);
    //merge trees help function
    int mergeSort(Node mergedArray[],Node array1[],int size1,Node array2[],int size2);
    void fillSemiFull(Node* semiFullTree ,Node array[],int& index, int size);
    // consrtuctor that builds a semifull tree filled with NULL
    Node* fullTree(int height,Node* tree, Node* parent=NULL);
    void semiFull(Node* head,int& toDelete);
    //destructor help function
    void clearTreeAux(Node* head);
    

    // for ranks
    void updateRanks(Node* damagedHead,Node* damagedHeadRight,Node* headLeft,Node* headLeftLeft,Node* headLeftRight);
    void addPlayer(int level,int score);
    void updateRouteRank(Node* start, Node* removed);
    void removePlayer(int level, int score);
    void calcuateNewRanks(Node* tree);
    int RankScore(int level, int score);
    int RankScoreAux(Node* head,int level,int r, int score);
    int select(Node* head,int m, int* players);
    int RankAvg(int level);
    int RankAvgAux(Node* head,int level,int r);
};

#endif