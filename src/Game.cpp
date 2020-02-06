#include "Game.h"

shared_ptr<Game> Game::m_GameInstance = 0;
shared_ptr<Game> Game::getInstance
(
    std::shared_ptr<ConsoleSettingsHandler> console_handler
)
{
    if (m_GameInstance == 0)
        m_GameInstance = std::shared_ptr<Game>(new Game(console_handler));
    return m_GameInstance;
}
Game::Game
(
    std::shared_ptr<ConsoleSettingsHandler> console_handler
) :
    m_console_handler_           (console_handler),
    m_seconds_in_boost_by_level_ (SECODS_IN_BOOST_BY_LEVEL),
    m_seconds_to_mode_change_    (SECODS_TO_CHANGE_MODE),
    m_points_num_                (POINTS_NUMBER),
    m_level_counter_             (0),
    m_isPaused_                  (false),
    m_isGameover_                (false),
    m_check_to_unpause_          (true),
    m_timer_                     (0),
    m_temp_timer_                (0)
{
    if (m_console_handler_ == 0) 
        exit(NULL_POINTER_ERROR);
    SetConsoleTitle("PacMan");
    m_console_handler_->createGameWindow();
    m_console_handler_->hanldeCursorVisibility(false);
}
Game::~Game()
{
}
void Game::start() 
{
    m_menu_         = std::make_unique<GameMenu>   (m_console_handler_);
    m_renderer_     = std::make_unique<Renderer>   (m_console_handler_);
    m_score_keeper_ = std::make_unique<ScoreKeeper>(m_console_handler_);
    if (m_score_keeper_ == 0)
        exit(NULL_POINTER_ERROR);
    if (m_menu_     == 0) 
        exit(NULL_POINTER_ERROR);
    if (m_renderer_ == 0)
        exit(NULL_POINTER_ERROR);
    startMenuLoop();
}
void Game::startMenuLoop()
{
    if (m_menu_ == 0)
        exit(NULL_POINTER_ERROR);
    while (m_menu_->getChoise() != (NUM_OF_MENU_TITLES - 1))
    {
        m_menu_->renderMenu();
        handleMenuChoise(m_menu_->getChoise());
        m_menu_->setChoise(UNDEFINED_CHOISE);
        m_menu_->makeChoise();
    }
}
void Game::handleMenuChoise(const int choise)
{
    switch (choise)
    {
    case 0 : startNewGame()  ; break;
    case 1 : showBestScores(); break;
    }
}
void Game::startNewGame()
{
    m_pacman_ = std::make_unique<PacMan>(m_console_handler_, 
                                         Game::getInstance(m_console_handler_));
    for (int i = 0; i < NUMBER_OF_GHOSTS; ++i)
    {
        m_ghosts_[i] = std::make_unique<Ghost>(m_console_handler_,
                                               Game::getInstance(m_console_handler_),
                                              (Ghost_Name)i);
    }
    checkPointersToActors();
    startGameLoop();
    system("CLS");
}
void Game::showBestScores()
{
    if (m_score_keeper_ == 0)
        exit(NULL_POINTER_ERROR);
    system("CLS");
    resetAsyncKeyState(VK_ESCAPE);
    m_score_keeper_->readScores ();
    m_score_keeper_->printScores();
    while (isKeyPressed(VK_ESCAPE) == false);
    resetAsyncKeyState(VK_RETURN);
    system("CLS");
}
void Game::startGameLoop()
{
    checkPointersToActors();
    if (m_score_keeper_ == 0)
        exit(NULL_POINTER_ERROR);

    m_isGameover_ = false;
    m_pacman_->setLives(NUMBER_OF_LIVES);
    for (m_level_counter_ = 0; m_level_counter_ < LEVEL_NUM; ++m_level_counter_)
    {
        if (m_seconds_in_boost_by_level_ < 0.0) 
            m_seconds_in_boost_by_level_ = 0.0;
        else
            m_seconds_in_boost_by_level_ -= m_level_counter_; 

        loadLevel();
        setMazeText("Get Ready!", YELLOW);
        startLevel();

        if (isEndGame() == true)
            break;
    }
    m_score_keeper_->writeScores(m_pacman_->getScore());
    m_score_keeper_->saveScores();
}
void Game::startLevel()
{
    while (m_points_num_ != 0)
    {
        if (m_pacman_ == 0)
            exit(NULL_POINTER_ERROR);
        pause();
        if (renderPause(m_isPaused_) == true) 
            continue;

        m_pacman_->handlePacmanMovement(m_isPaused_);
        moveGhosts(); 
        if (collisionWithGhost() == true)
            resetMapInCollision();

        if (isDead() == true)
            break;
        handleTime();
        render();
    }
}
void Game::pause()
{
    if (isKeyOncePressed(VK_ESCAPE))
    {
        m_isPaused_ = true;
        m_check_to_unpause_ = true;
        m_temp_timer_ = m_timer_;
    }
    else
    {
        m_isPaused_ = false;
        m_timer_ = m_temp_timer_;
    }
}
void Game::loadLevel()
{
    if (m_console_handler_ == 0)
        exit(NULL_POINTER_ERROR);
    m_console_handler_->setTextColor(WHITE);
    m_console_handler_->setCursorPosition(X_1UP_OFFSET, 
                                         -Y_SCREEN_TOP_OFFSET);
    cout << "1UP";
    m_console_handler_->setCursorPosition(X_LEVEL_OFFSET,
                                         -Y_SCREEN_TOP_OFFSET);
    cout << "LEVEL: " << m_level_counter_;
    
    m_console_handler_->setCursorPosition(0, 0);
    fillMapToPrint();

    setPointsNum(POINTS_NUMBER);
    initializeAllActors();
    render();
}
void Game::render()
{
    checkPointersToActors();
    if (m_renderer_ == 0)
        exit(NULL_POINTER_ERROR);

    int p_x = m_pacman_->getPos_X();
    int p_y = m_pacman_->getPos_Y();
    m_renderer_->renderPacman (p_x, p_y, m_pacman_->getHead());

    int p_px = m_pacman_->getPrevPos_X();
    int p_py = m_pacman_->getPrevPos_Y();
    char map_element = getCharOfBuffer(p_px, p_py);
    m_renderer_->renderPacmanPrevPos(p_px, p_py, p_x, p_y, map_element);
    m_renderer_->renderScore (m_pacman_->getScore());
    m_renderer_->renderLives (m_pacman_->getLives());
    for (int i = 0; i < NUMBER_OF_GHOSTS; ++i)
    {
        m_ghosts_[i]->renderGhost();
    }
}
const bool Game::renderPause(const bool paused)
{
    if (m_console_handler_ == 0)
        exit(NULL_POINTER_ERROR);
    if (paused)
    {
        if (m_console_handler_ == 0) 
            exit(NULL_POINTER_ERROR);
        m_console_handler_->setTextColor(YELLOW);
        m_console_handler_->setCursorPosition(X_MIDDLE_POS, Y_MIDDLE_POS);
        std::cout << "  PAUSE  ";
        return true;
    }
    else if(m_check_to_unpause_ == true)
    {
        if (m_console_handler_ == 0)
            exit(NULL_POINTER_ERROR);
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
    if (m_pacman_ == 0)
        exit(NULL_POINTER_ERROR);
    if (m_pacman_->getLives() == 0)
    {
        m_isGameover_ = true;
        return true;
    }
    else
    {
        return false;
    }
    return false;
}
const bool Game::isEndGame()
{
    if (m_console_handler_ == 0)
        exit(NULL_POINTER_ERROR);
    if (m_isGameover_ == true)
    {
        m_console_handler_->setTextColor(RED);
        m_console_handler_->setCursorPosition(X_MIDDLE_POS, 
                                              Y_MIDDLE_POS);
        std::cout << "GAME OVER";
        Sleep(MILLISECONDS_GAMEOVER_TIME);
        return true;
    }
    return false;
}
const bool Game::collisionWithGhost()
{
    checkPointersToActors();
    for (int i = 0; i < NUMBER_OF_GHOSTS; ++i)
    {
        bool matchPosX  = m_pacman_->getPos_X() == m_ghosts_[i]->getPos_X();
        bool matchPosY  = m_pacman_->getPos_Y() == m_ghosts_[i]->getPos_Y();
        bool ghostAlive = m_ghosts_[i]->getMode() != Mode::MODE_DEAD;
        if (matchPosX && matchPosY && ghostAlive)
            return handleGhostsKill(i);
    }
    return false;
}
void Game::moveGhosts()
{
    checkPointersToActors();
    bool inkyReleasePoints = m_points_num_ <= POINTS_TO_RELEASE_INKY;
    bool inkyWaits = m_ghosts_[GHOST_NAME_INKY]->getMode() == Mode::MODE_WAIT;
    if (inkyReleasePoints && inkyWaits)
        m_ghosts_[GHOST_NAME_INKY]->setMode(Mode::MODE_EXIT_GATE);

    bool clydeReleasePoints = m_points_num_ <= POINTS_TO_RELEASE_CLYDE;
    bool clydeWaits = m_ghosts_[GHOST_NAME_CLYDE]->getMode() == Mode::MODE_WAIT;
    if (clydeReleasePoints && clydeWaits)
        m_ghosts_[GHOST_NAME_CLYDE]->setMode(Mode::MODE_EXIT_GATE);

    for (int i = 0; i < NUMBER_OF_GHOSTS; ++i)
        handleGhostsMovement(i);

    render();
}
void Game::handleTime() 
{
    checkPointersToActors();
    if (m_pacman_->getEnergizerInfo() == true)
    {
        for (int i = 0; i < NUMBER_OF_GHOSTS; ++i)
            if(m_ghosts_[i]->getPrevMode() != Mode::MODE_EXIT_GATE)
                 m_ghosts_[i]->scared();
        handleTimeEnergizer();
        m_timer_ = std::clock();
    }
    if (getTime() >= m_seconds_to_mode_change_)
    {
        changeGhostsModeByTimer();
        m_timer_ = 
            std::clock();
    }
}
void Game::handleTimeEnergizer()
{
    checkPointersToActors();
    if (m_pacman_->getTimeInBooste() >= m_seconds_in_boost_by_level_)
    {
        for (int i = 0; i < NUMBER_OF_GHOSTS; ++i)
            m_ghosts_[i]->braved();
        m_pacman_->setEnergizerInfo(false);
    }
}
void Game::fillMapToPrint()
{
    if (m_console_handler_ == 0)
        exit(NULL_POINTER_ERROR);
    char char_to_print;
    for (int y = 0; y < Y_SIZE; ++y)
    {
        for (int x = 0; x < X_SIZE; ++x)
        {
            char_to_print = Map[y][x];
            setCharFromMap(char_to_print, x, y);
            cout << m_MapToPrint_[y][x];
        }
        m_console_handler_->setCursorPosition(0, y + Y_SCREEN_BOTTOM_OFFSET);
    }
}
void Game::changeGhostsModeByTimer()
{
    checkPointersToActors();
    for (int i = 0; i < NUMBER_OF_GHOSTS; ++i)
        m_ghosts_[i]->changeModeToOpposite();
}
void Game::initializeAllActors()
{
    checkPointersToActors();
    m_pacman_                    ->resetPacMan(PACMAN_INIT_POS_X, PACMAN_INIT_POS_Y);
    m_ghosts_[GHOST_NAME_BLINCKY]->resetGhost (BLINKY_INIT_POS_X, BLINKY_INIT_POS_Y);
    m_ghosts_[GHOST_NAME_PINKY  ]->resetGhost (PINKY_INIT_POS_X , PINKY_INIT_POS_Y );
    m_ghosts_[GHOST_NAME_INKY   ]->resetGhost (INKY_INIT_POS_X  , INKY_INIT_POS_Y  );
    m_ghosts_[GHOST_NAME_CLYDE  ]->resetGhost (CLYDE_INIT_POS_X , CLYDE_INIT_POS_Y );
    m_timer_ = std::clock();
    for (int i = 0; i < NUMBER_OF_GHOSTS; ++i)
        m_ghosts_[i]->resetModes(m_ghosts_[i]->getName());
}
void Game::resetMapInCollision()
{
    checkPointersToActors();
    if (m_console_handler_ == 0) 
        exit(NULL_POINTER_ERROR);

    m_console_handler_->setCursorPosition(m_pacman_->getPos_X(), 
                                          m_pacman_->getPos_Y());
    std::cout << m_MapToPrint_[m_pacman_->getPos_Y()][m_pacman_->getPos_X()];

    for (int i = 0; i < NUMBER_OF_GHOSTS; i++)
        m_ghosts_[i]->renderMap();

    initializeAllActors();
    render();
    while (!_kbhit());
}
void Game::setMazeText(std::string text, int color)
{
    if (m_console_handler_ == 0) 
        exit(NULL_POINTER_ERROR);
    m_console_handler_->setTextColor(color);
    m_console_handler_->setCursorPosition(X_MIDDLE_POS, Y_MIDDLE_POS);
    std::cout << text;
    while (!_kbhit());
    memcpy(m_substring_, &m_MapToPrint_[Y_MIDDLE_POS][X_MIDDLE_POS], text.size());
    m_substring_[text.size()] = '\0';

    m_console_handler_->setCursorPosition(X_MIDDLE_POS, Y_MIDDLE_POS);
    m_console_handler_->setTextColor(WHITE);
    std::cout << m_substring_;
}
void Game::setCharFromMap(const char ch, const int x_offset, const int y_offset)
{
    if (m_console_handler_ == 0)
        exit(NULL_POINTER_ERROR);
    for (auto i = MapCharToPtint.begin(); i != MapCharToPtint.end(); ++i)
    {
        if (ch == '.' || ch == 'o')
            m_console_handler_->setTextColor(WHITE);
        else
            m_console_handler_->setTextColor(BLUE);
        if (ch == i->first)
            m_MapToPrint_[y_offset][x_offset] = i->second;
    }
}
void Game::checkPointersToActors() 
{
    if (m_pacman_ == 0) 
        exit(NULL_POINTER_ERROR);
    for (int i = 0; i < NUMBER_OF_GHOSTS; ++i)
    {
        if (m_ghosts_[i] == 0) 
            exit(NULL_POINTER_ERROR);
    }
}
void Game::handleGhostsMovement(const int ghost_num)
{
    if (m_ghosts_[ghost_num] == 0)
       exit(NULL_POINTER_ERROR);
    switch (ghost_num)
    {
        case 0: handleBlinkyFeature(); break;
        case 1: handlePinkyFeature (); break;
        case 2: handleInkyFeature  (); break;
        case 3: handleClydeFeature (); break;
    }    
}
const bool Game::handleGhostsKill(const int ghost_num)
{
    checkPointersToActors();
    if (m_renderer_ == 0)
        exit(NULL_POINTER_ERROR);

    if (m_ghosts_[ghost_num]->getMode() != Mode::MODE_FRIGHTENED)
    {
        m_pacman_->dead();
        m_renderer_->renderDeadPacman(m_pacman_->getPos_X(), 
                                      m_pacman_->getPos_Y(), 
                                      m_pacman_->getHead());
        return true;
    }
    else
    {
        m_ghosts_[ghost_num]->dead();
        m_pacman_->Kill();
        m_renderer_->renderPacmanKill(m_pacman_->getPos_X(),
                                      m_pacman_->getPos_Y(), 
                                      m_pacman_->getKillCounter());

        int x = m_renderer_->getKillPosX();
        int y = m_renderer_->getKillPosY();
        for (int i = 0; i < m_renderer_->getDigitNum(); ++i, ++x)
            m_renderer_->resetMapAfterPacmanKill(m_MapToPrint_[y][x]);
        return false;
    }
    return false;
}
void Game::handleBlinkyFeature()
{
    checkPointersToActors();
    m_ghosts_[GHOST_NAME_BLINCKY]->modeActivity
    (
        m_pacman_->getPos_X(), 
        m_pacman_->getPos_Y(), 
        m_isPaused_
    );
}
void Game::handlePinkyFeature()
{
    checkPointersToActors();
    m_ghosts_[GHOST_NAME_PINKY]->modeActivity
    (
        m_pacman_->getPos_X() + OFFSET_PACMAN_PINKY,
        m_pacman_->getPos_Y() + OFFSET_PACMAN_PINKY,
        m_isPaused_
    );
}
void Game::handleInkyFeature() 
{
    checkPointersToActors();
    m_ghosts_[GHOST_NAME_INKY]->modeActivity
    (
        m_ghosts_[GHOST_NAME_INKY]->getInkyPos_X(m_pacman_   ->getPos_X(),
                                   m_ghosts_[GHOST_NAME_INKY]->getPos_X(),
                                   m_pacman_                 ->getDirection()),
        m_ghosts_[GHOST_NAME_INKY]->getInkyPos_Y(m_pacman_   ->getPos_Y(),
                                   m_ghosts_[GHOST_NAME_INKY]->getPos_X(),
                                   m_pacman_                 ->getDirection()),
        m_isPaused_
    );
}
void Game::handleClydeFeature()
{
    checkPointersToActors();
    int sum = m_ghosts_[GHOST_NAME_CLYDE]->getClydeCountPos_X(m_pacman_->getPos_X()) +
              m_ghosts_[GHOST_NAME_CLYDE]->getClydeCountPos_Y(m_pacman_->getPos_Y());

    if (sum > PACMAN_DISTANCE_TO_CLYDE)
        m_ghosts_[GHOST_NAME_CLYDE]->modeActivity(m_pacman_->getPos_X(),
                                                  m_pacman_->getPos_Y(),
                                                  m_isPaused_);
    else
        m_ghosts_[GHOST_NAME_CLYDE]->modeActivity(CLYDE_SCATTER_POS_X,
                                                  CLYDE_SCATTER_POS_Y,
                                                  m_isPaused_);
}
