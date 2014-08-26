#include <QToolBar>
#include <QMenuBar>
#include <QLabel>
#include <QStatusBar>
#include <QDockWidget>
#include <QSettings>
#include <QMessageBox>
#include <QCloseEvent>
#include <QFileDialog>
#include <QDateTime>
#include <QPlastiqueStyle>
#include <QApplication>
#include <QSystemTrayIcon>

#include "mainwindow.h"
#include "historydialog.h"
#include "optionsdialog.h"
#include "minioptionsdialog.h"
#include "boardwidget.h"
#include "player.h"
#include "scorewidget.h"
#include "helpdialog.h"

/*!
 * Calls functions to greate GUI Elements
 */
MainWindow::MainWindow() {
    createGui();
    createActions();
    createMenus();
    createToolBars();
    createStatusBar();
    createSystemTrayIcon();
    readSettings();

    setWindowIcon(QIcon(":/images/icon.png"));

    isSaved = false;
    updateStatusBar(p1->getName() + "'s turn");

    QApplication::setStyle(new QPlastiqueStyle);

    if (!showMiniOptDialog)
        this->show();
}

/*!
 * Default destructor
 */
MainWindow::~MainWindow() {
    delete p1;
    delete p2;
}

/*!
 * Creates a system tray (if selected in settings)
 */
void MainWindow::createSystemTrayIcon() {
    trayMenu = new QMenu(this);

    minimizeAction = new QAction("Minimize",trayMenu);
    connect(minimizeAction, SIGNAL(triggered()), this, SLOT(hide()));
    restoreAction = new QAction("Restore", trayMenu);
    connect(restoreAction, SIGNAL(triggered()), this, SLOT(show()));

    trayMenu->addAction(newAction);
    trayMenu->addAction(saveAction);
    trayMenu->addAction(loadAction);
    trayMenu->addAction(scoreHistoryAction);
    trayMenu->addAction(minimizeAction);
    trayMenu->addAction(restoreAction);
    trayMenu->addAction(exitAction);

    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setContextMenu(trayMenu);

    trayIcon->setIcon(QIcon(":/images/icon.png"));
}

/*!
 * Draws the GUI Elements
 */
void MainWindow::createGui() {


    p1 = new Player(true, "Player1");
    p2 = new Player(false, "Eniac");

    boardwidget = new BoardWidget(p1,p2,this);
    setCentralWidget(boardwidget);

    scoreWidgetP1 = new ScoreWidget(this);
    scoreWidgetP2 = new ScoreWidget(this);

    p1->setScoreWidget(scoreWidgetP1);
    p2->setScoreWidget(scoreWidgetP2);

    dockWidgetP1 = new QDockWidget(p1->getName(), this);
    dockWidgetP1->setAllowedAreas(Qt::BottomDockWidgetArea);
    dockWidgetP1->setWidget(scoreWidgetP1);
    dockWidgetP1->setFeatures(QDockWidget::NoDockWidgetFeatures);
    addDockWidget(Qt::BottomDockWidgetArea, dockWidgetP1);

    dockWidgetP2 = new QDockWidget(p2->getName(),this);
    dockWidgetP2->setAllowedAreas(Qt::BottomDockWidgetArea);
    dockWidgetP2->setWidget(scoreWidgetP2);
    dockWidgetP2->setFeatures(QDockWidget::NoDockWidgetFeatures);
    addDockWidget(Qt::BottomDockWidgetArea, dockWidgetP2);

    setFixedSize(B_WIDTH * 8,695);
    setWindowTitle("Othello FPG");
}

/*!
 * Binds Actions to menus
 */
void MainWindow::createActions() {
    newAction = new QAction("&New Game",this);
    newAction->setIcon(QIcon(":/images/new-icon.png"));
    newAction->setShortcut(QKeySequence::New);
    connect(newAction, SIGNAL(triggered()), this, SLOT(newGame()));

    loadAction = new QAction("&Load Game...",this);
    loadAction->setIcon(QIcon(":/images/open-icon.png"));
    loadAction->setShortcut(tr("Ctrl+L"));
    connect(loadAction, SIGNAL(triggered()), this,SLOT(loadGame()));

    saveAction = new QAction("&Save Game As...",this);
    saveAction->setIcon(QIcon(":/images/save-icon.png"));
    saveAction->setShortcut(QKeySequence::SaveAs);
    connect(saveAction, SIGNAL(triggered()), this, SLOT(saveGame()));

    exitAction = new QAction("E&xit",this);
    exitAction->setIcon(QIcon(":/images/exit-icon.png"));
    exitAction->setShortcut(QKeySequence::Quit);
    connect(exitAction, SIGNAL(triggered()), this, SLOT(exitGame()));

    optionsAction = new QAction("&Options...",this);
    optionsAction->setIcon(QIcon(":/images/options-icon.png"));
    optionsAction->setShortcut(tr("Ctrl+O"));
    connect(optionsAction, SIGNAL(triggered()), this, SLOT(options()));

    scoreHistoryAction = new QAction("Score &History...",this);
    scoreHistoryAction->setIcon(QIcon(":/images/history-icon.png"));
    scoreHistoryAction->setShortcut(tr("Ctrl+H"));
    connect(scoreHistoryAction, SIGNAL(triggered()), this, SLOT(scoreHistory()));

    contextHelpAction = new QAction("Context Help...",this);
    contextHelpAction->setIcon(QIcon(":/images/help-icon.png"));
    contextHelpAction->setShortcut(tr("F1"));
    connect(contextHelpAction, SIGNAL(triggered()), this, SLOT(helpContents()));

    aboutAction = new QAction("About Us...",this);
    aboutAction->setIcon(QIcon(":/images/about-icon.png"));
    connect(aboutAction, SIGNAL(triggered()), this, SLOT(about()));
}

