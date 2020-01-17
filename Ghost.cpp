#include "Ghost.h"

Ghost::Ghost(std::shared_ptr<ConsoleSettingsHandler> console_handler, Game* game, Ghosts_Names his_name) :
    m_console_handler(console_handler),
    game_instance(game),
    name(his_name),
    x(0),
    y(0),
    prev_x(0),
    prev_y(0),
    head(GHOST_HEAD),
    direction(DEFAULT_DIRECTION),
    old_direction(DEFAULT_DIRECTION),
    speed(GHOST_SPEED),
    move_counter(0),
    timer(0),
    timer_on_pause(0),
    check_to_unpause(false)
{
	resetModes(his_name);
	setGhostColor(his_name);
	if (his_name == Ghosts_Names::INKY || 
		his_name == Ghosts_Names::CLYDE) 
	{
		timer = std::clock();
	}
	if (m_console_handler == 0) exit(1);
}
Ghost::~Ghost()
{
}
void Ghost::dead()
{
	setGhostColor(WHITE);
	setPrevMode(current_mode);
	setMode(Mode::DEAD);
	head = '"';
}
void Ghost::renderGhost()
{
	m_console_handler->setCursorPosition(x, y);
	m_console_handler->setTextColor(color);
	std::cout << head;
}
void Ghost::renderMap()
{
	m_console_handler->setTextColor(WHITE);
	m_console_handler->setCursorPosition(x, y);
	std::cout << game_instance->getCharOfMap(x, y);
}
void Ghost::resetGhost(int x, int y)
{
	setPos_X(x);
	setPos_Y(y);
	setGhostColor(name);
}
void Ghost::resetModes(int name) {
	if	    (name == Ghosts_Names::BLINCKY)	current_mode = Mode::CHASE;
	else if (name == Ghosts_Names::PINKY  ) current_mode = Mode::EXIT_GATE;
	else if (name == Ghosts_Names::INKY   ) current_mode = Mode::WAIT;
	else if (name == Ghosts_Names::CLYDE  ) current_mode = Mode::WAIT;
	prev_mode = current_mode;
}
void Ghost::modeActivity(int pacman_x, int pacman_y, bool paused)
{
    if (isPaused(paused)) return;
	switch (current_mode) 
	{
	case Mode::CHASE :
	{
        handleChaseMode(pacman_x, pacman_y);
	}
	break;
	case Mode::SCATTER:
	{
		handleScatterMode();
	}
	break;
	case Mode::FRIGHTENED: 
	{
        handleFrightenedMode(pacman_x, pacman_y);
	}
	break;
	case Mode::DEAD : 
	{
        handleDeadMode();
	}
	break;
	case Mode::WAIT: 
	{
        handleWaitMode();
	}
	break;
	case Mode::EXIT_GATE: 
	{
        handleExitMode();
	}
	break;
	}
}
void Ghost::handleChaseMode(int x, int y)
{
    char dir = determineClosestMove(x, y);
    move(dir);
}
void Ghost::handleScatterMode()
{
	if(name == Ghosts_Names::BLINCKY)
	{
		char dir = determineClosestMove(BLINKY_SCATTER_POS_X, BLINKY_SCATTER_POS_Y); // right-up corner
		move(dir);
	}
	else if (name == Ghosts_Names::PINKY) 
	{
		char dir = determineClosestMove(PINKY_SCATTER_POS_X, PINKY_SCATTER_POS_Y); // right-down corner
		move(dir);
	}
	else if (name == Ghosts_Names::INKY) 
	{
		char dir = determineClosestMove(INKY_SCATTER_POS_X, INKY_SCATTER_POS_Y); // left-up corner
		move(dir);
	}
	else if (name == Ghosts_Names::CLYDE) 
	{
		char dir = determineClosestMove(CLYDE_SCATTER_POS_X, CLYDE_SCATTER_POS_Y); // left-down corner
		move(dir);
	}
}
void Ghost::handleFrightenedMode(int x, int y)
{
    char dir = determineFurthestMove(x, y);
    move(dir);
}
void Ghost::handleExitMode()
{
    if (x == X_GATE + 1 && y == Y_GATE)
    {
        direction = 'a';
        old_direction = 'a';
        renderMap();
        --x;
        renderMap();
        renderGhost();
        setMode(Mode::CHASE);
        return;
    }
    char dir = determineClosestMove(X_GATE + 1, Y_GATE);
    move(dir);
}
void Ghost::handleDeadMode()
{
    if (x == X_GATE - 1 && y == Y_GATE)
    {
        move('d');
        return;
    }
    else if (x == X_GATE + 1 && y == Y_GATE)
    {
        setGhostColor(name);
        head = 'G';
        setMode(Mode::EXIT_GATE);
    }
    char dir = determineClosestMove(X_GATE + 1, Y_GATE);
    if (x == 4 && y == 10) dir = 's';
    else if (x == 4 && y == 13) dir = 'd';
    move(dir);
    old_direction = dir;
}
void Ghost::handleWaitMode()
{
    if (getTimeInWait() >= 1.0)
    {
        renderMap();
        if (x == X_GATE + 1) ++x;
        else --x;
        renderGhost();
        timer = std::clock();
    }
}

