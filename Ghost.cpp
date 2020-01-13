#include "Ghost.h"

Ghost::Ghost(ConsoleSettingsHandler* console_handler, Game* game, Ghosts_Names his_name)
{
	m_console_handler = console_handler;
	game_instance	  = game;
	name			  = his_name;
	head			  = 'G';
	direction		  = 'w';
	old_direction	  = 'w';
	speed			  = 58;
	move_counter	  = 0;
	ResetModes(his_name);
	SetGhostColor(his_name);
	if(his_name == Ghosts_Names::INKY || his_name == Ghosts_Names::CLYDE) timer = std::clock();
}
Ghost::~Ghost(){}
void Ghost::Dead()
{
	SetGhostColor(WHITE);
	SetPrevMode(current_mode);
	SetMode(Mode::DEAD);
	head = '"';
}
void Ghost::RenderGhost()
{
	m_console_handler->SetCursorPosition(x, y);
	m_console_handler->SetTextColor(color);
	std::cout << head;
}
void Ghost::RenderMap()
{
	m_console_handler->SetTextColor(WHITE);
	m_console_handler->SetCursorPosition(x, y);
	std::cout << game_instance->GetCharOfMap(x, y);
}
void Ghost::ResetGhost(int x, int y)
{
	SetPos_X(x);
	SetPos_Y(y);
	SetGhostColor(name);
}
void Ghost::ResetModes(int name) {
	if	    (name == Ghosts_Names::BLINCKY)	current_mode = Mode::CHASE;
	else if (name == Ghosts_Names::PINKY  ) current_mode = Mode::EXIT_GATE;
	else if (name == Ghosts_Names::INKY   ) current_mode = Mode::WAIT;
	else if (name == Ghosts_Names::CLYDE  ) current_mode = Mode::WAIT;
}
void Ghost::ModeActivity(int pacman_x, int pacman_y)
{
	switch (current_mode) 
	{
	case Mode::CHASE :
	{
		char dir = DetermineClosestMove(pacman_x, pacman_y);
		Move(dir);
	}
	break;
	case Mode::SCATTER:
	{
		HandleScatterMode();
	}
	break;
	case Mode::FRIGHTENED: 
	{
		char dir = DetermineFurthestMove(pacman_x, pacman_y);
		Move(dir);
	}
	break;
	case Mode::DEAD : 
	{
		if (x == X_GATE - 1 && y == Y_GATE)
		{
			Move('d');
			break;
		}
		if (x == X_GATE + 1 && y == Y_GATE)
		{
			SetGhostColor(name);
			head = 'G';
			SetMode(Mode::EXIT_GATE);
		}
		char dir = DetermineClosestMove(X_GATE + 1, Y_GATE);
		if (x == 4 && y == 10) dir = 's';
		else if (x == 4 && y == 13) dir = 'd';
		Move(dir);
		old_direction = dir;
	}
	break;
	case Mode::WAIT: 
	{
		if (GetTimeInWait() >= 1.0) 
		{
			RenderMap();
			if (x == X_GATE + 1) ++x;
			else --x;
			RenderGhost();
			timer = std::clock();
		}
	}
	break;
	case Mode::EXIT_GATE: 
	{
		if (x == X_GATE + 1 && y == Y_GATE) 
		{

			direction = 'a';
			old_direction = 'a';
			RenderMap();
			--x;
			RenderMap();
			RenderGhost();
			if (color == BLUE) SetMode(Mode::FRIGHTENED);
			else			   SetMode(Mode::CHASE);
			break;
		}
		char dir = DetermineClosestMove(X_GATE + 1, Y_GATE);
		Move(dir);
	}
	break;
	}
}
void Ghost::HandleScatterMode()
{
		if(name == Ghosts_Names::BLINCKY)
		{
			char dir = DetermineClosestMove(44, 1); // right-up corner
			Move(dir);
		}
		else if (name == Ghosts_Names::PINKY) 
		{
			char dir = DetermineClosestMove(44, 20); // right-down corner
			Move(dir);
		}
		else if (name == Ghosts_Names::INKY) 
		{
			char dir = DetermineClosestMove(1, 1); // left-up corner
			Move(dir);
		}
		else if (name == Ghosts_Names::CLYDE) 
		{
			char dir = DetermineClosestMove(4, 20); // left-down corner
			Move(dir);
		}
	
}
char Ghost::DetermineClosestMove(int pm_x, int pm_y)
{
	std::vector<int> dirs_num;
	if (Direction[0] != GetOppositeDirection() && !CheckCollision(Direction[0])) dirs_num.push_back(0);
	if (Direction[1] != GetOppositeDirection() && !CheckCollision(Direction[1])) dirs_num.push_back(1);
	if (Direction[2] != GetOppositeDirection() && !CheckCollision(Direction[2])) dirs_num.push_back(2);
	if (Direction[3] != GetOppositeDirection() && !CheckCollision(Direction[3])) dirs_num.push_back(3);
	if (dirs_num.size() == 1) return Direction[dirs_num[0]];
	else 
	{
		std::vector<int> counter_nums;
		std::pair<int, int>* pointer_counter_direction = new std::pair<int, int>[dirs_num.size()];
		for (int i = 0; i < dirs_num.size(); ++i)
		{
			int ghost_x = x + OffsetCoordinatesX(dirs_num[i]);
			int ghost_y = y + OffsetCoordinatesY(dirs_num[i]);
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
			if (pointer_counter_direction[i].first < min) {
				min = pointer_counter_direction[i].first;
				closest = pointer_counter_direction[i].second;
			}
		}
		delete[] pointer_counter_direction;
		return Direction[closest];
	}
}
char Ghost::DetermineFurthestMove(int pm_x, int pm_y)
{
	std::vector<int> dirs_num;
	if (Direction[0] != GetOppositeDirection() && !CheckCollision(Direction[0])) dirs_num.push_back(0);
	if (Direction[1] != GetOppositeDirection() && !CheckCollision(Direction[1])) dirs_num.push_back(1);
	if (Direction[2] != GetOppositeDirection() && !CheckCollision(Direction[2])) dirs_num.push_back(2);
	if (Direction[3] != GetOppositeDirection() && !CheckCollision(Direction[3])) dirs_num.push_back(3);
	if (dirs_num.size() == 1) return Direction[dirs_num[0]];
	else
	{
		std::vector<int> counter_nums;
		std::pair<int, int>* pointer_counter_direction = new std::pair<int, int>[dirs_num.size()];
		for (int i = 0; i < dirs_num.size(); ++i)
		{
			int ghost_x = x + OffsetCoordinatesX(dirs_num[i]);
			int ghost_y = y + OffsetCoordinatesY(dirs_num[i]);
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
			if (pointer_counter_direction[i].first > max) {
				max = pointer_counter_direction[i].first;
				furthest = pointer_counter_direction[i].second;
			}
		}
		delete[] pointer_counter_direction;
		return Direction[furthest];
	}
}
char Ghost::GetOppositeDirection()
{
	if (old_direction == 'w') return 's';
	if (old_direction == 'a') return 'd';
	if (old_direction == 's') return 'w';
	if (old_direction == 'd') return 'a';
}
int  Ghost::OffsetCoordinatesX(int dir)
{
	if		(dir == 1) return -1; // Offset LEFT
	else if (dir == 3) return 1;// Offset RIGHT
	return 0;
}
int  Ghost::OffsetCoordinatesY(int dir)
{
	if		(dir == 0) return -1; // Offset UP, '0' - W
	else if (dir == 2) return 1;// Offset DOWN, '2' - D
	return 0;
}
void Ghost::SetGhostColor(Ghosts_Names name)
{
	switch (name) 
	{
	case Ghosts_Names::BLINCKY : { color = RED;		   } break;
	case Ghosts_Names::PINKY   : { color = PINK;	   } break;
	case Ghosts_Names::INKY    : { color = CYAN;	   } break;
	case Ghosts_Names::CLYDE   : { color = YELLOW; 	   } break;
	}
}
int  Ghost::GetInkyPos_X(int pacman_x, int blincky_x)
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
int  Ghost::GetInkyPos_Y(int pacman_y, int blincky_y)
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
int  Ghost::GetClydeCountPos_X(int pacman_x)
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
int  Ghost::GetClydeCountPos_Y(int pacman_y)
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
char Ghost::GetDirection()
{
	if (_kbhit())
	{
		direction = tolower(_getch());
		return direction;
	}
}
bool Ghost::CheckCollision(char dir)
{
	switch (dir) {
	case 'w': if (strchr(CharNotToCollide, game_instance->GetCharOfMap(x, y - 1)))return false;
	break;
	case 'a': if (x == 0 || strchr(CharNotToCollide, game_instance->GetCharOfMap(x - 1, y))) return false;
	break;
	case 's': if (strchr(CharNotToCollide, game_instance->GetCharOfMap(x, y + 1))) return false;
	break;
	case 'd': if (x == X_SIZE - 1 || strchr(CharNotToCollide, game_instance->GetCharOfMap(x + 1, y))) return false;
	break;
	}
	return true;
}
void Ghost::Move(char dir)
{
	if (move_counter) move_counter--;
	else
	{
		RenderMap();
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