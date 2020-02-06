#ifndef RENDERER_H
#define RENDERER_H

#include "ConsoleSettingsHandler.h"
#include "constants.h"

using namespace game_constants;

class Renderer
{
public:
    Renderer
    (
        std::shared_ptr<ConsoleSettingsHandler> console_handler
    );
    ~Renderer();

    void renderLives            (const int) const;
    void renderScore            (const int) const;
    void resetMapAfterPacmanKill(const unsigned char);
    void renderPacmanKill       (const int, const int, const int);
    void renderPacman           (const int, const int, const unsigned char) const;
    void renderDeadPacman       (const int, const int, const unsigned char);
    void renderPacmanPrevPos    (const int, const int, const int, const int, const char) const;

    const int getKillPosX()     { return m_kill_pos_x_; }
    const int getKillPosY()     { return m_kill_pos_y_; }
    const int getDigitNum()     { return m_digit_num_;  }

private:

    int m_digit_num_                       ;
    int m_kill_pos_x_                      ;
    int m_kill_pos_y_                      ;
    std::shared_ptr<ConsoleSettingsHandler>
                         m_console_handler_;
};

#endif //RENDERER_H
