#include "PlayersManager.h"


StatusTypeManager PlayersManager::MergeGroupsManager(int GroupID1, int GroupID2)
{
	if (GroupID1 <= 0 || GroupID2 <= 0 || GroupID2 > k || GroupID1 > k) {
		return INVALID_INPUT_MANAGER;
	}
	int g1 = alliances.find(GroupID1);
	int g2 = alliances.find(GroupID2);
	if (g1 == g2) {
		return SUCCESS_MANAGER;
	}
	AVLTree* newTree;
	if (allGroups[g1 - 1].players->size == 0 && allGroups[g2 - 1].players->size == 0) {
		newTree = new AVLTree();
	} else {
		newTree = allGroups[g1 - 1].players->mergedAVL(allGroups[g1 - 1].players, allGroups[g2 - 1].players);
		allGroups[g1 - 1].players->clearTree();
		allGroups[g2 - 1].players->clearTree();
	};
	if (alliances.size[g1 - 1] >= alliances.size[g2 - 1]) {
		AVLTree* temp = allGroups[g1 - 1].players;
		allGroups[g1 - 1].players = newTree;
		delete temp;
		for (int i = 0; i < 201; i++) {
			allGroups[g1 - 1].zero[i] += allGroups[g2 - 1].zero[i];
		}
		alliances.uniongroups(GroupID1, GroupID2);
		return SUCCESS_MANAGER;
	}
	AVLTree* temp = allGroups[g2 - 1].players;
	allGroups[g2 - 1].players = newTree;
	delete temp;
	for (int i = 0; i < 201; i++) {
		allGroups[g2 - 1].zero[i] += allGroups[g1 - 1].zero[i];
	}
	alliances.uniongroups(GroupID1, GroupID2);
	return SUCCESS_MANAGER;
}

StatusTypeManager PlayersManager::AddPlayerManager(int PlayerID, int GroupID, int score)
{
	if (GroupID > k || GroupID <= 0 || PlayerID <= 0 || score > scale || score <= 0) {
		return INVALID_INPUT_MANAGER;
	}
	if (allPlayersTable.exist(PlayerID)) {
		return FAILURE_MANAGER;
	}
	allPlayersTable.add(PlayerID, GroupID, score);
	allPlayersGroup.addPlayerToGroup(0, score);
	int g = alliances.find(GroupID);
	allGroups[g - 1].addPlayerToGroup(0, score);
	return SUCCESS_MANAGER;
}

StatusTypeManager PlayersManager::RemovePlayerManager(int PlayerID)
{
	if (PlayerID <= 0) {
		return INVALID_INPUT_MANAGER;
	}
	if (!allPlayersTable.exist(PlayerID)) {
		return FAILURE_MANAGER;
	}
	int group = allPlayersTable.getGroup(PlayerID);
	int score = allPlayersTable.getScore(PlayerID);
	int level = allPlayersTable.getLevel(PlayerID);
	allPlayersTable.remove(PlayerID);
	allPlayersGroup.removePlayerFromGroup(level, score);
	int g = alliances.find(group);
	allGroups[g - 1].removePlayerFromGroup(level, score);
	return SUCCESS_MANAGER;

}

StatusTypeManager PlayersManager::IncreasePlayerIDLevelManager(int PlayerID, int LevelIncrease)
{
	if (PlayerID <= 0 || LevelIncrease <= 0) {
		return INVALID_INPUT_MANAGER;
	}
	if (!allPlayersTable.exist(PlayerID)) {
		return FAILURE_MANAGER;
	}
	int group = allPlayersTable.getGroup(PlayerID);
	int score = allPlayersTable.getScore(PlayerID);
	int level = allPlayersTable.getLevel(PlayerID);
	int g = alliances.find(group);
	allPlayersTable.remove(PlayerID);
	allPlayersGroup.removePlayerFromGroup(level, score);
	allGroups[g - 1].removePlayerFromGroup(level, score);
	allPlayersTable.add(PlayerID, group, score, level + LevelIncrease);
	allPlayersGroup.addPlayerToGroup(level + LevelIncrease, score);
	allGroups[g - 1].addPlayerToGroup(level + LevelIncrease, score);
	return SUCCESS_MANAGER;

}

