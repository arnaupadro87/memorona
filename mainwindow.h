#pragma once

#include "ClickableCard.h"
#include "difficultylevel.h"
#include "ranking.h"

#include <QElapsedTimer>
#include <QMainWindow>
#include <QMap>
#include <QTimer>

#include <optional>
#include <utility>
#include <vector>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onNewGameActionTriggered();
    void onRankingActionTriggered();
    void onDifficultyActionTriggered();
    void onCardClicked(CardPosition);
    void updateStatusBar();

private:
    Ui::MainWindow *ui;
    QTimer *statusBarUpdateTimer;
    Ranking ranking;
    const std::vector<DifficultyLevel> difficultyLevels;
    QMap<CardPosition, std::optional<QString>> cardHiddenImages;
    std::optional<CardPosition> currentCardSelected;
    QStringList availableCardImagePaths;
    QStringList currentImagesSubset;
    QElapsedTimer gameElapsedTime;
    constexpr static auto CARD_BACKGROUND_FILEPATH{":/images/CardBackground.jpg"};
    constexpr static auto MSECS_TO_SECS_FACTOR{1000u};
    constexpr static auto SECS_TO_MINUTES_FACTOR{60u};
    constexpr static auto CARD_PIXMAP_WIDTH{120u};
    constexpr static auto CARD_PIXMAP_HEIGHT{120u};
    constexpr static auto EASY_LEVEL_NAME{"Easy"};
    constexpr static auto MEDIUM_LEVEL_NAME{"Medium"};
    constexpr static auto HARD_LEVEL_NAME{"Hard"};
    constexpr static auto EASY_LEVEL_NUMBER_OF_ROWS{4u};
    constexpr static auto EASY_LEVEL_NUMBER_OF_COLUMNS{5u};
    constexpr static auto MEDIUM_LEVEL_NUMBER_OF_ROWS{6u};
    constexpr static auto MEDIUM_LEVEL_NUMBER_OF_COLUMNS{8u};
    constexpr static auto HARD_LEVEL_NUMBER_OF_ROWS{10u};
    constexpr static auto HARD_LEVEL_NUMBER_OF_COLUMNS{10u};
    DifficultyLevels currentLevel{DifficultyLevels::EASY};
    uint32_t movementsCounter{0u};
    bool isMovementOngoing{false};

    void setupCardsGrid();
    ClickableCard* getCardWidget(const CardPosition &p_position) const;
    void loadAvailableCardImagePaths();
    QList<uint16_t> getRandomIndexesForImagesList() const;
    void chooseRandomSubsetOfImages();
    bool isGameOver() const;
    bool canCardBeTurnedFaceUp(const CardPosition &p_position) const;
    bool isFirstCardToBeSelected() const;
    bool areSelectedCardsEqual(const CardPosition &p_clickedCardPosition) const;
    void resetCards();
    void triggerGameOver();
    void triggerFirstCardSelection(const CardPosition &p_clickedCardPosition);
    void triggerCorrectCardsPair(const CardPosition &p_clickedCardPosition);
    void triggerIncorrectCardsPair(const CardPosition &p_clickedCardPosition);
    void turnClickedCardFaceUp(const CardPosition &p_clickedCardPosition) const;
};
