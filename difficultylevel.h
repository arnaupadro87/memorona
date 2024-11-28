#pragma once

#include <QString>

enum DifficultyLevels { EASY = 0, MEDIUM = 1, HARD = 2 };

class DifficultyLevel
{
public:
    DifficultyLevel(const QString &p_name, uint8_t p_numberOfRows, uint8_t p_numberOfColumns);

    QString getName() const;
    uint8_t getNumberOfRows() const;
    uint8_t getNumberOfColumns() const;

private:
    QString name;
    uint8_t numberOfRows;
    uint8_t numberOfColumns;
};
