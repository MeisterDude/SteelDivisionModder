
#include <QDebug>
#include <QMutex>
#include <filesystem>
#include <fstream>

#include "readSupplyThread.h"
#include "constants.h"


ReadSupplyThread::ReadSupplyThread(QObject *parent, const std::string modPath) : QThread{parent}, modPath{modPath} {}

ReadSupplyThread::~ReadSupplyThread() {}


void ReadSupplyThread::run() {
    emit supplyRead(0, true);
    QMutex mutex;
    mutex.lock();
    std::string path = this->modPath;
    mutex.unlock();
    emit supplyRead(readSupply(path));
}

void ReadSupplyThread::setModPath(const std::string &modPath) {
    QMutex mutex;
    mutex.lock();
    this->modPath = modPath;
    mutex.unlock();
}

int ReadSupplyThread::readSupply(const std::string &modPath) {
    std::string path = modPath + UNITEDESCRIPTOR;;
    if(std::filesystem::exists(path)) {
        std::ifstream file(path);
        int high{0}, mid{0}, low{INT_MAX};
        if (file.is_open()) {
            std::string line;
            while(getline(file, line)) {
                //qDebug() << QString::fromStdString(line);
                if(line.find("SupplyCapacity") != std::string::npos) {
                    int found(stoi(line.substr(line.find("=") + 1)));
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
        } else {
            qDebug() << "Could not open UniteDescriptor.ndf file!";
        }
        file.close();
        return mid / 10000;
    } else {
        qDebug() << "UniteDescriptor.ndf file not found!";
        return 0;
    }
}