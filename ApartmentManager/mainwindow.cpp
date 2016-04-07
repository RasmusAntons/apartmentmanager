#include "mainwindow.h"
#include "ui_mainwindow.h"



QStringList RssText;




// ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////7

void MainWindow::onTimeChanged() {
    QTime time = QTime::currentTime();
    QString text = time.toString("hh:mm");
    if ((time.second() % 2) == 0)
        text[2] = ' ';
    ui->clock->display(text);
}


int placeAppo(Ui::MainWindow* ui, QStringList CalDate, QStringList CalText, QStringList CalName)
{
    int i = CalDate.length();
    ui->TableCal->setRowCount(i);
    i = 0;
    foreach(const QString &Date, CalDate) {
            ui->TableCal->setItem(i,0,new QTableWidgetItem(Date));
            ui->tableWidgetMain->setItem(i,0,new QTableWidgetItem(Date));
            i++;
    }
    i = 0;
    foreach(const QString &Text, CalText) {
            ui->TableCal->setItem(i,1,new QTableWidgetItem(Text));
            ui->tableWidgetMain->setItem(i,1,new QTableWidgetItem(Text));
            i++;
    }
    i = 0;
    foreach(const QString &Name, CalName) {
            ui->TableCal->setItem(i,2,new QTableWidgetItem(Name));
            ui->tableWidgetMain->setItem(i,2,new QTableWidgetItem(Name));
            i++;
    }
    return 0;
}



// //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



void placeRss(Ui::MainWindow* ui) {
    int i = 0;
    int j = 0;
    QFont TitleFont( "Ubuntu", 15, QFont::Bold);
    foreach(const QString &sRssElement, RssText) {
         if((i%2 == 0)) {
             ui->RssList->addItem(sRssElement);
             ui->RssList->item(j)->setFont(TitleFont);
             j++;
         } else {
             ui->RssList->addItem(sRssElement);
             ui->RssList->addItem(" ");
             j = j +2;
         }
         i++;
    }
}


void ListElements(QDomElement root, QString tagname, Ui::MainWindow* ui) {
    QDomNodeList items = root.elementsByTagName(tagname);
    qDebug() << "TotalItems = " << items.count();

    for(int i = 0; i < items.count(); i++) {
        QDomNode itemnode = items.at(i);
        if(itemnode.isElement()) {
            QDomElement title = itemnode.firstChildElement();
            QDomElement summary = itemnode.lastChildElement();
            RssText << title.text();
            RssText << summary.text();
            //qDebug() << title.text();
            //qDebug() << summary.text() << "\n";

        }
    }
    placeRss(ui);
}


void MainWindow::fileIsReady(QNetworkReply *reply){
    QByteArray data = reply->readAll();
    qDebug() << "XML download size:" << data.size() << "bytes";
    //qDebug() << QString::​fromUtf8(data);

    QDomDocument versionXML;

    QString errorStr;
    int errorLine;
    int errorColumn;

    if(!versionXML.setContent(data, true, &errorStr, &errorLine, &errorColumn))
    {
        qWarning() << "Failed to parse XML";
        qDebug() << errorStr << errorLine << errorColumn;
    } else {
        //qDebug() << "Opened!";TableCal
        QDomElement root = versionXML.firstChildElement();
        ListElements(root, "entry", ui);
    }

}



