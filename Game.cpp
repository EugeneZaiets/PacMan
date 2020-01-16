#include "Game.h"

Game::Game(std::shared_ptr<ConsoleSettingsHandler> console_handler) :
	m_console_handler(console_handler),
	points_num(POINTS_NUMBER),                    // num of pills and energyzers - 360
	seconds_in_boost_by_level(20.0),
	seconds_to_mode_change(20.0),
	level_counter(0),
	number_of_ghosts(4),
	timer(0),
	timer2(0)
{
	SetConsoleTitle("PacMan");
	m_console_handler->CreateGameWindow();
	m_console_handler->HanldeCursorVisibility(false);

	if (m_console_handler == 0) exit(1);
}
Game::~Game()
{
}
void Game::Start() 
{
	pacman = std::make_unique<PacMan>(m_console_handler, this);
	if (pacman == 0) exit(1);
    ghost.push_back(std::unique_ptr<Ghost>{new Ghost{ m_console_handler, this, Ghosts_Names::BLINCKY }});
    ghost.push_back(std::unique_ptr<Ghost>{new Ghost{ m_console_handler, this, Ghosts_Names::PINKY }});
    ghost.push_back(std::unique_ptr<Ghost>{new Ghost{ m_console_handler, this, Ghosts_Names::INKY }});
    ghost.push_back(std::unique_ptr<Ghost>{new Ghost{ m_console_handler, this, Ghosts_Names::CLYDE }});
    for (int i = 0; i < number_of_ghosts; i++)
    {
        if (ghost[i] == 0) exit(1);
    }
	while (true) 
	{ 
		Game_Loop(); 
	}
}
void Game::Game_Loop() 
{
	bool gameover = false;
	pacman->SetLives(3);
	for (level_counter = 0; level_counter < 255; level_counter++)
	{
		if (seconds_in_boost_by_level < 0) seconds_in_boost_by_level = 0.0; //decrease boost time with level till 0
		else seconds_in_boost_by_level -= level_counter;

		LoadLevel();
		SetMazeText("Get Ready!", YELLOW);

		while (points_num) 
		{
			pacman->Move();
			MoveGhosts(); // move pacman then ghosts then check 
			if (CollisionWithGhost())
			{
				ResetMapInCollision();
				InitAllActors();
				Render();
				while (!_kbhit());
			}
			if (isDead()) 
			{
				gameover = true;
				break;
			}
			HandleTime();
		}
		if (gameover) 
		{
			m_console_handler->SetTextColor(RED);
			m_console_handler->SetCursorPosition(19, 7);
			std::cout << "GAME OVER";
			Sleep(1500);
			break;
		}
	}
}
void Game::LoadLevel()
{
	char Map[Y_SIZE][X_SIZE + 1] =
	{
		"1555555555555555555555555555555555555555555552", //PacMan Maze
		"6..................%%....%%..................6",
		"6.$####*.$*.$####*.%%.$*.%%.$####*.$####*.$*.6",
		"6.%$###^.@^.@####^.@^.%%.@^.@####^.%$###^.%%.6",
		"6.%%..o...............%%...........%%..o..%%.6",
		"6.%%.$####*.$####*.$##^@##*.$####*.%%.$###^%.6",
		"6.@^.@####^.@####^.@######^.@####^.@^.@####^.6",
		"6.................      .....................6",
		"3552.$###*.$#####* 1552 152.$#####*.$###*.1554",
		"5554.%$#*%.@#####^ 3  6 6 6.@#####^.%$#*%.3555",
		".....%% %%........ %  6 6 6........ %% %%.....",
		"5552.%@#^%.$#####* 1  6 6 6.$#####*.%@#^%.1555",
		"1554.@###^.@#####^ 3554 354.@#####^.@###^.3552",
		"6.................      .....................6",
		"6.$*.$*.$*.$#####*.$######*.$*.$*.$#####*.$*.6",
		"6.%%.@^.@^.@#####^.@##*$##^.@^.@^.@#####^.%%.6",
		"6.%%o.................%%.................o%%.6",
		"6.%%.$*.$*.$#*.$#*.$*.%%.$*.$*.$*.$#*.$#*.%%.6",
		"6.@^.@^.@^.@#^.@#^.%%.@^.%%.@^.@^.@#^.@#^.@^.6",
		"6..................%%....%%..................6",
		"3555555555555555555555555555555555555555555554"
	};
	char char_to_print;
	m_console_handler->SetTextColor(WHITE);
	m_console_handler->SetCursorPosition(0, -2);
	cout << "1UP";
	m_console_handler->SetCursorPosition(36, -2);
	cout << "LEVEL: " << level_counter;

	SetPointsNum(POINTS_NUMBER);
	 
	m_console_handler->SetCursorPosition(0, 0);
	for (int y = 0; y < Y_SIZE; y++)
	{
		for (int x = 0; x < X_SIZE; x++)
		{
			char_to_print = Map[y][x];
			GetCharFromMap(char_to_print, x, y);
			cout << m_MapToPrint[y][x];
		}
		m_console_handler->SetCursorPosition(0, y + Y_GAME_SCREEN_TOP_SIDE_OFFSET);
	}
	InitAllActors();
	Render();
}
void Game::Render()
{
	pacman->RenderPacman();
	pacman->RenderScore();
	pacman->RenderLives();
	for(int i = 0; i < number_of_ghosts; ++i) ghost[i]->RenderGhost();
}
bool Game::isDead()
{
	return !pacman->GetLives();
}
bool Game::CollisionWithGhost()
{
	for (int i = 0; i < number_of_ghosts; ++i)
	{
		if (pacman->GetPos_X() == ghost[i]->GetPos_X() && 
			pacman->GetPos_Y() == ghost[i]->GetPos_Y() &&
			ghost[i]->GetMode() != Mode::DEAD)
		{
			if (ghost[i]->GetMode() != Mode::FRIGHTENED) 
			{
				pacman->Dead();
				return true;
			}
			else
			{
				ghost[i]->Dead();
				pacman->RenderKill();
				return false;
			}
		}
	}
	return false;
}
void Game::MoveGhosts()
{
	if (pacman->GetEnergizerInfo()) 
	{
		for (int i = 0; i < number_of_ghosts; ++i)
		{
			if (ghost[i]->GetMode() != Mode::DEAD) ghost[i]->SetGhostColor(LIGHT_BLUE);
			if (ghost[i]->GetMode() == Mode::CHASE || ghost[i]->GetMode() == Mode::SCATTER) ghost[i]->SetMode(Mode::FRIGHTENED);
		}
	}
	if (points_num <= 330 && ghost[2]->GetMode() == Mode::WAIT) ghost[2]->SetMode(Mode::EXIT_GATE); // 30 points are ate
	if (points_num <= 240 && ghost[3]->GetMode() == Mode::WAIT) ghost[3]->SetMode(Mode::EXIT_GATE); // 1/3 of points are ate
	for (int i = 0; i < number_of_ghosts; ++i)
	{
		if (i == 0)
		{
			ghost[0]->ModeActivity(pacman->GetPos_X(), pacman->GetPos_Y());
		}
		if (i == 1)
		{
			ghost[i]->ModeActivity(pacman->GetPos_X() + OFFSET_PINKY_POSITION, pacman->GetPos_Y() + OFFSET_PINKY_POSITION); // feature of Pinky, offset from pacman position on 4 tiles
		}
		if (i == 2)
		{
			ghost[i]->ModeActivity(ghost[i]->GetInkyPos_X(pacman->GetPos_X(), ghost[0]->GetPos_X()), ghost[i]->GetInkyPos_Y(pacman->GetPos_Y(), ghost[0]->GetPos_X())); // feature of Inky
		}
		if (i == 3)
		{
			int sum = ghost[i]->GetClydeCountPos_X(pacman->GetPos_X()) + ghost[i]->GetClydeCountPos_Y(pacman->GetPos_Y()); // distance to pacman
			if (sum > 8) ghost[i]->ModeActivity(pacman->GetPos_X(), pacman->GetPos_Y()); // go where pacman
			else ghost[i]->ModeActivity(CLYDE_SCATTER_POS_X, CLYDE_SCATTER_POS_Y); // go where clyde's scatter position
		}
	}
	
	Render();
}
void Game::HandleTime() 
{
	if (pacman->GetEnergizerInfo()) 
	{
		if (pacman->GetTimeInBooste() >= seconds_in_boost_by_level) pacman->SetEnergizerInfo(false); //20 seconds for boosted mode at start // timer increases
		if (pacman->GetTimeInBooste() >= seconds_in_boost_by_level * 0.75)  // after 3/4 of boost, ghost is white
		{ 
			for (int i = 0; i < number_of_ghosts; ++i)
			{
				if (ghost[i]->GetGhostColor() == BLUE)
				{
					ghost[i]->SetGhostColor(WHITE);
					Render();
				}
			}
		}
	}
	else 
	{
		for (int i = 0; i < number_of_ghosts; ++i)
		{
			if (ghost[i]->GetMode() != Mode::DEAD)
			{
				ghost[i]->SetGhostColor(ghost[i]->GetGhostName());
			}
			if (ghost[i]->GetMode() == Mode::FRIGHTENED)
			{
				ghost[i]->SetPrevMode(ghost[i]->GetMode());
				ghost[i]->SetMode(Mode::CHASE);
				timer = std::clock();
			}
		}
	}

	if (GetTime2() >= seconds_to_mode_change)
	{
		for (int i = 0; i < number_of_ghosts; ++i)
		{
			if (ghost[i]->GetMode() == Mode::CHASE) ghost[i]->SetMode(Mode::SCATTER);
			else if (ghost[i]->GetMode() == Mode::SCATTER) ghost[i]->SetMode(Mode::CHASE);
		}
	}
	timer2 = std::clock();
}
void Game::InitAllActors()
{
	pacman->SetEnergizerInfo(false);
	pacman->ResetPacMan(PACMAN_INIT_POS_X, PACMAN_INIT_POS_Y);
	ghost[0]->ResetGhost(BLINKY_INIT_POS_X, BLINKY_INIT_POS_Y);
	ghost[1]->ResetGhost(PINKY_INIT_POS_X, PINKY_INIT_POS_Y);
	ghost[2]->ResetGhost(INKY_INIT_POS_X, INKY_INIT_POS_Y);
	ghost[3]->ResetGhost(CLYDE_INIT_POS_X, CLYDE_INIT_POS_Y);
	timer = std::clock();

	for (int i = 0; i < number_of_ghosts; ++i)
	{
		ghost[i]->ResetModes(ghost[i]->GetGhostName());
		ghost[i]->SetGhostColor(ghost[i]->GetGhostName());
		ghost[i]->SetHead(GHOST_HEAD);
	}
}
void Game::ResetMapInCollision()
{
	m_console_handler->SetCursorPosition(pacman->GetPos_X(), pacman->GetPos_Y());
	std::cout << m_MapToPrint[pacman->GetPos_Y()][pacman->GetPos_X()];
	for (int i = 0; i < number_of_ghosts; i++)
	{
		m_console_handler->SetCursorPosition(ghost[i]->GetPos_X(), ghost[i]->GetPos_Y());
		std::cout << m_MapToPrint[ghost[i]->GetPos_Y()][ghost[i]->GetPos_X()];
	}
}
void Game::SetMazeText(std::string text, int color)
{
	m_console_handler->SetTextColor(color);
	m_console_handler->SetCursorPosition(X_MIDDLE_POS, Y_MIDDLE_POS);
	std::cout << text;

	while (!_kbhit());
    //std::unique_ptr<char> substring(new char[text.size() + 1]);
	char* substring = new char[text.size() + 1];          //a substring to recover the maze in the console
	memcpy(substring, &m_MapToPrint[Y_MIDDLE_POS][X_MIDDLE_POS], text.size());
	substring[text.size()] = '\0';

	m_console_handler->SetCursorPosition(X_MIDDLE_POS, Y_MIDDLE_POS);
	m_console_handler->SetTextColor(WHITE);
	std::cout << substring;
	delete substring;
}
void Game::GetCharFromMap(char ch, int x_offset, int y_offset) 
{
	for (auto iterator = MapCharToPtint.begin(); iterator != MapCharToPtint.end(); ++iterator)
	{
		if (ch == '.' || ch == 'o')
		{
			m_console_handler->SetTextColor(WHITE);
		}
		else
		{
			m_console_handler->SetTextColor(BLUE);
		}
		if (ch == iterator->first)
		{
			m_MapToPrint[y_offset][x_offset] = iterator->second;
		}
	}
}