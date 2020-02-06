#include "Renderer.h"

Renderer::Renderer(std::shared_ptr<ConsoleSettingsHandler> console_handler) : 
    m_console_handler_(console_handler),
    m_digit_num_      (0),
    m_kill_pos_x_     (0),
    m_kill_pos_y_     (0)
{
}

Renderer::~Renderer()
{
}

void Renderer::renderLives(const int lives) const
{
    if (m_console_handler_ == 0)
        exit(NULL_POINTER_ERROR);
    SetConsoleOutputCP(CP_UTF8);
    m_console_handler_->setTextColor(YELLOW);
    m_console_handler_->setCursorPosition(1, Y_SIZE);
    std::cout << "Lives: ";
    m_console_handler_->setTextColor(RED);
    for (unsigned int i = 0; i < lives; ++i)
    {
        //"\xE2\x99\xA5" heart sign in UTF-8
        std::cout << "\xE2\x99\xA5 ";
    }
    std::cout << ' ';
    m_console_handler_->resetSettingsToDefault();
}

void Renderer::renderScore(const int score) const
{
    if (m_console_handler_ == 0)
        exit(NULL_POINTER_ERROR);
    m_console_handler_->setTextColor(WHITE);
    m_console_handler_->setCursorPosition(0, -Y_SCREEN_BOTTOM_OFFSET);
    std::cout << "SCORE: " << score;
}

void Renderer::renderPacman(const int x_pacman, const int y_pacman, const unsigned char head) const
{
    if (m_console_handler_ == 0)
        exit(NULL_POINTER_ERROR);
    m_console_handler_->setCursorPosition(x_pacman, y_pacman);
    m_console_handler_->setTextColor(YELLOW);
    std::cout << head;
}

void Renderer::renderDeadPacman(const int x_pacman, const int y_pacman, const unsigned char head)
{
    unsigned char head_curr = head;
    for (int i = 0; i < BLINKYNG_COUNTER; ++i)
    {
        if (i % 2)
            head_curr = ' ';
        else
            head_curr = head;
        renderPacman(x_pacman, y_pacman, head_curr);
        Sleep(MILLISECONDS_BLINKING_TIME);
    }
}

void Renderer::renderPacmanKill(const int pacman_x, const int pacman_y, const int kill_counter)
{
    if (m_console_handler_ == 0)
        exit(NULL_POINTER_ERROR);
    int sum = SCORE_POINTS_GHOST * static_cast<int>(pow(2, kill_counter - 1));
    int temp = sum;
    m_digit_num_ = 1;
    while ((temp /= 10) > 0)
    {
        ++m_digit_num_;
    }

    m_kill_pos_x_ = pacman_x - 1;
    m_kill_pos_y_ = pacman_y;
    if (pacman_x == 0)
    {
        m_kill_pos_x_ = pacman_x;
    }
    if (pacman_x > X_SIZE - m_digit_num_)
    {
        m_kill_pos_x_ = X_SIZE - m_digit_num_;
    }
    m_console_handler_->setTextColor(CYAN);
    m_console_handler_->setCursorPosition(m_kill_pos_x_, m_kill_pos_y_);
    std::cout << sum;
    Sleep(MILLISECONDS_AFTER_GHOST_DEATH);
}

void Renderer::resetMapAfterPacmanKill(const unsigned char element) 
{
    if (m_console_handler_ == 0)
        exit(NULL_POINTER_ERROR);
    m_console_handler_->setCursorPosition(m_kill_pos_x_, m_kill_pos_y_);
    m_console_handler_->setTextColor(BLUE);
    if (element == static_cast<char>(250) || element == 'o')
        m_console_handler_->setTextColor(WHITE);
    std::cout << element;
    ++m_kill_pos_x_;
}

void Renderer::renderPacmanPrevPos
(
    const int  prev_x, 
    const int  prev_y, 
    const int  pacman_x, 
    const int  pacman_y,
    const char map_element
) const
{
    if (m_console_handler_ == 0)
        exit(NULL_POINTER_ERROR);
    if (prev_x != pacman_x || prev_y != pacman_y)
    {
        m_console_handler_->setCursorPosition(prev_x, prev_y);
        m_console_handler_->resetSettingsToDefault();
        std::cout << map_element;
    }
}