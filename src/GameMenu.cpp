#include "GameMenu.h"

GameMenu::GameMenu
(
    std::shared_ptr<ConsoleSettingsHandler> console_handler
) :
    m_choise_(UNDEFINED_CHOISE),
    m_cursor_position_(0),
    m_console_handler_(console_handler)
{
    if (m_console_handler_ == 0) 
        exit(1);
}
GameMenu::~GameMenu()
{
}
void GameMenu::renderMenu()
{
    m_console_handler_->setTextColor(WHITE);
    for (int i = 0; i < NUM_OF_MENU_TITLES; ++i)
    {
        m_console_handler_->setCursorPosition(X_MIDDLE_POS, 
                                              Y_MIDDLE_POS + i);
        std::cout << menu_titles[i] << std::endl;
    }
    m_console_handler_->setCursorPosition(X_MIDDLE_POS - 2, 
                                          Y_MIDDLE_POS + m_cursor_position_);
    std::cout << MENU_CURSOR;
}
void GameMenu::makeChoise()
{
    if (isKeyPressed(VK_S) || isKeyPressed(VK_DOWN))
    {
        moveCursorDown();
    }
    else if (isKeyPressed(VK_W) || isKeyPressed(VK_UP))
    {
        moveCursorUp();
    }
    else if (isKeyPressed(VK_RETURN))
    {
        m_choise_ = m_cursor_position_;
    }
}
void GameMenu::moveCursorUp()
{
    if (m_cursor_position_ > 0)
    {
        m_console_handler_->setTextColor(WHITE);
        m_console_handler_->setCursorPosition(X_MIDDLE_POS - 2, 
                                              Y_MIDDLE_POS + m_cursor_position_);
        std::cout << ' ';
        --m_cursor_position_;
    }
}
void GameMenu::moveCursorDown()
{
    if (m_cursor_position_ < NUM_OF_MENU_TITLES - 1)
    {
        m_console_handler_->setTextColor(WHITE);
        m_console_handler_->setCursorPosition(X_MIDDLE_POS - 2, 
                                              Y_MIDDLE_POS + m_cursor_position_);
        std::cout << ' ';
        ++m_cursor_position_;
    }
}


