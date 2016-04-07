#include "mainwindow.h"
#include <QApplication>

#include <QtGui>
#include <QtWebKit>
#include <QWebView>
#include <QUrl>

#include <QtSql>
#include <QtDebug>
#include <sqlite3.h>

#include <iostream>
using namespace std;


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow w;
    w.show();

    int r = a.exec();

    return r;
}
