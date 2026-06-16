#include "mainwindow.h"
#include <QApplication>
#include <QFile>
#include <QTranslator>
#include <QLocale>
#include <QStyleFactory>
#include <QDebug>

int main(int argc, char *argv[]){
    QApplication a(argc, argv);
    a.setStyle(QStyleFactory::create("Fusion"));

    QTranslator translator;
    if (translator.load(":/i18n/" + QLocale::system().name() + ".qm")) {
        a.installTranslator(&translator);
    }

    QFile styleFile(":/style.qss");
    if (styleFile.open(QFile::ReadOnly)) {
        a.setStyleSheet(styleFile.readAll());
    } else {
        qDebug() << "Ошибка: файл style.qss не найден в ресурсах!";
    }

    MainWindow w;
    w.show();
    return a.exec();
}