#include "competition.h"
#include <cstdlib>
#include <iostream>
#include <cmath>

void getNumPlayersMatchingInTree(AvlTree<PlayerNode,PlayerNode::idLess>& tree, int minID, int maxId, int minSalary, int minLvl, int *totalNumOfPlayers,
                           int *numOfPlayers);

PlayerNode::PlayerNode(int id, GroupNode &group, int level, int salary)
        :id(id),
         group(&group),
         level(level),
         salary(salary)
{}

void PlayerNode::increaseSalary(int salaryInc, int bump) {
    salary += salaryInc;
    if (bump > 0){
        level++;
    }
}

void PlayerNode::changeGroup(GroupNode &group) {
    this->group = &group;
}

GroupNode& PlayerNode::getGroup() const {
    return *group;
}

int PlayerNode::getId() const {
    return id;
}

int PlayerNode::getSalary() const {
    return salary;
}

int PlayerNode::getLevel() const {
    return level;
}

bool PlayerNode::idLess::operator()(const PlayerNode& player1, const PlayerNode& player2) {
    return (player1.id < player2.id);
}

bool PlayerNode::salaryLess::operator()(const PlayerNode& player1, const PlayerNode& player2) {
    if(player1.salary == player2.salary){
        return player1.id > player2.id;
    }else{
        return player1.salary < player2.salary;
    }
}


GroupNode::GroupNode(int id, int value)
        : id(id),
          value(value),
          idTree(PlayerNode::idLess()),
          salaryTree(PlayerNode::salaryLess())
{}

void GroupNode::addPlayer(PlayerNode &player) {
    idTree.add(player);
    salaryTree.add(player);
}
PlayerNode& GroupNode::getPlayer(int id){
    PlayerNode temp(id, *this, 0, 0);
    return idTree.find(temp);

}

void GroupNode::removePlayer(int playerId) {
    PlayerNode& toDelete = getPlayer(playerId);
    idTree.remove(toDelete);
    salaryTree.remove(toDelete);
}

PlayerNode &GroupNode::getHighest() {
    return salaryTree.getHighest();
}

bool GroupNode::idLess::operator()(const GroupNode &group1, const GroupNode &group2) {
    return (group1.id < group2.id);
}

int* GroupNode::getAllPlayersBySalary(int* size) const{
    int arr_size = salaryTree.getSize();
    if (arr_size == 0){
        throw TreeEmpty();
    }
    int* arr = (int*) malloc(sizeof(int) * arr_size);
    if (arr == nullptr){
        throw std::bad_alloc();
    }
    int* cur = arr;
    auto i = salaryTree.beginHighest();
    while(i != salaryTree.end()){
        *cur = (*i).getId();
        --i;
        cur++;
    }
    *size = arr_size;
    return arr;
}

bool GroupNode::isEmpty() {
    return idTree.isEmpty();
}

void GroupNode::merge(GroupNode &toCopy, double factor) {
    idTree.merge(toCopy.idTree);
    salaryTree.merge(toCopy.salaryTree);
    this->value = (int) floor((this->value + toCopy.value) * factor);
}

void GroupNode::updatePlayersGroup() {
    auto i = salaryTree.beginLowest();
    while(i != salaryTree.end()){
        (*i).changeGroup(*this);
        ++i;
    }
}

int GroupNode::getValue() const {
    return value;
}

void GroupNode::getNumPlayersMatching(int minID, int maxId, int minSalary, int minLvl, int *totalNumOfPlayers,
                                      int *numOfPlayers) {
    getNumPlayersMatchingInTree(this->idTree, minID, maxId, minSalary,
                                minLvl, totalNumOfPlayers, numOfPlayers);
}

int GroupNode::getPlayersNum() const {
    return this->idTree.getSize();
}

int GroupNode::getID() const {
    return id;
}

void GroupNode::increaseValue(int inc) {
    value += inc;
}

Competition::Competition()
        :playerIdTree(PlayerNode::idLess()),
         playerSalaryTree(PlayerNode::salaryLess()),
         groupIdTree(GroupNode::idLess()),
         nonEmptyGroupTree(GroupNode::idLess())
{}

GroupNode& Competition::getGroup(int id){
    GroupNode temp(id, 0);
    return groupIdTree.find(temp);
}