/*!
 * Creates the menus
 */
void MainWindow::createMenus() {
    gameMenu = menuBar()->addMenu("&Game");
    gameMenu->addAction(newAction);
    gameMenu->addAction(loadAction);
    gameMenu->addAction(saveAction);
    gameMenu->addSeparator();
    gameMenu->addAction(exitAction);

    toolsMenu = menuBar()->addMenu("&Tools");
    toolsMenu->addAction(scoreHistoryAction);
    toolsMenu->addSeparator();
    toolsMenu->addAction(optionsAction);

    helpMenu = menuBar()->addMenu("&Help");
    helpMenu->addAction(contextHelpAction);
    helpMenu->addSeparator();
    helpMenu->addAction(aboutAction);
}

/*!
 * Creates the toolbars
 */
void MainWindow::createToolBars() {
    gameToolBar = addToolBar(("&Game"));
    gameToolBar->addAction(newAction);
    gameToolBar->addAction(saveAction);
    gameToolBar->addAction(scoreHistoryAction);
    gameToolBar->addAction(exitAction);
    gameToolBar->setMovable(false);
}

/**
  * Returns the human player
  */
Player* MainWindow::getPlayer1() {
    return this->p1;
}

/**
  * Creates the status bar
  */
void MainWindow::createStatusBar() {
    statusLabel = new QLabel(this);
    statusLabel->setIndent(3);
    statusBar()->addWidget(statusLabel,1);

    connect(boardwidget, SIGNAL(btnClicked(const QString &)), this, SLOT(updateStatusBar(const QString &)));
    connect(boardwidget, SIGNAL(gameOver()), this, SLOT(saveScore()));

}

/**
  * Updates the status bar
  */
void MainWindow::updateStatusBar(const QString &objName) {
    statusLabel->setText(objName);
    this->update();
}

/**
  * Creates a new game
  */
void MainWindow::newGame() {
    p1->clearAll();
    p2->clearAll();
    boardwidget->clearBoard();
    isSaved = false;
}

/**
  * Loads a game from a saved file
  */
void MainWindow::loadGame() {
    QString fileName = QFileDialog::getOpenFileName(this, "Othello-FPG - Load Game", ".", "Othello FPG files (*.fpgoth)");
    if (!fileName.isEmpty()) {
        if (!boardwidget->readFile(fileName))
            statusLabel->setText("Loading canceled");
    }
}

/**
  * Saves the game into a file
  */
bool MainWindow::saveGame() {
   QString fileName = QFileDialog::getSaveFileName(this, "Othello-FPG - Save Game",  ".", "Othello FPG files (*.fpgoth)");
    if (fileName.isEmpty())
        return false;
    else {
        if (!fileName.endsWith(".fpgoth"))
            fileName.append(".fpgoth");
        isSaved = boardwidget->writeFileAs(fileName);
        if (!isSaved) {
            statusLabel->setText("Saving canceled");
            return false;
        } else {
            statusLabel->setText("Game saved");
            return true;
        }
    }
}

void MainWindow::closeEvent(QCloseEvent *event) {
    if (!isSaved) {
        int reply = QMessageBox::warning(this, "Othello-FPG", "Do you want to save the game ?", QMessageBox::Yes | QMessageBox::No| QMessageBox::Cancel);
        if (reply == QMessageBox::Yes) {
            if (saveGame()) {
                writeSettings();
                event->accept();
            }
            else
                event->ignore();
        }
        else if (reply == QMessageBox::Cancel) {
           event->ignore();
           return;
       }
    }
    writeSettings();
    event->accept();

}

void MainWindow::exitGame() {

    if (!isSaved) {
        int reply = QMessageBox::warning(this, "Othello-FPG", "Do you want to save the game ?", QMessageBox::Yes | QMessageBox::No| QMessageBox::Cancel);
        if (reply == QMessageBox::Yes)
            saveGame();
        else if (reply == QMessageBox::Cancel)
            return;
        else {
            isSaved = true;
            emit close();
        }
    } else {
        emit close();
    }
}

/**
  * Shows the options dialog
  */
