#include "Game.h"

Game::Game(ConsoleSettingsHandler* console_handler)
{
	m_console_handler         = console_handler;
	points_num	              = 360;			// num of pills and energyzers - 360
	seconds_in_boost_by_level = 20.0;
	
	SetConsoleTitle("PacMan");
	m_console_handler->SetCustomWindowSize(X_SCREEN_SIZE, Y_SCREEN_SIZE);
	m_console_handler->SetCursorVisibility(false);

	pacman = new PacMan(console_handler, this);
	for (int i = 0; i < 4; i++) ghost[i] = new Ghost(console_handler, this, (Ghosts_Names)i);
}
Game::~Game()
{
	delete pacman;
	delete[] ghost;
}
void Game::Start() {
	while (true) 
	{ 
		Game_Loop(); 
	}
}
void Game::Game_Loop() {
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
		if (gameover) {
			m_console_handler->SetTextColor(RED);
			m_console_handler->SetCursorPosition(19, 7);
			std::cout << "GAME OVER";
			Sleep(1500);
			break;
		}
	}
}
void Game::LoadLevel() {
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

	SetPointsNum(360);
	 
	m_console_handler->SetCursorPosition(0, 0);
	for (int y = 0; y < Y_SIZE; y++)
	{
		for (int x = 0; x < X_SIZE; x++)
		{
			m_console_handler->SetTextColor(BLUE);
			char_to_print = Map[y][x];
			if (char_to_print == ' ')
			{
				m_MapToPrint[y][x] = ' ';
			}
			else if (char_to_print == '.') 
			{
				m_console_handler->SetTextColor(WHITE);
				m_MapToPrint[y][x] = 250;
			}
			else if(char_to_print == 'o')
			{
				m_console_handler->SetTextColor(WHITE);
				m_MapToPrint[y][x] = 'o';
			}
			else if(char_to_print == 'P')
			{

			}
			else if (char_to_print == '1') {
				m_MapToPrint[y][x] = 201;
			}
			else if (char_to_print == '2') {
				m_MapToPrint[y][x] = 187;
			}
			else if (char_to_print == '3') {
				m_MapToPrint[y][x] = 200;
			}
			else if (char_to_print == '4') {
				m_MapToPrint[y][x] = 188;
			}
			else if (char_to_print == '5') {
				m_MapToPrint[y][x] = 205;
			}
			else if (char_to_print == '6') {
				m_MapToPrint[y][x] = 186;
			}
			else if (char_to_print == '^') {
				m_MapToPrint[y][x] = 217;
			}
			else if (char_to_print == '$') {
				m_MapToPrint[y][x] = 218;
			}
			else if (char_to_print == '%') {
				m_MapToPrint[y][x] = 179;
			}
			else if (char_to_print == '#') {
				m_MapToPrint[y][x] = 196;
			}
			else if (char_to_print == '@') {
				m_MapToPrint[y][x] = 192;
			}
			else if (char_to_print == '*') {
				m_MapToPrint[y][x] = 191;
			}
			cout << m_MapToPrint[y][x];
		}
		m_console_handler->SetCursorPosition(0, y + 1);
	}
	InitAllActors();
	Render();
}
void Game::Render() {
	pacman->RenderPacman();
	pacman->RenderScore();
	pacman->RenderLives();
	for(int i = 0; i < 4; ++i) ghost[i]->RenderGhost();
}
bool Game::isDead()
{
	return pacman->GetLives() == 0 ? true : false;
}
bool Game::CollisionWithGhost()
{
	for (int i = 0; i < 4; ++i) {
		if (pacman->GetPos_X() == ghost[i]->GetPos_X() && 
			pacman->GetPos_Y() == ghost[i]->GetPos_Y() &&
			ghost[i]->GetMode() != Mode::DEAD) {
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
		for (int i = 0; i < 4; ++i)
		{
			if (ghost[i]->GetMode() != Mode::DEAD) ghost[i]->SetGhostColor(LIGHT_BLUE);
			if (ghost[i]->GetMode() == Mode::CHASE || ghost[i]->GetMode() == Mode::SCATTER) ghost[i]->SetMode(Mode::FRIGHTENED);
		}
	}
	if (points_num <= 330 && ghost[2]->GetMode() == Mode::WAIT) ghost[2]->SetMode(Mode::EXIT_GATE); // 30 points are ate
	if (points_num <= 240 && ghost[3]->GetMode() == Mode::WAIT) ghost[3]->SetMode(Mode::EXIT_GATE); // 1/3 of points are ate
	for (int i = 0; i < 4; ++i)
	{
		if (i == 0) ghost[0]->ModeActivity(pacman->GetPos_X(), pacman->GetPos_Y());
		if (i == 1) ghost[i]->ModeActivity(pacman->GetPos_X() + 4, pacman->GetPos_Y() + 4); // feature of Pinky
		if (i == 2) ghost[i]->ModeActivity(
			ghost[i]->GetInkyPos_X(pacman->GetPos_X(), ghost[0]->GetPos_X()),
			ghost[i]->GetInkyPos_Y(pacman->GetPos_Y(), ghost[0]->GetPos_X())); // feature of Inky
		if (i == 3)
		{
			int sum = ghost[i]->GetClydeCountPos_X(pacman->GetPos_X()) + ghost[i]->GetClydeCountPos_Y(pacman->GetPos_Y()); // distance to pacman
			if (sum > 8) ghost[i]->ModeActivity(pacman->GetPos_X(), pacman->GetPos_Y()); // go where pacman
			else ghost[i]->ModeActivity(4, 20); // go where clyde's scatter position
		}
	}
	
	Render();
}
void Game::HandleTime() {
	if (pacman->GetEnergizerInfo()) 
	{
		if (pacman->GetTimeInBooste() >= seconds_in_boost_by_level) pacman->SetEnergizerInfo(false); //20 seconds for boosted mode at start // timer increases
		if (pacman->GetTimeInBooste() >= seconds_in_boost_by_level * 0.75)  // after 3/4 of boost, ghost is white
		{ 
			for (int i = 0; i < 4; ++i)
			{
				if (ghost[i]->GetGhostColor() == BLUE) {
					ghost[i]->SetGhostColor(WHITE);
					Render();
				}
			}
		}
	}
	else 
	{
		for (int i = 0; i < 4; ++i)
		{
			if (ghost[i]->GetMode() != Mode::DEAD) ghost[i]->SetGhostColor(ghost[i]->GetGhostName());
			if (ghost[i]->GetMode() == Mode::FRIGHTENED)
			{
				ghost[i]->SetPrevMode(ghost[i]->GetMode());
				ghost[i]->SetMode(Mode::CHASE);
				timer = std::clock();
			}
		}
	}

	if (GetTime() >= 20.0)
	{
		for (int i = 0; i < 4; ++i)
		{
			if (ghost[i]->GetMode() == Mode::CHASE) ghost[i]->SetMode(Mode::SCATTER);
			else if (ghost[i]->GetMode() == Mode::SCATTER) ghost[i]->SetMode(Mode::CHASE);
		}
		timer = std::clock();
	}
}
void Game::InitAllActors()
{
	pacman->ResetPacMan(35, 10);
	pacman->SetEnergizerInfo(false);
	ghost[0]->ResetGhost(18, 10);
	ghost[1]->ResetGhost(20, 10);
	ghost[2]->ResetGhost(21, 11);
	ghost[3]->ResetGhost(21, 9);
	timer = std::clock();
	for (int i = 0; i < 4; ++i)
	{
		ghost[i]->ResetModes(ghost[i]->GetGhostName());
		ghost[i]->SetGhostColor(ghost[i]->GetGhostName());
		ghost[i]->SetHead('G');
	}
}
void Game::ResetMapInCollision() {
	m_console_handler->SetCursorPosition(pacman->GetPos_X(), pacman->GetPos_Y());
	std::cout << m_MapToPrint[pacman->GetPos_Y()][pacman->GetPos_X()];
	for (int i = 0; i < 4; i++)
	{
		m_console_handler->SetCursorPosition(ghost[i]->GetPos_X(), ghost[i]->GetPos_Y());
		std::cout << m_MapToPrint[ghost[i]->GetPos_Y()][ghost[i]->GetPos_X()];
	}
}
void Game::SetMazeText(std::string text, int color)
{
	m_console_handler->SetTextColor(color);
	m_console_handler->SetCursorPosition(19, 7);
	std::cout << text;

	while (!_kbhit());
	char* substring = new char[text.size() + 1];          //a substring to recover the maze in the console
	memcpy(substring, &m_MapToPrint[7][19], text.size());
	substring[text.size()] = '\0';

	m_console_handler->SetCursorPosition(19, 7);
	m_console_handler->SetTextColor(WHITE);
	std::cout << substring;
	delete substring;
}