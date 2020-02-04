#include "PacMan.h"

PacMan::PacMan
(
    std::shared_ptr<ConsoleSettingsHandler> console_handler, 
    shared_ptr<Game> game
) :
    m_console_handler_  (console_handler),
    m_game_instance_    (game),
    m_x_                (0),
    m_y_                (0),
    m_prev_x_           (0),
    m_prev_y_           (0),
    m_score_            (0),
    m_color_            (YELLOW),
    m_lives_            (NUMBER_OF_LIVES),
    m_head_             (Head::HEAD_RIGHT),
    m_direction_        (Directions::DIRECTION_DEFAULT),
    m_old_direction_    (Directions::DIRECTION_DEFAULT),
    m_speed_            (PACMAN_SPEED),
    m_move_counter_     (0),
    m_kill_counter_     (0),
    m_score_offset_     (0),
    m_got_energizer_    (false),
    m_check_to_unpause_ (false),
    m_timer_            (0),
    m_timer_on_pause_   (0)
{
    if (m_console_handler_ == 0)
        exit(NULL_POINTER_ERROR);
}
PacMan::~PacMan()
{
}
void PacMan::handlePacmanMovement(const bool paused)
{
    if (isPaused(paused)) 
        return;
    if (m_move_counter_ != 0)
        m_move_counter_--;
    else 
    {
        getDirectionFromKeyboard();
        move();
    }
}
Directions PacMan::getDirectionFromKeyboard()
{
    if      (isKeyDown(VK_W) || isKeyDown(VK_UP))    return m_direction_ = Directions::DIRECTION_W;
    else if (isKeyDown(VK_A) || isKeyDown(VK_LEFT))  return m_direction_ = Directions::DIRECTION_A;
    else if (isKeyDown(VK_S) || isKeyDown(VK_DOWN))  return m_direction_ = Directions::DIRECTION_S;
    else if (isKeyDown(VK_D) || isKeyDown(VK_RIGHT)) return m_direction_ = Directions::DIRECTION_D;
    return Directions::DIRECTION_DEFAULT;
}
void PacMan::moveUp()
{
    if (m_game_instance_ == 0)
        exit(NULL_POINTER_ERROR);
    char buffer_char = m_game_instance_->getCharOfBuffer(m_x_, m_y_ - 1);
    if (strchr(CharNotToCollide, buffer_char))
    {
        --m_y_;
        m_head_ = Head::HEAD_UP;
    }
}
void PacMan::moveLeft()
{
    if (m_game_instance_ == 0)
        exit(NULL_POINTER_ERROR);
    char buffer_char = m_game_instance_->getCharOfBuffer(m_x_ - 1, m_y_);
    if (m_x_ == 0)
    {
        m_x_ = X_SIZE - 1;
        m_head_ = Head::HEAD_LEFT;
    }
    else if (strchr(CharNotToCollide, buffer_char))
    {
        --m_x_;
        m_head_ = Head::HEAD_LEFT;
    }
}
void PacMan::moveDown()
{
    if (m_game_instance_ == 0)
        exit(NULL_POINTER_ERROR);
    char buffer_char = m_game_instance_->getCharOfBuffer(m_x_, m_y_ + 1);
    if (strchr(CharNotToCollide, buffer_char))
    {
        ++m_y_;
        m_head_ = Head::HEAD_DOWN;
    }
}
void PacMan::moveRight()
{
    if (m_game_instance_ == 0)
        exit(NULL_POINTER_ERROR);
    char buffer_char = m_game_instance_->getCharOfBuffer(m_x_ + 1, m_y_);
    if (m_x_ == X_SIZE - 1)
    {
        m_x_ = 0;
        m_head_ = Head::HEAD_RIGHT;
    }
    else if (strchr(CharNotToCollide, buffer_char))
    {
        ++m_x_;
        m_head_ = Head::HEAD_RIGHT;
    }
}
void PacMan::move()
{
    if (checkCollision(m_direction_) == false)
        moveWithDirection(m_direction_);
    else if (checkCollision(m_old_direction_) == false)
        moveWithDirection(m_old_direction_);
}
void PacMan::moveWithDirection(Directions direction)
{
    renderPrevPos();
    eatFood();
    renderPacman();
    m_old_direction_ = direction;
    m_move_counter_ = m_speed_;
}
void PacMan::renderPrevPos()
{
    if (m_console_handler_ == 0)
        exit(NULL_POINTER_ERROR);
    if (m_game_instance_ == 0)
        exit(NULL_POINTER_ERROR);
    m_console_handler_->setCursorPosition(m_prev_x_, m_prev_y_);
    m_console_handler_->resetSettingsToDefault();
    std::cout << m_game_instance_->getCharOfBuffer(m_prev_x_, m_prev_y_);
}
void PacMan::eatFood()
{
    if (m_game_instance_ == 0)
        exit(NULL_POINTER_ERROR);
    if (m_game_instance_->getCharOfBuffer(m_x_, m_y_) != ' ')
    {
        scoreUp();
        renderScore();
        m_game_instance_->decreasePointsNum();
        m_game_instance_->setCharOfMap(m_x_, m_y_, ' ');
    }
}
const bool PacMan::checkCollision(const Directions direction)
{
    m_prev_x_ = m_x_;
    m_prev_y_ = m_y_;
    switch (direction)
    {
    case Directions::DIRECTION_W : moveUp()   ; break;
    case Directions::DIRECTION_A : moveLeft() ; break;
    case Directions::DIRECTION_S : moveDown() ; break;
    case Directions::DIRECTION_D : moveRight(); break;
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
    for (int i = 0; i < BLINKYNG_COUNTER; ++i) 
    {
        if (i % 2) 
            m_head_ = ' ';
        else 
            m_head_ = head_prev;
        renderPacman();
        Sleep(MILLISECONDS_BLINKING_TIME);
    }
    --m_lives_;
}
void PacMan::scoreUp()
{
    if (m_game_instance_ == 0)
        exit(NULL_POINTER_ERROR);
    if (m_game_instance_->getCharOfBuffer(m_x_, m_y_) == 'o') 
    { 
        m_score_offset_ = SCORE_POINTS_ENERGIZER;
        m_score_ += SCORE_POINTS_ENERGIZER;
        m_got_energizer_ = true;
        m_kill_counter_ = 0;
        m_timer_ = std::clock();
    } 
    else if (m_game_instance_->getCharOfBuffer(m_x_, m_y_) == static_cast<char>(250))
    {
        m_score_offset_ = SCORE_POINTS_PILL;
        m_score_ += SCORE_POINTS_PILL;
    } 
    else if (m_game_instance_->getCharOfBuffer(m_x_, m_y_) == '%') {} 
    gainLife();
}
void PacMan::gainLife()
{
    double before_sroce_up = m_score_ / LIFE_GAIN_BORDER;
    double after_sroce_up  = (m_score_ + m_score_offset_) / LIFE_GAIN_BORDER;
    if (before_sroce_up < after_sroce_up)
    {
        if (m_lives_ < NUMBER_OF_LIVES) ++m_lives_;
        renderLives();
    }
}
void PacMan::resetPacMan(const int x, const int y)
{
    setPos_X(x);
    setPos_Y(y);
    setEnergizerInfo(false);
}
void PacMan::resetMapAfterKill(const int x, const int y, const int num_elements)
{
    if (m_console_handler_ == 0)
        exit(NULL_POINTER_ERROR);
    if (m_game_instance_ == 0)
        exit(NULL_POINTER_ERROR);
    m_console_handler_->setCursorPosition(x, y);
    for (int i = x; i < x + num_elements; ++i)
    {
        m_console_handler_->setTextColor(BLUE);
        if (m_game_instance_->getCharOfBuffer(i, y) == static_cast<char>(250) ||
            m_game_instance_->getCharOfBuffer(i, y) == 'o')
                m_console_handler_->setTextColor(WHITE);
        std::cout << m_game_instance_->getCharOfBuffer(i, y);
    }
}
void PacMan::renderPacman()
{
    if (m_console_handler_ == 0)
        exit(NULL_POINTER_ERROR);
    m_console_handler_->setCursorPosition(m_x_, m_y_);
    m_console_handler_->setTextColor(YELLOW);
    std::cout << m_head_;
}
void PacMan::renderScore()
{
    if (m_console_handler_ == 0)
        exit(NULL_POINTER_ERROR);
    m_console_handler_->setTextColor(WHITE);
    m_console_handler_->setCursorPosition(0, -Y_SCREEN_BOTTOM_OFFSET);
    std::cout << "SCORE: " << m_score_;
}
void PacMan::renderLives()
{
    if (m_console_handler_ == 0)
        exit(NULL_POINTER_ERROR);
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
    if (m_console_handler_ == 0)
        exit(NULL_POINTER_ERROR);
    ++m_kill_counter_;
    int sum = SCORE_POINTS_GHOST * static_cast<int>(pow(2, m_kill_counter_ - 1));
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

    Sleep(MILLISECONDS_AFTER_GHOST_DEATH);
    resetMapAfterKill(kill_pos_x, m_y_, digit_num);
    gainLife();
}