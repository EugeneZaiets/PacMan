#include "PacMan.h"

PacMan::PacMan(std::shared_ptr<ConsoleSettingsHandler> console_handler, Game* game) :
	m_console_handler(console_handler),
	game_instance(game), 
	x(0), 
	y(0),
	prev_x(0),
	prev_y(0),
	color(YELLOW),
	lives(3), 
	score(0),
	head(Head::RIGHT), 
	direction('N'), 
	speed(28),
	move_counter(0),
	kill_counter(0),
	score_offset(0), 
	got_energizer(false),
	timer(0)
{
	if (m_console_handler == 0) exit(1);
}
PacMan::~PacMan()
{
}
void PacMan::Move()
{
	if (move_counter) move_counter--;
	else {
		GetDirection();
		if (!CheckCollision(direction))
		{
			m_console_handler->SetCursorPosition(prev_x, prev_y);
			//check = game_instance->GetCharOfMap(prev_x, prev_y);
			m_console_handler->ResetSettingsToDefault();
			std::cout << game_instance->GetCharOfMap(prev_x, prev_y);
			if (game_instance->GetCharOfMap(x, y) != ' ')
			{
				ScoreUp();
				RenderScore();
				game_instance->DecreasePointsNum();
				game_instance->SetCharOfMap(x, y, ' ');
			}
			RenderPacman();
			old_direction = direction;
			move_counter = speed;
		}
		else if(!CheckCollision(old_direction))
		{
			m_console_handler->SetCursorPosition(prev_x, prev_y);
			//check = game_instance->GetCharOfMap(prev_x, prev_y);
			m_console_handler->ResetSettingsToDefault();
			std::cout << game_instance->GetCharOfMap(prev_x, prev_y);
			if (game_instance->GetCharOfMap(x, y) != ' ')
			{
				ScoreUp();
				RenderScore();
				game_instance->DecreasePointsNum();
				game_instance->SetCharOfMap(x, y, ' ');
			}
			RenderPacman();
			move_counter = speed;
		}
	}
}
char PacMan::GetDirection()
{
	if (_kbhit()) 
	{
		direction = tolower(_getch());
		return direction;
	}
	return 0;
}
bool PacMan::CheckCollision(char dir)
{
	prev_x = x;
	prev_y = y;
	switch (dir)
	{
	case 'w':
		if (strchr(CharNotToCollide,  game_instance->GetCharOfMap(x, y - 1)))
		{
			--y;
			head = Head::UP;
		}
		break;
	case 'a':
		if (x == 0)
		{
			x = X_SIZE - 1;
			head = Head::LEFT;
		}
		else if(strchr(CharNotToCollide, game_instance->GetCharOfMap(x - 1, y)))
		{
			--x;
			head = Head::LEFT;
		}
		break;
	case 's':
		if (strchr(CharNotToCollide, game_instance->GetCharOfMap(x, y + 1)))
		{
			++y;
			head = Head::DOWN;
		}
		break;
	case 'd':
		if(x == X_SIZE - 1)
		{
			x = 0;
			head = Head::RIGHT;
		}
		else if(strchr(CharNotToCollide, game_instance->GetCharOfMap(x + 1, y)))
		{
			++x;
			head = Head::RIGHT;
		}
		break;
	}
	return (prev_x == x && prev_y == y) ? true :  false;
}
void PacMan::Dead()
{
	unsigned char head_prev = head;
	for (int i = 0; i < 9; i++) //blincking
	{
		if (i % 2) head = ' ';
		else head = head_prev;
		RenderPacman();
		Sleep(150);
	}
	--lives;
}
void PacMan::ScoreUp()
{
	if (game_instance->GetCharOfMap(x, y) == 'o') // energizer
	{ 
		score_offset = 50;
		score += 50;
		got_energizer = true;
		kill_counter = 0;
		timer = std::clock();
	} 
	else if (game_instance->GetCharOfMap(x, y) == static_cast<char>(250))  // pill
	{
		score_offset = 10;
		score += 10;
	} 
	else if (game_instance->GetCharOfMap(x, y) == '%') {} // cherry
	if ((score / 10000) < ((score + score_offset) / 10000)) // each 10k gains lives_point;
	{
		if(lives < 3) ++lives;
		RenderLives();
	}
}
void PacMan::ResetPacMan(int x, int y)
{
	SetPos_X(x);
	SetPos_Y(y);
}
void PacMan::RenderPacman()
{
	m_console_handler->SetCursorPosition(x, y);
	m_console_handler->SetTextColor(YELLOW);
	std::cout << head;
}
void PacMan::RenderScore()
{
	m_console_handler->SetTextColor(WHITE);
	m_console_handler->SetCursorPosition(0, -1);
	std::cout << "SCORE: " << score;
}
void PacMan::RenderLives()
{
	SetConsoleOutputCP(CP_UTF8); // to handle render heart sign
	m_console_handler->SetTextColor(YELLOW);
	m_console_handler->SetCursorPosition(1, Y_SIZE);
	std::cout << "Lives: ";
	m_console_handler->SetTextColor(RED);
	for (unsigned int i = 0; i < lives; ++i)
	{
		//"\xE2\x99\xA5" heart sign in UTF-8
		std::cout << "\xE2\x99\xA5 ";
	}
	std::cout << ' ';
	m_console_handler->ResetSettingsToDefault();
}
void PacMan::RenderKill() 
{
	++kill_counter;
	int sum	 = 200 * (int)pow(2, kill_counter - 1);
	int temp = sum;
	score_offset = sum;
	score += sum;

	int digit_num = 1;
	while ((temp /= 10) > 0)
	{
		++digit_num;
	}

	int kill_pos_x = x - 1;
	if (x == 0)
	{
		kill_pos_x = x;
	}
	if (x > X_SIZE - digit_num)
	{
		x = X_SIZE - digit_num;
	}

	m_console_handler->SetTextColor(CYAN);
	m_console_handler->SetCursorPosition(kill_pos_x, y);
	std::cout << sum;
	Sleep(500);

	m_console_handler->SetCursorPosition(kill_pos_x, y);
	for (int i = kill_pos_x; i < kill_pos_x + digit_num; ++i)
	{
		m_console_handler->SetTextColor(BLUE);
		if (game_instance->GetCharOfMap(i, y) == static_cast<char>(250) ||
			game_instance->GetCharOfMap(i, y) == 'o')
		{
			m_console_handler->SetTextColor(WHITE);
		}
		std::cout << game_instance->GetCharOfMap(i, y);
	}
	if ((score / 10000) < ((score + score_offset) / 10000))
	{
		if (lives < 3) ++lives;
		RenderLives();
	}
}