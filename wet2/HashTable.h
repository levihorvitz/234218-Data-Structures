#ifndef HashTable_H_
#define HashTable_H_
#include <stdio.h>
#include <iostream>
#include <math.h>

class Player
{
public:
   int playerID;
   int group;
   int score;
   Player* next;
   int level;
   Player(int playerID,int group,int score,int level) : playerID(playerID), group( group), score(score),next(NULL), level(level) {}
   ~Player()=default;
};

class HashTable
{
    public:
    int hashSize;
    int playerSize;
    Player** array;

    HashTable() : hashSize(16),playerSize(0)
    {
        this->array=new Player*[hashSize];
        for(int i=0;i<hashSize;i++)
        {
            array[i]= new Player(-1,-1,-1,0);
        }
    }

    int hashFunc(int playerID);
    bool exist (int playerID);
    void expand(double factor);
    void add (int playerID,int group,int score,int level=0);
    void remove (int playerID);
    void deletePlayers(Player* p);
    int getGroup(int playerID);
    int getLevel(int playerID);
    int getScore(int playerID);
    ~HashTable();
};





#endif