// //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void showWeather(QString info, Ui::MainWindow* ui) {
    QStringList nTemps;
    QStringList dTemps;
    QStringList pics;
    int i = 0;
    foreach(const QString &nTemp, info.split( "font color=#0000ff>" )) {
        if(i > 0 && i < 5) {
            nTemps << nTemp.left(5).split("°").at(0);
        }
        i++;
    }
    i = 0;
    foreach(const QString &dTemp, info.split( "font color=#ff0000>" )) {
        if(i > 0 && i < 5) {
            dTemps << dTemp.left(5).split("°").at(0);
        }
        i++;
    }
    i = 0;
    foreach(const QString &pic, info.split( "http://www.wetter-vista.de/images/wetter/" )) {
        if(i > 0 && i < 9) {
            pics << (pic.left(23).split(".").at(0));
        }
        i++;
    }
    //qDebug() << nTemps;
    //qDebug() << dTemps;
    //qDebug() << pics;

    ui->wTodayText->setText(nTemps.at(0) + "°C  /  " + dTemps.at(0) + "°C");
    ui->wTomorrowText->setText(nTemps.at(1) + "°C  /  " + dTemps.at(1) + "°C");
    ui->wOvermorrowText->setText(nTemps.at(1) + "°C  /  " + dTemps.at(2) + "°C");

    ui->wTodayImage1->setStyleSheet("background-image: url(weatherImages/" + pics.at(0) + ");");
    ui->wTodayImage2->setStyleSheet("background-image: url(weatherImages/" + pics.at(1) + ");");
    ui->wTomorrowImage1->setStyleSheet("background-image: url(weatherImages/" + pics.at(2) + ");");
    ui->wTomorrowImage2->setStyleSheet("background-image: url(weatherImages/" + pics.at(3) + ");");
    ui->wOvermorrowImage1->setStyleSheet("background-image: url(weatherImages/" + pics.at(4) + ");");
    ui->wOvermorrowImage2->setStyleSheet("background-image: url(weatherImages/" + pics.at(5) + ");");
}

void getWeather(QDomElement root, Ui::MainWindow* ui) {
    QString info;
    QDomNodeList channel = root.elementsByTagName("channel");
    QDomNode channelN = channel.at(0);
    QDomElement channelE = channelN.toElement();
    QDomNodeList item = channelE.elementsByTagName("item");
    QDomNode itemN = item.at(0);
    QDomElement itemE = itemN.lastChildElement();
    //qDebug() << itemE.text();
    showWeather(itemE.text(), ui);

}



void MainWindow::fileIsReadyW(QNetworkReply *reply){
    QByteArray data = reply->readAll();
    qDebug() << "XML download size:" << data.size() << "bytes";
    QDomDocument versionXML;

    QString errorStr;
    int errorLine;
    int errorColumn;

    if(!versionXML.setContent(data, true, &errorStr, &errorLine, &errorColumn))
    {
        qWarning() << "Failed to parse XML";
        qDebug() << errorStr << errorLine << errorColumn;
    } else {
        //qDebug() << "Opened!";
        QDomElement root = versionXML.firstChildElement();
        getWeather(root,ui);
    }

}



