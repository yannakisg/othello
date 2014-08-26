#include <QWebView>

#include "helpdialog.h"

/**
  * Constructor of Help Dialog
  */
HelpDialog::HelpDialog(QWidget *parent) : QWebPage(parent) {
    view = new QWebView();
}

/**
  * Shows the help dialog
  */
void HelpDialog::showHelpDialog() {
    createWindow(QWebPage::WebModalDialog);
    view->setPage(this);
    view->setWindowIcon(QIcon(":/images/help-icon.png"));
    view->setWindowTitle("Othello FPG - Help");
    view->load(QUrl("qrc:/html/index.html"));
    view->show();
}