PlayerNode& Competition::getPlayer(int id){
    GroupNode tempGroup(0, 0);
    PlayerNode tempPlayer(id, tempGroup, 0, 0);
    return playerIdTree.find(tempPlayer);
}

void Competition::addGroup(int id, int value) {
    try{
        getGroup(id);
        throw NodeAlreadyExists();
    }catch(NodeNotExists&){
        //do nothing
    }
    GroupNode* toAddPtr = new GroupNode(id, value);
    try{
        groupIdTree.add(*toAddPtr);
    }catch(std::bad_alloc&){
        delete toAddPtr;
        throw std::bad_alloc();
    }

}

void Competition::addPlayer(int playerId, int groupId, int lvl, int salary) {
    try{
        getPlayer(playerId);
        throw NodeAlreadyExists();
    }catch(NodeNotExists&){
        //do nothing
    }
    GroupNode& toAddTo = getGroup(groupId);
    if (toAddTo.isEmpty()){
        nonEmptyGroupTree.add(toAddTo);
    }
    PlayerNode* toAddPtr = nullptr;
    try {
        toAddPtr = new PlayerNode(playerId, toAddTo, lvl, salary);
        toAddTo.addPlayer(*toAddPtr);
        playerIdTree.add(*toAddPtr);
        playerSalaryTree.add(*toAddPtr);
    }catch(std::bad_alloc&){
        nonEmptyGroupTree.remove(toAddTo);
        if (toAddPtr != nullptr) {
            try {
                toAddTo.removePlayer(toAddPtr->getId());
                playerIdTree.remove(*toAddPtr);
                playerSalaryTree.remove(*toAddPtr);
                delete toAddPtr;
            } catch (NodeNotExists &) {
                //do nothing
            }
        }
        nonEmptyGroupTree.remove(toAddTo);
        throw std::bad_alloc();
    }

}

void Competition::removePlayer(int id) {
    PlayerNode& toRemove = getPlayer(id);
    playerIdTree.remove(toRemove);
    playerSalaryTree.remove(toRemove);
    GroupNode& group = toRemove.getGroup();
    group.removePlayer(id);
    delete &toRemove;
    if (group.isEmpty()){
        nonEmptyGroupTree.remove(group);
    }
}

//an allocation error in this case almost always results in an invalid program state
void Competition::replaceGroup(int oldId, int newId, double factor) {
    GroupNode& oldGroup = getGroup(oldId);
    GroupNode& newGroup = getGroup(newId);
    if (oldGroup.getValue() * 10 > newGroup.getValue()){
        throw Failure();
    }
    if (newGroup.isEmpty() && !oldGroup.isEmpty()){
        nonEmptyGroupTree.add(newGroup);
    }
    newGroup.merge(oldGroup, factor);
    groupIdTree.remove(oldGroup);
    if (!oldGroup.isEmpty()) {
        nonEmptyGroupTree.remove(oldGroup);
    }
    newGroup.updatePlayersGroup();
    delete &oldGroup;
}

//in case of an allocation error the level tree may no longer be valid
void Competition::increaseSalary(int playerId, int salaryInc, int bump) {
    PlayerNode& toInc = getPlayer(playerId);
    playerSalaryTree.remove(toInc);
    toInc.getGroup().removePlayer(playerId);
    toInc.increaseSalary(salaryInc, bump);
    playerSalaryTree.add(toInc);
    toInc.getGroup().addPlayer(toInc);
}

const PlayerNode &Competition::getHighestSalaryPlayerInGroup(int groupId){
    if (groupId < 0){
        return playerSalaryTree.getHighest();
    }else{
        GroupNode& group = getGroup(groupId);
        return group.getHighest();
    }
}

int* Competition::getAllPlayersBySalary(int groupId, int *size){
    if (groupId < 0){
        int arr_size = playerSalaryTree.getSize();
        if (arr_size == 0){
            throw TreeEmpty();
        }
        int* arr = (int*) malloc(sizeof(int) * arr_size);
        if (arr == nullptr){
            throw std::bad_alloc();
        }
        int* cur = arr;
        auto i = playerSalaryTree.beginHighest();
        while(i != playerSalaryTree.end()){
            *cur = (*i).getId();
            --i;
            cur++;
        }
        *size = arr_size;
        return arr;
    }else{
        GroupNode& group = getGroup(groupId);
        return group.getAllPlayersBySalary(size);
    }
}

