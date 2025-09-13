
#include <QDebug>
#include <QMutex>
#include <filesystem>
#include <fstream>

#include "writeRepairThread.h"
#include "../../constants.h"


WriteRepairThread::WriteRepairThread(QObject *parent) : QThread{parent} {}

WriteRepairThread::~WriteRepairThread() {}


void WriteRepairThread::run() {
    emit repairWritten(false, true);
    QMutex mutex;
    mutex.lock();
    std::string path = this->modPath;
    bool isRepairModded = this->isRepairmodded;
    mutex.unlock();
    emit repairWritten(writeRepair(path, isRepairModded));
}

void WriteRepairThread::setModPath(const std::string &modPath) {
    QMutex mutex;
    mutex.lock();
    this->modPath = modPath;
    mutex.unlock();
}

void WriteRepairThread::setIsRepairModded(const bool isRepairModded) {
    QMutex mutex;
    mutex.lock();
    this->isRepairmodded = isRepairModded;
    mutex.unlock();
}

bool WriteRepairThread::writeRepair(const std::string &modPath, bool isRepairModded) {
    std::string path = modPath + RAVITAILLEMENT;
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
                    if(isRepairModded) {
                        lines.push_back("        //EUnitMovingType/Track");
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
    return !isRepairModded;
}