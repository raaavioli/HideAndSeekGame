#pragma once
#include <map>
#include <ctime>

#include "Protocol.h"
#include "Utils/MazeGenerator.h"
#include "GameObjects/GroundPlane.h"
#include "GameObjects/Wall.h"
#include "GameObjects/Item.h"
#include "GameObjects/Player.h"

class KeepTheFlag {
public:
	KeepTheFlag(int boardWidth, int boardHeight, 
		int mazeWidth, int mazeHeight, int mazeSpace);

	~KeepTheFlag();

	bool Update();
	Player* GetPlayer(int id);
	int GetWinner() { return m_WinnerID; }
	std::string GetGameMap() { return m_GameMap; }
	std::string GetGameState(int clientID);
	inline bool HasWinner() { return m_WinnerID != 0; }

private:
	MazeGenerator m_MazeGenerator;
	GroundPlane m_Floor;
	Item* m_Flag;
	std::map<int, Item*> m_Items;
	std::map<int, Player*> m_Players;
	int m_WinnerID;
	std::string m_GameMap;

	std::string getGameMessage(int clientID);
	long m_CurrentTime;
	long m_TimeAccumulated;

};