#include "PacMan.h"
#include <WinUser.h>

PacMan::PacMan(std::shared_ptr<ConsoleSettingsHandler> console_handler, Game* game) :
    m_console_handler_(console_handler),
    m_game_instance_(game),
    m_x_(0),
    m_y_(0),
    m_prev_x_(0),
    m_prev_y_(0),
    m_color_(YELLOW),
    m_lives_(NUMBER_OF_LIVES),
    m_score_(0),
    m_head_(Head::HEAD_RIGHT),
    m_direction_(NO_DIRECTION),
    m_old_direction_(NO_DIRECTION),
    m_speed_(PACMAN_SPEED),
    m_move_counter_(0),
    m_kill_counter_(0),
    m_score_offset_(0),
    m_got_energizer_(false),
    m_check_to_unpause_(false),
    m_timer_(0),
    m_timer_on_pause_(0)
{
    if (m_console_handler_ == 0)
        exit(1);
}
PacMan::~PacMan()
{
}
void PacMan::move(const bool paused)
{
    if (isPaused(paused)) 
        return;

    if (m_move_counter_)
    {
        m_move_counter_--;
    }
    else 
    {
        getDirection();
        if (!checkCollision(m_direction_))
        {
            m_console_handler_->setCursorPosition(m_prev_x_, m_prev_y_);
            m_console_handler_->resetSettingsToDefault();
            std::cout << m_game_instance_->getCharOfBuffer(m_prev_x_, m_prev_y_);

            if (m_game_instance_->getCharOfBuffer(m_x_, m_y_) != ' ')
            {
                scoreUp();
                renderScore();
                m_game_instance_->decreasePointsNum();
                m_game_instance_->setCharOfMap(m_x_, m_y_, ' ');
            }
            renderPacman();
            m_old_direction_ = m_direction_;
            m_move_counter_ = m_speed_;
        }
        else if (!checkCollision(m_old_direction_))
        {
            m_console_handler_->setCursorPosition(m_prev_x_, m_prev_y_);
            m_console_handler_->resetSettingsToDefault();
            std::cout << m_game_instance_->getCharOfBuffer(m_prev_x_, m_prev_y_);

            if (m_game_instance_->getCharOfBuffer(m_x_, m_y_) != ' ')
            {
                scoreUp();
                renderScore();
                m_game_instance_->decreasePointsNum();
                m_game_instance_->setCharOfMap(m_x_, m_y_, ' ');
            }
            renderPacman();
            m_move_counter_ = m_speed_;
        }
    }
}
const char PacMan::getDirection()
{
    if      (isKeyDown(VK_W) || isKeyDown(VK_UP))    return m_direction_ = Direction[0];
    else if (isKeyDown(VK_A) || isKeyDown(VK_LEFT))  return m_direction_ = Direction[1];
    else if (isKeyDown(VK_S) || isKeyDown(VK_DOWN))  return m_direction_ = Direction[2];
    else if (isKeyDown(VK_D) || isKeyDown(VK_RIGHT)) return m_direction_ = Direction[3];
	return 0;
}
const bool PacMan::checkCollision(const char dir)
{
    m_prev_x_ = m_x_;
    m_prev_y_ = m_y_;
    switch (dir)
    {
        case 'w':
            if (strchr(CharNotToCollide, m_game_instance_->getCharOfBuffer(m_x_, m_y_ - 1)))
            {
                --m_y_;
                m_head_ = Head::HEAD_UP;
            }
            break;

        case 'a':
            if (m_x_ == 0)
            {
                m_x_ = X_SIZE - 1;
                m_head_ = Head::HEAD_LEFT;
            }
            else if (strchr(CharNotToCollide, m_game_instance_->getCharOfBuffer(m_x_ - 1, m_y_)))
            {
                --m_x_;
                m_head_ = Head::HEAD_LEFT;
            }
            break;

        case 's':
            if (strchr(CharNotToCollide, m_game_instance_->getCharOfBuffer(m_x_, m_y_ + 1)))
            {
                ++m_y_;
                m_head_ = Head::HEAD_DOWN;
            }
            break;

        case 'd':
            if (m_x_ == X_SIZE - 1)
            {
                m_x_ = 0;
                m_head_ = Head::HEAD_RIGHT;
            }
            else if (strchr(CharNotToCollide, m_game_instance_->getCharOfBuffer(m_x_ + 1, m_y_)))
            {
                ++m_x_;
                m_head_ = Head::HEAD_RIGHT;
            }
            break;
    }
    return (m_prev_x_ == m_x_ && m_prev_y_ == m_y_) ? true :  false;
}
const bool PacMan::isPaused(const bool paused)
{
    if (paused == true)
    {
        m_timer_on_pause_   = m_timer_;
        m_check_to_unpause_ = true  ;
        return true;
    }
    else if (paused == false && m_check_to_unpause_)
    {
        m_timer_ = m_timer_on_pause_;
        m_check_to_unpause_ = false ;
        return false;
    }
    return false;
}
void PacMan::dead()
{
    unsigned char head_prev = m_head_;
    for (int i = 0; i < 9; ++i) //blincking
    {
        if (i % 2) 
            m_head_ = ' ';
        else 
            m_head_ = head_prev;
        renderPacman();
        Sleep(150);
    }
    --m_lives_;
}
void PacMan::scoreUp()
{
    if (m_game_instance_->getCharOfBuffer(m_x_, m_y_) == 'o') // energizer
    { 
        m_score_offset_ = 50;
        m_score_ += 50;
        m_got_energizer_ = true;
        m_kill_counter_ = 0;
        m_timer_ = std::clock();
    } 
    else if (m_game_instance_->getCharOfBuffer(m_x_, m_y_) == static_cast<char>(250))  // pill
    {
        m_score_offset_ = 10;
        m_score_ += 10;
    } 
    else if (m_game_instance_->getCharOfBuffer(m_x_, m_y_) == '%') {} // cherry
    if ((m_score_ / 10000) < ((m_score_ + m_score_offset_) / 10000))  // each 10k gains lives_point;
    {
        if(m_lives_ < 3) ++m_lives_;
        renderLives();
    }
}
void PacMan::resetPacMan(const int x, const int y)
{
    setPos_X(x);
    setPos_Y(y);
}
void PacMan::renderPacman()
{
    m_console_handler_->setCursorPosition(m_x_, m_y_);
    m_console_handler_->setTextColor(YELLOW);
    std::cout << m_head_;
}
void PacMan::renderScore()
{
    m_console_handler_->setTextColor(WHITE);
    m_console_handler_->setCursorPosition(0, -1);
    std::cout << "SCORE: " << m_score_;
}
void PacMan::renderLives()
{
    SetConsoleOutputCP(CP_UTF8);
    m_console_handler_->setTextColor(YELLOW);
    m_console_handler_->setCursorPosition(1, Y_SIZE);
	std::cout << "Lives: ";
    m_console_handler_->setTextColor(RED);
    for (unsigned int i = 0; i < m_lives_; ++i)
    {
        //"\xE2\x99\xA5" heart sign in UTF-8
        std::cout << "\xE2\x99\xA5 ";
    }
    std::cout << ' ';
    m_console_handler_->resetSettingsToDefault();
}
void PacMan::renderKill()
{
    ++m_kill_counter_;
    int sum = 200 * static_cast<int>(pow(2, m_kill_counter_ - 1));
    int temp = sum;
    m_score_offset_ = sum;
    m_score_ += sum;

    int digit_num = 1;
    while ((temp /= 10) > 0)
    {
        ++digit_num;
    }

    int kill_pos_x = m_x_ - 1;
    if (m_x_ == 0)
    {
        kill_pos_x = m_x_;
    }
    if (m_x_ > X_SIZE - digit_num)
    {
        m_x_ = X_SIZE - digit_num;
    }

    m_console_handler_->setTextColor(CYAN);
    m_console_handler_->setCursorPosition(kill_pos_x, m_y_);
    std::cout << sum;
    Sleep(500);

    m_console_handler_->setCursorPosition(kill_pos_x, m_y_);
    for (int i = kill_pos_x; i < kill_pos_x + digit_num; ++i)
    {
        m_console_handler_->setTextColor(BLUE);
        if (m_game_instance_->getCharOfBuffer(i, m_y_) == static_cast<char>(250) ||
            m_game_instance_->getCharOfBuffer(i, m_y_) == 'o')
        {
            m_console_handler_->setTextColor(WHITE);
        }
        std::cout << m_game_instance_->getCharOfBuffer(i, m_y_);
    }
    if ((m_score_ / 10000) < ((m_score_ + m_score_offset_) / 10000))
    {
        if (m_lives_ < 3)
            ++m_lives_;
        renderLives();
    }
}