void MainWindow::options() {
    OptionsDialog *dialog = new OptionsDialog(this);
    dialog->isSystrayChecked(showTrayIcon);
    dialog->isGOptDialogChecked(showMiniOptDialog);
    dialog->show();
}

/*!
 * Sets the icons in the BoardWidget object
 * @param icon1 first icon
 * @param icon2 second icon
 */
void MainWindow::setIcons(QIcon *icon1, QIcon *icon2) {
    boardwidget->setIcons(icon1,icon2, true);
}

/**
  * Shows the score history dialog
  */
void MainWindow::scoreHistory() {
    HistoryDialog *dialog = new HistoryDialog(p1->getName(), this);
    dialog->show();
}

/**
  * Shows the help dialog
  */
void MainWindow::helpContents() {
    HelpDialog *help = new HelpDialog(this);
    help->showHelpDialog();
}

/**
  * Shows the about dialog
  */
void MainWindow::about() {
  QMessageBox::about(this, "About Othello FPG",
                     "<h2>Othello-FPG 1.0</h2>"
                     "<p>Copyright &copy; 2010 FPG Inc."
                     "<p>Othello-FPG is a Qt/C++ implementation of the game Othello/Reversi."
                     "<h3>Developers</h3>"
                     "<ul>"
                    "<li>Charalampos Fokas</li>"
                    "<li>Marios Pomonis</li>"
                    "<li>John Gasparis</li>"
                     "</ul>"
                     );

}

/**
  * Shows that window. This function is called from minioptionsdialog
  */
void MainWindow::showWindow() {
    this->show();
    if (p2->getTurn()) {
        updateStatusBar(p2->getName() + "'s turn");
        this->setCursor(Qt::WaitCursor);
        qApp->processEvents();
        this->repaint();
        boardwidget->pcPlaying();
         this->setCursor(Qt::ArrowCursor);
    }
}

/**
  * Stores the settings in a platform-dependent file.
  * On windows settings are stored in the system registry and on unix systems in text files
  */
void MainWindow::writeSettings() {
    QSettings settings("FPG Inc.", "Othello");

    if (list.size() != 0)
        settings.setValue("player1/score", list);

    settings.setValue("trayIcon", showTrayIcon);
    settings.setValue("miniopt", showMiniOptDialog);

    settings.setValue("max_depth", boardwidget->getMaxDepth());
    settings.setValue("p1turn", p1Turn);

    if (nickname.size() != 0)
        settings.setValue("nickname", nickname);
}

/**
  * Reads the settings and updates the appropriate variables.
  */
void MainWindow::readSettings() {
    QSettings settings("FPG Inc.", "Othello");

    list = settings.value("player1/score").toStringList();


    showTrayIcon = settings.value("trayIcon").toBool();
    showMiniOptDialog = settings.value("miniopt").toBool();
    nickname = settings.value("nickname").toString();
    if (nickname == NULL)
        nickname = "Player1";
    p1Turn = settings.value("p1turn").toString();

    if (p1Turn == NULL)
        p1Turn = "First";

    boardwidget->setMaxDepth(settings.value("max_depth").toInt());

    if (nickname.size() != 0) {
        p1->setName(nickname);
        dockWidgetP1->setWindowTitle(nickname);
    }

    if (showTrayIcon)
        trayIcon->show();

   if (showMiniOptDialog) {
        MiniOptionsDialog *opt = new MiniOptionsDialog(this);
        opt->setWindowIcon(QIcon(":/images/icon.png"));
        opt->show();
    }
}

/**
  * Saves the final score
  */
void MainWindow::saveScore() {
    QString score = QString::number(p1->getScore()) + ":" + QString::number(p2->getScore());
    QString dateTime = QDateTime::currentDateTime().toString("M/d/yy hh:mm:ss");

    list.append(dateTime + "|" + score);

}

void MainWindow::saveSystemTrayChoice(bool isChecked) {
    if (isChecked)
        trayIcon->show();
    else
        trayIcon->hide();

    showTrayIcon = isChecked;
}

void MainWindow::saveGameplayOptDialogChoice(bool isChecked) {
    showMiniOptDialog = isChecked;
}

void MainWindow::savePlayersTurn(QString turn) {
    bool isBlack;

    p1Turn = turn;

    if (turn.at(0) == 'F')
        isBlack = true;
    else
        isBlack = false;

    p1->setTurn(isBlack);
    p2->setTurn(!isBlack);
}

/**
  * Saves the name of human player
  */
void MainWindow::savePlayersName(QString name) {
    nickname = name;
    p1->setName(nickname);
    dockWidgetP1->setWindowTitle(nickname);
}

QStringList& MainWindow::getScoreList() {
    return list;
}

/**
  * Saves the maximum depth of minimax
  */
void MainWindow::saveDepth(int depth) {
    boardwidget->setMaxDepth(depth);
}

/**
  * Returns the maximum depth of minimax
  */
int MainWindow::getDepth() {
    return boardwidget->getMaxDepth();
}

/**
  * Returns the turn of human player
  */
QString& MainWindow::getTurn() {
    return this->p1Turn;
}
