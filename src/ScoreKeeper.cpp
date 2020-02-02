#include "ScoreKeeper.h"

ScoreKeeper::ScoreKeeper
(
    std::shared_ptr<ConsoleSettingsHandler> console_handler
) :
    m_console_handler_  (console_handler),
    m_scores            (vector<int>(SCORE_LINES, 0)),
    m_score             (0),
    m_file              ("Scores.txt")
{
}

ScoreKeeper::~ScoreKeeper()
{
}

void ScoreKeeper::writeScores(const int score)
{
    m_file.open("src/Scores.txt", ios::out);
    if (!m_file)
        exit(OPEN_FILE_ERROR);
    else
        writeMaxScore(score);     
    m_file.close();
}

void ScoreKeeper::writeMaxScore(const int max_score)
{
    int j = 0;
    for (std::vector<int>::iterator i = m_scores.begin(); i != m_scores.end(); ++i)
        if (isMaxScore(i, j, max_score) == true) return;
}
const bool ScoreKeeper::isMaxScore(vector<int>::iterator i, int& insert_pos, const int value)
{
    if (*i <= value)
    {
        m_scores.pop_back();
        m_scores.insert(m_scores.begin() + insert_pos, value);
        return true;
    }
    ++insert_pos;
    return false;
}

void ScoreKeeper::readScores()
{
    m_file.open("src/Scores.txt", ios::in);
    if (!m_file)
        exit(OPEN_FILE_ERROR);
    else
        for (auto i = m_scores.begin(); m_file >> m_score; ++i)
            *i = m_score;
    m_file.close();
}

void ScoreKeeper::saveScores()
{
    m_file.open("src/Scores.txt", ios::out);
    if (!m_file)
        exit(OPEN_FILE_ERROR);
    else
        for (auto i = m_scores.begin(); i != m_scores.end(); ++i)
            m_file << *i << endl;
    m_file.close();
}

void ScoreKeeper::printScores()
{
    m_console_handler_->setTextColor(WHITE);
    m_console_handler_->setCursorPosition(
        X_MIDDLE_POS - X_SCORES_CURSOR_OFFSET,
        Y_MIDDLE_POS - Y_SCORES_CURSOR_OFFSET);
    cout << "BEST SCORES";
    for (int i = 0, j = SCORE_LINES - 1; i < SCORE_LINES; ++i, --j)
    {
        m_console_handler_->setCursorPosition
        (X_MIDDLE_POS - X_SCORES_CURSOR_OFFSET,
         Y_MIDDLE_POS + i);
        cout << i + 1 << "." << m_scores[i];
    }
}
