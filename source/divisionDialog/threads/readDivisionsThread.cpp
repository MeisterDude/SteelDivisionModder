
#include <QDebug>
#include <QMutex>
#include <filesystem>
#include <fstream>

#include "readDivisionsThread.h"
//#include "../../../constants.h"


ReadDivisionsThread::ReadDivisionsThread(QObject *parent) : QThread{parent} {}
ReadDivisionsThread::~ReadDivisionsThread() {}

void ReadDivisionsThread::run() {
    mutex.lock();
    std::filesystem::path path = this->path;
    mutex.unlock();

    readDivisions(path);
    
    analizeContent();
}

void ReadDivisionsThread::setFilePath(std::filesystem::path &path) {
    mutex.lock();
    this->path = path;
    mutex.unlock();
}

void ReadDivisionsThread::readDivisions(std::filesystem::path &path) {
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
            std::string divisionName = line.substr(32, line.find(" ", 8) - 32);
            if(divisionName.find("multi") != std::string::npos) {

                sendDivision(divisionName);
                this->divisionContent[divisionName] = QMap<std::string, std::vector<std::string>>();
            
                getline(file, line);
                while(getline(file, line)) {
                    if(line == ")") {
                        break;
                    }
                    if(line.find("MAP") != std::string::npos || line.find("=") == line.length() - 1 || line.find("[") == line.length() - 1) {
                        readDivisionsHelper(file, divisionName, line);
                    
                    } else {
                        this->divisionContent[divisionName][line] = std::vector<std::string>{line};
                    }
                } 
                
            } else {
                while(getline(file, line)) {
                    if(line == ")") {
                        break;
                    }
                }
            }
        } else {
            qDebug() << "unknown: " << line;
        }
    }
    emit sendDivisions(this->divisionContent);
}

void ReadDivisionsThread::readDivisionsHelper(std::ifstream &file, const std::string &name, const std::string &tmp) {
    this->divisionContent[name][tmp] = std::vector<std::string>();
    std::string line;
    while(getline(file, line)) {
        this->divisionContent[name][tmp].push_back(line);
        if(line == "    ]") {
            break;
        }
    }
}

void ReadDivisionsThread::analizeContent() {
    int max{0}, min{INT_MAX};
    std::map<std::string, int> count;
    std::map<std::string, int> units;
    int unitCount{0}, maxUnit{0};
    for(auto &division: this->divisionContent) {
        //qDebug() << division.first;
        if(division.size() < min) {
            min = division.size();
        }
        if(division.size() > max) {
            max = division.size();
        }

        for(auto [key, item]: division.asKeyValueRange()) {
            std::string str = key.substr(0, key.find("="));
            if(count.find(str) == count.end()) {
                count[str] = 1;
            } else {
                count[str]++;
            }
            //if(item.first.find("DivisionTags") != std::string::npos) {
            //    qDebug() << item.second[0];
            //}

            if(key == "    PackList = MAP") {
                unitCount += item.size() - 2;
                for(auto &unit: item) {
                    if(unit != "    [" && unit != "    ]") {
                        std::string unitName = unit.substr(unit.find(key) + key.length() + 1);
                        unitName = unitName.substr(0, unitName.find(","));
                        if(units.find(unitName) == units.end()) {
                            units[unitName] = 1;
                        } else {
                            units[unitName]++;
                        }
                    }
                }
            }
        }
    }
    //qDebug() << min << " : " << max << " : " << count.size();
    //for(auto &item: count) {
        //qDebug() << item;
    //}
    for(auto &unit: units) {
        //qDebug() << unit;
        if(unit.second > maxUnit) {
            maxUnit = unit.second;
        }
    }
    qDebug() << "Divisions: " << maxUnit << " : " << units.size() << " : " << unitCount;
}