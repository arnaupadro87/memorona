#include "ranking.h"

RankingEntry::RankingEntry(uint32_t p_numberOfMovements, qint64 p_elapsedTimeInMinutes, qint64 p_elapsedTimeSecsMod)
    : playerName(UNKNOWN_PLAYER_NAME)
    , creationDate(QDateTime::currentDateTime())
    , numberOfMovements(p_numberOfMovements)
    , elapsedTimeInMinutes(p_elapsedTimeInMinutes)
    , elapsedTimeSecsMod(p_elapsedTimeSecsMod)
{
}

RankingEntry::RankingEntry(const QString &p_playerName, uint32_t p_numberOfMovements,
                           qint64 p_elapsedTimeInMinutes, qint64 p_elapsedTimeSecsMod)
    : playerName(p_playerName)
    , creationDate(QDateTime::currentDateTime())
    , numberOfMovements(p_numberOfMovements)
    , elapsedTimeInMinutes(p_elapsedTimeInMinutes)
    , elapsedTimeSecsMod(p_elapsedTimeSecsMod)
{
}

RankingEntry::RankingEntry(const QString &p_playerName, const QDateTime &p_creationDate,
                           uint32_t p_numberOfMovements, qint64 p_elapsedTimeInMinutes, qint64 p_elapsedTimeSecsMod)
    : playerName(p_playerName)
    , creationDate(p_creationDate)
    , numberOfMovements(p_numberOfMovements)
    , elapsedTimeInMinutes(p_elapsedTimeInMinutes)
    , elapsedTimeSecsMod(p_elapsedTimeSecsMod)
{
}

bool RankingEntry::operator>(const RankingEntry &another)
{
    if(numberOfMovements < another.numberOfMovements) {
        return true;
    } else if (numberOfMovements == another.numberOfMovements) {
        if (elapsedTimeInMinutes < another.elapsedTimeInMinutes) {
            return true;
        } else if ((elapsedTimeInMinutes == another.elapsedTimeInMinutes) &&
                   (elapsedTimeSecsMod < another.elapsedTimeSecsMod)) {
            return true;
        }
    }
    return false;
}

QString RankingEntry::getPlayerName() const
{
    return playerName;
}

QDateTime RankingEntry::getDate() const
{
    return creationDate;
}

QString RankingEntry::getElapsedTime() const
{
    return (QString("%1 minute(s) %2 second(s)").arg(elapsedTimeInMinutes).arg(elapsedTimeSecsMod));
}

uint32_t RankingEntry::getNumberOfMovements() const
{
    return numberOfMovements;
}

qint64 RankingEntry::getElapsedTimeInMinutes() const
{
    return elapsedTimeInMinutes;
}

qint64 RankingEntry::getElapsedTimeSecsMod() const
{
    return elapsedTimeSecsMod;
}


Ranking::Ranking()
    : settings{ORGANIZATION_NAME, APPLICATION_NAME}
    , currentRankingName{UNKNOWN_RANKING_NAME}
{
}

Ranking::~Ranking()
{
    save();
}

bool Ranking::canBeAdded(uint32_t p_numberOfMovements, qint64 p_elapsedTimeInMinutes, qint64 p_elapsedTimeSecsMod)
{
    RankingEntry newEntry(p_numberOfMovements, p_elapsedTimeInMinutes, p_elapsedTimeSecsMod);
    for(auto i{0u}; i < entries.size(); i++) {
        if(newEntry > entries.at(i)) {
            return true;
        }
    }

    if(entries.size() < MAXIMUM_NUMBER_OF_ENTRIES) {
        return true;
    }

    return false;
}

int16_t Ranking::insertNewResult(const QString &p_playerName, uint32_t p_numberOfMovements, qint64 p_elapsedTimeInMinutes, qint64 p_elapsedTimeSecsMod)
{
    RankingEntry newEntry(p_playerName, p_numberOfMovements, p_elapsedTimeInMinutes, p_elapsedTimeSecsMod);
    for(auto i{0u}; i < entries.size(); i++) {
        if(newEntry > entries.at(i)) {
            entries.insert(i, newEntry);
            if(entries.size() > MAXIMUM_NUMBER_OF_ENTRIES) {
                entries.takeLast();
            }
            return i;
        }
    }

    if(entries.size() < MAXIMUM_NUMBER_OF_ENTRIES) {
        entries.push_back(newEntry);
        return entries.size() - 1;
    }

    qWarning("New ranking result was not inserted!");
    return -1;
}

QList<RankingEntry> Ranking::getList() const
{
    return entries;
}

QString Ranking::getName() const
{
    return currentRankingName;
}

void Ranking::load(const QString &p_name)
{
    currentRankingName = p_name;
    entries.clear();

    QSettings settings(ORGANIZATION_NAME, APPLICATION_NAME);
    settings.beginGroup(RANKING_CFG_GROUP_NAME);
    const auto size{settings.beginReadArray(currentRankingName.toLower())};
    for(auto i{0u}; i < size; i++) {
        settings.setArrayIndex(i);
        RankingEntry entry{settings.value(PLAYER_NAME_FIELD_NAME).toString(), settings.value(CREATION_DATE_FIELD_NAME).toDateTime(),
                           settings.value(NUMBER_OF_MOVEMENTS_FIELD_NAME).toUInt(), settings.value(ELAPSED_TIME_IN_MINUTES_FIELD_NAME).toInt(),
                           settings.value(ELAPSED_TIME_SECONDS_MOD_FIELD_NAME).toInt()};
        entries.push_back(entry);
    }
    settings.endArray();
    settings.endGroup();
}

void Ranking::save() const
{
    QSettings settings(ORGANIZATION_NAME, APPLICATION_NAME);
    settings.beginGroup(RANKING_CFG_GROUP_NAME);
    settings.beginWriteArray(currentRankingName.toLower());
    for(auto i{0u}; i < entries.size(); i++) {
        settings.setArrayIndex(i);
        settings.setValue(PLAYER_NAME_FIELD_NAME, entries.at(i).getPlayerName());
        settings.setValue(CREATION_DATE_FIELD_NAME, entries.at(i).getDate());
        settings.setValue(NUMBER_OF_MOVEMENTS_FIELD_NAME, entries.at(i).getNumberOfMovements());
        settings.setValue(ELAPSED_TIME_IN_MINUTES_FIELD_NAME, entries.at(i).getElapsedTimeInMinutes());
        settings.setValue(ELAPSED_TIME_SECONDS_MOD_FIELD_NAME, entries.at(i).getElapsedTimeSecsMod());
    }
    settings.endArray();
    settings.endGroup();
}
