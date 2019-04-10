#include "KeepTheFlag.h"
#include <sstream>
#include <iostream>
#include <random>
#include <algorithm>

#include "GameObjects/Collision/Collider.h"

KeepTheFlag::KeepTheFlag(int boardWidth, int boardHeight, 
	int mazeWidth, int mazeHeight, int mazeSpace)
	: m_Floor(GroundPlane(boardWidth, boardHeight)),
	m_MazeGenerator(mazeWidth, mazeHeight)
{
	Collider::Add(&m_Floor, MovementType::STATIC);
	m_GameMap.append(m_Floor.ToProtocolString(PLANE));

	m_MazeGenerator.GenerateMaze();
	m_MazeGenerator.CutLongerWalls(mazeSpace);
	m_MazeGenerator.PrintMaze();

	for (Wall* w : m_MazeGenerator.GetGameWalls(m_Floor)) {
		Collider::Add(w, MovementType::STATIC);
		m_GameMap.append(w->ToProtocolString(WALL));
		w->Update();
	}

	m_Flag = new Flag(m_Floor, 1, (Flag::ACTIVE));
	m_GameMap.append(m_Flag->ToProtocolString(ITEM));
	Collider::Add(m_Flag, MovementType::LOOTABLE);
	m_Flag->Update();

	m_CurrentTime = std::clock();
	m_TimeAccumulated = 0;
	m_WinnerID = 0;
}

KeepTheFlag::~KeepTheFlag()
{
	for(auto& pair : m_Players)
	{
		delete pair.second;
	}
}

bool KeepTheFlag::Update()
{
	std::clock_t t = std::clock();
	m_TimeAccumulated += (t - m_CurrentTime);
	m_CurrentTime = t;

	Collider::Interact();

	for (auto[id, player] : m_Players)
	{
		if (m_TimeAccumulated > 1000 && player->HasItem(m_Flag))
			player->IncrementFlagTime();
		// Player wins if he/she has more than 100 points and have hit all other players.
		if (player->GetScore() >= 100 && player->GetUniquePlayersHit().size() == m_Players.size() - 1) 
			if (!m_WinnerID || player->GetScore() >= m_Players.at(m_WinnerID)->GetScore())
				m_WinnerID = id;
	}
	m_TimeAccumulated %= 1000;
	//Continue playing if there's still no winner.
	return m_WinnerID == 0;
}

std::string KeepTheFlag::GetGameState(int clientID)
{
	Player *clientPlayer = GetPlayer(clientID);
	std::string gameState;
	gameState.append(clientPlayer->ToProtocolString(PLAYER));
	for (auto[id, player] : m_Players) 
	{
		if (id == clientID) continue;
		gameState.append(player->ToProtocolString(PLAYER));
	}
	gameState.append(m_Flag->ToProtocolString(ITEM));
	for (auto[id, item] : m_Items)
	{
		gameState.append(item->ToProtocolString(ITEM));
	}
	gameState.append(getGameMessage(clientID));
	return gameState;
}

Player* KeepTheFlag::GetPlayer(int id)
{
	auto p = m_Players.find(id);
	if (p != m_Players.end())
		return (*p).second;
	else {
		Player *player = new Player(
			id,
			-m_Floor.GetWidth() / 2 + (m_Floor.GetWidth() / (m_MazeGenerator.GetWidth())) * ((std::rand() % m_MazeGenerator.GetWidth()) + 0.5),
			-m_Floor.GetHeight() / 2 + (m_Floor.GetHeight() / (2 * m_MazeGenerator.GetHeight())) * ((std::rand() % m_MazeGenerator.GetHeight()) + 0.5),
			3
		);
		Collider::Add(player, MovementType::DYNAMIC);
		m_Players.insert(std::make_pair(id, player));
		return player;
	}
}

std::string KeepTheFlag::getGameMessage(int clientID)
{
	std::stringstream status;
	InstructionType it = m_WinnerID == 0 ? InstructionType::MESSAGE : InstructionType::ENDGAME;

	if (it == ENDGAME)
	{
		if (m_WinnerID == clientID)
			status << "Congratulations, you won!\n\n";
		else
			status << "You lost... Player " << m_WinnerID << " won this time.\n\n";
	}

	status << "\t\tSCOREBOARD" << std::endl;
	status << "--------------------------------------------------------------" << std::endl;
	status << "| ID\t| SCORE\t| HIT_GIVEN\t| HITS_TAKEN\t| FLAGTIME\t| REMAINING TARGETS" << std::endl;

	for (auto[id, player] : m_Players)
	{
		if (id == clientID)
			status << "| You";
		else
			status << "| " << id;
		status << "\t| " << player->GetScore();
		status << "\t| " << player->GetHitsGiven();
		status << "\t\t| " << player->GetHitsTaken();
		status << "\t\t| " << player->GetFlagTime();
		status << "\t\t| [ - ";
		for (auto[oid, oplayer] : m_Players)
		{
			bool found = false;
			for (auto hitPlayer : player->GetUniquePlayersHit())
			{
				if (oid == hitPlayer->GetId())
				{
					found = true;
					break;
				}
			}
			if (!found && oid != clientID && id != oid)
				status << oid << " - ";
			else if (!found && oid == clientID && id != clientID)
				status << "You - ";
		}
		status << "]" << std::endl;
	}

	std::string s = status.str();
	if (s.size() > sizeof(pString512))
		std::cout << "Error: GameStatus larger than pString size" << std::endl;
	else
	{
		pString512 sc;
		std::strcpy(sc.Message, s.c_str());
		return Protocol::Stringify(it, Attribute::STATUS, &sc);
	}
}