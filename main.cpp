#include "mainwindow.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <QCoreApplication>
#include <QDebug>
#include <QWidget>
#include <QStyleFactory>
#include <QtCharts>
#include <QLabel>
#include <chrono>
#include <iostream>
#include "darkpalette.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "FitnessJournal_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }
    MainWindow w;
    w.show();
    qApp->setStyle(QStyleFactory::create("Fusion"));
    darkPalette palette;
    auto darkPollete = palette.setDarkPalette();
    qApp->setPalette(darkPollete);
    return a.exec();
}
