
#include <QDebug>
#include <QMutex>
#include <filesystem>
#include <fstream>

#include "writeSupplyThread.h"
#include "../../constants.h"


WriteSupplyThread::WriteSupplyThread(QObject *parent) : QThread{parent} {}

WriteSupplyThread::~WriteSupplyThread() {}


void WriteSupplyThread::run() {
    emit supplyWritten(0, true);
    QMutex mutex;
    mutex.lock();
    std::string path = this->modPath;
    int supplyMultiplier = this->supplyMultiplier;
    int oldMultiplier = this->oldMultiplier;
    mutex.unlock();
    writeSupply(path, supplyMultiplier, oldMultiplier);
    emit supplyWritten(supplyMultiplier);
    //qDebug() << "Supply writing finished.";
}

void WriteSupplyThread::setModPath(const std::string &modPath) {
    QMutex mutex;
    mutex.lock();
    this->modPath = modPath;
    mutex.unlock();
}

void WriteSupplyThread::setsupplyMultiplier(const int supplyMultiplier, const int oldMultiplier) {
    QMutex mutex;
    mutex.lock();
    this->supplyMultiplier = supplyMultiplier;
    this->oldMultiplier = oldMultiplier;
    mutex.unlock();
}

void WriteSupplyThread::writeSupply(const std::string &modPath, const int multiplier, const int old) {
    std::string path = modPath + UNITEDESCRIPTOR;
    if(std::filesystem::exists(path)) {
        // Create backup, if not already existing
        std::string backupPath = path.substr(0, path.find_last_of(".")) + ".backup";
        if(!std::filesystem::exists(backupPath)) {
            std::filesystem::copy(path, backupPath);
            qDebug() << "Backup created at" << QString::fromStdString(backupPath);
        }

        // Read file, find and edit supply
        std::ifstream file(path);
        std::vector<std::string> lines;
        //qDebug() << old << " : " << multiplier;
        if(file.is_open()) {
            std::string line;
            while(getline(file, line)) {
                if(line.find("SupplyCapacity") != std::string::npos) {
                    int found(stoi(line.substr(line.find("=") + 1)));
                    //qDebug() << found << " : " << (found / old) * multiplier;
                    line = line.substr(0, line.find("=") + 1) + " " + std::to_string((found / old) * multiplier) + ".0";
                }
                lines.push_back(line);
            }
        } else {
            qDebug() << "Could not open UniteDescriptor.ndf file!";
        }
        file.close();

        // Write changes
        std::ofstream outfile(path, std::ios::trunc);
        if(outfile.is_open()) {
            for(const auto &l: lines) {
                outfile << l << "\n";
            }
        } else {
            qDebug() << "Could not open UniteDescriptor.ndf file to write changes!";
        }
        outfile.close();

    } else {
        qDebug() << "UniteDescriptor.ndf file not found!";
    }
}