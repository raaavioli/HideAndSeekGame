#include "MazeGenerator.h"

MazeGenerator::MazeGenerator(int width, int height)
	: m_Width(width), m_Height(height), m_Size(width*height)
{
	m_Maze.reserve(width * height);

	for (int y = 0; y < m_Height; y++)
	{
		for (int x = 0; x < m_Width; x++) {
			Cell *c = new Cell();
			c->X = x;
			c->Y = y;
			if (x > 0) {
				c->Neighbors.insert(std::pair<Direction, Cell*>(WEST, m_Maze.at(y*m_Width + x - 1)));
				m_Maze.at(y*m_Width + x - 1)->Neighbors.insert(std::pair<Direction, Cell*>(EAST, c));
				if (x == m_Width - 1)
					c->Neighbors.insert(std::pair<Direction, Cell*>(EAST, nullptr));
			}
			else {
				c->Neighbors.insert(std::pair<Direction, Cell*>(WEST, nullptr));
			}

			if (y > 0) {
				c->Neighbors.insert(std::pair<Direction, Cell*>(NORTH, m_Maze.at((y - 1)*m_Width + x)));
				m_Maze.at((y - 1)*m_Width + x)->Neighbors.insert(std::pair<Direction, Cell*>(SOUTH, c));
				if (y == m_Height - 1)
					c->Neighbors.insert(std::pair<Direction, Cell*>(SOUTH, m_Maze.at((y - 1)*m_Width + x)));
			}
			else {
				c->Neighbors.insert(std::pair<Direction, Cell*>(NORTH, nullptr));
			}

			m_Maze.push_back(c);
		}
	}
};

MazeGenerator::~MazeGenerator()
{
	for (auto cell : m_Maze)
		delete cell;
}

void MazeGenerator::GenerateMaze() {
	std::stack<Cell> queue;
	std::srand(std::time(0));
	int xStart = rand() % m_Width;
	int yStart = rand() % m_Height;

	Cell current = *m_Maze.at(yStart*m_Width + xStart);
	m_Maze.at(yStart*m_Width + xStart)->isVisited = true;

	queue.push(current);

	while (queue.size() > 0)
	{
		Cell& neighbor = GotoUnvisitedNeighbor(current);
		if (neighbor.isVisited)
		{
			//All valid neighbors for current were visited
			queue.pop();
			if (queue.size() > 0) {
				current = *m_Maze.at(queue.top().Y * m_Width + queue.top().X);
			}
		}
		else
		{
			m_Maze.at(neighbor.Y * m_Width + neighbor.X)->isVisited = true;
			current = neighbor;
			queue.push(current);
		}
	}
}

void MazeGenerator::CutLongerWalls(int wallLength) {
	std::vector<int> columns(m_Width, 1);

	for (int y = 0; y < m_Height; y++)
	{
		int inRow = 1;
		for (int x = 0; x < m_Width; x++)
		{
			//Calculate number of walls in a row vertically on column x
			//if nr of walls in a row is larger than wallLength, remove 1 of them at random.
			if (x < m_Width - 1 && m_Maze.at(y*m_Width + x)->Neighbors.at(EAST) != nullptr)
			{
				if (columns.at(x) < wallLength) columns.at(x)++;
				else
				{
					int random = rand() % wallLength;
					destroyWall(EAST, m_Maze.at((y - random)*m_Width + x));
				}
			}
			else columns.at(x) = 1;

			//Calculate number of walls in a row horizontally on a row y
			//if nr of walls in a row is larger than wallLength, remove 1 of them at random.
			if (y < m_Height - 1 && m_Maze.at(y*m_Width + x)->Neighbors.at(SOUTH) != nullptr)
			{
				if (inRow < wallLength) inRow++;
				else
				{
					int random = rand() % wallLength;
					destroyWall(SOUTH, m_Maze.at(y*m_Width + x - random));
				}
			}
			else inRow = 1;
		}
	}
}

