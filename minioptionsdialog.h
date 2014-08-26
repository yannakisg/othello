#ifndef MINIOPTIONSDIALOG_H
#define MINIOPTIONSDIALOG_H

#include <QDialog>


class QGroupBox;
class QLabel;
class QLineEdit;
class QComboBox;
class QSpinBox;
class QPushButton;
class MainWindow;

class MiniOptionsDialog : public QDialog {
    Q_OBJECT

public:
    MiniOptionsDialog(QWidget *parent);

private slots:
    void showMainWindow();

private:
    QWidget *gameplay;

    QGroupBox *aiGroupBox;
    QLabel *depthL;
    QSpinBox *depthBox;

    QPushButton *okBtn;
    QPushButton *exitBtn;

    QGroupBox *playerBox;
    QLabel *nameL;
    QLineEdit *nameEdit;
    QLabel *playersTurn;
    QComboBox *playersTurnBox;

    MainWindow *mainWindow;
};

#endif // MINIOPTIONSDIALOG_H
