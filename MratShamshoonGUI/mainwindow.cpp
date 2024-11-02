#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QFileDialog>

MainWindow::MainWindow(Machine* p_machine, QWidget *parent)
    : QMainWindow(parent), machine(p_machine)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    SpecialKeysEvent *filter[4] = {new SpecialKeysEvent, new SpecialKeysEvent, new SpecialKeysEvent, new SpecialKeysEvent};

    memIndex = 0;
    speedOption = 0;

    ui->registerWindow->setAlignment(Qt::AlignCenter);
    ui->memoryWindow->setAlignment(Qt::AlignLeft);
    ui->screenWindow->setAlignment(Qt::AlignTop);
    ui->screenWindow->setAlignment(Qt::AlignHCenter);
    ui->VariablesWindow->setAlignment(Qt::AlignLeft);
    ui->screenWindow->setWordWrap(true);

    textboxes[0] = &(*(ui->InputA));
    textboxes[1] = &(*(ui->InputB));
    textboxes[2] = &(*(ui->InputC));
    textboxes[3] = &(*(ui->InputD));

    for (int i = 0; i < 4; ++i) {
        textboxes[i]->setMaxLength(1);
        connect(textboxes[i], &QLineEdit::textChanged, this, [=](){onTextChanged(i);});
        textboxes[i]->installEventFilter(filter[i]);
        connect(filter[i], &SpecialKeysEvent::keyPressed, textboxes[i], [=](int type){changeBox(i, type);});
    }

    printRegister(machine->getRegister(), -1);
    printMemory(machine->getMemory(), -10, 0);
    printToScreen("Screen", 1);
    printPCIR(machine->getCPU());

    connect(textboxes[3], &QLineEdit::returnPressed, this, [=](){addInstruction(textboxes[0]->text() + textboxes[1]->text() + textboxes[2]->text() + textboxes[3]->text());});
    connect(ui->AddInstructionButton, &QPushButton::clicked, this, [=](){addInstruction(textboxes[0]->text() + textboxes[1]->text() + textboxes[2]->text() + textboxes[3]->text());});
    connect(ui->LoadFileButton, &QPushButton::clicked, this, [=](){machine->clear();loadFile();});
    connect(&(machine->getRegister()), &Register::registerUpdated, this, [=](int idx){printRegister(machine->getRegister(), idx);});
    connect(ui->StepOverButton, &QPushButton::clicked, this, [=](){if(machine->getCPU().getPC() <= 254){machine->getCPU().runNextStep(machine->getMemory());}});
    connect(&(machine->getMemory()), &Memory::MemoryUpdated, this, [=](int change){printMemory(machine->getMemory(), change, machine->getCPU().getPC());});
    connect(&(machine->getCPU()), &CPU::CPUupdated, this, [=](){printPCIR(machine->getCPU());printMemory(machine->getMemory(), -2, machine->getCPU().getPC());});
    connect(&(machine->getCPU()), &CPU::printUpdate, this, [=](string str, int type){printToScreen(str, type);});
    connect(ui->Clear, &QPushButton::clicked, this, [=](){machine->clear();memIndex = 0;ui->screenWindow->clear();printToScreen("Screen", 1);});
    connect(ui->Reset, &QPushButton::clicked, this, [=](){machine->reset();ui->screenWindow->clear();printToScreen("Screen", 1);});
    connect(ui->Speed1, &QPushButton::clicked, this, [=](){speedOption=0;});
    connect(ui->Speed05, &QPushButton::clicked, this, [=](){speedOption=1;});
    connect(ui->Speed025, &QPushButton::clicked, this, [=](){speedOption=2;});
    connect(ui->Speed2, &QPushButton::clicked, this, [=](){speedOption=3;});
    connect(ui->PlayButton, &QPushButton::clicked, this, [=](){machine->play(speedOption);});
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

void MainWindow::changeBox(int idx, int type){
    if(type == 0 || type == 2){
        if(idx > 0){
            textboxes[idx - 1]->setFocus();
        }
    }
    else if(type == 1){
        if(idx < 3){
            textboxes[idx + 1]->setFocus();
        }
    }
}