void MazeGenerator::PrintMaze() {
	for (int y = 0; y < m_Height; y++) {
		std::cout << "|";
		for (int x = 0; x < m_Width; x++) {
			Cell &c = *m_Maze.at(y*m_Width + x);
			bool south = false;
			bool east = false;

			if (c.Neighbors.at(SOUTH) != nullptr) {
				south = true;
			}
			if (c.Neighbors.at(EAST) != nullptr) {
				east = true;
			}

			if (south && east)
				std::cout << "_|";
			else if (south)
				std::cout << "__";
			else if (east)
				std::cout << " |";
			else
				std::cout << "  ";
		}
		std::cout << "|\n";
	}
}

std::vector<Wall*> MazeGenerator::GetGameWalls(GroundPlane &gp)
{
	std::vector<Wall*> walls = std::vector<Wall*>();
	int maxWalls = (m_Width - 1)*(int)m_Height / 2 + (m_Height - 1)*(int)m_Width / 2;
	walls.reserve(maxWalls);

	static auto xToPlane = [&](int x) { return x * gp.GetWidth() / m_Width; };
	static auto yToPlane = [&](int y) { return y * gp.GetHeight() / m_Height; };

	APP_INFO("Width: {0}, Height: {1}", gp.GetWidth(), gp.GetHeight());
	APP_INFO("Width: {0}, Height: {1}", xToPlane(m_Width), yToPlane(m_Height));

	std::vector<int> columnRowCount(m_Width, 0);

	for (int y = 0; y < m_Height; y++)
	{
		int inRow = 0;
		for (int x = 0; x < m_Width; x++)
		{
			if (y < m_Height - 1 && m_Maze.at(y*m_Width + x)->Neighbors.at(SOUTH) != nullptr)
				inRow++;
			else if(inRow > 0)
			{
				walls.push_back(new Wall(gp, xToPlane(x - inRow), yToPlane(y + 1 + 0.5), glm::vec3(xToPlane(inRow), 0.5, 3)));
				inRow = 0;
			}

			if (x < m_Width - 1 && m_Maze.at(y*m_Width + x)->Neighbors.at(EAST) != nullptr)
				columnRowCount.at(x)++;
			else if (columnRowCount.at(x) > 0)
			{
				walls.push_back(new Wall(gp, xToPlane(x + 1 + 0.5), yToPlane(y - columnRowCount.at(x)), glm::vec3(0.5, yToPlane(columnRowCount.at(x)), 3)));
				columnRowCount.at(x) = 0;
			}
		}
	}
	APP_INFO("WALLS: {0}", walls.size());
	return walls;
}

Cell &MazeGenerator::GotoUnvisitedNeighbor(const Cell from)
{
	static Direction dirs[4] = { NORTH, EAST, SOUTH, WEST };

	//Get random start direction
	int random = rand() % 4;
	for (int i = 0; i < 4; i++) {
		random += i;
		random %= 4;
		//Check if neighbor in random direction is not null
		if (from.Neighbors.at(dirs[random]) != nullptr) {
			Direction dir = dirs[random];
			Cell *neighbor = from.Neighbors.at(dir);
			//if neighbor wasn't null, it must not be visited
			if (!neighbor->isVisited)
			{
				//Break wall between 'from' and the non visited neighbor
				//return the neighbor and use it as the new 'current'
				destroyWall(dir, m_Maze.at(from.Y*m_Width + from.X));
				return *neighbor;
			}
		}
		//Otherwise, try next direction if that one is valid
	}
	//If no directions were valid, return invalid cell which is already "visited"
	return Cell{ -1,-1, std::map<Direction, Cell*>(), true };
}

void MazeGenerator::destroyWall(Direction d, Cell* cell)
{
	if (cell->Neighbors.at(d) != nullptr)
	{
		cell->Neighbors.at(d)->Neighbors.at(Opposite(d)) = nullptr;
		cell->Neighbors.at(d) = nullptr;
	}
}