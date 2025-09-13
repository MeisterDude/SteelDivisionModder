
#include <QDebug>
#include <QMutex>
#include <filesystem>
#include <fstream>

#include "readPointsThread.h"
#include "../../constants.h"


ReadPointsThread::ReadPointsThread(QObject *parent) : QThread{parent} {}

ReadPointsThread::~ReadPointsThread() {}

void ReadPointsThread::run() {
    emit pointsRead(false, true);
    QMutex mutex;
    mutex.lock();
    std::string path = this->modPath;
    mutex.unlock();
    //int points = readPoints(path);
    emit pointsRead(readPoints(path));
}

void ReadPointsThread::setModPath(const std::string &modPath) {
    QMutex mutex;
    mutex.lock();
    this->modPath = modPath;
    mutex.unlock();
}

bool ReadPointsThread::readPoints(const std::string &modPath) {
    std::string path = modPath + GDCONSTANTES;
    if(std::filesystem::exists(path)) {
        std::ifstream file(path);
        if(file.is_open()) {
            std::string line;
            while(getline(file, line)) {
                if(line == "    ConquestPossibleScores =") {
                    getline(file, line);
                    getline(file, line);
                    getline(file, line);
                    getline(file, line);
                    getline(file, line);
                    if(line == "    ]") {
                        return false;
                    }
                    return true;
                }
            }
        } else {
            qDebug() << "Could not open GDConstates.ndf file!";
        }
    } else {
        qDebug() << "GDConstates.ndf file not found!";
    }
    return false;
}