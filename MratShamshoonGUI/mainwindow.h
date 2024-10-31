#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QPushButton>
#include "VoleMachine.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(Machine* = nullptr, QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onTextChanged(int index);
    void addInstruction(QString instruction);
    void loadFile();
    void printRegister(Register& reg);
    void printMemory(Memory & mem);
    void printPCIR(CPU& cp);

private:
    Ui::MainWindow *ui;
    QLineEdit* textboxes[4];
    int memIndex;
    Machine* machine;
};
#endif // MAINWINDOW_H