// //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //QWebView *WebView = new QWebView(this);
    //ui->tabWidget->addTab(WebView,"Wetter");
    //WebView->setGeometry(0,0,1100,900);
    //WebView->load(QUrl("http://www.wetter-vista.de/wettervorhersage"));

    QPixmap bedeckt("weatherImages/bedeckt.png");
    QPixmap heiter("weatherImages/heiter.png");
    QPixmap regen("weatherImages/regen.png");
    QPixmap regen_leicht("weatherImages/regen-leicht.png");
    QPixmap regenschauer_leicht("weatherImages/regenschauer-leicht.png");
    QPixmap sonne("weatherImages/sonne.png");
    QPixmap wolkig("weatherImages/wolkig.png");

    //Time
    TThread = new TimeThread(this);
    connect(TThread, SIGNAL(TimeChanged()), this, SLOT(onTimeChanged()));
    connect(TThread, SIGNAL(finished()), TThread, SLOT(deleteLater()));
    TThread->start();

    //Calender
    ui->TableCal->setColumnCount(3);
    ui->TableCal->setRowCount(10);
    ui->TableCal->setColumnWidth(0, 300);
    ui->TableCal->setColumnWidth(1, 600);
    ui->TableCal->setColumnWidth(2, 200);
    ui->TableCal->horizontalHeader()->setStretchLastSection(true);
    ui->TableCal->setHorizontalHeaderLabels(QString("Datum; Text; Name").split(";"));

    //Calender main
    ui->tableWidgetMain->setColumnCount(3);
    ui->tableWidgetMain->setRowCount(7);
    ui->tableWidgetMain->setColumnWidth(0, 300);
    ui->tableWidgetMain->setColumnWidth(1, 600);
    ui->tableWidgetMain->setColumnWidth(2, 200);
    ui->tableWidgetMain->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidgetMain->setHorizontalHeaderLabels(QString("Datum; Text; Name").split(";"));

    //RSS News
    QNetworkAccessManager * manager = new QNetworkAccessManager(this);
    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(fileIsReady(QNetworkReply*)));
    manager->setNetworkAccessible(QNetworkAccessManager::Accessible);
    manager->get(QNetworkRequest(QUrl("http://www.tagesschau.de/xml/atom/")));
    ui->RssList->setWordWrap(true);
    ui->RssList->setTextElideMode(Qt::ElideNone);

    //Weather
    QNetworkAccessManager * managerW = new QNetworkAccessManager(this);
    connect(managerW, SIGNAL(finished(QNetworkReply*)), this, SLOT(fileIsReadyW(QNetworkReply*)));
    managerW->setNetworkAccessible(QNetworkAccessManager::Accessible);
    managerW->get(QNetworkRequest(QUrl("http://www.wetter-vista.de/wettervorhersage/wetter-braunschweig-1056.xml")));
    ui->wTodayText->setAlignment(Qt::AlignCenter);
    ui->wTomorrowText->setAlignment(Qt::AlignCenter);
    //ui->textBrowser->setSource(QUrl("w.htm"));
    //ui->webViewAuto->load(QUrl::fromLocalFile("w.htm"));

    //Set Calender
    QStringList CalDate;
    CalDate << "2016-03-24 09:00:00" << "2016-04-01 11:30:00" << "2016-05-01 11:30:00" << "2016-04-01 13:30:00";
    QStringList CalText;
    CalText << "apple" << "pear" << "peach" << "tomato" << "test";
    QStringList CalName;
    CalName << "Rasmus" << "Maximilian" << "Rasmus" << "Maximilian";

    placeAppo(ui, CalDate, CalText, CalName);
    //placeRss(ui);

}



MainWindow::~MainWindow()
{
    if (TThread != 0 && TThread->isRunning() ) {
        TThread->requestInterruption();
        TThread->wait();
    }
    delete ui;
}

/*
void QParent::resizeEvent(QResizeEvent *event) {
    ui->TableCal->setColumnWidth(0, this->width()/3);
    ui->TableCal->setColumnWidth(1, this->width()/3);
    ui->TableCal->setColumnWidth(2, this->width()/3);

    QMainWindow::resizeEvent(event);
}
*/

/*
int placeAppo(Ui::MainWindow* ui, QString Dates[], QString Texts[], QString Names[])
{
    int x = Dates->length();
    QString s = QString::number(x);
    qDebug() << s;

    for(int i = 0; i < Dates->length()-1; i++) {
        ui->DateList->addItem(Dates[i]);
    }

    for(int i = 0; i < Texts->length()-1; i++) {
        ui->TextList->addItem(Texts[i]);
    }

    for(int i = 0; i < Names->length()-1; i++) {
        ui->NameList->addItem(Names[i]);
    }

    return 0;
}

    QString *Dates = new QString[20];
    QString *Texts = new QString[20];
    QString *Names = new QString[20];
    Dates[0] = "2016-03-24 09:00:00";
    Dates[1] = "2016-04-01 11:30:00";
    Dates[2] = "2016-03-24 09:00:00";
    Dates[3] = "2016-04-01 11:30:00";
    Dates[4] = "2016-03-24 09:00:00";
    Dates[5] = "2016-04-01 11:30:00";

    Texts[0] = "Termi2";
    Texts[1] = "test";

    Names[0] = "Maximilian";
    Names[1] = "Rasmus";

    placeAppo(ui, Dates, Texts, Names);
*/
