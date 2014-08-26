#include <QHBoxLayout>
#include <QLabel>
#include <QLCDNumber>

#include "scorewidget.h"

/**
  * The constructor of score widget
  */
ScoreWidget::ScoreWidget(QWidget *parent)
    :QWidget(parent)
{

    scoreLCD = new QLCDNumber(3,this);

    scoreLCD->setSegmentStyle(QLCDNumber::Flat);
    score = 2;
    scoreLCD->display(score);

    label = new QLabel(this);
    label->setText("SCORE:");

    QHBoxLayout *hLayout = new QHBoxLayout(this);
    hLayout->addWidget(label);
    hLayout->addWidget(scoreLCD);
    setLayout(hLayout);
    setFixedSize(60 * 4,105);
}

/**
  * Sets the score and it is displayed by the lcdnumber widget.
  */
void ScoreWidget::setPoints(short points) {
    score = points;
    scoreLCD->display(score);
}

/**
  * Adds the specified points to the score.
  */
void ScoreWidget::addPoints(short points) {
    score += points;
    scoreLCD->display(score);
}

/**
  * Initializes the score
  */
void ScoreWidget::clearAll() {
    setPoints(2);
}
