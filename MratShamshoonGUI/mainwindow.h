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
    void printToScreen(string, int);
    void printRegister(Register& reg, int idx);
    void printMemory(Memory & mem, int, int);
    void printPCIR(CPU& cp);

private:
    Ui::MainWindow *ui;
    QLineEdit* textboxes[4];
    int memIndex;
    int speedOption;
    Machine* machine;
    bool validExtension(string);
};
#endif // MAINWINDOW_H
