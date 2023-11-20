#include "mainwindow.h"
#include "auth_window.h"

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

    qApp->setStyle(QStyleFactory::create("Fusion"));
    darkPalette palette;
    auto darkPollete = palette.setDarkPalette();
    qApp->setPalette(darkPollete);

    Auth_window auth;
    if (auth.exec() == QDialog::Accepted) {
        MainWindow w;
        w.show();
        return a.exec();
    } else {
        return 0;
    }

    MainWindow w;
    w.show();

    return a.exec();
}
