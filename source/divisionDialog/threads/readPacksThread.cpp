
#include <QDebug>
#include <QMutex>
#include <filesystem>
#include <fstream>

#include "readPacksThread.h"


ReadPacksThread::ReadPacksThread(QObject *parent) : QThread{parent} {}
ReadPacksThread::~ReadPacksThread() {}

void ReadPacksThread::run() {
    mutex.lock();
    std::filesystem::path path = this->path;
    mutex.unlock();

    //readDivisions(path);
    //int x{0};
    //analizeContent();
}

void ReadPacksThread::setFilePath(std::filesystem::path &path) {
    mutex.lock();
    this->path = path;
    mutex.unlock();
}

void ReadPacksThread::readPacks(std::filesystem::path &path) {
    if(!std::filesystem::exists(path)) {
        qDebug() << "File not found:" << QString::fromStdString(path.string());
        return;
    }
    std::ifstream file(path);
    if(!file.is_open()) {
        qDebug() << "Could not open:" << QString::fromStdString(path.string());
        return;
    }

    std::string line;
    while(getline(file, line)) {
        if(line.find("export") != std::string::npos) {
            
        }
    }
}
