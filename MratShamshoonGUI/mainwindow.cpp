#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QFileDialog>

MainWindow::MainWindow(Machine* p_machine, QWidget *parent)
    : QMainWindow(parent), machine(p_machine)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    SpecialKeysEvent *filter[4] = {new SpecialKeysEvent, new SpecialKeysEvent, new SpecialKeysEvent, new SpecialKeysEvent};

    memIndex = 2;
    speedOption = 0;

    ui->registerWindow->setAlignment(Qt::AlignCenter);
    ui->memoryWindow->setAlignment(Qt::AlignCenter);
    ui->textEdit->setAlignment(Qt::AlignTop);
    ui->textEdit->setAlignment(Qt::AlignHCenter);
    ui->VariablesWindow->setAlignment(Qt::AlignLeft);
    ui->textEdit->setReadOnly(true);


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
    printMemory(machine->getMemory(), -10, 2);
    printToScreen("Screen", 1);
    printPCIR(machine->getCPU());
    speed1();
    PlayButton(machine->isPlaying());

    connect(textboxes[3], &QLineEdit::returnPressed, this, [=](){addInstruction(textboxes[0]->text() + textboxes[1]->text() + textboxes[2]->text() + textboxes[3]->text());});
    connect(ui->AddInstructionButton, &QPushButton::clicked, this, [=](){addInstruction(textboxes[0]->text() + textboxes[1]->text() + textboxes[2]->text() + textboxes[3]->text());});
    connect(ui->LoadFileButton, &QPushButton::clicked, this, [=](){machine->clear();loadFile();});
    connect(&(machine->getRegister()), &Register::registerUpdated, this, [=](int idx){printRegister(machine->getRegister(), idx);});
    connect(ui->StepOverButton, &QPushButton::clicked, this, [=](){if(machine->getCPU().getPC() <= 254){machine->getCPU().runNextStep(machine->getMemory());}});
    connect(&(machine->getMemory()), &Memory::MemoryUpdated, this, [=](int change){printMemory(machine->getMemory(), change, machine->getCPU().getPC());});
    connect(&(machine->getCPU()), &CPU::CPUupdated, this, [=](){printPCIR(machine->getCPU());printMemory(machine->getMemory(), -2, machine->getCPU().getPC());});
    connect(&(machine->getCPU()), &CPU::printUpdate, this, [=](string str, int type){printToScreen(str, type);});
    connect(ui->Clear, &QPushButton::clicked, this, [=](){machine->clear();ui->FilePathLabel->setText("file path");memIndex = 2;ui->textEdit->clear();printToScreen("Screen", 1);PlayButton(machine->isPlaying());});
    connect(ui->Reset, &QPushButton::clicked, this, [=](){machine->reset();ui->textEdit->clear();printToScreen("Screen", 1);PlayButton(machine->isPlaying());});
    connect(ui->Speed1, &QPushButton::clicked, this, [=](){speed1();if(machine->isPlaying()){machine->stop();machine->play(speedOption);}});
    connect(ui->Speed05, &QPushButton::clicked, this, [=](){speed05();if(machine->isPlaying()){machine->stop();machine->play(speedOption);}});
    connect(ui->Speed025, &QPushButton::clicked, this, [=](){speed025();if(machine->isPlaying()){machine->stop();machine->play(speedOption);}});
    connect(ui->Speed2, &QPushButton::clicked, this, [=](){speed2();if(machine->isPlaying()){machine->stop();machine->play(speedOption);}});
    connect(ui->PlayButton, &QPushButton::clicked, this, [=](){PlayButton(!(machine->isPlaying()));machine->play(speedOption);});
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
            string outPath = filePathStr;
            QString out = QString::fromStdString(outPath);
            ui->FilePathLabel->setText(out);
            ui->FilePathLabel->setToolTip(out);
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

