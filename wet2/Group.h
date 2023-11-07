#ifndef GROUP_H_
#define GROUP_H_
#include "AVLTree.h"
#include <iostream>

class Group
{
    public:
        AVLTree* players;
        int* zero;   
        Group();
        void addPlayerToGroup(int level, int score);
        void removePlayerFromGroup(int level, int score);
        int getNumPlayersInBounds(int lowerLevel, int higherLevel, int score);
        int getNumPlayers();
        int select(int m , int* numPlayers);
        ~Group();
};

#endif