#include "Game.h"

Game* Game::m_GameInstance = 0;
Game* Game::getInstance(std::shared_ptr<ConsoleSettingsHandler> console_handler)
{
    if (m_GameInstance == 0)
    {
        m_GameInstance = new Game(console_handler);
    }
    return m_GameInstance;
}

Game::Game(std::shared_ptr<ConsoleSettingsHandler> console_handler) :
    m_console_handler_(console_handler),
    m_seconds_in_boost_by_level_(SECODS_IN_BOOST_BY_LEVEL),
    m_seconds_to_mode_change_(SECODS_TO_CHANGE_MODE),
    m_number_of_ghosts_(NUMBER_OF_GHOSTS),
    m_points_num_(POINTS_NUMBER),
    m_level_counter_(0),
    m_isPaused_(false),
    m_isGameover_(false),
    m_check_to_unpause_(true),
    m_timer_(0),
    m_timer2_(0),
    m_temp_timer1_(0),
    m_temp_timer2_(0)
{
    if (m_console_handler_ == 0) 
        exit(1);
    SetConsoleTitle("PacMan");
    m_console_handler_->createGameWindow();
    m_console_handler_->hanldeCursorVisibility(false);
}
Game::~Game()
{
}
void Game::start() 
{
    m_menu_ = std::make_unique<GameMenu>(m_console_handler_);
    if (m_menu_ == 0) 
        exit(1);

    while (m_menu_->getChoise() != (NUM_OF_MENU_TITLES - 1))
    {
        m_menu_->renderMenu();
        if (m_menu_->getChoise() == 0)
        {
            m_pacman_ = std::make_unique<PacMan>(m_console_handler_, Game::getInstance(m_console_handler_));
            for (int i = 0; i < m_number_of_ghosts_; ++i)
            {
                m_ghosts_[i] = std::make_unique<Ghost>(m_console_handler_, Game::getInstance(m_console_handler_), (Ghost_Name)i);
            }
            checkPointersToActors();
            game_Loop();
            system("CLS");
        }
        m_menu_->setChoise(UNDEFINED_CHOISE);
        m_menu_->makeChoise();
    }
}
void Game::pause()
{
    if (isKeyOncePressed(VK_ESCAPE))
    {
        m_isPaused_ = true;
        m_check_to_unpause_ = true;
        m_temp_timer1_ = m_timer_;
        m_temp_timer2_ = m_timer2_;
    }
    else
    {
        m_isPaused_ = false;
        m_timer_ = m_temp_timer1_;
        m_timer2_ = m_temp_timer2_;
    }
}
void Game::game_Loop() 
{
    m_isGameover_ = false;
    checkPointersToActors();
    m_pacman_->setLives(NUMBER_OF_LIVES);

    for (m_level_counter_ = 0; m_level_counter_ < 255; ++m_level_counter_)
    {
        if (m_seconds_in_boost_by_level_ < 0.0) 
            m_seconds_in_boost_by_level_ = 0.0;
        else
            m_seconds_in_boost_by_level_ -= m_level_counter_; 
        loadLevel();
        setMazeText("Get Ready!", YELLOW);
        startLevel();
        if (m_isGameover_)
        {
            m_console_handler_->setTextColor(RED);
            m_console_handler_->setCursorPosition(19, 7);
            std::cout << "GAME OVER";
            Sleep(1500);
            break;
        }
    }
}
void Game::startLevel()
{
    while (m_points_num_ != 0)
    {
        pause();
        if (renderPause(m_isPaused_)) 
            continue;

        m_pacman_->move(m_isPaused_);
        moveGhosts(); // move pacman then ghosts, then check 
        if (collisionWithGhost())
        {
            resetMapInCollision();
            initializeAllActors();
            render();
            while (!_kbhit());
        }
        if (isDead())
        {
            m_isGameover_ = true;
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
    m_console_handler_->setTextColor(WHITE);
    m_console_handler_->setCursorPosition(0, -2);
    cout << "1UP";
    m_console_handler_->setCursorPosition(36, -2);
    cout << "LEVEL: " << m_level_counter_;

    m_console_handler_->setCursorPosition(0, 0);
    for (int y = 0; y < Y_SIZE; ++y)
    {
        for (int x = 0; x < X_SIZE; ++x)
        {
            char_to_print = Map[y][x];
            setCharFromMap(char_to_print, x, y);
            cout << m_MapToPrint_[y][x];
        }
        m_console_handler_->setCursorPosition(0, y + Y_GAME_SCREEN_TOP_SIDE_OFFSET);
    }
    setPointsNum(POINTS_NUMBER);
    initializeAllActors();
    render();
}
void Game::render()
{
    checkPointersToActors();
    m_pacman_->renderPacman();
    m_pacman_->renderScore();
    m_pacman_->renderLives();
    for (int i = 0; i < m_number_of_ghosts_; ++i)
    {
        m_ghosts_[i]->renderGhost();
    }
}
const bool Game::renderPause(const bool paused)
{
    if (paused)
    {
        if (m_console_handler_ == 0) 
            exit(1);
        m_console_handler_->setTextColor(YELLOW);
        m_console_handler_->setCursorPosition(X_MIDDLE_POS, Y_MIDDLE_POS);
        std::cout << "  PAUSE  ";
        return true;
    }
    else if(m_check_to_unpause_)
    {
        if (m_console_handler_ == 0)
            exit(1);
        memcpy(m_substring_, &m_MapToPrint_[Y_MIDDLE_POS][X_MIDDLE_POS], 9);
        m_substring_[9] = '\0';
        m_console_handler_->setCursorPosition(X_MIDDLE_POS, Y_MIDDLE_POS);
        m_console_handler_->setTextColor(WHITE);
        std::cout << m_substring_;
        m_check_to_unpause_ = false;
        return false;
    }
    return false;
}
const bool Game::isDead()
{
    return !m_pacman_->getLives();
}
const bool Game::collisionWithGhost()
{
    checkPointersToActors();
    for (int i = 0; i < m_number_of_ghosts_; ++i)
    {
        if (m_pacman_->getPos_X()  == m_ghosts_[i]->getPos_X() &&
            m_pacman_->getPos_Y()  == m_ghosts_[i]->getPos_Y() &&
            m_ghosts_[i]->getMode() != Mode::MODE_DEAD)
        {
            if (m_ghosts_[i]->getMode() != Mode::MODE_FRIGHTENED)
            {
                m_pacman_->dead();
                return true;
            }
            else
            {
                m_ghosts_[i]->dead();
                m_pacman_->renderKill();
                return false;
            }
        }
    }
    return false;
}
void Game::moveGhosts()
{
    checkPointersToActors();
    if (m_pacman_->getEnergizerInfo())
    {
        for (int i = 0; i < m_number_of_ghosts_; ++i)
        {
            if (m_ghosts_[i]->getMode() != Mode::MODE_DEAD)
                m_ghosts_[i]->setColor(LIGHT_BLUE);
            
            if (m_ghosts_[i]->getMode() == Mode::MODE_CHASE || m_ghosts_[i]->getMode() == Mode::MODE_SCATTER)
                m_ghosts_[i]->setMode(Mode::MODE_FRIGHTENED);
        }
    }
    if (m_points_num_ <= 330 && m_ghosts_[2]->getMode() == Mode::MODE_WAIT)
        m_ghosts_[2]->setMode(Mode::MODE_EXIT_GATE);

    if (m_points_num_ <= 240 && m_ghosts_[3]->getMode() == Mode::MODE_WAIT)
        m_ghosts_[3]->setMode(Mode::MODE_EXIT_GATE);

    determinatePositionForModeActivity();
    render();
}
void Game::handleTime() 
{
    checkPointersToActors();
    if (m_pacman_->getEnergizerInfo())
    {
        if (m_pacman_->getTimeInBooste() >= m_seconds_in_boost_by_level_) 
            m_pacman_->setEnergizerInfo(false); 
        if (m_pacman_->getTimeInBooste() >= m_seconds_in_boost_by_level_ * 0.75) 
        { 
            for (int i = 0; i < m_number_of_ghosts_; ++i)
            {
                if (m_ghosts_[i]->getColor() == BLUE)
                {
                    m_ghosts_[i]->setColor(WHITE);
                    render();
                }
            }
        }
    }
    else 
    {
        for (int i = 0; i < m_number_of_ghosts_; ++i)
        {
            if (m_ghosts_[i]->getMode() != Mode::MODE_DEAD)
            {
                m_ghosts_[i]->setColor(m_ghosts_[i]->getName());
            }
            if (m_ghosts_[i]->getMode() == Mode::MODE_FRIGHTENED)
            {
                m_ghosts_[i]->setPrevMode(m_ghosts_[i]->getMode());
                m_ghosts_[i]->setMode(Mode::MODE_CHASE);
                m_timer_ = std::clock();
            }
        }
    }

    if (getTime2() >= m_seconds_to_mode_change_)
    {
        for (int i = 0; i < m_number_of_ghosts_; ++i)
        {
            if      (m_ghosts_[i]->getMode() == Mode::MODE_CHASE)
                     m_ghosts_[i]->setMode(Mode::MODE_SCATTER);
            else if (m_ghosts_[i]->getMode() == Mode::MODE_SCATTER)
                     m_ghosts_[i]->setMode(Mode::MODE_CHASE);
        }
    }
    else m_timer2_ = std::clock();
}
void Game::initializeAllActors()
{
    checkPointersToActors();
    m_pacman_->setEnergizerInfo(false);
    m_pacman_->resetPacMan  (PACMAN_INIT_POS_X, PACMAN_INIT_POS_Y);
    m_ghosts_[0]->resetGhost (BLINKY_INIT_POS_X, BLINKY_INIT_POS_Y);
    m_ghosts_[1]->resetGhost (PINKY_INIT_POS_X , PINKY_INIT_POS_Y );
    m_ghosts_[2]->resetGhost (INKY_INIT_POS_X  , INKY_INIT_POS_Y  );
    m_ghosts_[3]->resetGhost (CLYDE_INIT_POS_X , CLYDE_INIT_POS_Y );
    m_timer_ = std::clock();

    for (int i = 0; i < m_number_of_ghosts_; ++i)
    {
        m_ghosts_[i]->resetModes   (m_ghosts_[i]->getName());
        m_ghosts_[i]->setColor(m_ghosts_[i]->getName());
        m_ghosts_[i]->setHead      (GHOST_HEAD);
    }
}
void Game::resetMapInCollision()
{
    checkPointersToActors();
    if (m_console_handler_ == 0) 
        exit(1);

    m_console_handler_->setCursorPosition(m_pacman_->getPos_X(), m_pacman_->getPos_Y());
    std::cout << m_MapToPrint_[m_pacman_->getPos_Y()][m_pacman_->getPos_X()];

    for (int i = 0; i < m_number_of_ghosts_; i++)
    {
        m_console_handler_->setCursorPosition(m_ghosts_[i]->getPos_X(), m_ghosts_[i]->getPos_Y());
        std::cout << m_MapToPrint_[m_ghosts_[i]->getPos_Y()][m_ghosts_[i]->getPos_X()];
    }
}
void Game::setMazeText(std::string text, int color)
{
    if (m_console_handler_ == 0) 
        exit(1);

    m_console_handler_->setTextColor(color);
    m_console_handler_->setCursorPosition(X_MIDDLE_POS, Y_MIDDLE_POS);
    std::cout << text;
    
    while (!_kbhit());
    //char* substring = new char[text.size() + 1];          //a substring to recover the maze in the console
    memcpy(m_substring_, &m_MapToPrint_[Y_MIDDLE_POS][X_MIDDLE_POS], text.size());
    m_substring_[text.size()] = '\0';

    m_console_handler_->setCursorPosition(X_MIDDLE_POS, Y_MIDDLE_POS);
    m_console_handler_->setTextColor(WHITE);
    std::cout << m_substring_;
    //delete substring;
}
void Game::setCharFromMap(const char ch, const int x_offset, const int y_offset)
{
    for (auto i = MapCharToPtint.begin(); i != MapCharToPtint.end(); ++i)
    {
        if (ch == '.' || ch == 'o')
        {
            m_console_handler_->setTextColor(WHITE);
        }
        else
        {
            m_console_handler_->setTextColor(BLUE);
        }
        if (ch == i->first)
        {
            m_MapToPrint_[y_offset][x_offset] = i->second;
        }
    }
}
void Game::checkPointersToActors() 
{
    if (m_pacman_ == 0) 
        exit(1);

    for (int i = 0; i < m_number_of_ghosts_; ++i)
    {
        if (m_ghosts_[i] == 0) 
            exit(1);
    }
}
void Game::determinatePositionForModeActivity()
{
    for (int i = 0; i < m_number_of_ghosts_; ++i)
    {
        if (m_ghosts_[i] == 0) 
            exit(1);

        switch (i)
        {
        case 0:
        {
            m_ghosts_[0]->modeActivity(
                m_pacman_->getPos_X(), 
                m_pacman_->getPos_Y(), 
                m_isPaused_);
        }
        break;

        case 1:
        {
            m_ghosts_[1]->modeActivity(
                m_pacman_->getPos_X() + OFFSET_PINKY_POSITION, 
                m_pacman_->getPos_Y() + OFFSET_PINKY_POSITION, 
                m_isPaused_);
        }
        break;

        case 2:
        {
            m_ghosts_[2]->modeActivity(
                m_ghosts_[2]->getInkyPos_X(m_pacman_->getPos_X(), m_ghosts_[0]->getPos_X()), 
                m_ghosts_[2]->getInkyPos_Y(m_pacman_->getPos_Y(), m_ghosts_[0]->getPos_X()),
                m_isPaused_);
        }
        break;

        case 3:
        {
            int sum = m_ghosts_[3]->getClydeCountPos_X(m_pacman_->getPos_X()) +
                      m_ghosts_[i]->getClydeCountPos_Y(m_pacman_->getPos_Y()); 

            if (sum > 8) 
                m_ghosts_[3]->modeActivity(m_pacman_->getPos_X(), m_pacman_->getPos_Y(), m_isPaused_);
            else 
                m_ghosts_[3]->modeActivity(CLYDE_SCATTER_POS_X, CLYDE_SCATTER_POS_Y, m_isPaused_);
        }
        break;

        }
    }
}