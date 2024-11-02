#include "mainwindow.h"
#include <QApplication>
#include <QIcon>
#include <QPalette>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Set the application icon
    a.setWindowIcon(QIcon(":/new/Logo/resources/Karen_screen_stock_art.ico")); // Replace with your icon path

    // Apply dark palette
    QPalette darkPalette;
    darkPalette.setColor(QPalette::Window, QColor(53, 53, 53));
    darkPalette.setColor(QPalette::WindowText, Qt::white);
    darkPalette.setColor(QPalette::Base, QColor(42, 42, 42));
    darkPalette.setColor(QPalette::AlternateBase, QColor(66, 66, 66));
    darkPalette.setColor(QPalette::ToolTipBase, Qt::white);
    darkPalette.setColor(QPalette::ToolTipText, Qt::white);
    darkPalette.setColor(QPalette::Text, Qt::white);
    darkPalette.setColor(QPalette::Button, QColor(53, 53, 53));
    darkPalette.setColor(QPalette::ButtonText, Qt::white);
    darkPalette.setColor(QPalette::BrightText, Qt::red);
    darkPalette.setColor(QPalette::Link, QColor(42, 130, 218));
    darkPalette.setColor(QPalette::Highlight, QColor(42, 130, 218));
    darkPalette.setColor(QPalette::HighlightedText, Qt::black);

    a.setPalette(darkPalette);
    a.setStyle("Fusion");

    Machine machine;
    MainWindow w(&machine);
    w.setWindowTitle("Merat Shamshoon");

    w.showMaximized(); // Shows the window maximized but keeps the menu bar visible

    return a.exec();
}
