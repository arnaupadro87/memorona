#pragma once

#include "ranking.h"

#include <QDialog>
#include <QList>
#include <QTableWidgetItem>

namespace Ui {
class RankingDialog;
}

class RankingDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RankingDialog(const Ranking &p_ranking, QWidget *parent = nullptr);
    ~RankingDialog();

    void selectEntry(int16_t p_position);

private:
    Ui::RankingDialog *ui;

    QTableWidgetItem* createTableWidgetItem(const QString &itemText) const;
};