void MainWindow::printToScreen(string str, int type) {
    QString tmp = QString::fromStdString(str);
    QString out;

    if (type == 0) {
        out = "<font color='#cc0000'>" + tmp + "</font><br>";
    } else if (type == 1) {
        out = "<font color='#FAFAFB'>" + tmp + "</font><br>";
    } else if (type == 2) {
        out = "<font color='#4E9A06'>" + tmp + "</font><br>";
    }

    // Append the text to the screen
    ui->textEdit->append(out);

    // Scroll to the bottom
    QTextCursor cursor = ui->textEdit->textCursor();
    cursor.movePosition(QTextCursor::End);
    ui->textEdit->setTextCursor(cursor);
}

void MainWindow::printRegister(Register& reg, int idx){
    QString out;
    out = "<font color='#FAFAFB'>" + QString::fromStdString("Registers") + "</font><div style='line-height: 0.5;'><br></div>";
    for (int i = 0; i < 16; ++i) {
        if(i != idx){
            out += "<font color='#FAFAFB'>" + QString::fromStdString("0x" + ALU::decToHex(i) + " ") + "<\font>";
            out += "<font color='#4E9A06'>" + QString::fromStdString(ALU::decToHex(reg.getCell(i))) + "<\font><div style='line-height: 0.5;'><br></div>";
        } else {
            out += "<font color='#1391DB'>" + QString::fromStdString("0x" + ALU::decToHex(i) + " ") + "<\font>";
            out += "<font color='#1391DB'>" + QString::fromStdString(ALU::decToHex(reg.getCell(i))) + "<\font><div style='line-height: 0.5;'><br></div>";
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
    out += "<font color='#FAFAFB'>" + QString::fromStdString(tmp) + "<\font><div style='line-height: 0.48;'><br></div>";
    for(int i = 0; i < 16; i++){
        out += "<font color='#FAFAFB'>" + QString::fromStdString("0x" + ALU::decToHex(i*16)) + "<\font>";
        out += QString::fromStdString(" | ");
        for(int j = 0 ; j < 16; j++){
            if((i * 16 + j) == change || (i * 16 + j) == change + 1){
                out += "<font style='color:#1391DB;'>" + QString::fromStdString(memo.getCell(i * 16 + j)) ;
                if (j!=15) {
                    out += "<font style='color:#FAFAFB;'> | </font>";
                }
            } else if((i * 16 + j) == wait || (i * 16 + j) == wait + 1){
                out += "<font style='color:#ffd91e;'>" + QString::fromStdString(memo.getCell(i * 16 + j));
                if (j!=15) {
                    out += "<font style='color:#FAFAFB;'> | </font>";
                }
            } else{
                out += "<font style='color:#4E9A06;'>" + QString::fromStdString(memo.getCell(i * 16 + j)) ;
                if (j!=15) {
                    out += "<font style='color:#FAFAFB;'> | </font>";
                }
            }
        }
        out += "<br><div style='line-height: 0.48;'><br></div>";
    }
    ui->memoryWindow->setText(out);
}

void MainWindow::printPCIR(CPU& cp){
    string output = "PC: 0x" + ALU::decToHex(cp.getPC());
    output += "\nIR: " + cp.getIR();

    ui->VariablesWindow->setText(QString::fromStdString(output));
}

void MainWindow::speed1(){
    ui->Speed1->setStyleSheet("/* Normal state */QPushButton {background-color: #2E95C1;    color: #FFFFFF;/* Text color */	border-radius: 2px;	font: 9pt 'Retro Gaming';background-repeat: no-repeat; /* Ensures the image is not repeated */background-size: contain;/* Adjusts the size of the image */  background-position: center;  /* Center the image */}QPushButton:hover {	background-color: #69A4BE;}QPushButton:pressed {	background-color: #0B2D3C;    color: #575757;/* Text color */padding-top: 2px; /* Moves content down */  padding-bottom: 2px;}QPushButton .text {    font-size: 10px;    margin-left: 5px;}");
    ui->Speed025->setStyleSheet("/* Normal state */QPushButton {background-color: rgb(57, 117, 0);    color: #FFFFFF;/* Text color */	border-radius: 2px;	font: 9pt 'Retro Gaming';background-repeat: no-repeat; /* Ensures the image is not repeated */background-size: contain;/* Adjusts the size of the image */  background-position: center;  /* Center the image */}QPushButton:hover {	background-color: rgb(118, 142, 96);}QPushButton:pressed {	background-color: rgb(25, 44, 8);    color: #575757;/* Text color */padding-top: 2px; /* Moves content down */  padding-bottom: 2px;}QPushButton .text {    font-size: 10px;    margin-left: 5px;}");
    ui->Speed05->setStyleSheet("/* Normal state */QPushButton {background-color: rgb(57, 117, 0);    color: #FFFFFF;/* Text color */	border-radius: 2px;	font: 9pt 'Retro Gaming';background-repeat: no-repeat; /* Ensures the image is not repeated */background-size: contain;/* Adjusts the size of the image */  background-position: center;  /* Center the image */}QPushButton:hover {	background-color: rgb(118, 142, 96);}QPushButton:pressed {	background-color: rgb(25, 44, 8);    color: #575757;/* Text color */padding-top: 2px; /* Moves content down */  padding-bottom: 2px;}QPushButton .text {    font-size: 10px;    margin-left: 5px;}");
    ui->Speed2->setStyleSheet("/* Normal state */QPushButton {background-color: rgb(57, 117, 0);    color: #FFFFFF;/* Text color */	border-radius: 2px;	font: 9pt 'Retro Gaming';background-repeat: no-repeat; /* Ensures the image is not repeated */background-size: contain;/* Adjusts the size of the image */  background-position: center;  /* Center the image */}QPushButton:hover {	background-color: rgb(118, 142, 96);}QPushButton:pressed {	background-color: rgb(25, 44, 8);    color: #575757;/* Text color */padding-top: 2px; /* Moves content down */  padding-bottom: 2px;}QPushButton .text {    font-size: 10px;    margin-left: 5px;}");
    speedOption = 0;
}

void MainWindow::speed05(){
    ui->Speed05->setStyleSheet("/* Normal state */QPushButton {background-color: #2E95C1;    color: #FFFFFF;/* Text color */	border-radius: 2px;	font: 9pt 'Retro Gaming';background-repeat: no-repeat; /* Ensures the image is not repeated */background-size: contain;/* Adjusts the size of the image */  background-position: center;  /* Center the image */}QPushButton:hover {	background-color: #69A4BE;}QPushButton:pressed {	background-color: #0B2D3C;    color: #575757;/* Text color */padding-top: 2px; /* Moves content down */  padding-bottom: 2px;}QPushButton .text {    font-size: 10px;    margin-left: 5px;}");
    ui->Speed025->setStyleSheet("/* Normal state */QPushButton {background-color: rgb(57, 117, 0);    color: #FFFFFF;/* Text color */	border-radius: 2px;	font: 9pt 'Retro Gaming';background-repeat: no-repeat; /* Ensures the image is not repeated */background-size: contain;/* Adjusts the size of the image */  background-position: center;  /* Center the image */}QPushButton:hover {	background-color: rgb(118, 142, 96);}QPushButton:pressed {	background-color: rgb(25, 44, 8);    color: #575757;/* Text color */padding-top: 2px; /* Moves content down */  padding-bottom: 2px;}QPushButton .text {    font-size: 10px;    margin-left: 5px;}");
    ui->Speed1->setStyleSheet("/* Normal state */QPushButton {background-color: rgb(57, 117, 0);    color: #FFFFFF;/* Text color */	border-radius: 2px;	font: 9pt 'Retro Gaming';background-repeat: no-repeat; /* Ensures the image is not repeated */background-size: contain;/* Adjusts the size of the image */  background-position: center;  /* Center the image */}QPushButton:hover {	background-color: rgb(118, 142, 96);}QPushButton:pressed {	background-color: rgb(25, 44, 8);    color: #575757;/* Text color */padding-top: 2px; /* Moves content down */  padding-bottom: 2px;}QPushButton .text {    font-size: 10px;    margin-left: 5px;}");
    ui->Speed2->setStyleSheet("/* Normal state */QPushButton {background-color: rgb(57, 117, 0);    color: #FFFFFF;/* Text color */	border-radius: 2px;	font: 9pt 'Retro Gaming';background-repeat: no-repeat; /* Ensures the image is not repeated */background-size: contain;/* Adjusts the size of the image */  background-position: center;  /* Center the image */}QPushButton:hover {	background-color: rgb(118, 142, 96);}QPushButton:pressed {	background-color: rgb(25, 44, 8);    color: #575757;/* Text color */padding-top: 2px; /* Moves content down */  padding-bottom: 2px;}QPushButton .text {    font-size: 10px;    margin-left: 5px;}");
    speedOption = 1;
}

void MainWindow::speed025(){
    ui->Speed025->setStyleSheet("/* Normal state */QPushButton {background-color: #2E95C1;    color: #FFFFFF;/* Text color */	border-radius: 2px;	font: 9pt 'Retro Gaming';background-repeat: no-repeat; /* Ensures the image is not repeated */background-size: contain;/* Adjusts the size of the image */  background-position: center;  /* Center the image */}QPushButton:hover {	background-color: #69A4BE;}QPushButton:pressed {	background-color: #0B2D3C;    color: #575757;/* Text color */padding-top: 2px; /* Moves content down */  padding-bottom: 2px;}QPushButton .text {    font-size: 10px;    margin-left: 5px;}");
    ui->Speed1->setStyleSheet("/* Normal state */QPushButton {background-color: rgb(57, 117, 0);    color: #FFFFFF;/* Text color */	border-radius: 2px;	font: 9pt 'Retro Gaming';background-repeat: no-repeat; /* Ensures the image is not repeated */background-size: contain;/* Adjusts the size of the image */  background-position: center;  /* Center the image */}QPushButton:hover {	background-color: rgb(118, 142, 96);}QPushButton:pressed {	background-color: rgb(25, 44, 8);    color: #575757;/* Text color */padding-top: 2px; /* Moves content down */  padding-bottom: 2px;}QPushButton .text {    font-size: 10px;    margin-left: 5px;}");
    ui->Speed05->setStyleSheet("/* Normal state */QPushButton {background-color: rgb(57, 117, 0);    color: #FFFFFF;/* Text color */	border-radius: 2px;	font: 9pt 'Retro Gaming';background-repeat: no-repeat; /* Ensures the image is not repeated */background-size: contain;/* Adjusts the size of the image */  background-position: center;  /* Center the image */}QPushButton:hover {	background-color: rgb(118, 142, 96);}QPushButton:pressed {	background-color: rgb(25, 44, 8);    color: #575757;/* Text color */padding-top: 2px; /* Moves content down */  padding-bottom: 2px;}QPushButton .text {    font-size: 10px;    margin-left: 5px;}");
    ui->Speed2->setStyleSheet("/* Normal state */QPushButton {background-color: rgb(57, 117, 0);    color: #FFFFFF;/* Text color */	border-radius: 2px;	font: 9pt 'Retro Gaming';background-repeat: no-repeat; /* Ensures the image is not repeated */background-size: contain;/* Adjusts the size of the image */  background-position: center;  /* Center the image */}QPushButton:hover {	background-color: rgb(118, 142, 96);}QPushButton:pressed {	background-color: rgb(25, 44, 8);    color: #575757;/* Text color */padding-top: 2px; /* Moves content down */  padding-bottom: 2px;}QPushButton .text {    font-size: 10px;    margin-left: 5px;}");
    speedOption = 2;
}

void MainWindow::speed2(){
    ui->Speed2->setStyleSheet("/* Normal state */QPushButton {background-color: #2E95C1;    color: #FFFFFF;/* Text color */	border-radius: 2px;	font: 9pt 'Retro Gaming';background-repeat: no-repeat; /* Ensures the image is not repeated */background-size: contain;/* Adjusts the size of the image */  background-position: center;  /* Center the image */}QPushButton:hover {	background-color: #69A4BE;}QPushButton:pressed {	background-color: #0B2D3C;    color: #575757;/* Text color */padding-top: 2px; /* Moves content down */  padding-bottom: 2px;}QPushButton .text {    font-size: 10px;    margin-left: 5px;}");
    ui->Speed025->setStyleSheet("/* Normal state */QPushButton {background-color: rgb(57, 117, 0);    color: #FFFFFF;/* Text color */	border-radius: 2px;	font: 9pt 'Retro Gaming';background-repeat: no-repeat; /* Ensures the image is not repeated */background-size: contain;/* Adjusts the size of the image */  background-position: center;  /* Center the image */}QPushButton:hover {	background-color: rgb(118, 142, 96);}QPushButton:pressed {	background-color: rgb(25, 44, 8);    color: #575757;/* Text color */padding-top: 2px; /* Moves content down */  padding-bottom: 2px;}QPushButton .text {    font-size: 10px;    margin-left: 5px;}");
    ui->Speed05->setStyleSheet("/* Normal state */QPushButton {background-color: rgb(57, 117, 0);    color: #FFFFFF;/* Text color */	border-radius: 2px;	font: 9pt 'Retro Gaming';background-repeat: no-repeat; /* Ensures the image is not repeated */background-size: contain;/* Adjusts the size of the image */  background-position: center;  /* Center the image */}QPushButton:hover {	background-color: rgb(118, 142, 96);}QPushButton:pressed {	background-color: rgb(25, 44, 8);    color: #575757;/* Text color */padding-top: 2px; /* Moves content down */  padding-bottom: 2px;}QPushButton .text {    font-size: 10px;    margin-left: 5px;}");
    ui->Speed1->setStyleSheet("/* Normal state */QPushButton {background-color: rgb(57, 117, 0);    color: #FFFFFF;/* Text color */	border-radius: 2px;	font: 9pt 'Retro Gaming';background-repeat: no-repeat; /* Ensures the image is not repeated */background-size: contain;/* Adjusts the size of the image */  background-position: center;  /* Center the image */}QPushButton:hover {	background-color: rgb(118, 142, 96);}QPushButton:pressed {	background-color: rgb(25, 44, 8);    color: #575757;/* Text color */padding-top: 2px; /* Moves content down */  padding-bottom: 2px;}QPushButton .text {    font-size: 10px;    margin-left: 5px;}");
    speedOption = 3;
}

void MainWindow::PlayButton(bool playing) {
    if (playing) {
        ui->PlayButton->setStyleSheet("QPushButton { background-image: url(:/new/Butoon/resources/Play all_Pause.png); border-radius: 1px; background-repeat: no-repeat; background-size: contain; background-position: center; } QPushButton:hover { background-image: url(:/new/Button_H/resources/Play all_h.png); } QPushButton:pressed { background-image: url(:/new/Button_P/resources/Play all_p.png); }");
    } else {
        ui->PlayButton->setStyleSheet("QPushButton { background-image: url(:/new/Butoon/resources/Play all_Play.png); border-radius: 1px; background-repeat: no-repeat; background-size: contain; background-position: center; } QPushButton:hover { background-image: url(:/new/Button_H/resources/Play all_h.png); } QPushButton:pressed { background-image: url(:/new/Button_P/resources/Play all_p.png); }");
    }
}