char Ghost::determineClosestMove(int pm_x, int pm_y)
{
	std::vector<int> dirs_num;
	if (Direction[0] != getOppositeDirection() && !checkCollision(Direction[0])) dirs_num.push_back(0);
	if (Direction[1] != getOppositeDirection() && !checkCollision(Direction[1])) dirs_num.push_back(1);
	if (Direction[2] != getOppositeDirection() && !checkCollision(Direction[2])) dirs_num.push_back(2);
	if (Direction[3] != getOppositeDirection() && !checkCollision(Direction[3])) dirs_num.push_back(3);
	if (dirs_num.size() == 1) return Direction[dirs_num[0]];
	else 
	{
		std::vector<int> counter_nums;
		std::pair<int, int>* pointer_counter_direction = new std::pair<int, int>[dirs_num.size()];
		for (int i = 0; i < dirs_num.size(); ++i)
		{
			int ghost_x = x + offsetCoordinatesX(dirs_num[i]);
			int ghost_y = y + offsetCoordinatesY(dirs_num[i]);
			int counter = 0;
			while(ghost_x != pm_x)
			{
				if (ghost_x > pm_x) --ghost_x;
				else ++ghost_x;
				++counter;
			}
			while (ghost_y != pm_y)
			{
				if (ghost_y > pm_y) --ghost_y;
				else ++ghost_y;
				++counter;
			}
			pointer_counter_direction[i].first  = counter;
			pointer_counter_direction[i].second = dirs_num[i];
		}

		int min = pointer_counter_direction[0].first;
		int closest = pointer_counter_direction[0].second;
		for (int i = 0; i < dirs_num.size(); ++i)
		{
			if (pointer_counter_direction[i].first < min)
			{
				min = pointer_counter_direction[i].first;
				closest = pointer_counter_direction[i].second;
			}
		}
		delete[] pointer_counter_direction;
		return Direction[closest];
	}
}
char Ghost::determineFurthestMove(int pm_x, int pm_y)
{
	std::vector<int> dirs_num;
	if (Direction[0] != getOppositeDirection() && !checkCollision(Direction[0])) dirs_num.push_back(0);
	if (Direction[1] != getOppositeDirection() && !checkCollision(Direction[1])) dirs_num.push_back(1);
	if (Direction[2] != getOppositeDirection() && !checkCollision(Direction[2])) dirs_num.push_back(2);
	if (Direction[3] != getOppositeDirection() && !checkCollision(Direction[3])) dirs_num.push_back(3);
	if (dirs_num.size() == 1) return Direction[dirs_num[0]];
	else
	{
		std::vector<int> counter_nums;
		std::pair<int, int>* pointer_counter_direction = new std::pair<int, int>[dirs_num.size()];
		for (int i = 0; i < dirs_num.size(); ++i)
		{
			int ghost_x = x + offsetCoordinatesX(dirs_num[i]);
			int ghost_y = y + offsetCoordinatesY(dirs_num[i]);
			int counter = 0;
			while (ghost_x != pm_x)
			{
				if (ghost_x > pm_x) --ghost_x;
				else ++ghost_x;
				++counter;
			}
			while (ghost_y != pm_y)
			{
				if (ghost_y > pm_y) --ghost_y;
				else ++ghost_y;
				++counter;
			}
			pointer_counter_direction[i].first = counter;
			pointer_counter_direction[i].second = dirs_num[i];
		}

		int max = pointer_counter_direction[0].first;
		int furthest = pointer_counter_direction[0].second;
		for (int i = 0; i < dirs_num.size(); ++i)
		{
			if (pointer_counter_direction[i].first > max) 
			{
				max = pointer_counter_direction[i].first;
				furthest = pointer_counter_direction[i].second;
			}
		}
		delete[] pointer_counter_direction;
		return Direction[furthest];
	}
}
char Ghost::getOppositeDirection()
{
	if (old_direction == 'w') return 's';
	if (old_direction == 'a') return 'd';
	if (old_direction == 's') return 'w';
	if (old_direction == 'd') return 'a';
}
int  Ghost::offsetCoordinatesX(int dir)
{
	if		(dir == 1) return -1; // offset LEFT
	else if (dir == 3) return 1;// offset RIGHT
	return 0;
}
int  Ghost::offsetCoordinatesY(int dir)
{
	if		(dir == 0) return -1; // offset UP, '0' - W
	else if (dir == 2) return 1;// offset DOWN, '2' - D
	return 0;
}
void Ghost::setGhostColor(Ghosts_Names name)
{
	switch (name) 
	{
	case Ghosts_Names::BLINCKY : { color = RED;		   } break;
	case Ghosts_Names::PINKY   : { color = PINK;	   } break;
	case Ghosts_Names::INKY    : { color = CYAN;	   } break;
	case Ghosts_Names::CLYDE   : { color = YELLOW; 	   } break;
	}
}
int  Ghost::getInkyPos_X(int pacman_x, int blincky_x)
{
	int ghost_x = blincky_x;
	int counter = 0;
	while (ghost_x != pacman_x + 2)
	{
		if (ghost_x > pacman_x + 2) --ghost_x;
		else ++ghost_x;
		++counter;
	}
	return pacman_x + 2 + counter;
}
int  Ghost::getInkyPos_Y(int pacman_y, int blincky_y)
{
	int ghost_y = blincky_y;
	int counter = 0;
	while (ghost_y != pacman_y + 2)
	{
		if (ghost_y > pacman_y + 2) --ghost_y;
		else ++ghost_y;
		++counter;
	}
	return pacman_y + 2 + counter;
}
int  Ghost::getClydeCountPos_X(int pacman_x)
{
	int ghost_x = pacman_x;
	int counter = 0;
	while (ghost_x != pacman_x)
	{
		if (ghost_x > pacman_x) --ghost_x;
		else ++ghost_x;
		++counter;
	}
	return counter;
}
int  Ghost::getClydeCountPos_Y(int pacman_y)
{
	int ghost_y = pacman_y;
	int counter = 0;
	while (ghost_y != pacman_y)
	{
		if (ghost_y > pacman_y) --ghost_y;
		else ++ghost_y;
		++counter;
	}
	return counter;
}
bool Ghost::checkCollision(char dir)
{
	switch (dir) 
	{
	case 'w': if (strchr(CharNotToCollide, game_instance->getCharOfMap(x, y - 1)))return false;
	break;
	case 'a': if (x == 0 || strchr(CharNotToCollide, game_instance->getCharOfMap(x - 1, y))) return false;
	break;
	case 's': if (strchr(CharNotToCollide, game_instance->getCharOfMap(x, y + 1))) return false;
	break;
	case 'd': if (x == X_SIZE - 1 || strchr(CharNotToCollide, game_instance->getCharOfMap(x + 1, y))) return false;
	break;
	}
	return true;
}
bool Ghost::isPaused(bool paused)
{
    if (paused)
    {
        timer_on_pause = timer;
        check_to_unpause = true;
        return true;
    }
    else if(!paused && check_to_unpause)
    {
        timer = timer_on_pause;
        check_to_unpause = false;
        return false;
    }
    return false;
}
void Ghost::move(char dir)
{

	if (move_counter) move_counter--;
	else
	{
		renderMap();
		if (dir == 'w') --y;
		if (dir == 'a') 
		{
			if (x == 0) x = X_SIZE - 1;
			else --x;
		}
		if (dir == 's') ++y;
		if (dir == 'd') 
		{
			if (x == X_SIZE - 1) x = 0;
			else ++x;
		}
		old_direction = dir;
		move_counter = speed;
	}
}