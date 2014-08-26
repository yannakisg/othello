#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QMenu;
class QToolBar;
class QAction;
class QLabel;
class QDockWidget;
class QIcon;
class QSystemTrayIcon;
class Player;
class ScoreWidget;
class BoardWidget;

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow();
    ~MainWindow();
    QStringList& getScoreList();
    void setIcons(QIcon *icon1, QIcon *icon2);
    void saveSystemTrayChoice(bool isChecked);
    void saveGameplayOptDialogChoice(bool isChecked);
    void saveDepth(int depth);
    void savePlayersName(QString name);
    void savePlayersTurn(QString turn);
    void showWindow();
    Player* getPlayer1();
    int getDepth();
    QString& getTurn();

protected:
    void closeEvent(QCloseEvent *event);

private slots:
    void updateStatusBar(const QString &objName);
    void newGame();
    void loadGame();
    bool saveGame();
    void saveScore();
    void exitGame();
    void options();
    void scoreHistory();
    void helpContents();
    void about();

private:
    void createActions();
    void createMenus();
    void createToolBars();
    void createGui();
    void createStatusBar();
    void createSystemTrayIcon();
    void readSettings();
    void writeSettings();


    BoardWidget *boardwidget;
    ScoreWidget *scoreWidgetP1;
    ScoreWidget *scoreWidgetP2;
    QDockWidget *dockWidgetP1;
    QDockWidget *dockWidgetP2;
    QSystemTrayIcon *trayIcon;
    QMenu *gameMenu;
    QMenu *toolsMenu;
    QMenu *helpMenu;
    QMenu *trayMenu;

    QLabel *statusLabel;

    QToolBar *gameToolBar;

    QAction *newAction;
    QAction *loadAction;
    QAction *saveAction;
    QAction *exitAction;
    QAction *optionsAction;
    QAction *scoreHistoryAction;
    QAction *contextHelpAction;
    QAction *aboutAction;
    QAction *minimizeAction;
    QAction *restoreAction;

    bool isSaved;
    bool showTrayIcon;
    bool showMiniOptDialog;
    QString p1Turn;
    QString nickname;
    Player *p1, *p2;
    QStringList list;
};

#endif // MAINWINDOW_H
