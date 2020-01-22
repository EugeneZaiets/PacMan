#include "Game.h"

Game* Game::m_p_GameInstance = 0;
Game* Game::getInstance(std::shared_ptr<ConsoleSettingsHandler> console_handler)
{
    if (!m_p_GameInstance)
    {
        m_p_GameInstance = new Game(console_handler);
    }
    return m_p_GameInstance;
}

Game::Game(std::shared_ptr<ConsoleSettingsHandler> console_handler) :
	m_console_handler(console_handler),         
	seconds_in_boost_by_level(SECODS_IN_BOOST_BY_LEVEL),
	seconds_to_mode_change(SECODS_TO_CHANGE_MODE),
    number_of_ghosts(NUMBER_OF_GHOSTS), // num of pills and energyzers - 360
    points_num(POINTS_NUMBER),
	level_counter(0),
    m_isPaused(false),
    m_gameover(false),
    return_to_unpause(true),
	timer(0),
	timer2(0),
    temp_timer1(0),
    temp_timer2(0)
{
	SetConsoleTitle("PacMan");
	m_console_handler->createGameWindow();
	m_console_handler->hanldeCursorVisibility(false);

	if (m_console_handler == 0) exit(1);
}
Game::~Game()
{
}
void Game::start() 
{
    pacman = std::make_unique<PacMan>(m_console_handler, Game::getInstance(m_console_handler));
    for (int i = 0; i < number_of_ghosts; i++)
    {
        ghost[i] = std::make_unique<Ghost>(m_console_handler, Game::getInstance(m_console_handler), (Ghosts_Names)i);
    }
    checkPointersToActors();
	while (true) 
	{ 
		game_Loop(); 
	}
}
void Game::pause()
{
    if (isKeyOncePressed(VK_ESCAPE))
    {
        m_isPaused = true;
        return_to_unpause = true;
        temp_timer1 = timer;
        temp_timer2 = timer2;
    }
    else
    {
        m_isPaused = false;
        timer = temp_timer1;
        timer2 = temp_timer2;
    }
    
}
void Game::game_Loop() 
{
    m_gameover = false;
    checkPointersToActors();
	pacman->setLives(NUMBER_OF_LIVES);

	for (level_counter = 0; level_counter < 255; level_counter++)
	{
		if (seconds_in_boost_by_level < 0) seconds_in_boost_by_level = 0.0; //decrease boost time with level till 0
		else seconds_in_boost_by_level -= level_counter; 
		loadLevel();
		setMazeText("Get Ready!", YELLOW);
        startLevel();
		if (m_gameover)
		{
			m_console_handler->setTextColor(RED);
			m_console_handler->setCursorPosition(19, 7);
			std::cout << "GAME OVER";
			Sleep(1500);
			break;
		}
	}
}
void Game::startLevel()
{
    while (points_num)
    {
        pause();
        if (renderPause(m_isPaused)) continue;

        pacman->move(m_isPaused);
        moveGhosts(); // move pacman then ghosts, then check 
        if (collisionWithGhost())
        {
            resetMapInCollision();
            initAllActors();
            render();
            while (!_kbhit());
        }
        if (isDead())
        {
            m_gameover = true;
            break;
        }
        handleTime();
    }
}
void Game::loadLevel()
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
	m_console_handler->setTextColor(WHITE);
	m_console_handler->setCursorPosition(0, -2);
	cout << "1UP";
	m_console_handler->setCursorPosition(36, -2);
	cout << "LEVEL: " << level_counter;
	 
	m_console_handler->setCursorPosition(0, 0);
	for (int y = 0; y < Y_SIZE; y++)
	{
		for (int x = 0; x < X_SIZE; x++)
		{
			char_to_print = Map[y][x];
			setCharFromMap(char_to_print, x, y);
			cout << m_MapToPrint[y][x];
		}
		m_console_handler->setCursorPosition(0, y + Y_GAME_SCREEN_TOP_SIDE_OFFSET);
	}
    setPointsNum(POINTS_NUMBER);
	initAllActors();
	render();
}
void Game::render()
{
    checkPointersToActors();
	pacman->renderPacman();
	pacman->renderScore();
	pacman->renderLives();
    for (int i = 0; i < number_of_ghosts; ++i)
    {
        ghost[i]->renderGhost();
    }
}
bool Game::renderPause(bool paused)
{
    if (paused)
    {
        if (!m_console_handler) exit(1);
        m_console_handler->setTextColor(YELLOW);
        m_console_handler->setCursorPosition(X_MIDDLE_POS, Y_MIDDLE_POS);
        std::cout << "  PAUSE  ";
        return true;
    }
    else if(return_to_unpause)
    {
        memcpy(substring, &m_MapToPrint[Y_MIDDLE_POS][X_MIDDLE_POS], 9);
        substring[9] = '\0';
        m_console_handler->setCursorPosition(X_MIDDLE_POS, Y_MIDDLE_POS);
        m_console_handler->setTextColor(WHITE);
        std::cout << substring;
        return_to_unpause = false;
        return false;
    }
    return false;
}
const bool Game::isDead()
{
	return !pacman->getLives();
}
const bool Game::collisionWithGhost()
{
    checkPointersToActors();
	for (int i = 0; i < number_of_ghosts; ++i)
	{
		if (pacman->getPos_X() == ghost[i]->getPos_X() && 
			pacman->getPos_Y() == ghost[i]->getPos_Y() &&
			ghost[i]->getMode() != Mode::DEAD)
		{
			if (ghost[i]->getMode() != Mode::FRIGHTENED) 
			{
				pacman->dead();
				return true;
			}
			else
			{
				ghost[i]->dead();
				pacman->renderKill();
				return false;
			}
		}
	}
	return false;
}
void Game::moveGhosts()
{
    checkPointersToActors();
	if (pacman->getEnergizerInfo()) 
	{
		for (int i = 0; i < number_of_ghosts; ++i)
		{
			if (ghost[i]->getMode() != Mode::DEAD) ghost[i]->setGhostColor(LIGHT_BLUE);
			if (ghost[i]->getMode() == Mode::CHASE || ghost[i]->getMode() == Mode::SCATTER) ghost[i]->setMode(Mode::FRIGHTENED);
		}
	}
	if (points_num <= 330 && ghost[2]->getMode() == Mode::WAIT) ghost[2]->setMode(Mode::EXIT_GATE); // 30 points are ate
	if (points_num <= 240 && ghost[3]->getMode() == Mode::WAIT) ghost[3]->setMode(Mode::EXIT_GATE); // 1/3 of points are ate
    determitePositionForModeActivity();
	render();
}
void Game::handleTime() 
{
    checkPointersToActors();
	if (pacman->getEnergizerInfo()) 
	{
		if (pacman->getTimeInBooste() >= seconds_in_boost_by_level) pacman->setEnergizerInfo(false); //20 seconds for boosted mode at start // timer increases
		if (pacman->getTimeInBooste() >= seconds_in_boost_by_level * 0.75)  // after 3/4 of boost, ghost is white
		{ 
			for (int i = 0; i < number_of_ghosts; ++i)
			{
				if (ghost[i]->getGhostColor() == BLUE)
				{
					ghost[i]->setGhostColor(WHITE);
					render();
				}
			}
		}
	}
	else 
	{
		for (int i = 0; i < number_of_ghosts; ++i)
		{
			if (ghost[i]->getMode() != Mode::DEAD)
			{
				ghost[i]->setGhostColor(ghost[i]->getGhostName());
			}
			if (ghost[i]->getMode() == Mode::FRIGHTENED)
			{
				ghost[i]->setPrevMode(ghost[i]->getMode());
				ghost[i]->setMode(Mode::CHASE);
				timer = std::clock();
			}
		}
	}

	if (getTime2() >= seconds_to_mode_change)
	{
		for (int i = 0; i < number_of_ghosts; ++i)
		{
			if (ghost[i]->getMode() == Mode::CHASE) ghost[i]->setMode(Mode::SCATTER);
			else if (ghost[i]->getMode() == Mode::SCATTER) ghost[i]->setMode(Mode::CHASE);
		}
	}
    else timer2 = std::clock();
}
void Game::initAllActors()
{
    checkPointersToActors();
	pacman->setEnergizerInfo(false);
	pacman->resetPacMan(PACMAN_INIT_POS_X, PACMAN_INIT_POS_Y);
	ghost[0]->resetGhost(BLINKY_INIT_POS_X, BLINKY_INIT_POS_Y);
	ghost[1]->resetGhost(PINKY_INIT_POS_X, PINKY_INIT_POS_Y);
	ghost[2]->resetGhost(INKY_INIT_POS_X, INKY_INIT_POS_Y);
	ghost[3]->resetGhost(CLYDE_INIT_POS_X, CLYDE_INIT_POS_Y);
	timer = std::clock();

	for (int i = 0; i < number_of_ghosts; ++i)
	{
		ghost[i]->resetModes(ghost[i]->getGhostName());
		ghost[i]->setGhostColor(ghost[i]->getGhostName());
		ghost[i]->setHead(GHOST_HEAD);
	}
}
void Game::resetMapInCollision()
{
    checkPointersToActors();
    if (m_console_handler == 0) exit(1);
	m_console_handler->setCursorPosition(pacman->getPos_X(), pacman->getPos_Y());
	std::cout << m_MapToPrint[pacman->getPos_Y()][pacman->getPos_X()];
	for (int i = 0; i < number_of_ghosts; i++)
	{
		m_console_handler->setCursorPosition(ghost[i]->getPos_X(), ghost[i]->getPos_Y());
		std::cout << m_MapToPrint[ghost[i]->getPos_Y()][ghost[i]->getPos_X()];
	}
}
void Game::setMazeText(std::string text, int color)
{
    if (!m_console_handler) exit(1);
	m_console_handler->setTextColor(color);
	m_console_handler->setCursorPosition(X_MIDDLE_POS, Y_MIDDLE_POS);
	std::cout << text;

	while (!_kbhit());
	//char* substring = new char[text.size() + 1];          //a substring to recover the maze in the console
	memcpy(substring, &m_MapToPrint[Y_MIDDLE_POS][X_MIDDLE_POS], text.size());
	substring[text.size()] = '\0';

	m_console_handler->setCursorPosition(X_MIDDLE_POS, Y_MIDDLE_POS);
	m_console_handler->setTextColor(WHITE);
	std::cout << substring;
	//delete substring;
}
void Game::setCharFromMap(const char ch, const int x_offset, const int y_offset)
{
	for (auto iterator = MapCharToPtint.begin(); iterator != MapCharToPtint.end(); ++iterator)
	{
		if (ch == '.' || ch == 'o')
		{
			m_console_handler->setTextColor(WHITE);
		}
		else
		{
			m_console_handler->setTextColor(BLUE);
		}
		if (ch == iterator->first)
		{
			m_MapToPrint[y_offset][x_offset] = iterator->second;
		}
	}
}
void Game::checkPointersToActors() 
{
    if (!pacman) exit(1);
    for (int i = 0; i < number_of_ghosts; ++i)
    {
        if (!ghost[i]) exit(1);
    }
}
void Game::determitePositionForModeActivity()
{
    for (int i = 0; i < number_of_ghosts; ++i)
    {
        if (ghost[i] == 0) exit(1);
        switch (i)
        {
        case 0:
        {
            ghost[0]->modeActivity(pacman->getPos_X(), pacman->getPos_Y(), m_isPaused);
        }
        break;
        case 1: // feature of Pinky, offset from pacman position on 4 tiles
        {
            ghost[1]->modeActivity(pacman->getPos_X() + OFFSET_PINKY_POSITION, pacman->getPos_Y() + OFFSET_PINKY_POSITION, m_isPaused);
        }
        break;
        case 2: // feature of Inky
        {
            ghost[2]->modeActivity(ghost[2]->getInkyPos_X(pacman->getPos_X(), ghost[0]->getPos_X()), ghost[2]->getInkyPos_Y(pacman->getPos_Y(), ghost[0]->getPos_X()), m_isPaused);
        }
        break;
        case 3:
        {
            int sum = ghost[3]->getClydeCountPos_X(pacman->getPos_X()) + ghost[i]->getClydeCountPos_Y(pacman->getPos_Y()); // distance to pacman
            if (sum > 8) ghost[3]->modeActivity(pacman->getPos_X(), pacman->getPos_Y(), m_isPaused); // go where pacman
            else ghost[3]->modeActivity(CLYDE_SCATTER_POS_X, CLYDE_SCATTER_POS_Y, m_isPaused); // go where clyde's scatter position
        }
        break;
        }
    }
}
