#include "Ghost.h"

Ghost::Ghost
(
    std::shared_ptr<ConsoleSettingsHandler> console_handler, 
    Game* game, 
    Ghost_Name his_name
) :
    m_console_handler_  (console_handler),
    m_game_instance_    (game),
    m_name_             (his_name),
    m_x_                (0),
    m_y_                (0),
    m_prev_x_           (0),
    m_prev_y_           (0),
    m_head_             (GHOST_HEAD),
    m_direction_        (Directions::DIRECTION_W),
    m_old_direction_    (Directions::DIRECTION_W),
    m_speed_            (GHOST_SPEED),
    m_move_counter_     (0),
    m_timer_            (0),
    m_timer_on_pause_   (0),
    m_check_to_unpause_ (false),
    m_first_exit        (true)
{
    if (m_console_handler_ == 0)
        exit(NULL_POINTER_ERROR);

    resetModes(his_name);
    setColor  (his_name);

    bool isInky  = his_name == Ghost_Name::GHOST_NAME_INKY;
    bool isClyde = his_name == Ghost_Name::GHOST_NAME_CLYDE;
    if (isInky || isClyde)
        m_timer_ = std::clock();
}
Ghost::~Ghost()
{
}
void Ghost::dead()
{
    setColor(WHITE);
    setPrevMode(m_current_mode_);
    setMode(Mode::MODE_DEAD);
    setPrevMode(Mode::MODE_FRIGHTENED);
    setSpeed(DEAD_GHOST_SPEED);
    m_head_ = '"';
}
void Ghost::scared()
{
    if (m_current_mode_ != Mode::MODE_DEAD)
        setColor(LIGHT_BLUE);

    bool isChasing    = m_current_mode_ == Mode::MODE_CHASE;
    bool isScattering = m_current_mode_ == Mode::MODE_SCATTER;
    if (isChasing || isScattering)
    {
        setMode(Mode::MODE_FRIGHTENED);
        setPrevMode(Mode::MODE_CHASE);
    }
}
void Ghost::braved()
{
    if (m_current_mode_ != Mode::MODE_DEAD)
        setColor(m_name_);
    else
        setMode(Mode::MODE_DEAD);

    if (m_current_mode_ == Mode::MODE_FRIGHTENED)
    {
        setPrevMode(m_current_mode_);
        setMode(Mode::MODE_CHASE);
    }
}
void Ghost::changeModeToOpposite()
{
    if (m_current_mode_ == Mode::MODE_CHASE)
    {
        setMode(Mode::MODE_SCATTER);
        setPrevMode(Mode::MODE_CHASE);
    }       
    else if (m_current_mode_ == Mode::MODE_SCATTER)
    {
        setMode(Mode::MODE_CHASE);
        setPrevMode(Mode::MODE_SCATTER);
    }
}
void Ghost::renderGhost()
{
    if (m_console_handler_ == 0)
        exit(NULL_POINTER_ERROR);
    m_console_handler_->setCursorPosition(m_x_, m_y_);
    m_console_handler_->setTextColor(m_color_);
    std::cout << m_head_;
}
void Ghost::renderMap()
{
    if (m_console_handler_ == 0)
        exit(NULL_POINTER_ERROR);
    if (m_game_instance_ == 0)
        exit(NULL_POINTER_ERROR);
    m_console_handler_->setTextColor(WHITE);
    m_console_handler_->setCursorPosition(m_x_, m_y_);
    std::cout << m_game_instance_->getCharOfBuffer(m_x_, m_y_);
}
void Ghost::resetGhost(const int x, const int y)
{
    setPos_X(x);
    setPos_Y(y);
    setColor(m_name_);
    setHead (GHOST_HEAD);
    setSpeed(GHOST_SPEED);
}
void Ghost::resetModes(const int name) {
    if      (name == Ghost_Name::GHOST_NAME_BLINCKY) m_current_mode_ = Mode::MODE_CHASE;
    else if (name == Ghost_Name::GHOST_NAME_PINKY  ) m_current_mode_ = Mode::MODE_EXIT_GATE;
    else if (name == Ghost_Name::GHOST_NAME_INKY   ) m_current_mode_ = Mode::MODE_WAIT;
    else if (name == Ghost_Name::GHOST_NAME_CLYDE  ) m_current_mode_ = Mode::MODE_WAIT;
    m_prev_mode_ = Mode::MODE_SCATTER;
}
void Ghost::modeActivity(const int pacman_x, const int pacman_y, const bool paused)
{
    if (isPaused(paused)) 
        return;
    switch (m_current_mode_) 
    {
    case Mode::MODE_CHASE      : handleChaseMode(pacman_x, pacman_y);      break;
    case Mode::MODE_SCATTER    : handleScatterMode();                      break;
    case Mode::MODE_FRIGHTENED : handleFrightenedMode(pacman_x, pacman_y); break;
    case Mode::MODE_DEAD       : handleDeadMode();                         break;
    case Mode::MODE_WAIT       : handleWaitMode();                         break;
    case Mode::MODE_EXIT_GATE  : handleExitMode();                         break;
    }
}
void Ghost::handleChaseMode(const int x, const int y)
{
    Directions dir = determineClosestMove(x, y);
    move(dir);
}
void Ghost::handleScatterMode()
{
    if      (m_name_ == Ghost_Name::GHOST_NAME_BLINCKY)
    {
        Directions dir = determineClosestMove(BLINKY_SCATTER_POS_X,
                                              BLINKY_SCATTER_POS_Y); 
        move(dir);
    }
    else if (m_name_ == Ghost_Name::GHOST_NAME_PINKY)
    {
        Directions dir = determineClosestMove(PINKY_SCATTER_POS_X,
                                              PINKY_SCATTER_POS_Y); 
        move(dir);
    }
    else if (m_name_ == Ghost_Name::GHOST_NAME_INKY)
    {
        Directions dir = determineClosestMove(INKY_SCATTER_POS_X,
                                              INKY_SCATTER_POS_Y); 
        move(dir);
    }
    else if (m_name_ == Ghost_Name::GHOST_NAME_CLYDE)
    {
        Directions dir = determineClosestMove(CLYDE_SCATTER_POS_X,
                                              CLYDE_SCATTER_POS_Y); 
        move(dir);
    }
}
void Ghost::handleFrightenedMode(const int x, const int y)
{
    Directions dir = determineFurthestMove(x, y);
    move(dir);
}
void Ghost::handleExitMode()
{
    if (m_x_ == X_GATE + 1 && m_y_ == Y_GATE)
    {
        m_direction_     = Directions::DIRECTION_A;
        m_old_direction_ = Directions::DIRECTION_A;
        renderMap();
        --m_x_;
        renderMap();
        renderGhost();
        setColor(m_name_);
        setMode(Mode::MODE_CHASE);
        setPrevModeAfterExit();
        setSpeed(GHOST_SPEED);
        return;
    }
    Directions dir = determineClosestMove(X_GATE + 1, Y_GATE);
    move(dir);
}
void Ghost::setPrevModeAfterExit()
{
    setPrevMode(Mode::MODE_EXIT_GATE);
    if (m_name_ == GHOST_NAME_PINKY && m_first_exit == true)
    {
        setPrevMode(Mode::MODE_SCATTER);
        m_first_exit = false;
    }
}
void Ghost::handleDeadMode()
{
    if (m_x_ == X_GATE - 1 && m_y_ == Y_GATE)
    {
        move(Directions::DIRECTION_D);
        return;
    }
    else if (m_x_ == X_GATE + 1 && m_y_ == Y_GATE)
    {
        setColor(m_name_);
        m_head_ = 'G';
        setMode(Mode::MODE_EXIT_GATE);
        setPrevMode(Mode::MODE_DEAD);
    }

    Directions dir = determineClosestMove(X_GATE + 1, Y_GATE);
    if (m_x_ == 4 && m_y_ == 10)
        dir = Directions::DIRECTION_S;
    else if (m_x_ == 4 && m_y_ == 13)
        dir = Directions::DIRECTION_D;

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
Directions Ghost::determineClosestMove(int pm_x, int pm_y)
{
    std::vector<Directions> appropriate_directions;
    determineAppropriateDirections(appropriate_directions);
    if (appropriate_directions.size() == 1)
        return appropriate_directions[0];
    else 
    {
        std::vector<int> counter_nums;
        std::unique_ptr<std::pair<int, Directions>[]> 
            pointer_counter_direction(new std::pair<int, Directions>[appropriate_directions.size()]);

        if (pointer_counter_direction == 0)
            exit(NULL_POINTER_ERROR);

        for (int i = 0; i < appropriate_directions.size(); ++i)
        {
            int ghost_x = m_x_ + offsetCoordinatesX(appropriate_directions[i], OFFSET_NEXT_STEP);
            int ghost_y = m_y_ + offsetCoordinatesY(appropriate_directions[i], OFFSET_NEXT_STEP);
            int counter = countDistance(ghost_x, pm_x) +
                          countDistance(ghost_y, pm_y);

            pointer_counter_direction[i].first  = counter;
            pointer_counter_direction[i].second = appropriate_directions[i];
        }

        int min = pointer_counter_direction[0].first;
        Directions closest = pointer_counter_direction[0].second;

        for (int i = 0; i < appropriate_directions.size(); ++i)
        {
            if (pointer_counter_direction[i].first < min)
            {
                min = pointer_counter_direction[i].first;
                closest = pointer_counter_direction[i].second;
            }
        }
        return closest;
    }
}
Directions Ghost::determineFurthestMove(int pm_x, int pm_y)
{
    std::vector<Directions> appropriate_directions;
    determineAppropriateDirections(appropriate_directions);
    if (appropriate_directions.size() == 1)
        return appropriate_directions[0];
    else
    {
        std::vector<int> counter_nums;
        std::unique_ptr<std::pair<int, Directions>[]>
            pointer_counter_direction(new std::pair<int, Directions>[appropriate_directions.size()]);

        if (pointer_counter_direction == 0)
            exit(NULL_POINTER_ERROR);

        for (int i = 0; i < appropriate_directions.size(); ++i)
        {
            int ghost_x = m_x_ + offsetCoordinatesX(appropriate_directions[i], OFFSET_NEXT_STEP);
            int ghost_y = m_y_ + offsetCoordinatesY(appropriate_directions[i], OFFSET_NEXT_STEP);
            int counter = countDistance(ghost_x, pm_x) +
                          countDistance(ghost_y, pm_y);

            pointer_counter_direction[i].first = counter;
            pointer_counter_direction[i].second = appropriate_directions[i];
        }

        int max = pointer_counter_direction[0].first;
        Directions furthest = pointer_counter_direction[0].second;

        for (int i = 0; i < appropriate_directions.size(); ++i)
        {
            if (pointer_counter_direction[i].first > max) 
            {
                max = pointer_counter_direction[i].first;
                furthest = pointer_counter_direction[i].second;
            }
        }
        return furthest;
    }
}
Directions Ghost::getOppositeDirection()
{
    switch (m_old_direction_)
    {
    case Directions::DIRECTION_W : return Directions::DIRECTION_S; break;
    case Directions::DIRECTION_A : return Directions::DIRECTION_D; break;
    case Directions::DIRECTION_S : return Directions::DIRECTION_W; break;
    case Directions::DIRECTION_D : return Directions::DIRECTION_A; break;
    default :  return Directions::DIRECTION_W; break;
    }
}
const int  Ghost::offsetCoordinatesX(const Directions direction, const int offset_num)
{
    if      (direction == Directions::DIRECTION_A) return -offset_num;
    else if (direction == Directions::DIRECTION_D) return  offset_num;
    return 0;
}
const int  Ghost::offsetCoordinatesY(const Directions direction, const int offset_num)
{
    if      (direction == 0) return -offset_num;
    else if (direction == 2) return  offset_num;
    return 0;
}
const int  Ghost::countDistance(const int start_point, const int end_point)
{
    int counter = 0;
    int ghost_start = start_point;
    while (ghost_start != end_point)
    {
        if (ghost_start > end_point)
            --ghost_start;
        else
            ++ghost_start;
        ++counter;
    }
    return counter;
}
void       Ghost::determineAppropriateDirections(std::vector<Directions>& vector_directions)
{
    addAppropriateDirection(vector_directions, Directions::DIRECTION_W);
    addAppropriateDirection(vector_directions, Directions::DIRECTION_A);
    addAppropriateDirection(vector_directions, Directions::DIRECTION_S);
    addAppropriateDirection(vector_directions, Directions::DIRECTION_D);
}
void       Ghost::addAppropriateDirection(std::vector<Directions>& vector_directions, Directions direction)
{
    bool moving_ahead = direction != getOppositeDirection();
    bool move_opportunity = !checkCollision(direction);
    if (moving_ahead && move_opportunity)
        vector_directions.push_back(direction);
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
const int  Ghost::getInkyPos_X(const int pacman_x, const int blincky_x, const Directions pacman_direction)
{
    int target_position = pacman_x + offsetCoordinatesX(pacman_direction, OFFSET_PACMAN_INKY);
    int start_position = blincky_x;
    int counter = countDistance(start_position, target_position);

    if      (pacman_direction == Directions::DIRECTION_A) return blincky_x - (counter * DOUBLE_DISTANSE);
    else if (pacman_direction == Directions::DIRECTION_S) return blincky_x + (counter * DOUBLE_DISTANSE);
    return pacman_x;
}
const int  Ghost::getInkyPos_Y(const int pacman_y, const int blincky_y, const Directions pacman_direction)
{
    int target_position = pacman_y + offsetCoordinatesX(pacman_direction, OFFSET_PACMAN_INKY);
    int start_position = blincky_y;
    int counter = countDistance(start_position, target_position);

    if      (pacman_direction == Directions::DIRECTION_W) return blincky_y - (counter * DOUBLE_DISTANSE);
    else if (pacman_direction == Directions::DIRECTION_S) return blincky_y + (counter * DOUBLE_DISTANSE);
    return pacman_y;
}
const int  Ghost::getClydeCountPos_X(const int pacman_x)
{
    return countDistance(m_x_, pacman_x);
}
const int  Ghost::getClydeCountPos_Y(const int pacman_y)
{
    return countDistance(m_y_, pacman_y);
}
const bool Ghost::checkCollision(const Directions direction)
{
    if (m_game_instance_ == 0)
        exit(NULL_POINTER_ERROR);
    switch (direction)
    {
    case Directions::DIRECTION_W :
        if (strchr(CharNotToCollide, 
            m_game_instance_->getCharOfBuffer(m_x_, m_y_ - 1)))
            return false;
        break;

    case Directions::DIRECTION_A :
        if (m_x_ == 0 || strchr(CharNotToCollide, 
            m_game_instance_->getCharOfBuffer(m_x_ - 1, m_y_))) 
            return false;
        break;

    case Directions::DIRECTION_S :
        if (strchr(CharNotToCollide, 
            m_game_instance_->getCharOfBuffer(m_x_, m_y_ + 1))) 
            return false;
        break;

    case Directions::DIRECTION_D :
        if (m_x_ == X_SIZE - 1 || strchr(CharNotToCollide, 
            m_game_instance_->getCharOfBuffer(m_x_ + 1, m_y_))) 
            return false;
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
void       Ghost::move(const Directions direction)
{
    if (m_move_counter_ != 0)
    {
        m_move_counter_--;
    }
    else
    {
        renderMap();
        if (direction == Directions::DIRECTION_W)
            --m_y_;

        if (direction == Directions::DIRECTION_A)
        {
            if (m_x_ == 0)
                m_x_ = X_SIZE - 1;
            else 
                --m_x_;
        }
        if (direction == Directions::DIRECTION_S)
            ++m_y_;

        if (direction == Directions::DIRECTION_D)
        {
            if (m_x_ == X_SIZE - 1)
                m_x_ = 0;
            else 
                ++m_x_;
        }
        m_old_direction_ = direction;
        m_move_counter_  = m_speed_;
    }
}