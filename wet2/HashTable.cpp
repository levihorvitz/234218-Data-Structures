#include "HashTable.h"
#include <iostream>

using namespace std;


int HashTable::hashFunc(int playerID)
{
	return playerID % hashSize;
}

bool HashTable::exist(int playerID)
{
	int i = hashFunc(playerID);
	Player* temp = array[i]->next;
	while (temp != NULL) {
		if (temp->playerID == playerID) {
			return true;
		}
		temp = temp->next;
	}
	return false;
}

void HashTable::expand(double factor)
{
	Player** newArray = new Player* [int(hashSize * factor)];
	for (int i = 0; i < hashSize * factor; i++) {
		newArray[i] = new Player(-1, -1, -1, 0);
	}
	int oldSize = hashSize;
	this->hashSize = this->hashSize * factor;
	for (int i = 0; i < oldSize; i++) {
		Player* temp = array[i]->next;
		while (temp != NULL) {
			Player* p = new Player(temp->playerID, temp->group, temp->score, temp->level);
			int j = hashFunc(p->playerID);
			Player* next = newArray[j]->next;
			newArray[j]->next = p;
			p->next = next;
			temp = temp->next;
		}
	}
	for (int i = 0; i < oldSize; i++) {
		deletePlayers(array[i]);
	}
	delete[] array;
	this->array = newArray;

}

void HashTable::add(int playerID, int group, int score, int level)
{
	int i = hashFunc(playerID);
	if (exist(playerID)) {
		return;
	}
	Player* p = new Player(playerID, group, score, level);
	Player* next = array[i]->next;
	array[i]->next = p;
	p->next = next;
	playerSize++;
	if (hashSize <= playerSize) {
		expand(2);
	}
}

void HashTable::remove(int playerID)
{
	int i = hashFunc(playerID);
	Player* temp = array[i];
	Player* toRemove = NULL;
	while (temp->next != NULL) {
		if (temp->next->playerID == playerID) {
			toRemove = temp->next;
			break;
		}
		temp = temp->next;
	}
	if (toRemove == NULL) {
		return;
	}
	temp->next = toRemove->next;
	delete toRemove;
	playerSize--;
	if (hashSize >= 4 * playerSize && hashSize != 2) {
		expand(0.5);
	}
}

HashTable::~HashTable()
{
	for (int i = 0; i < hashSize; i++) {
		deletePlayers(array[i]);
	}
	delete[] array;
}

void HashTable::deletePlayers(Player* p)
{
	while (p != NULL) {
		Player* temp = p->next;
		delete p;
		p = temp;
	}
}

int HashTable::getGroup(int playerID)
{
	int i = hashFunc(playerID);
	Player* temp = array[i];
	while (temp != NULL) {
		if (temp->playerID == playerID) {
			return temp->group;
		}
		temp = temp->next;
	}
	return -1;
}

int HashTable::getLevel(int playerID)
{
	int i = hashFunc(playerID);
	Player* temp = array[i];
	while (temp != NULL) {
		if (temp->playerID == playerID) {
			return temp->level;
		}
		temp = temp->next;
	}
	return -1;

}

int HashTable::getScore(int playerID)
{
	int i = hashFunc(playerID);
	Player* temp = array[i];
	while (temp != NULL) {
		if (temp->playerID == playerID) {
			return temp->score;
		}
		temp = temp->next;
	}
	return -1;

}