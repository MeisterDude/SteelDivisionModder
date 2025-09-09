
#include <QDebug>
#include <QProcess>
#include <QFileDialog>
#include <filesystem>
//#include <iostream>
#include <fstream>
//#include <stdlib.h>

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "constants.h"


SDStuffWidget::SDStuffWidget(QWidget *parent): QWidget(parent), ui(new Ui::SDStuffWidget), modPath(""), isRepairModded(false), supplyMultiplier(1) {
    ui->setupUi(this);
    //connect(ui->countryComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(changeLabels()));
    connect(ui->createModButton, SIGNAL(clicked()), this, SLOT(createNewMod()));
    connect(ui->openModButton, SIGNAL(clicked()), this, SLOT(openMod()));
    connect(ui->repairButton, SIGNAL(clicked()), this, SLOT(switchRepair()));
    fillCCB();
    setSupplyLabel();
}

SDStuffWidget::~SDStuffWidget() {
    delete ui;
}



void SDStuffWidget::fillCCB() {
    const QString countryText[] = {"Germany", "USSR", "France"};
    const QString countryStuff[] = {"ABC", "123", "abc"};
    for(int i = 0; i < 3; i++) {
        //QStringWidgetItem
        //QString text
        ui->countryComboBox->insertItem(i, countryText[i], countryStuff[i]);
    }
}

void SDStuffWidget::readRavitaillement() {
    std::string path = this->modPath.toStdString() + RAVITAILLEMENT;
    //qDebug() << QString::fromStdString(path);
    if(std::filesystem::exists(path)) {
        std::ifstream file(path);
        if (file.is_open()) {
            std::string line;
            while(getline(file, line)) {
                //qDebug() << QString::fromStdString(line);
                if(line == "    HealthSuppliableMovingTypes = [") {
                    //qDebug() << "Found";
                    getline(file, line);
                    if(line == "        //EUnitMovingType/Track") {
                        this->isRepairModded = false;
                    } else if(line == "EUnitMovingType/Infantry,") {
                        this->isRepairModded = true;
                    } else {
                        qDebug() << "Unknown line after HealthSuppliableMovingTypes = [: " << QString::fromStdString(line);
                    }
                }
            }
            //qDebug() << "Ravitaillement file end";
        } else {
            qDebug() << "Could not open Ravitaillement.ndf file!";
        }
        file.close();
    } else {
        qDebug() << "Ravitaillement.ndf file not found!";
    }
}

void SDStuffWidget::setRepairLabel() {
    if(this->isRepairModded) {
        ui->repairLabel->setText("Repair modded");
    } else {
        ui->repairLabel->setText("Repair not modded");
    }
}

void SDStuffWidget::readUniteDescriptor() {
    std::string path = this->modPath.toStdString() + UNITEDESCRIPTOR;;
    if(std::filesystem::exists(path)) {
        std::ifstream file(path);
        if (file.is_open()) {
            std::string line;
            //bool found_5k{false}, found_10k{false}, found_15k{false};
            int high{0}, mid{0}, low{INT_MAX};
            while(getline(file, line)) {
                //qDebug() << QString::fromStdString(line);
                if(line.find("SupplyCapacity") != std::string::npos) {
                    int found(stoi(line.substr(line.find("=") + 1)));
                    //qDebug() << "found";
                    //qDebug() << found;
                    if(found > high) {
                        high = found;
                    }
                    if(found < low) {
                        low = found;
                    }
                    if(found != high && found != low) {
                        if(found != mid and mid != 0) {
                            qDebug() << "Found fourth supply value: " << found;
                            continue;
                        } else {
                            mid = found;
                        }
                    }
                }
            }
            //qDebug() << "high:" << high << " mid:" << mid << " low:" << low;
            this->supplyMultiplier = mid / 10000;
        } else {
            qDebug() << "Could not open UniteDescriptor.ndf file!";
        }
        file.close();
        //setSupplyLabel();
    } else {
        qDebug() << "UniteDescriptor.ndf file not found!";
    }
}

void SDStuffWidget::setSupplyLabel() {
    ui->supplyLabel->setText(QString("Supply multiplier: ").append(std::to_string(this->supplyMultiplier)));
}

// Slots

//void SDStuffWidget::changeLabels() {
//    ui->label1->setText(ui->countryComboBox->currentText());
//    ui->label2->setText(ui->countryComboBox->currentData().toString());
//}

int SDStuffWidget::createNewMod() {
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
}

void SDStuffWidget::openMod() {
    //QString modName = QFileDialog::getOpenFileName(this, tr("Select Mod"), QString::fromStdString(GAMEPATH + MODSPATH), "Folder (.);;Files (*.*)");
    this->modPath = QFileDialog::getExistingDirectory(this, tr("Select Mod"), QString::fromStdString(GAMEPATH + MODSFOLDER), QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    ui->modNameLabel->setText(this->modPath);
    readRavitaillement();
    setRepairLabel();
    readUniteDescriptor();
    setSupplyLabel();
}

void SDStuffWidget::switchRepair() {
    std::string path = this->modPath.toStdString() + RAVITAILLEMENT;
    if(std::filesystem::exists(path)) {
        // Create backup, if not already existing
        std::string backupPath = path.substr(0, path.find_last_of(".")) + ".backup";
        if(!std::filesystem::exists(backupPath)) {
            std::filesystem::copy(path, backupPath);
            qDebug() << "Backup created at" << QString::fromStdString(backupPath);
        }

        std::ifstream file(path);
        std::vector<std::string> lines;
        //file.open(path);
        if(file.is_open()) {
            std::string line;
            while(getline(file, line)) {
                lines.push_back(line);
                if(line == "    HealthSuppliableMovingTypes = [") {
                    //qDebug() << "Found";
                    if(this->isRepairModded) {
                        lines.push_back("        //EUnitMovingType/Track");
                        this->isRepairModded = false;
                        setRepairLabel();
                        for(int i = 0; i < 8; i++) {
                            getline(file, line);
                        }
                    } else {
                        lines.push_back("        EUnitMovingType/Infantry,");
                        lines.push_back("        EUnitMovingType/InfantryAmphibious,");
                        lines.push_back("        EUnitMovingType/Track,");
                        lines.push_back("        EUnitMovingType/TrackAmphibious,");
                        lines.push_back("        EUnitMovingType/AllTerrainWheelAmphibious,");
                        lines.push_back("        EUnitMovingType/HalfTrack,");
                        lines.push_back("        EUnitMovingType/AllTerrainWheel,");
                        lines.push_back("        EUnitMovingType/Wheel,");
                        this->isRepairModded = true;
                        setRepairLabel();
                        getline(file, line);
                    }
                }
            }
        } else {
            qDebug() << "Could not open Ravitaillement.ndf file!";
        }
        file.close();

        // Write changes
        std::ofstream outfile(path, std::ios::trunc);
        if(outfile.is_open()) {
            for(const auto &l: lines) {
                outfile << l << "\n";
            }
        } else {
            qDebug() << "Could not open Ravitaillement.ndf file to write changes!";
        }
        outfile.close();

    } else {
        qDebug() << "Ravitaillement.ndf file not found!";
    }
}

void SDStuffWidget::setSupply() {
    
}