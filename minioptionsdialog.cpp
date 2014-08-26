#include "minioptionsdialog.h"
#include "mainwindow.h"
#include "player.h"

#include <QGroupBox>
#include <QVBoxLayout>
#include <QLabel>
#include <QSpinBox>
#include <QComboBox>
#include <QPushButton>
#include <QLineEdit>

/**
  * Constructor of MiniOptions Dialog
  */
MiniOptionsDialog::MiniOptionsDialog(QWidget *parent) : QDialog(parent) {

    mainWindow = (MainWindow *)parent;
    okBtn = new QPushButton("OK", this);
    exitBtn = new QPushButton("Exit", this);

    aiGroupBox = new QGroupBox("AI Options",this);
    depthL = new QLabel("Max Depth :",this);
    depthBox = new QSpinBox(this);
    depthBox->setValue(mainWindow->getDepth());

    playerBox = new QGroupBox("Player Options",this);
    nameL = new QLabel("Nickname :",this);
    nameEdit = new QLineEdit(mainWindow->getPlayer1()->getName(),this);
    playersTurn = new QLabel("Player's Turn :",this);
    playersTurnBox = new QComboBox(this);

    if (mainWindow->getTurn().at(0) == 'F') {
        playersTurnBox->insertItem(0, "First");
        playersTurnBox->insertItem(1, "Second");
    } else {
        playersTurnBox->insertItem(0, "Second");
        playersTurnBox->insertItem(1, "First");
    }
    QHBoxLayout *aiLayout = new QHBoxLayout();
    aiLayout->addWidget(depthL);
    aiLayout->addWidget(depthBox);
    aiGroupBox->setLayout(aiLayout);

    QHBoxLayout *nameLayout = new QHBoxLayout();
    nameLayout->addWidget(nameL);
    nameLayout->addWidget(nameEdit);

    QHBoxLayout *turnLayout = new QHBoxLayout();
    turnLayout->addWidget(playersTurn);
    turnLayout->addWidget(playersTurnBox);

    QHBoxLayout *btnLayout = new QHBoxLayout();
    btnLayout->addWidget(okBtn);
    btnLayout->addWidget(exitBtn);

    QVBoxLayout *playerLayout = new QVBoxLayout();
    playerLayout->addLayout(nameLayout);
    playerLayout->addLayout(turnLayout);

    playerBox->setLayout(playerLayout);

    QVBoxLayout *gameplayLayout = new QVBoxLayout(this);
    gameplayLayout->setMargin(0);
    gameplayLayout->addWidget(aiGroupBox);
    gameplayLayout->addWidget(playerBox);
    gameplayLayout->addLayout(btnLayout);
    gameplayLayout->addStretch();


   gameplayLayout->setSizeConstraint(QLayout::SetFixedSize);
   setLayout(gameplayLayout);

   connect(okBtn, SIGNAL(clicked()), this, SLOT(showMainWindow()));
   connect(exitBtn, SIGNAL(clicked()), this, SLOT(close()));

   setWindowTitle("Othello FPG - Gameplay Options");
}

/**
  * Saves the changes and shows the main window.
  */
void MiniOptionsDialog::showMainWindow() {
    mainWindow->savePlayersName(nameEdit->text());
    mainWindow->saveDepth(depthBox->value());
    mainWindow->savePlayersTurn(playersTurnBox->itemText(playersTurnBox->currentIndex()));
    this->hide();
    mainWindow->showWindow();
    emit close();
}
