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
    MainWindow(QWidget *parent = nullptr, Machine* = nullptr);
    ~MainWindow();

private slots:
    void onTextChanged(int index);
    void addInstruction(QString instruction);

    void on_pushButton_5_clicked();

private:
    Ui::MainWindow *ui;
    QLineEdit* textboxes[4];
    int memIndex;
    Machine* machine;
};
#endif // MAINWINDOW_H
