
#include <QDebug>
#include <QMutex>
#include <filesystem>
#include <fstream>

#include "writePointsThread.h"
#include "../../constants.h"


WritePointsThread::WritePointsThread(QObject *parent) : QThread{parent} {}

WritePointsThread::~WritePointsThread() {}

void WritePointsThread::run() {
    emit pointsWritten(false, true);
    QMutex mutex;
    mutex.lock();
    std::string path = this->modPath;
    mutex.unlock();
    emit pointsWritten(writePoints(modPath, isPointsModded));
    //qDebug() << "Points writing finished.";
}

void WritePointsThread::setModPath(const std::string &modPath) {
    QMutex mutex;
    mutex.lock();
    this->modPath = modPath;
    mutex.unlock();
}

void WritePointsThread::setIsPointsModded(const bool isPointsModded) {
    QMutex mutex;
    mutex.lock();
    this->isPointsModded = isPointsModded;
    mutex.unlock();
}

bool WritePointsThread::writePoints(const std::string &modPath, const bool isPointsModded) {
    std::string path = modPath + GDCONSTANTES;
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
        if(file.is_open()) {
            std::string line;
            while(getline(file, line)) {
                if(line == "    ConquestPossibleScores =") {
                    lines.push_back(line);
                    getline(file, line);
                    lines.push_back(line);
                    getline(file, line);
                    lines.push_back(line);
                    getline(file, line);
                    lines.push_back(line);
                    getline(file, line);
                    lines.push_back(line);
                    if(isPointsModded) {
                        // Points are modded, remove them
                        while(line != "    ]") {
                            getline(file, line);
                        }
                        lines.push_back("    ]");
                    } else {
                        // Points are not modded, add them
                        lines.push_back("        (\"COHE_SUPE\", 8000),");
                        lines.push_back("        (\"COHE_ULTR\", 24000),");
                        lines.push_back("        (\"COHE_MAXI\", 64000),");
                        //lines.push_back("    ]");
                    }
                } else {
                    lines.push_back(line);
                }
            }
            file.close();

            // Write file
            std::ofstream outFile(path, std::ios::trunc);
            if(outFile.is_open()) {
                for(const auto &l : lines) {
                    outFile << l << "\n";
                }
                outFile.close();
            } else {
                qDebug() << "Could not open GDConstates.ndf for writing!";
            }
        } else {
            qDebug() << "Could not open GDConstates.nd file";
        }
    } else {
        qDebug() << "GDConstates.ndf file not found!";
    }

    path = modPath + LOCALISATION;
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
        if(file.is_open()) {
            std::string line;
            if(isPointsModded) {
                while(getline(file, line)) {
                    if(line != "\"COHE_SUPE\";\"COMMENT\";\"DO NOT TRANSLATE\";CONSTRAINT;\"Super\";\"CLASSEMENT\";\"ExisteDansSlayer\"" 
                    && line != "\"COHE_ULTR\";\"COMMENT\";\"DO NOT TRANSLATE\";CONSTRAINT;\"Ultra\";\"CLASSEMENT\";\"ExisteDansSlayer\""
                    && line != "\"COHE_MAXI\";\"COMMENT\";\"DO NOT TRANSLATE\";CONSTRAINT;\"Max\";\"CLASSEMENT\";\"ExisteDansSlayer\"") {
                        lines.push_back(line);
                    }   
                }
            } else {
                while(getline(file, line)) {
                    lines.push_back(line);
                }
                lines.push_back("\"COHE_SUPE\";\"COMMENT\";\"DO NOT TRANSLATE\";CONSTRAINT;\"Super\";\"CLASSEMENT\";\"ExisteDansSlayer\"");
                lines.push_back("\"COHE_ULTR\";\"COMMENT\";\"DO NOT TRANSLATE\";CONSTRAINT;\"Ultra\";\"CLASSEMENT\";\"ExisteDansSlayer\"");
                lines.push_back("\"COHE_MAXI\";\"COMMENT\";\"DO NOT TRANSLATE\";CONSTRAINT;\"Max\";\"CLASSEMENT\";\"ExisteDansSlayer\"");
            }
            file.close();

            // Write file
            std::ofstream outFile(path, std::ios::trunc);
            if(outFile.is_open()) {
                for(const auto &l : lines) {
                    outFile << l << "\n";
                }
                outFile.close();
            } else {
                qDebug() << "Could not open MODDING.csv for writing!";
            }
        } else {
            qDebug() << "Could not open MODDING.csv file";
        }
    } else {
        qDebug() << "MODDING.csv file not found!";
    }

    return !isPointsModded;
}

