#include "Ghost.h"

Ghost::Ghost(std::shared_ptr<ConsoleSettingsHandler> console_handler, Game* game, Ghost_Name his_name) :
    m_console_handler_(console_handler),
    m_game_instance_(game),
    m_name_(his_name),
    m_x_(0),
    m_y_(0),
    m_prev_x_(0),
    m_prev_y_(0),
    m_head_(GHOST_HEAD),
    m_direction_(DEFAULT_DIRECTION),
    m_old_direction_(DEFAULT_DIRECTION),
    m_speed_(GHOST_SPEED),
    m_move_counter_(0),
    m_timer_(0),
    m_timer_on_pause_(0),
    m_check_to_unpause_(false)
{
    if (m_console_handler_ == 0)
        exit(1);

    resetModes(his_name);
    setColor(his_name);
    if (his_name == Ghost_Name::GHOST_NAME_INKY ||
        his_name == Ghost_Name::GHOST_NAME_CLYDE)
    {
        m_timer_ = std::clock();
    }
    //caretaker_ghost = std::make_unique<CareTakerGhost>();
}
Ghost::~Ghost()
{
}
void Ghost::dead()
{
    setColor(WHITE);
    setPrevMode(m_current_mode_);
    setMode(Mode::MODE_DEAD);
    m_head_ = '"';
}
void Ghost::renderGhost()
{
    m_console_handler_->setCursorPosition(m_x_, m_y_);
    m_console_handler_->setTextColor(m_color_);
    std::cout << m_head_;
}
void Ghost::renderMap()
{
    m_console_handler_->setTextColor(WHITE);
    m_console_handler_->setCursorPosition(m_x_, m_y_);
    std::cout << m_game_instance_->getCharOfBuffer(m_x_, m_y_);
}
void Ghost::resetGhost(const int x, const int y)
{
    setPos_X(x);
    setPos_Y(y);
    setColor(m_name_);
}
void Ghost::resetModes(const int name) {
    if      (name == Ghost_Name::GHOST_NAME_BLINCKY) m_current_mode_ = Mode::MODE_CHASE;
    else if (name == Ghost_Name::GHOST_NAME_PINKY  ) m_current_mode_ = Mode::MODE_EXIT_GATE;
    else if (name == Ghost_Name::GHOST_NAME_INKY   ) m_current_mode_ = Mode::MODE_WAIT;
    else if (name == Ghost_Name::GHOST_NAME_CLYDE  ) m_current_mode_ = Mode::MODE_WAIT;
    m_prev_mode_ = m_current_mode_;
}
void Ghost::modeActivity(const int pacman_x, const int pacman_y, const bool paused)
{
    if (isPaused(paused)) 
        return;

    switch (m_current_mode_) 
    {
        case Mode::MODE_CHASE:
        {
            handleChaseMode(pacman_x, pacman_y);
        }
        break;

        case Mode::MODE_SCATTER:
        {
            handleScatterMode();
        }
        break;

        case Mode::MODE_FRIGHTENED:
        {
            handleFrightenedMode(pacman_x, pacman_y);
        }
        break;

        case Mode::MODE_DEAD:
        {
            handleDeadMode();
        }
        break;

        case Mode::MODE_WAIT:
        {
            handleWaitMode();
        }
        break;

        case Mode::MODE_EXIT_GATE:
        {
            handleExitMode();
        }
        break;
    }
}
void Ghost::handleChaseMode(const int x, const int y)
{
    char dir = determineClosestMove(x, y);
    move(dir);
}
void Ghost::handleScatterMode()
{
    if (m_name_ == Ghost_Name::GHOST_NAME_BLINCKY)
    {
        char dir = determineClosestMove(BLINKY_SCATTER_POS_X, BLINKY_SCATTER_POS_Y); // right-up corner
        move(dir);
    }
    else if (m_name_ == Ghost_Name::GHOST_NAME_PINKY)
    {
        char dir = determineClosestMove(PINKY_SCATTER_POS_X, PINKY_SCATTER_POS_Y); // right-down corner
        move(dir);
    }
    else if (m_name_ == Ghost_Name::GHOST_NAME_INKY)
    {
        char dir = determineClosestMove(INKY_SCATTER_POS_X, INKY_SCATTER_POS_Y); // left-up corner
        move(dir);
    }
    else if (m_name_ == Ghost_Name::GHOST_NAME_CLYDE)
    {
        char dir = determineClosestMove(CLYDE_SCATTER_POS_X, CLYDE_SCATTER_POS_Y); // left-down corner
        move(dir);
    }
}
void Ghost::handleFrightenedMode(const int x, const int y)
{
    char dir = determineFurthestMove(x, y);
    move(dir);
}
void Ghost::handleExitMode()
{
    if (m_x_ == X_GATE + 1 && m_y_ == Y_GATE)
    {
        m_direction_ = 'a';
        m_old_direction_ = 'a';
        renderMap();
        --m_x_;
        renderMap();
        renderGhost();
        setMode(Mode::MODE_CHASE);
        return;
    }
    char dir = determineClosestMove(X_GATE + 1, Y_GATE);
    move(dir);
}
void Ghost::handleDeadMode()
{
    if (m_x_ == X_GATE - 1 && m_y_ == Y_GATE)
    {
        move('d');
        return;
    }
    else if (m_x_ == X_GATE + 1 && m_y_ == Y_GATE)
    {
        setColor(m_name_);
        m_head_ = 'G';
        setMode(Mode::MODE_EXIT_GATE);
    }

    char dir = determineClosestMove(X_GATE + 1, Y_GATE);
    if (m_x_ == 4 && m_y_ == 10)
        dir = 's';
    else if (m_x_ == 4 && m_y_ == 13)
        dir = 'd';

    move(dir);
    m_old_direction_ = dir;
}
void Ghost::handleWaitMode()
{
    if (getTimeInWait() >= 1.0)
    {
        renderMap();

        if (m_x_ == X_GATE + 1)
            ++m_x_;
        else 
            --m_x_;

        renderGhost();
        m_timer_ = std::clock();
    }
}
const char Ghost::determineClosestMove(int pm_x, int pm_y)
{
    std::vector<int> dirs_num;
    if (Direction[0] != getOppositeDirection() && !checkCollision(Direction[0])) dirs_num.push_back(0);
    if (Direction[1] != getOppositeDirection() && !checkCollision(Direction[1])) dirs_num.push_back(1);
    if (Direction[2] != getOppositeDirection() && !checkCollision(Direction[2])) dirs_num.push_back(2);
    if (Direction[3] != getOppositeDirection() && !checkCollision(Direction[3])) dirs_num.push_back(3);
    if (dirs_num.size() == 1)
    {
        return Direction[dirs_num[0]];
    }
    else 
    {
        std::vector<int> counter_nums;
        std::pair<int, int>* pointer_counter_direction = new std::pair<int, int>[dirs_num.size()];

        for (int i = 0; i < dirs_num.size(); ++i)
        {
            int ghost_x = m_x_ + offsetCoordinatesX(dirs_num[i]);
            int ghost_y = m_y_ + offsetCoordinatesY(dirs_num[i]);
            int counter = 0;

            while (ghost_x != pm_x)
            {
                if (ghost_x > pm_x) 
                    --ghost_x;
                else 
                    ++ghost_x;

                ++counter;
            }

            while (ghost_y != pm_y)
            {
                if (ghost_y > pm_y) 
                    --ghost_y;
                else 
                    ++ghost_y;

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
const char Ghost::determineFurthestMove(int pm_x, int pm_y)
{
    std::vector<int> dirs_num;
    if (Direction[0] != getOppositeDirection() && !checkCollision(Direction[0])) dirs_num.push_back(0);
    if (Direction[1] != getOppositeDirection() && !checkCollision(Direction[1])) dirs_num.push_back(1);
    if (Direction[2] != getOppositeDirection() && !checkCollision(Direction[2])) dirs_num.push_back(2);
    if (Direction[3] != getOppositeDirection() && !checkCollision(Direction[3])) dirs_num.push_back(3);
    if (dirs_num.size() == 1)
    {
        return Direction[dirs_num[0]];
    }
    else
    {
        std::vector<int> counter_nums;
        std::pair<int, int>* pointer_counter_direction = new std::pair<int, int>[dirs_num.size()];
        for (int i = 0; i < dirs_num.size(); ++i)
        {
            int ghost_x = m_x_ + offsetCoordinatesX(dirs_num[i]);
            int ghost_y = m_y_ + offsetCoordinatesY(dirs_num[i]);
            int counter = 0;

            while (ghost_x != pm_x)
            {
                if (ghost_x > pm_x) 
                    --ghost_x;
                else 
                    ++ghost_x;

                ++counter;
            }

            while (ghost_y != pm_y)
            {
                if (ghost_y > pm_y) 
                    --ghost_y;
                else 
                    ++ghost_y;

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
const char Ghost::getOppositeDirection()
{
    if (m_old_direction_ == 'w') return 's';
    if (m_old_direction_ == 'a') return 'd';
    if (m_old_direction_ == 's') return 'w';
    if (m_old_direction_ == 'd') return 'a';
}
const int  Ghost::offsetCoordinatesX(const int dir)
{
    if      (dir == 1) return -1; // offset LEFT
    else if (dir == 3) return  1; // offset RIGHT
    return 0;
}
const int  Ghost::offsetCoordinatesY(const int dir)
{
    if      (dir == 0) return -1; // offset UP, '0' - W
    else if (dir == 2) return 1 ; // offset DOWN, '2' - D
    return 0;
}
void       Ghost::setColor(const Ghost_Name ghost_name)
{
    switch (ghost_name)
    {
        case Ghost_Name::GHOST_NAME_BLINCKY : { m_color_ = RED;    } break;
        case Ghost_Name::GHOST_NAME_PINKY   : { m_color_ = PINK;   } break;
        case Ghost_Name::GHOST_NAME_INKY    : { m_color_ = CYAN;   } break;
        case Ghost_Name::GHOST_NAME_CLYDE   : { m_color_ = YELLOW; } break;
    }
}
const int  Ghost::getInkyPos_X(const int pacman_x, const int blincky_x)
{
    int ghost_x = blincky_x;
    int counter = 0;
    while (ghost_x != pacman_x + 2)
    {
        if (ghost_x > pacman_x + 2) 
            --ghost_x;
        else 
            ++ghost_x;

        ++counter;
    }
    return pacman_x + 2 + counter;
}
const int  Ghost::getInkyPos_Y(const int pacman_y, const int blincky_y)
{
    int ghost_y = blincky_y;
    int counter = 0;
    while (ghost_y != pacman_y + 2)
    {
        if (ghost_y > pacman_y + 2) 
            --ghost_y;
        else 
            ++ghost_y;

        ++counter;
    }
    return pacman_y + 2 + counter;
}
const int  Ghost::getClydeCountPos_X(const int pacman_x)
{
    int ghost_x = pacman_x;
    int counter = 0;
    while (ghost_x != pacman_x)
    {
        if (ghost_x > pacman_x) 
            --ghost_x;
        else 
            ++ghost_x;

        ++counter;
    }
    return counter;
}
const int  Ghost::getClydeCountPos_Y(const int pacman_y)
{
    int ghost_y = pacman_y;
    int counter = 0;
    while (ghost_y != pacman_y)
    {
        if (ghost_y > pacman_y)
            --ghost_y;
        else 
            ++ghost_y;

        ++counter;
    }
    return counter;
}
const bool Ghost::checkCollision(const char dir)
{
    switch (dir) 
    {
        case 'w': if (strchr(CharNotToCollide, m_game_instance_->getCharOfBuffer(m_x_, m_y_ - 1)))return false;
        break;

        case 'a': if (m_x_ == 0 || strchr(CharNotToCollide, m_game_instance_->getCharOfBuffer(m_x_ - 1, m_y_))) return false;
        break;

        case 's': if (strchr(CharNotToCollide, m_game_instance_->getCharOfBuffer(m_x_, m_y_ + 1))) return false;
        break;

        case 'd': if (m_x_ == X_SIZE - 1 || strchr(CharNotToCollide, m_game_instance_->getCharOfBuffer(m_x_ + 1, m_y_))) return false;
        break;
    }
    return true;
}
const bool Ghost::isPaused(const bool paused)
{
    if (paused == true)
    {
        m_timer_on_pause_  = m_timer_;
        m_check_to_unpause_ = true;
        return true;
    }
    else if (m_check_to_unpause_ == true)
    {
        m_timer_ = m_timer_on_pause_;
        m_check_to_unpause_ = false;
        return false;
    }
    return false;
}
void       Ghost::move(const char dir)
{
    if (m_move_counter_)
    {
        m_move_counter_--;
    }
    else
    {
        renderMap();
        if (dir == 'w') 
            --m_y_;

        if (dir == 'a') 
        {
            if (m_x_ == 0)
                m_x_ = X_SIZE - 1;
            else 
                --m_x_;
        }
        if (dir == 's') 
            ++m_y_;

        if (dir == 'd') 
        {
            if (m_x_ == X_SIZE - 1)
                m_x_ = 0;
            else 
                ++m_x_;
        }
        m_old_direction_ = dir;
        m_move_counter_  = m_speed_;
    }
}

inline const int        Ghost::getPos_X()                 { return m_x_;                  }
inline const int        Ghost::getPos_Y()                 { return m_y_;                  }
inline const int        Ghost::getColor()                 { return m_color_;              }
inline const char       Ghost::getDirection()             { return 0;                     }
inline const Mode       Ghost::getMode()                  { return m_current_mode_;       }
inline const Ghost_Name Ghost::getName()                  { return m_name_;               }

inline void Ghost::setMode         (const Mode mode)      { m_current_mode_ = mode;       }
inline void Ghost::setHead         (const char head)      { m_head_ = head;               }
inline void Ghost::setPrevMode     (const Mode mode)      { m_prev_mode_ = mode;          }
inline void Ghost::setColor        (const int color)      { m_color_ = color;             }
inline void Ghost::setPos_X        (const int x_pos)      { m_x_ = x_pos;                 }
inline void Ghost::setPos_Y        (const int y_pos)      { m_y_ = y_pos;                 }
inline void Ghost::setDirection    (const char direction) { m_direction_ = direction;     }
inline void Ghost::setOldDirection (const char direction) { m_old_direction_ = direction; }

inline const double Ghost::getTimeInWait()                { return (std::clock() - m_timer_) / (double)CLOCKS_PER_SEC; }