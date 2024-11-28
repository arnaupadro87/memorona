#include "rankingdialog.h"
#include "ui_rankingdialog.h"

RankingDialog::RankingDialog(const Ranking &p_ranking, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RankingDialog)
{
    ui->setupUi(this);
    setWindowTitle("Ranking - " + p_ranking.getName());
    ui->rankingWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    constexpr auto PLAYER_NAME_COLUMN_INDEX{0};
    constexpr auto NUMBER_OF_MOVEMENTS_COLUMN_INDEX{1};
    constexpr auto ELAPSED_TIME_COLUMN_INDEX{2};
    constexpr auto ENTRY_DATE_COLUMN_INDEX{3};
    constexpr auto DATE_FORMAT_STR{"dd/MM/yyyy hh:mm:ss"};
    const auto entries{p_ranking.getList()};
    for(auto i{0u}; i < entries.size(); i++) {
        ui->rankingWidget->insertRow(i);

        ui->rankingWidget->setItem(i, PLAYER_NAME_COLUMN_INDEX, createTableWidgetItem(entries.at(i).getPlayerName()));
        ui->rankingWidget->setItem(i, NUMBER_OF_MOVEMENTS_COLUMN_INDEX, createTableWidgetItem(
            QString::number(entries.at(i).getNumberOfMovements())));
        ui->rankingWidget->setItem(i, ELAPSED_TIME_COLUMN_INDEX, createTableWidgetItem(entries.at(i).getElapsedTime()));
        ui->rankingWidget->setItem(i, ENTRY_DATE_COLUMN_INDEX, createTableWidgetItem(entries.at(i).getDate().toString(DATE_FORMAT_STR)));
    }
}

RankingDialog::~RankingDialog()
{
    delete ui;
}

void RankingDialog::selectEntry(int16_t p_position)
{
    ui->rankingWidget->selectRow(p_position);
}


QTableWidgetItem *RankingDialog::createTableWidgetItem(const QString &itemText) const
{
    auto item = new QTableWidgetItem(itemText);
    item->setTextAlignment(Qt::AlignCenter);
    return item;
}