StatusTypeManager PlayersManager::ChangePlayerIDScoreManager(int PlayerID, int NewScore)
{
	if (PlayerID <= 0 || NewScore <= 0 || NewScore > scale) {
		return INVALID_INPUT_MANAGER;
	}
	if (!allPlayersTable.exist(PlayerID)) {
		return FAILURE_MANAGER;
	}
	int group = allPlayersTable.getGroup(PlayerID);
	int score = allPlayersTable.getScore(PlayerID);
	int level = allPlayersTable.getLevel(PlayerID);
	int g = alliances.find(group);
	allPlayersGroup.removePlayerFromGroup(level, score);
	allGroups[g - 1].removePlayerFromGroup(level, score);
	allPlayersTable.remove(PlayerID);
	allPlayersTable.add(PlayerID, group, NewScore, level);
	allPlayersGroup.addPlayerToGroup(level, NewScore);
	allGroups[g - 1].addPlayerToGroup(level, NewScore);
	return SUCCESS_MANAGER;
}

StatusTypeManager PlayersManager::GetPercentOfPlayersWithScoreInBoundsManager(int GroupID, int score, int lowerLevel,
																			  int higherLevel, double* players)
{
	if (GroupID < 0 || GroupID > k || players == NULL) {
		return INVALID_INPUT_MANAGER;
	}
	if (GroupID == 0) {
		int NumPlayers = allPlayersGroup.getNumPlayersInBounds(lowerLevel, higherLevel, 0);
		if (NumPlayers <= 0) {
			return FAILURE_MANAGER;
		}
		int sumScore = allPlayersGroup.getNumPlayersInBounds(lowerLevel, higherLevel, score);
		*players = (((double) sumScore) / NumPlayers) * 100;
		return SUCCESS_MANAGER;
	}
	int g = alliances.find(GroupID);
	int NumPlayers = allGroups[g - 1].getNumPlayersInBounds(lowerLevel, higherLevel, 0);
	if (NumPlayers <= 0) {
		return FAILURE_MANAGER;
	}
	int sumScore = allGroups[g - 1].getNumPlayersInBounds(lowerLevel, higherLevel, score);
	*players = (((double) sumScore) / NumPlayers) * 100;
	return SUCCESS_MANAGER;
}

StatusTypeManager PlayersManager::AverageHighestPlayerLevelByGroupManager(int GroupID, int m, double* level)
{
	if (GroupID < 0 || GroupID > k || level == NULL || m <= 0) {
		return INVALID_INPUT_MANAGER;
	}
	if (GroupID == 0) {
		if (m > allPlayersGroup.getNumPlayers()) {
			return FAILURE_MANAGER;
		}
		int lowPlayers = 0, highPlayers = 0;
		if (allPlayersGroup.players->head == NULL) {
			*level = 0.0;
			return SUCCESS_MANAGER;
		}
		int lowlevel = allPlayersGroup.select(allPlayersGroup.players->head->sumScores[0] - m + 1, &lowPlayers);
		int highlevel = allPlayersGroup.select(allPlayersGroup.players->head->sumScores[0], &highPlayers);
		int lowAvg = allPlayersGroup.players->RankAvg(lowlevel);
		int highAvg = allPlayersGroup.players->RankAvg(highlevel);
		int sum = (highAvg - lowAvg) +
				  (lowPlayers - (allPlayersGroup.getNumPlayersInBounds(lowlevel, highlevel, 0) - m)) * lowlevel;
		*level = (double) sum / m;
		return SUCCESS_MANAGER;
	}
	int g = alliances.find(GroupID);
	if (m > allGroups[g - 1].getNumPlayers()) {
		return FAILURE_MANAGER;
	}
	if (allGroups[g - 1].players->head == NULL) {
		*level = 0.0;
		return SUCCESS_MANAGER;
	}
	int lowPlayers = 0, highPlayers = 0;
	int lowlevel = allGroups[g - 1].select(allGroups[g - 1].players->head->sumScores[0] - m + 1, &lowPlayers);
	int highlevel = allGroups[g - 1].select(allGroups[g - 1].players->head->sumScores[0], &highPlayers);
	int lowAvg = allGroups[g - 1].players->RankAvg(lowlevel);
	int highAvg = allGroups[g - 1].players->RankAvg(highlevel);
	int sum = (highAvg - lowAvg) +
			  (lowPlayers - (allGroups[g - 1].getNumPlayersInBounds(lowlevel, highlevel, 0) - m)) * lowlevel;
	*level = (double) sum / m;
	return SUCCESS_MANAGER;
}

PlayersManager::~PlayersManager()
{
	delete[] allGroups;
}