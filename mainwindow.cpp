
#include <QDebug>
#include <QFileDialog>
#include <QtConcurrent/QtConcurrent>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "constants.h"


SDStuffWidget::SDStuffWidget(QWidget *parent): QWidget(parent), ui(new Ui::SDStuffWidget), modPath(""), isRepairModded(false) {
    this->ui->setupUi(this);

    this->rst = new ReadSupplyThread(this);
    connect(rst, SIGNAL(supplyRead(int, bool)), this, SLOT(setSupplyLabel(int, bool)));

    this->rrt = new ReadRepairThread(this);
    connect(rrt, SIGNAL(repairRead(bool, bool)), this, SLOT(setRepairLabel(bool, bool)));

    this->wst = new WriteSupplyThread(this);
    connect(wst, SIGNAL(supplyWritten(int, bool)), this, SLOT(setSupplyLabel(int, bool)));
    
    this->wrt = new WriteRepairThread(this);
    connect(wrt, SIGNAL(repairWritten(bool, bool)), this, SLOT(setRepairLabel(bool, bool)));

    //connect(ui->createModButton, SIGNAL(clicked()), this, SLOT(createNewMod()));
    connect(this->ui->openModButton, SIGNAL(clicked()), this, SLOT(openMod()));
    connect(this->ui->supplyButton, SIGNAL(clicked()), this, SLOT(setSupply()));
    connect(ui->repairButton, SIGNAL(clicked()), this, SLOT(switchRepair()));
    
    //fillCCB();
    //setSupplyLabel();
}

SDStuffWidget::~SDStuffWidget() {
    delete this->ui;
    delete this->rst;
    delete this->rrt;
    delete this->wst;
    delete this->wrt;
}



/*void SDStuffWidget::fillCCB() {
    const QString countryText[] = {"Germany", "USSR", "France"};
    const QString countryStuff[] = {"ABC", "123", "abc"};
    for(int i = 0; i < 3; i++) {
        //QStringWidgetItem
        //QString text
        ui->countryComboBox->insertItem(i, countryText[i], countryStuff[i]);
    }
}*/

void SDStuffWidget::openMod() {
    this->modPath = QFileDialog::getExistingDirectory(this, tr("Select Mod"), QString::fromStdString(GAMEPATH + MODSFOLDER), QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    this->ui->modNameLabel->setText(this->modPath);
    
    this->rst->setModPath(this->modPath.toStdString());;
    this->rst->start();
    this->rrt->setModPath(this->modPath.toStdString());
    this->rrt->start();
}

void SDStuffWidget::setSupply() {
    bool ok;
    int newMultiplier = this->ui->SupplyLineEdit->text().toInt(&ok);
    if(!ok || newMultiplier < 1) {
        qDebug() << "Invalid supply multiplier:" << this->ui->SupplyLineEdit->text();
        return;
    }
    this->wst->setModPath(this->modPath.toStdString());
    this->wst->setsupplyMultiplier(newMultiplier, this->supplyMultiplier);
    this->wst->start();
    //this->supplyMultiplier = newMultiplier;
}

void SDStuffWidget::switchRepair() {
    this->wrt->setModPath(this->modPath.toStdString());
    this->wrt->setIsRepairModded(this->isRepairModded);
    this->wrt->start();
}

void SDStuffWidget::setSupplyLabel(const int supplyMultiplier, const bool running) {
    qDebug() << "setSupplyLabel called with" << supplyMultiplier << running;
    if(running) {
        this->ui->supplyLabel->setText("Working...");
        return;
    }
    this->supplyMultiplier = supplyMultiplier;
    this->ui->supplyLabel->setText(QString("Supply multiplier: ").append(std::to_string(this->supplyMultiplier)));
}

void SDStuffWidget::setRepairLabel(const bool isRepairModded, const bool running) {
    if(running) {
        this->ui->repairLabel->setText("Working...");
        return;
    }
    this->isRepairModded = isRepairModded;
    if(isRepairModded) {
        this->ui->repairLabel->setText("Repair modded");
    } else {
        this->ui->repairLabel->setText("Repair not modded");
    }
}

/*int SDStuffWidget::createNewMod() {
    QString batchPath = QString::fromStdString(GAMEPATH + MODSFOLDER + CREATEBATCH);
    QString modName = ui->modNameEdit->text();
    QString command = QString("\"%1\" %2").arg(batchPath, modName);
    //ui->modNameLabel->setText(QString::fromStdString(batchPath));
    qDebug().noquote() << "Executing:" << command;

    QProcess process;
    process.start(command);
    //process.start("cmd.exe", QStringList() << "/c" << command);
    process.waitForFinished();

    QString output = process.readAllStandardOutput();
    QString error = process.readAllStandardError();
    qDebug() << "Output:" << output;
    qDebug() << "Error:" << error;

    return process.exitCode();
}*/






