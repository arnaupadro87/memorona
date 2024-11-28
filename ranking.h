#pragma once

#include <QDateTime>
#include <QList>
#include <QSettings>
#include <QString>

class RankingEntry
{
public:
    RankingEntry(uint32_t p_numberOfMovements, qint64 p_elapsedTimeInMinutes, qint64 p_elapsedTimeSecsMod);
    RankingEntry(const QString &p_playerName, uint32_t p_numberOfMovements,
                 qint64 p_elapsedTimeInMinutes, qint64 p_elapsedTimeSecsMod);
    RankingEntry(const QString &p_playerName, const QDateTime &p_creationDate,
                 uint32_t p_numberOfMovements, qint64 p_elapsedTimeInMinutes, qint64 p_elapsedTimeSecsMod);

    bool operator>(const RankingEntry &another);

    QString getPlayerName() const;
    QDateTime getDate() const;
    QString getElapsedTime() const;
    uint32_t getNumberOfMovements() const;
    qint64 getElapsedTimeInMinutes() const;
    qint64 getElapsedTimeSecsMod() const;

private:
    constexpr static auto UNKNOWN_PLAYER_NAME{"UNKNOWN"};
    QString playerName;
    QDateTime creationDate;
    uint32_t numberOfMovements;
    qint64 elapsedTimeInMinutes;
    qint64 elapsedTimeSecsMod;
};

class Ranking
{
public:
    Ranking();
    ~Ranking();

    bool canBeAdded(uint32_t p_numberOfMovements, qint64 p_elapsedTimeInMinutes, qint64 p_elapsedTimeSecsMod);
    int16_t insertNewResult(const QString &p_playerName, uint32_t p_numberOfMovements, qint64 p_elapsedTimeInMinutes, qint64 p_elapsedTimeSecsMod);

    QList<RankingEntry> getList() const;
    QString getName() const;

    void load(const QString &name);
    void save() const;

private:
    constexpr static auto ORGANIZATION_NAME{"Arnau Padro"};
    constexpr static auto APPLICATION_NAME{"Memorona"};
    constexpr static auto RANKING_CFG_GROUP_NAME{"ranking"};
    constexpr static auto UNKNOWN_RANKING_NAME{"UNKNOWN"};
    constexpr static auto PLAYER_NAME_FIELD_NAME{"playerName"};
    constexpr static auto CREATION_DATE_FIELD_NAME{"creationDate"};
    constexpr static auto NUMBER_OF_MOVEMENTS_FIELD_NAME{"numberOfMovements"};
    constexpr static auto ELAPSED_TIME_IN_MINUTES_FIELD_NAME{"elapsedTimeInMinutes"};
    constexpr static auto ELAPSED_TIME_SECONDS_MOD_FIELD_NAME{"elapsedTimeSecsMod"};
    constexpr static auto MAXIMUM_NUMBER_OF_ENTRIES{10u};
    QList<RankingEntry> entries;
    QSettings settings;
    QString currentRankingName;
};
