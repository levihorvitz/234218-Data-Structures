//
// Created by osama on 11/16/2021.
//

#ifndef HW1_WET_COMPETITION_H
#define HW1_WET_COMPETITION_H

#include "avl_tree.h"
#include <exception>
#include <iostream>

class GroupNode;
class PlayerNode;

class PlayerNode{
private:
    const int id;
    GroupNode* group;
    int level;
    int salary;
public:
    PlayerNode(int id, GroupNode& group, int level, int salary);
    void increaseSalary(int salaryInc, int bump);
    void changeGroup(GroupNode& group);
    GroupNode& getGroup() const;
    int getSalary() const;
    int getLevel() const;
    int getId() const;
    class idLess{
    public:
        bool operator()(const PlayerNode& player1, const PlayerNode& player2);
    };
    class salaryLess{
    public:
        bool operator()(const PlayerNode& player1, const PlayerNode& player2);
    };



};

class GroupNode{
private:
    const int id;
    int value;
    AvlTree<PlayerNode, PlayerNode::idLess> idTree;
    AvlTree<PlayerNode, PlayerNode::salaryLess> salaryTree;
public:
    explicit GroupNode(int id, int value);
    ~GroupNode() = default;
    int getValue() const;
    int getPlayersNum() const;
    int getID() const;
    bool isEmpty();
    void increaseValue(int inc);
    void addPlayer(PlayerNode& player);
    void removePlayer(int playerId);
    PlayerNode& getPlayer(int id);
    PlayerNode& getHighest();
    int* getAllPlayersBySalary(int* size) const;
    void merge(GroupNode& toCopy, double factor);
    void updatePlayersGroup();
    void getNumPlayersMatching(int minID, int maxId, int minSalary, int minLvl, int *totalNumOfPlayers,
                                       int *numOfPlayers);
    class idLess{
    public:
        bool operator()(const GroupNode& group1, const GroupNode& group2);
    };
};

class Competition{
private:
    AvlTree<PlayerNode, PlayerNode::idLess> playerIdTree;
    AvlTree<PlayerNode, PlayerNode::salaryLess> playerSalaryTree;
    AvlTree<GroupNode, GroupNode::idLess> groupIdTree;
    AvlTree<GroupNode, GroupNode::idLess> nonEmptyGroupTree;
public:
    Competition();
    ~Competition();
    GroupNode& getGroup(int id);
    PlayerNode& getPlayer(int id);
    void addGroup(int id, int value);
    void removeGroup(int id);
    void addPlayer(int playerId, int groupId, int lvl, int salary);
    void removePlayer(int id);
    void getGroupInfo(int groupID, int* value, int* NumPlayers);
    void getPlayerInfo(int playerID, int *groupID, int *salary, int *lvl);
    void increaseGroupValue(int groupId, int valInc);
    void movePlayer(int playerId, int groupId);
    void replaceGroup(int oldId, int newId, double factor);
    void increaseSalary(int playerId, int salaryInc, int bump);
    const PlayerNode& getHighestSalaryPlayerInGroup(int groupId);
    int* getAllPlayersBySalary(int groupId, int* size);
    int* GetGroupsHighestSalary (int numOfGroups);
    void getNumPlayersMatching(int groupID, int minID, int
            maxId, int minSalary, int minLvl, int *totalNumOfPlayers, int *numOfPlayers);
};

class Failure : public std::exception{};

#endif
//HW1_WET_COMPETITION_H
