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
    ui->memoryWindow->setAlignment(Qt::AlignLeft);
    ui->screenWindow->setAlignment(Qt::AlignCenter);

    textboxes[0] = &(*(ui->InputA));
    textboxes[1] = &(*(ui->InputB));
    textboxes[2] = &(*(ui->InputC));
    textboxes[3] = &(*(ui->InputD));

    for (int i = 0; i < 4; ++i) {
        textboxes[i]->setMaxLength(1);
        connect(textboxes[i], &QLineEdit::textChanged, this, [=](){onTextChanged(i);});
    }

    printRegister(machine->getRegister());
    printMemory(machine->getMemory());
    printToScreen("Screen");

    connect(textboxes[3], &QLineEdit::returnPressed, this, [=](){addInstruction(textboxes[0]->text() + textboxes[1]->text() + textboxes[2]->text() + textboxes[3]->text());});
    connect(ui->AddInstructionButton, &QPushButton::clicked, this, [=](){addInstruction(textboxes[0]->text() + textboxes[1]->text() + textboxes[2]->text() + textboxes[3]->text());});
    connect(ui->LoadFileButton, &QPushButton::clicked, this, [=](){loadFile();});
    connect(&(machine->getRegister()), &Register::registerUpdated, this, [=](){printRegister(machine->getRegister());});
    connect(&(machine->getMemory()), &Memory::MemoryUpdated, this, [=](){printMemory(machine->getMemory());});
    connect(ui->StepOverButton, &QPushButton::clicked, this, [=](){machine->getCPU().runNextStep(machine->getMemory());});
    connect(&(machine->getCPU()), &CPU::printUpdate, this, [=](string str){printToScreen(str);});
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

void MainWindow::printToScreen(string str){
    str += "\n";
    QString tmp = QString::fromStdString(str);
    ui->screenWindow->setText(ui->screenWindow->text() + tmp);
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
void MainWindow::printMemory(Memory& memo){
    string output = "Memory\n";
    for(int i = 0; i < 16;i++){
        output += "0x" + ALU::decToHex(i) + " ";
    }
    output += "\n";
    for(int i = 0 ;i <16 ;i++){
        output+= "0x" + ALU::decToHex(i) + " ";
        for(int j = 0 ; j < 16 ; j++){
            output += memo.getCell(i * 16 + j) +  " ";
        }
        output += "\n";
    }
    ui->memoryWindow->setText(QString::fromStdString(output));
}

