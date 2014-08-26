#ifndef HELPDIALOG_H
#define HELPDIALOG_H

#include <QWebPage>

class QWebView;

class HelpDialog : public QWebPage {
    Q_OBJECT

public:
    HelpDialog(QWidget *parent);

    void showHelpDialog();

private:
    QWebView *view;
};

#endif // HELPDIALOG_H
