#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QPushButton>
#include <QEvent>
#include <QKeyEvent>
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
    void changeBox(int, int);

private:
    Ui::MainWindow *ui;
    QLineEdit* textboxes[4];
    int memIndex;
    int speedOption;
    Machine* machine;
    bool validExtension(string);
    void speed1();
    void speed05();
    void speed025();
    void speed2();
    void PlayButton(bool);
};

class SpecialKeysEvent : public QObject{
    Q_OBJECT

public:
    using QObject::QObject;
signals:
    void keyPressed(int type);
protected:
    bool eventFilter(QObject* obj, QEvent* event) override{
        if(event->type() == QEvent::KeyPress){
            QKeyEvent* key = static_cast<QKeyEvent*>(event);
            if(key->key() == Qt::Key_Backspace){
                emit keyPressed(0);
            }
            else if(key->key() == Qt::Key_Right){
                emit keyPressed(1);
            }
            else if(key->key() == Qt::Key_Left){
                emit keyPressed(2);
            }
            return false;
        }
        return QObject::eventFilter(obj, event);
    }
};

#endif // MAINWINDOW_H
