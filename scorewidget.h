#ifndef SCOREWIDGET_H
#define SCOREWIDGET_H

#include <QWidget>

class QLCDNumber;
class QLabel;
class QString;

class ScoreWidget : public QWidget {
    Q_OBJECT

public:
    ScoreWidget(QWidget *parent = 0);

    void clearAll();
    void setPoints(short points);
    void addPoints(short points);

private:
    QLCDNumber *scoreLCD;
    QLabel *label;
    short score;
};

#endif // SCOREWIDGET_H
