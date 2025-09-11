
#include <QDebug>
#include <QMutex>
#include <filesystem>
#include <fstream>

#include "readRepairThread.h"
#include "constants.h"


ReadRepairThread::ReadRepairThread(QObject *parent, const std::string modPath) : QThread{parent}, modPath{modPath} {}

ReadRepairThread::~ReadRepairThread() {}


void ReadRepairThread::run() {
    emit repairRead(false, true);
    QMutex mutex;
    mutex.lock();
    std::string path = this->modPath;
    mutex.unlock();
    emit repairRead(readRepair(path));
}

void ReadRepairThread::setModPath(const std::string &modPath) {
    QMutex mutex;
    mutex.lock();
    this->modPath = modPath;
    mutex.unlock();
}

bool ReadRepairThread::readRepair(const std::string &modPath) {
    std::string path = modPath + RAVITAILLEMENT;
    bool isRepairModded = false;
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
                        isRepairModded = false;
                    } else if(line == "        EUnitMovingType/Infantry,") {
                        isRepairModded = true;
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
    return isRepairModded;
}