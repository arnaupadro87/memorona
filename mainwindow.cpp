#include "mainwindow.h"
#include "rankingdialog.h"
#include "./ui_mainwindow.h"

#include <QDebug>
#include <QDirIterator>
#include <QInputDialog>
#include <QLabel>
#include <QMessageBox>
#include <QPixmap>
#include <QRandomGenerator>


using namespace std::chrono_literals;


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , statusBarUpdateTimer(new QTimer(this))
    , difficultyLevels{DifficultyLevel(EASY_LEVEL_NAME, EASY_LEVEL_NUMBER_OF_ROWS, EASY_LEVEL_NUMBER_OF_COLUMNS),
                       DifficultyLevel(MEDIUM_LEVEL_NAME, MEDIUM_LEVEL_NUMBER_OF_ROWS, MEDIUM_LEVEL_NUMBER_OF_COLUMNS),
                       DifficultyLevel(HARD_LEVEL_NAME, HARD_LEVEL_NUMBER_OF_ROWS, HARD_LEVEL_NUMBER_OF_COLUMNS)}
{
    ui->setupUi(this);
    connect(ui->actionNewGame, &QAction::triggered, this, &MainWindow::onNewGameActionTriggered);
    connect(ui->actionRanking, &QAction::triggered, this, &MainWindow::onRankingActionTriggered);
    connect(ui->actionSetDifficultyToEasy, &QAction::triggered, this, &MainWindow::onDifficultyActionTriggered);
    connect(ui->actionSetDifficultyToMedium, &QAction::triggered, this, &MainWindow::onDifficultyActionTriggered);
    connect(ui->actionSetDifficultyToHigh, &QAction::triggered, this, &MainWindow::onDifficultyActionTriggered);
    connect(statusBarUpdateTimer, &QTimer::timeout, this, &MainWindow::updateStatusBar);

    constexpr auto timeout{1000ms};
    statusBarUpdateTimer->start(timeout);

    loadAvailableCardImagePaths();
    onDifficultyActionTriggered();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onNewGameActionTriggered()
{
    resetCards();
}

void MainWindow::onRankingActionTriggered()
{
    RankingDialog dialog(ranking, this);
    dialog.exec();
}

void MainWindow::onDifficultyActionTriggered()
{
    ranking.save();

    if(ui->actionSetDifficultyToHigh->isChecked()) {
        currentLevel = DifficultyLevels::HARD;
    } else if (ui->actionSetDifficultyToMedium->isChecked()) {
        currentLevel = DifficultyLevels::MEDIUM;
    } else {
        currentLevel = DifficultyLevels::EASY;
    }

    const QString currentLevelName{difficultyLevels.at(currentLevel).getName()};
    ranking.load(currentLevelName);
    setupCardsGrid();
    resetCards();
}

void MainWindow::onCardClicked(CardPosition p_clickedCardPosition)
{
    if(!isMovementOngoing && canCardBeTurnedFaceUp(p_clickedCardPosition)) {
        qDebug() << "Selected image at " << p_clickedCardPosition.first << ", " << p_clickedCardPosition.second;
        isMovementOngoing = true;
        turnClickedCardFaceUp(p_clickedCardPosition);
        if(isFirstCardToBeSelected()) {
            triggerFirstCardSelection(p_clickedCardPosition);
        } else {
            if(areSelectedCardsEqual(p_clickedCardPosition)) {
                triggerCorrectCardsPair(p_clickedCardPosition);
                if(isGameOver()) {
                    triggerGameOver();
                    return;
                }
            } else {
                triggerIncorrectCardsPair(p_clickedCardPosition);
            }
        }
    }
}

void MainWindow::updateStatusBar()
{
    if(gameElapsedTime.isValid()) {
        const auto elapsedTimeInMinutes{gameElapsedTime.elapsed() / MSECS_TO_SECS_FACTOR / SECS_TO_MINUTES_FACTOR};
        const auto elapsedTimeSecsMod{gameElapsedTime.elapsed() / MSECS_TO_SECS_FACTOR % SECS_TO_MINUTES_FACTOR};
        ui->statusbar->showMessage(QString(tr("Number of movements: %1 | Elapsed time: %2 minutes and %3 seconds"))
                                       .arg(movementsCounter)
                                       .arg(elapsedTimeInMinutes)
                                       .arg(elapsedTimeSecsMod));
    } else {
        ui->statusbar->clearMessage();
    }
}


void MainWindow::setupCardsGrid()
{
    QLayoutItem *child;
    while ((child = ui->gridLayout->takeAt(0)) != nullptr) {
        delete child->widget();
        delete child;
    }

    for(auto i{0u}; i < difficultyLevels.at(currentLevel).getNumberOfRows(); i++) {
        for(auto j{0u}; j < difficultyLevels.at(currentLevel).getNumberOfColumns(); j++) {
            const auto cardWidget{new ClickableCard(ui->centralwidget, {i, j})};
            ui->gridLayout->addWidget(cardWidget, i, j);
            connect(cardWidget, &ClickableCard::clicked, this, &MainWindow::onCardClicked);
        }
    }

    ui->centralwidget->adjustSize();
    adjustSize();
}

ClickableCard* MainWindow::getCardWidget(const CardPosition &p_position) const
{
    return static_cast<ClickableCard*>(ui->gridLayout->itemAtPosition(p_position.first, p_position.second)->widget());
}

void MainWindow::loadAvailableCardImagePaths()
{
    constexpr auto CARD_IMAGES_FOLDER{":/images/Cards/"};
    QDirIterator it(CARD_IMAGES_FOLDER, QDirIterator::Subdirectories);
    while (it.hasNext()) {
        auto path{it.next()};
        availableCardImagePaths << path << path;
    }
}

QList<uint16_t> MainWindow::getRandomIndexesForImagesList() const
{
    const auto numberOfCards{difficultyLevels.at(currentLevel).getNumberOfRows() *
                             difficultyLevels.at(currentLevel).getNumberOfColumns()};
    QList<uint16_t> indexes;
    while(indexes.size() < numberOfCards) {
        const uint16_t newIndex = QRandomGenerator::global()->bounded(numberOfCards);
        if(!indexes.contains(newIndex)) {
            indexes.append(newIndex);
        }
    }
    return indexes;
}

void MainWindow::chooseRandomSubsetOfImages()
{
    uint16_t cutIndex = QRandomGenerator::global()->bounded(availableCardImagePaths.size());
    (cutIndex % 2 == 0) ? cutIndex += 0 : cutIndex += 1;
    currentImagesSubset.clear();
    for(auto i{0u}; i < availableCardImagePaths.size(); i++) {
        currentImagesSubset.push_back(availableCardImagePaths.at((cutIndex + i) % availableCardImagePaths.size()));
    }
}

bool MainWindow::isGameOver() const
{
    for(const auto &card : cardHiddenImages) {
        if(card.has_value()) {
            return false;
        }
    }
    return true;
}

bool MainWindow::canCardBeTurnedFaceUp(const CardPosition &p_position) const
{
    return (cardHiddenImages.contains(p_position) && cardHiddenImages.value(p_position).has_value() &&
            (!currentCardSelected.has_value() || (currentCardSelected.value() != p_position)));
}

bool MainWindow::isFirstCardToBeSelected() const
{
    return (!currentCardSelected.has_value());
}

bool MainWindow::areSelectedCardsEqual(const CardPosition &p_clickedCardPosition) const
{
    return (cardHiddenImages.value(currentCardSelected.value()).value() == cardHiddenImages.value(p_clickedCardPosition).value());
}

void MainWindow::resetCards()
{
    const QPixmap pixmap{CARD_BACKGROUND_FILEPATH};
    const auto scaledPixmap{pixmap.scaled(CARD_PIXMAP_WIDTH, CARD_PIXMAP_HEIGHT, Qt::KeepAspectRatio, Qt::SmoothTransformation)};
    const auto indexes{getRandomIndexesForImagesList()};
    auto counter{0u};

    chooseRandomSubsetOfImages();
    cardHiddenImages.clear();
    for(auto i{0u}; i < difficultyLevels.at(currentLevel).getNumberOfRows(); i++) {
        for(auto j{0u}; j < difficultyLevels.at(currentLevel).getNumberOfColumns(); j++) {
            getCardWidget({i, j})->setPixmap(scaledPixmap);
            cardHiddenImages[{i, j}] = currentImagesSubset.at(indexes.at(counter++));
        }
    }

    movementsCounter = 0u;
    gameElapsedTime.invalidate();
    currentCardSelected.reset();
}

void MainWindow::triggerGameOver()
{
    const auto elapsedTimeInMinutes{gameElapsedTime.elapsed() / MSECS_TO_SECS_FACTOR / SECS_TO_MINUTES_FACTOR};
    const auto elapsedTimeSecsMod{gameElapsedTime.elapsed() / MSECS_TO_SECS_FACTOR % SECS_TO_MINUTES_FACTOR};
    gameElapsedTime.invalidate();
    if(ranking.canBeAdded(movementsCounter, elapsedTimeInMinutes, elapsedTimeSecsMod)) {
        bool wasOKButtonClicked;
        auto userName{QInputDialog::getText(this, tr("Game Over"),
            QString(tr("Congratulations, you enter the ranking having completed the pairs with %1 movements in %2 minutes and %3 seconds! Please write your name:"))
                .arg(movementsCounter)
                .arg(elapsedTimeInMinutes)
                .arg(elapsedTimeSecsMod),
            QLineEdit::Normal, "Anonymous Player", &wasOKButtonClicked)};
        if(wasOKButtonClicked) {
            auto position{ranking.insertNewResult(userName, movementsCounter, elapsedTimeInMinutes, elapsedTimeSecsMod)};
            RankingDialog dialog(ranking, this);
            dialog.selectEntry(position);
            dialog.exec();
        }
    } else {
        QMessageBox::information(this, tr("Game Over"),
            QString(tr("Congratulations, you completed all the pairs with %1 movements in %2 minutes and %3 seconds!"))
                             .arg(movementsCounter)
                             .arg(elapsedTimeInMinutes)
                             .arg(elapsedTimeSecsMod));
    }
    resetCards();
}

void MainWindow::triggerFirstCardSelection(const CardPosition &p_clickedCardPosition)
{
    currentCardSelected = p_clickedCardPosition;
    movementsCounter++;
    isMovementOngoing = false;
    if(!gameElapsedTime.isValid()) {
        gameElapsedTime.start();
    }
}

void MainWindow::triggerCorrectCardsPair(const CardPosition &p_clickedCardPosition)
{
    qDebug() << "Correct cards pair!";
    cardHiddenImages[currentCardSelected.value()].reset();
    cardHiddenImages[p_clickedCardPosition].reset();
    currentCardSelected.reset();
    isMovementOngoing = false;
}

void MainWindow::triggerIncorrectCardsPair(const CardPosition &p_clickedCardPosition)
{
    constexpr auto timeout{1000ms};
    QTimer::singleShot(timeout, this, [this, p_clickedCardPosition] () {
        qDebug() << "Incorrect cards pair!";

        const QPixmap pixmap{CARD_BACKGROUND_FILEPATH};
        const auto scaledPixmap{pixmap.scaled(CARD_PIXMAP_WIDTH, CARD_PIXMAP_HEIGHT, Qt::KeepAspectRatio, Qt::SmoothTransformation)};
        getCardWidget(currentCardSelected.value())->setPixmap(scaledPixmap);
        getCardWidget(p_clickedCardPosition)->setPixmap(scaledPixmap);

        currentCardSelected.reset();
        isMovementOngoing = false;
       });
}

void MainWindow::turnClickedCardFaceUp(const CardPosition &p_clickedCardPosition) const
{
    const QPixmap pixmap{cardHiddenImages.value(p_clickedCardPosition).value()};
    getCardWidget(p_clickedCardPosition)->setPixmap(
        pixmap.scaled(CARD_PIXMAP_WIDTH, CARD_PIXMAP_HEIGHT, Qt::KeepAspectRatio, Qt::SmoothTransformation));
}
