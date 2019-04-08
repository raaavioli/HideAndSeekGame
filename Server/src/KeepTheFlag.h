#pragma once
#include <map>
#include <ctime>

#include "Protocol.h"
#include "Utils/MazeGenerator.h"
#include "GameObjects/GroundPlane.h"
#include "GameObjects/Wall.h"
#include "GameObjects/Flag.h"
#include "GameObjects/Player.h"

class KeepTheFlag {
public:
	KeepTheFlag(int boardWidth, int boardHeight, 
		int mazeWidth, int mazeHeight, int mazeSpace);

	~KeepTheFlag();

	bool Update();
	int GetWinner() { return m_WinnerID; }
	std::string GetGameMap() { return m_GameMap; }
	Player* GetPlayer(int id);
	void UpdateGameStatus(int clientID);
	inline std::string &GetGameStatus() { return m_GameStatus; };
	inline bool HasWinner() { return m_WinnerID != 0; }

private:
	MazeGenerator m_MazeGenerator;
	GroundPlane m_Floor;
	Flag* m_Flag;
	std::map<int, Player*> m_Players;
	int m_WinnerID;
	std::string m_GameMap;
	std::string m_GameStatus;

	long m_CurrentTime;
	long m_TimeAccumulated;

};