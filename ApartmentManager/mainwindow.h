#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "timethread.h"

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QXmlReader>
#include <QUrl>
#include <QDomDocument>
#include <QFile>

#include <QWebView>
#include <QUrl>
#include <QCoreApplication>

#include <QtSql>
#include <QtDebug>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT


public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    TimeThread *TThread;
    int exit();
    QStringList RssText;
    void placeRss(Ui::MainWindow* ui);
    void showWeather(QString info, Ui::MainWindow* ui);




private:
    Ui::MainWindow *ui;

public slots:
    void onTimeChanged();
    void fileIsReady(QNetworkReply * reply);
    void fileIsReadyW(QNetworkReply * reply);

};

#endif // MAINWINDOW_H
