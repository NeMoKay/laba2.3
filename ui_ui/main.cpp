#include "mainwindow.h"
#include <QApplication>
#include <QTranslator>
#include <QLocale>
#include <QStyleFactory>
#include <QFile>
#include <QDebug>

int main(int argc, char *argv[]){
    QApplication a(argc, argv);

    a.setStyle(QStyleFactory::create("Fusion"));

    QTranslator translator;
    QString locale = QLocale::system().name();
    QString path = ":/i18n/" + locale + ".qm";

    if (translator.load(path)) {
        a.installTranslator(&translator);
    }

    QFile styleFile(":/style.qss");
    if (styleFile.open(QFile::ReadOnly)) {
        a.setStyleSheet(styleFile.readAll());
    }
    else{
        qDebug() << "style.qss не найден";
    }

    MainWindow w;
    w.show();

    return a.exec();
}