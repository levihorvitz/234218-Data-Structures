#ifndef PLAYERSMANAGER_H_
#define PLAYERSMANAGER_H_
#include "AVLTree.h"
#include "HashTable.h"
#include "UnionFind.h"
#include "Group.h"
#include <iostream>

typedef enum {
    SUCCESS_MANAGER = 0,
    FAILURE_MANAGER = -1,
    ALLOCATION_ERROR_MANAGER = -2,
    INVALID_INPUT_MANAGER = -3
} StatusTypeManager;


class PlayersManager
{
    HashTable allPlayersTable;
    UnionFind alliances;
    Group allPlayersGroup;
    Group* allGroups;
    int k;
    int scale;
    public:
    PlayersManager(int k,int scale):alliances(k),k(k),scale(scale)
    {
        allGroups = new Group[k];
    }
    StatusTypeManager MergeGroupsManager(int GroupID1, int GroupID2);
    StatusTypeManager AddPlayerManager( int PlayerID, int GroupID, int score);
    StatusTypeManager RemovePlayerManager(int PlayerID);
    StatusTypeManager IncreasePlayerIDLevelManager(int PlayerID, int LevelIncrease);
    StatusTypeManager ChangePlayerIDScoreManager(int PlayerID, int NewScore);
    StatusTypeManager GetPercentOfPlayersWithScoreInBoundsManager(int GroupID, int score, int lowerLevel, int higherLevel,
                                                            double * players);

    StatusTypeManager AverageHighestPlayerLevelByGroupManager(int GroupID, int m, double * level);
    ~PlayersManager();
};

#endif