void MainWindow::addInstruction(QString instruction){
    string ins = instruction.toStdString();

    if(ins.size() < 4){
        printToScreen("Instruction size is too short", 0);
    }
    else{
        if(memIndex <= 254){
            for (int i = 0; i < 4; ++i) {
                textboxes[i]->clear();
            }
            textboxes[0]->setFocus();
            transform(ins.begin(), ins.end(), ins.begin(), ::toupper);
            machine->getMemory().addInstruction(ins, memIndex);
            memIndex+=2;
        }
        else{
            printToScreen("Memory is full", 0);
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
        if (file.fail() || !validExtension(filePathStr)) {
            printToScreen("Invalid File", 0);
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

bool MainWindow::validExtension(string fileName){
    int dotPos = fileName.find_last_of('.');

    if(dotPos == fileName.size()){
        return false;
    }

    string ext = fileName.substr(dotPos + 1);
    if(ext == "txt"){
        return true;
    } else{
        return false;
    }
}

void MainWindow::printToScreen(string str, int type){
    QString tmp = QString::fromStdString(str);
    QString out;
    if(type == 0){
        out = "<font color='#cc0000'>" + tmp + "</font><br>";
    } else if(type == 1){
        out = "<font color='#FAFAFB'>" + tmp + "</font><br>";
    } else if(type == 2){
        out = "<font color='#4E9A06'>" + tmp + "</font><br>";
    }
    ui->screenWindow->setText(ui->screenWindow->text() + out);
}

void MainWindow::printRegister(Register& reg, int idx){
    QString out;
    out = "<font color='#FAFAFB'>" + QString::fromStdString("Registers") + "</font><br>";
    for (int i = 0; i < 16; ++i) {
        if(i != idx){
            out += "<font color='#FAFAFB'>" + QString::fromStdString("0x" + ALU::decToHex(i) + " ") + "<\font>";
            out += "<font color='#4E9A06'>" + QString::fromStdString(ALU::decToHex(reg.getCell(i))) + "<\font><br>";
        } else {
            out += "<font color='#1391DB'>" + QString::fromStdString("0x" + ALU::decToHex(i) + " ") + "<\font>";
            out += "<font color='#1391DB'>" + QString::fromStdString(ALU::decToHex(reg.getCell(i))) + "<\font><br>";
        }
    }

    ui->registerWindow->setText(out);
}
void MainWindow::printMemory(Memory& memo, int change, int wait){
    QString out = "<font color='#FAFAFB'>" + QString::fromStdString("Memory") + "<\font>";
    string tmp;
    for(int i = 0; i < 16; i++){
        tmp += "  0x" + ALU::decToHex(i) + " ";
    }
    out += "<font style='color:#FAFAFB;'>" + QString::fromStdString(tmp) + " | " "<\font><div style='line-height: 0.48;'><br></div>";
    for(int i = 0; i < 16; i++){
        out += "<font style='color:#FAFAFB;'>" + QString::fromStdString("0x" + ALU::decToHex(i)) + " | " "<\font>";
        for(int j = 0 ; j < 16; j++){
            if((i * 16 + j) == change || (i * 16 + j) == change + 1){
                out += "<font style='color:#1391DB;'>" + QString::fromStdString(memo.getCell(i * 16 + j)) + " | " "<\font>";
            } else if((i * 16 + j) == wait || (i * 16 + j) == wait + 1){
                out += "<font style='color:#ffd91e;'>" + QString::fromStdString(memo.getCell(i * 16 + j)) + " | " "<\font>";
            } else{
                out += "<font style='color:#4E9A06;'>" + QString::fromStdString(memo.getCell(i * 16 + j)) + " | " "<\font>";
            }
        }
        out += "<br><div style='line-height: 0.48;'><br></div>";
    }
    ui->memoryWindow->setText(out);
}

void MainWindow::printPCIR(CPU& cp){
    string output = "PC: " + to_string(cp.getPC());
    output += "\nIR: " + cp.getIR();

    ui->VariablesWindow->setText(QString::fromStdString(output));
}
