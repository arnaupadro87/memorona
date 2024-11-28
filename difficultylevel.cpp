#include "difficultylevel.h"

DifficultyLevel::DifficultyLevel(const QString &p_name, uint8_t p_numberOfRows, uint8_t p_numberOfColumns)
    : name{p_name}
    , numberOfRows{p_numberOfRows}
    , numberOfColumns{p_numberOfColumns}
{
}

QString DifficultyLevel::getName() const
{
    return name;
}

uint8_t DifficultyLevel::getNumberOfRows() const
{
    return numberOfRows;
}

uint8_t DifficultyLevel::getNumberOfColumns() const
{
    return numberOfColumns;
}
