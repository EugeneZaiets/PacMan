#include "PacMan.h"
#include <WinUser.h>

PacMan::PacMan(std::shared_ptr<ConsoleSettingsHandler> console_handler, Game* game) :
	m_console_handler(console_handler),
	game_instance(game), 
	x(0), 
	y(0),
	prev_x(0),
	prev_y(0),
	color(YELLOW),
	lives(NUMBER_OF_LIVES),
	score(0),
	head(Head::RIGHT), 
	direction(NO_DIRECTION),
    old_direction(NO_DIRECTION),
	speed(PACMAN_SPEED),
	move_counter(0),
	kill_counter(0),
	score_offset(0), 
	got_energizer(false),
    check_to_unpause(false),
	timer(0),
    timer_on_pause(0)
{
	if (m_console_handler == 0) exit(1);
}
PacMan::~PacMan()
{
}
void PacMan::move(bool paused)
{
    if (isPaused(paused)) return;
	if (move_counter) move_counter--;
	else {
		getDirection();
		if (!checkCollision(direction))
		{
			m_console_handler->setCursorPosition(prev_x, prev_y);
			//check = game_instance->getCharOfMap(prev_x, prev_y);
			m_console_handler->resetSettingsToDefault();
			std::cout << game_instance->getCharOfMap(prev_x, prev_y);
			if (game_instance->getCharOfMap(x, y) != ' ')
			{
				scoreUp();
				renderScore();
				game_instance->decreasePointsNum();
				game_instance->setCharOfMap(x, y, ' ');
			}
			renderPacman();
			old_direction = direction;
			move_counter = speed;
		}
		else if(!checkCollision(old_direction))
		{
			m_console_handler->setCursorPosition(prev_x, prev_y);
			//check = game_instance->getCharOfMap(prev_x, prev_y);
			m_console_handler->resetSettingsToDefault();
			std::cout << game_instance->getCharOfMap(prev_x, prev_y);
			if (game_instance->getCharOfMap(x, y) != ' ')
			{
				scoreUp();
				renderScore();
				game_instance->decreasePointsNum();
				game_instance->setCharOfMap(x, y, ' ');
			}
			renderPacman();
			move_counter = speed;
		}
	}
}
char PacMan::getDirection()
{
    if      (isKeyDown(VK_W) || isKeyDown(VK_UP))    return direction = Direction[0];
    else if (isKeyDown(VK_A) || isKeyDown(VK_LEFT))  return direction = Direction[1];
    else if (isKeyDown(VK_S) || isKeyDown(VK_DOWN))  return direction = Direction[2];
    else if (isKeyDown(VK_D) || isKeyDown(VK_RIGHT)) return direction = Direction[3];
	return 0;
}
bool PacMan::checkCollision(char dir)
{
	prev_x = x;
	prev_y = y;
	switch (dir)
	{
	case 'w':
		if (strchr(CharNotToCollide,  game_instance->getCharOfMap(x, y - 1)))
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
		else if(strchr(CharNotToCollide, game_instance->getCharOfMap(x - 1, y)))
		{
			--x;
			head = Head::LEFT;
		}
		break;
	case 's':
		if (strchr(CharNotToCollide, game_instance->getCharOfMap(x, y + 1)))
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
		else if(strchr(CharNotToCollide, game_instance->getCharOfMap(x + 1, y)))
		{
			++x;
			head = Head::RIGHT;
		}
		break;
	}
	return (prev_x == x && prev_y == y) ? true :  false;
}
bool PacMan::isPaused(bool paused)
{
    if (paused)
    {
        timer_on_pause = timer;
        check_to_unpause = true;
        return true;
    }
    else if (!paused && check_to_unpause)
    {
        timer = timer_on_pause;
        check_to_unpause = false;
        return false;
    }
    return false;
}
void PacMan::dead()
{
	unsigned char head_prev = head;
	for (int i = 0; i < 9; i++) //blincking
	{
		if (i % 2) head = ' ';
		else head = head_prev;
		renderPacman();
		Sleep(150);
	}
	--lives;
}
void PacMan::scoreUp()
{
	if (game_instance->getCharOfMap(x, y) == 'o') // energizer
	{ 
		score_offset = 50;
		score += 50;
		got_energizer = true;
		kill_counter = 0;
		timer = std::clock();
	} 
	else if (game_instance->getCharOfMap(x, y) == static_cast<char>(250))  // pill
	{
		score_offset = 10;
		score += 10;
	} 
	else if (game_instance->getCharOfMap(x, y) == '%') {} // cherry
	if ((score / 10000) < ((score + score_offset) / 10000)) // each 10k gains lives_point;
	{
		if(lives < 3) ++lives;
		renderLives();
	}
}
void PacMan::resetPacMan(int x, int y)
{
	setPos_X(x);
	setPos_Y(y);
}
void PacMan::renderPacman()
{
	m_console_handler->setCursorPosition(x, y);
	m_console_handler->setTextColor(YELLOW);
	std::cout << head;
}
void PacMan::renderScore()
{
	m_console_handler->setTextColor(WHITE);
	m_console_handler->setCursorPosition(0, -1);
	std::cout << "SCORE: " << score;
}
void PacMan::renderLives()
{
	SetConsoleOutputCP(CP_UTF8); // to handle render heart sign
	m_console_handler->setTextColor(YELLOW);
	m_console_handler->setCursorPosition(1, Y_SIZE);
	std::cout << "Lives: ";
	m_console_handler->setTextColor(RED);
	for (unsigned int i = 0; i < lives; ++i)
	{
		//"\xE2\x99\xA5" heart sign in UTF-8
		std::cout << "\xE2\x99\xA5 ";
	}
	std::cout << ' ';
	m_console_handler->resetSettingsToDefault();
}
void PacMan::renderKill() 
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

	m_console_handler->setTextColor(CYAN);
	m_console_handler->setCursorPosition(kill_pos_x, y);
	std::cout << sum;
	Sleep(500);

	m_console_handler->setCursorPosition(kill_pos_x, y);
	for (int i = kill_pos_x; i < kill_pos_x + digit_num; ++i)
	{
		m_console_handler->setTextColor(BLUE);
		if (game_instance->getCharOfMap(i, y) == static_cast<char>(250) ||
			game_instance->getCharOfMap(i, y) == 'o')
		{
			m_console_handler->setTextColor(WHITE);
		}
		std::cout << game_instance->getCharOfMap(i, y);
	}
	if ((score / 10000) < ((score + score_offset) / 10000))
	{
		if (lives < 3) ++lives;
		renderLives();
	}
}