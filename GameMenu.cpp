#include "GameMenu.h"

GameMenu::GameMenu(std::shared_ptr<ConsoleSettingsHandler> console_handler) :
    m_choise(UNDEFINED_CHOISE),
    m_cursor_position(0),
    m_console_handler(console_handler)
{
    if (!m_console_handler) exit(1);
}
GameMenu::~GameMenu()
{
}
void GameMenu::renderMenu()
{
    m_console_handler->setTextColor(WHITE);
    for (int i = 0; i < NUM_OF_MENU_TITLES; ++i)
    {
        m_console_handler->setCursorPosition(X_MIDDLE_POS, Y_MIDDLE_POS + i);
        std::cout << menu_titles[i] << std::endl;
    }
    m_console_handler->setCursorPosition(X_MIDDLE_POS - 2, Y_MIDDLE_POS + m_cursor_position);
    std::cout << menu_cursor;
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
        m_choise = m_cursor_position;
    }
}
void GameMenu::moveCursorUp()
{
    if (m_cursor_position > 0)
    {
        m_console_handler->setTextColor(WHITE);
        m_console_handler->setCursorPosition(X_MIDDLE_POS - 2, Y_MIDDLE_POS + m_cursor_position);
        std::cout << ' ';
        --m_cursor_position;
    }
}
void GameMenu::moveCursorDown()
{
    if (m_cursor_position < NUM_OF_MENU_TITLES - 1)
    {
        m_console_handler->setTextColor(WHITE);
        m_console_handler->setCursorPosition(X_MIDDLE_POS - 2, Y_MIDDLE_POS + m_cursor_position);
        std::cout << ' ';
        ++m_cursor_position;
    }
}
