#pragma once

#include <QLabel>

typedef std::pair<uint16_t, uint16_t> CardPosition;

class ClickableCard : public QLabel
{
   Q_OBJECT
public:
   ClickableCard(QWidget *parent, CardPosition p_position) : QLabel(parent), position(p_position) {}
   ~ClickableCard() {}

signals:
   void clicked(CardPosition position);

protected:
   void mousePressEvent(QMouseEvent *event) { emit clicked(position); }

private:
   CardPosition position;
};
