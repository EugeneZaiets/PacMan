#ifndef SCOREKEEPER_H 
#define SCOREKEEPER_H

#include <fstream>
#include <vector>

#include "ConsoleSettingsHandler.h"
#include "constants.h"
using namespace std;
using namespace game_constants;

class ScoreKeeper
{
public:
    ScoreKeeper
    (
        std::shared_ptr<ConsoleSettingsHandler> console_handler
    );
    ~ScoreKeeper();

    void writeScores(const int);
    void readScores ();
    void saveScores ();
    void printScores();

private:
    std::shared_ptr<ConsoleSettingsHandler> 
                m_console_handler_ ;
    fstream     m_file             ;
    vector<int> m_scores           ;
    int         m_score            ;


    const bool isMaxScore    (vector<int>::iterator, int&, const int);
    void       writeMaxScore (const int);
};

#endif // !SCOREKEEPER_H 