int* Competition::GetGroupsHighestSalary (int numOfGroups){
    int* arr = (int*) malloc(sizeof(int) * numOfGroups);
    if (arr == nullptr){
        throw std::bad_alloc();
    }
    int cur = 0;
    auto i = nonEmptyGroupTree.beginLowest();
    while(cur < numOfGroups){
        if (i == nonEmptyGroupTree.end()){
            free(arr);
            throw Failure();
        }
        GroupNode& group = (*i);
        PlayerNode& player = group.getHighest();
        arr[cur] = player.getId();
        ++i;
        cur++;
    }
    return arr;
}

Competition::~Competition() {
    //delete all players
    auto i_p = playerIdTree.beginLowest();
    while (i_p != playerIdTree.end()){
        delete &(*i_p);
        ++i_p;
    }
    //delete all groups
    auto i_g = groupIdTree.beginLowest();
    while(i_g != groupIdTree.end()){
        delete &(*i_g);
        ++i_g;
    }
}

void Competition::removeGroup(int id) {
    GroupNode& toDelete = getGroup(id);
    if (!toDelete.isEmpty()){
        throw GroupNotEmpty();
    }
    groupIdTree.remove(toDelete);
    delete &toDelete;
}

void getNumPlayersMatchingInTree(AvlTree<PlayerNode,PlayerNode::idLess>& tree, int minID, int maxId, int minSalary, int minLvl, int *totalNumOfPlayers,
                      int *numOfPlayers) {
    if (tree.isEmpty()){
        throw TreeEmpty();
    }
    int total = 0;
    int matching = 0;
    GroupNode tempGroup(0, 0);
    PlayerNode tempPlayer(minID,tempGroup, 0, 0);
    auto i = tree.findClosest(tempPlayer);
    if ((*i).getId() < minID){
        ++i;
    }
    while (i != tree.end() && (*i).getId() <= maxId){
        total++;
        if ((*i).getLevel() >= minLvl && (*i).getSalary() >= minSalary){
            matching++;
        }
        ++i;
    }
    *totalNumOfPlayers = total;
    *numOfPlayers = matching;

}
void
Competition::getNumPlayersMatching(int groupID, int minID, int maxId, int minSalary, int minLvl, int *totalNumOfPlayers,
                                   int *numOfPlayers) {
    if (groupID < 0){
        getNumPlayersMatchingInTree(this->playerIdTree, minID, maxId, minSalary,
                              minLvl, totalNumOfPlayers, numOfPlayers);
    }else{
        GroupNode& group = getGroup(groupID);
        group.getNumPlayersMatching(minID, maxId, minSalary,
                                    minLvl, totalNumOfPlayers, numOfPlayers);
    }
}

void Competition::getGroupInfo(int groupID, int *value, int *NumPlayers) {
    GroupNode& group = getGroup(groupID);
    *value = group.getValue();
    *NumPlayers = group.getPlayersNum();
}

void Competition::getPlayerInfo(int playerID, int *groupID, int *salary, int *lvl) {
    PlayerNode& player = getPlayer(playerID);
    GroupNode& group = player.getGroup();
    *groupID = group.getID();
    *salary = player.getSalary();
    *lvl = player.getLevel();
}

void Competition::increaseGroupValue(int groupId, int valInc) {
    GroupNode& group = getGroup(groupId);
    group.increaseValue(valInc);
}

void Competition::movePlayer(int playerId, int groupId) {
    PlayerNode& player = getPlayer(playerId);
    GroupNode& oldGroup = player.getGroup();
    GroupNode& newGroup = getGroup(groupId);
    if (oldGroup.getID() == newGroup.getID()){
        throw SameGroup();
    }
    bool newGroupWasEmpty = newGroup.isEmpty();
    oldGroup.removePlayer(playerId);
    if (oldGroup.isEmpty()){
        nonEmptyGroupTree.remove(oldGroup);
    }
    player.changeGroup(newGroup);
    newGroup.addPlayer(player);
    if (newGroupWasEmpty){
        nonEmptyGroupTree.add(newGroup);
    }
}
