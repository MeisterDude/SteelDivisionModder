
#include <QDebug>
#include <QMutex>
#include <filesystem>
#include <fstream>

#include "ReadDivisionRulesThread.h"


ReadDivisionRulesThread::ReadDivisionRulesThread(QObject *parent) : QThread{parent} {}
ReadDivisionRulesThread::~ReadDivisionRulesThread() {}


void ReadDivisionRulesThread::run() {
    this->mutex.lock();
    std::filesystem::path path = this->path;
    this->mutex.unlock();
    readDivisionRules(path);
}

void ReadDivisionRulesThread::setFilePath(std::filesystem::path &path) {
    this->mutex.lock();
    this->path = path;
    this->mutex.unlock();
}

void ReadDivisionRulesThread::readDivisionRules(std::filesystem::path &path) {
    if(!std::filesystem::exists(path)) {
        qDebug() << "File not found:" << QString::fromStdString(path.string());
        return;
    }
    std::ifstream file(path);
    if(!file.is_open()) {
        qDebug() << "Could not open:" << QString::fromStdString(path.string());
        return;
    }
    
    QMap<std::string, QMap<std::string, QMap<std::string, QMap<std::string, std::string>>>> divisionRules;
    std::string line;
    while(getline(file, line)) {
        if(line.find("Descriptor_Deck_Division_") != std::string::npos) {
            std::string divisionName = line.substr(39, line.length() - 40);
            divisionRules[divisionName] = QMap<std::string, QMap<std::string, QMap<std::string, std::string>>>();
            divisionRules[divisionName]["UnitRuleList"] = QMap<std::string, QMap<std::string, std::string>>();
            divisionRules[divisionName]["TransportRuleList"] = QMap<std::string, QMap<std::string, std::string>>();
            while(getline(file, line)) {
                if(line.find("UnitDescriptor =") != std::string::npos) {
                    std::string unitName = line.substr(59);
                    divisionRules[divisionName]["UnitRuleList"][unitName] = QMap<std::string, std::string>();
                    while(getline(file, line)) {
                        if(line == "                    ),") {
                            break;
                        }
                        divisionRules[divisionName]["UnitRuleList"][unitName][line.substr(24, line.find("=") - 25)] = line.substr(line.find("="));
                    }
                }
                if(line.find("TransportDescriptor =") != std::string::npos) {
                    std::string unitName = line.substr(64);
                    divisionRules[divisionName]["TransportRuleList"][unitName] = QMap<std::string, std::string>();
                    while(getline(file, line)) {
                        if(line == "                    ),") {
                            break;
                        }
                        divisionRules[divisionName]["TransportRuleList"][unitName][line.substr(24, line.find("=") - 25)] = line.substr(line.find("="));
                    }
                }
                if(line == "        ),") {
                    break;
                }
            }
        }
    }
    emit sendDivisionRules(divisionRules);
    /*for(auto& division: divisionRules) {
        qDebug() << "----------------" << division.first << "------------------";
        for(auto& unit: division.second["UnitRuleList"]) {
            for(auto& content: unit.second) {
                qDebug() << content;
            }
        }
        for(auto& unit: division.second["TransportRuleList"]) {
            for(auto& content: unit.second) {
                qDebug() << content;
            }
        }
    }*/
}

//void readDivisionRulesThread::analyzeContent() {

//}