#include "Group.h"


Group::Group()
{
    players=new AVLTree();
    zero = new int [201];
    for(int i=0; i<201; i++)
    {
        zero[i]=0;
    }
}
void Group::addPlayerToGroup(int level, int score)
{
    if(level==0)
    {
        zero[score]++;
        zero[0]++;
        return;
    }
    players->addPlayer(level,score);
}
void Group::removePlayerFromGroup(int level, int score)
{
    if(level==0)
    {
        zero[score]--;
        zero[0]--;
        return;
    }
    players->removePlayer(level,score);
}
int Group::getNumPlayersInBounds(int lowerLevel, int higherLevel, int score)
{
    if (players->head==NULL)
    {
        if(lowerLevel <= 0)
        {
            return zero[score];
        }
        return 0;
    }
    Node* lowNode = players->binarySearch(players->head, lowerLevel);
    Node* highNode = players->binarySearch(players->head, higherLevel);
    int lowplayers = players->RankScore(lowNode->level,score);
    int highplayers = players->RankScore(highNode->level,score);
    int numPlayers = highplayers-lowplayers;
    if(lowNode->level >= lowerLevel)
    {
        numPlayers+=lowNode->scores[score];
    }
    if(highNode->level > higherLevel)
    {
        numPlayers-=highNode->scores[score];
    }
    if(lowerLevel <= 0 && higherLevel>=0)
    {
        numPlayers+=zero[score];
    }
    return numPlayers;
}
int Group::getNumPlayers()
{
    if (players->head==NULL)
    {
        return zero[0];
    }
    return players->head->sumScores[0]+zero[0];
}
int Group::select(int m, int* numPlayers)
{
    return players->select(players->head, m, numPlayers);
}
Group::~Group()
{
    delete players;
    delete[] zero;
}