#include "library2.h"
#include "PlayersManager.h"


void *Init(int k, int scale)
{
    if(k<=0 || scale > 200|| scale <= 0)
    {
        return NULL;
    }
    PlayersManager *DS = new PlayersManager(k,scale);
    return (void*)DS;
}

StatusType MergeGroups(void *DS, int GroupID1, int GroupID2)
{
    if(DS == NULL)
    {
        return INVALID_INPUT;
    }
    StatusTypeManager result = SUCCESS_MANAGER;
    try{
        result = ((PlayersManager*)DS)->MergeGroupsManager(GroupID1,GroupID2);
    }
    catch(std::exception& e)
    {
        return ALLOCATION_ERROR;
    }
    if(result == INVALID_INPUT_MANAGER)
    {
        return INVALID_INPUT;
    }
    return SUCCESS;

}

StatusType AddPlayer(void *DS, int PlayerID, int GroupID, int score)
{
    if(DS == NULL)
    {
        return INVALID_INPUT;
    }
    StatusTypeManager result = SUCCESS_MANAGER;
    try{
        result = ((PlayersManager*)DS)->AddPlayerManager(PlayerID,GroupID,score);
    }
    catch(std::exception& e)
    {
        return ALLOCATION_ERROR;
    }
    if(result == INVALID_INPUT_MANAGER)
    {
        return INVALID_INPUT;
    }
    if(result == FAILURE_MANAGER)
    {
        return FAILURE;
    }
    return SUCCESS;
}

StatusType RemovePlayer(void *DS, int PlayerID)
{
    if(DS == NULL)
    {
        return INVALID_INPUT;
    }
    StatusTypeManager result = SUCCESS_MANAGER;
    try{
        result = ((PlayersManager*)DS)->RemovePlayerManager(PlayerID);
    }
    catch(std::exception& e)
    {
        return ALLOCATION_ERROR;
    }
    if(result == INVALID_INPUT_MANAGER)
    {
        return INVALID_INPUT;
    }
    if(result == FAILURE_MANAGER)
    {
        return FAILURE;
    }
    return SUCCESS;
}

StatusType IncreasePlayerIDLevel(void *DS, int PlayerID, int LevelIncrease)
{
    if(DS == NULL)
    {
        return INVALID_INPUT;
    }
    StatusTypeManager result = SUCCESS_MANAGER;
    try{
        result = ((PlayersManager*)DS)->IncreasePlayerIDLevelManager(PlayerID,LevelIncrease);
    }
    catch(std::exception& e)
    {
        return ALLOCATION_ERROR;
    }
    if(result == INVALID_INPUT_MANAGER)
    {
        return INVALID_INPUT;
    }
    if(result == FAILURE_MANAGER)
    {
        return FAILURE;
    }
    return SUCCESS;
}

StatusType ChangePlayerIDScore(void *DS, int PlayerID, int NewScore)
{
    if(DS == NULL)
    {
        return INVALID_INPUT;
    }
    StatusTypeManager result = SUCCESS_MANAGER;
    try{
        result = ((PlayersManager*)DS)->ChangePlayerIDScoreManager(PlayerID,NewScore);
    }
    catch(std::exception& e)
    {
        return ALLOCATION_ERROR;
    }
    if(result == INVALID_INPUT_MANAGER)
    {
        return INVALID_INPUT;
    }
    if(result == FAILURE_MANAGER)
    {
        return FAILURE;
    }
    return SUCCESS;
}

StatusType GetPercentOfPlayersWithScoreInBounds(void *DS, int GroupID, int score, int lowerLevel, int higherLevel,
                                            double * players)
                                            {
                                                if(DS == NULL)
    {
        return INVALID_INPUT;
    }
    StatusTypeManager result = SUCCESS_MANAGER;
    try{
        result = ((PlayersManager*)DS)->GetPercentOfPlayersWithScoreInBoundsManager(GroupID,score,lowerLevel,higherLevel,players);
    }
    catch(std::exception& e)
    {
        return ALLOCATION_ERROR;
    }
    if(result == INVALID_INPUT_MANAGER)
    {
        return INVALID_INPUT;
    }
    if(result == FAILURE_MANAGER)
    {
        return FAILURE;
    }
    return SUCCESS;
                                            }

StatusType AverageHighestPlayerLevelByGroup(void *DS, int GroupID, int m, double * level)
{
    if(DS == NULL)
    {
        return INVALID_INPUT;
    }
    StatusTypeManager result = SUCCESS_MANAGER;
    try{
        result = ((PlayersManager*)DS)->AverageHighestPlayerLevelByGroupManager(GroupID, m,level);
    }
    catch(std::exception& e)
    {
        return ALLOCATION_ERROR;
    }
    if(result == INVALID_INPUT_MANAGER)
    {
        return INVALID_INPUT;
    }
    if(result == FAILURE_MANAGER)
    {
        return FAILURE;
    }
    return SUCCESS;
}
StatusType GetPlayersBound(void *DS, int GroupID, int score, int m,
                                         int * LowerBoundPlayers, int * HigherBoundPlayers)
{
    return SUCCESS;
}

void Quit(void** DS)
{
    if(DS)
    {
        if(*DS)
        {
            delete((PlayersManager*)*DS);
            *DS=NULL;
        }
    }
}