#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QFileDialog>

MainWindow::MainWindow(Machine* p_machine, QWidget *parent)
    : QMainWindow(parent), machine(p_machine)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    memIndex = 0;

    ui->registerWindow->setAlignment(Qt::AlignCenter);

    textboxes[0] = &(*(ui->InputA));
    textboxes[1] = &(*(ui->InputB));
    textboxes[2] = &(*(ui->InputC));
    textboxes[3] = &(*(ui->InputD));

    for (int i = 0; i < 4; ++i) {
        textboxes[i]->setMaxLength(1);
        connect(textboxes[i], &QLineEdit::textChanged, this, [=](){onTextChanged(i);});
    }

    printRegister(machine->getRegister());

    connect(textboxes[3], &QLineEdit::returnPressed, this, [=](){addInstruction(textboxes[0]->text() + textboxes[1]->text() + textboxes[2]->text() + textboxes[3]->text());});
    connect(ui->AddInstructionButton, &QPushButton::clicked, this, [=](){addInstruction(textboxes[0]->text() + textboxes[1]->text() + textboxes[2]->text() + textboxes[3]->text());});
    connect(ui->LoadFileButton, &QPushButton::clicked, this, [=](){loadFile();});
    connect(&(machine->getRegister()), &Register::registerUpdated, this, [=](){printRegister(machine->getRegister());});
    connect(ui->StepOverButton, &QPushButton::clicked, this, [=](){machine->getCPU().runNextStep(machine->getMemory());});
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onTextChanged(int index){
    QString text = textboxes[index]->text();

    if(!text.isEmpty() && index < 3){
        textboxes[index + 1]->setFocus();
    }

    else if(text.isEmpty() && index > 0){
        textboxes[index - 1]->setFocus();
    }
}

void MainWindow::addInstruction(QString instruction){
    string ins = instruction.toStdString();

    if(ins.size() < 4){
        throw invalid_argument("Instruction size is too short");
    }
    else{
        if(memIndex <= 254){
            for (int i = 0; i < 4; ++i) {
                textboxes[i]->clear();
            }
            textboxes[0]->setFocus();
            if(machine){
                machine->getMemory().addInstruction(ins, memIndex);
                memIndex+=2;
            }
        }
        else{
            throw runtime_error("Memory is full");
        }
    }
}


void MainWindow::loadFile()
{
    // Open the file dialog
    QString filePath = QFileDialog::getOpenFileName(this, "Open File", "", "All Files (*)");

    if (!filePath.isEmpty()) {
        string filePathStr = filePath.toStdString();

        fstream file(filePathStr, ios::in);
        if (file.fail()) {
            throw invalid_argument("invalid file");
        }
        else{
            string outPath = "File Path: " + filePathStr;
            QString out = QString::fromStdString(outPath);
            ui->FilePathLabel->setText(out);
            if(machine)
                memIndex = machine->getMemory().loadMemory(file);
        }
        file.close();
    }
}

void MainWindow::printRegister(Register& reg){
    string output = "Registers\n";
    for (int i = 0; i < 16; ++i) {
        output += "0x" + ALU::decToHex(i) + " ";
        output += ALU::decToHex(reg.getCell(i));
        output += "\n";
    }

    ui->registerWindow->setText(QString::fromStdString(output));
}


