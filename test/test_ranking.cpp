#include "ranking.h"

#include <QObject>
#include <QtTest/QtTest>

class TestRanking: public QObject
{
    Q_OBJECT

private slots:
    void testRankingEntryConstructors() {
        RankingEntry entry1(18, 1, 50);
        QCOMPARE(entry1.getPlayerName(), "UNKNOWN");
        QCOMPARE(entry1.getNumberOfMovements(), 18);
        QCOMPARE(entry1.getElapsedTimeInMinutes(), 1);
        QCOMPARE(entry1.getElapsedTimeSecsMod(), 50);
        QCOMPARE(entry1.getElapsedTime(), "1 minute(s) 50 second(s)");

        RankingEntry entry2("test", 18, 1, 50);
        QCOMPARE(entry2.getPlayerName(), "test");
        QCOMPARE(entry2.getNumberOfMovements(), 18);
        QCOMPARE(entry2.getElapsedTimeInMinutes(), 1);
        QCOMPARE(entry2.getElapsedTimeSecsMod(), 50);
        QCOMPARE(entry2.getElapsedTime(), "1 minute(s) 50 second(s)");

        const auto date{QDateTime::currentDateTime()};
        RankingEntry entry3("test", date, 18, 1, 50);
        QCOMPARE(entry3.getPlayerName(), "test");
        QCOMPARE(entry3.getDate(), date);
        QCOMPARE(entry3.getNumberOfMovements(), 18);
        QCOMPARE(entry3.getElapsedTimeInMinutes(), 1);
        QCOMPARE(entry3.getElapsedTimeSecsMod(), 50);
        QCOMPARE(entry3.getElapsedTime(), "1 minute(s) 50 second(s)");
    }

    void testRankingEntryGreatherThanOperator() {
        RankingEntry entry1("test1", 18, 0, 50);
        RankingEntry entry2("test2", 19, 0, 49);
        RankingEntry entry3("test3", 18, 1, 20);
        RankingEntry entry4("test4", 18, 0, 51);
        RankingEntry entry5("test5", 18, 0, 49);

        QCOMPARE(entry1 > entry2, true);
        QCOMPARE(entry1 > entry3, true);
        QCOMPARE(entry1 > entry4, true);
        QCOMPARE(entry1 > entry5, false);
        QCOMPARE(entry1 > entry1, false);
    }

    void testRankingAddEntriesOverTheMaximum() {
        constexpr static auto MAXIMUM_NUMBER_OF_ENTRIES{10u};
        Ranking ranking;
        for(int i{MAXIMUM_NUMBER_OF_ENTRIES}; i >= 0 ; i--) {
            ranking.insertNewResult("TestPlayer", i, 0, 0);
        }
        QCOMPARE(ranking.getList().size(), MAXIMUM_NUMBER_OF_ENTRIES);
    }
};

QTEST_MAIN(TestRanking)
#include "test_ranking.moc"
