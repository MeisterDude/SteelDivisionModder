
#include <QListWidgetItem>

#include "divisionDialog.h"
#include "ui_divisionDialog.h"
#include "../../constants.h"


DivisionDialog::DivisionDialog(QWidget *parent, std::filesystem::path path): QDialog(parent), path{path}, ui(new Ui::DivisionDialog) {
    this->ui->setupUi(this);

    this->rdt = new ReadDivisionsThread(this);
    this->rdt->setFilePath(std::filesystem::path{this->path}.concat(DIVISIONS));
    connect(this->rdt, SIGNAL(sendDivision(std::string)), this, SLOT(addDivision(std::string)));
    connect(this->rdt, SIGNAL(sendDivisions(QMap<std::string, QMap<std::string, std::vector<std::string>>>)), this, SLOT(setDivisions(QMap<std::string, QMap<std::string, std::vector<std::string>>>)));
    connect(this->rdt, SIGNAL(finished()), this, SLOT(readDivisionsDone()));
    this->rdt->start();

    this->rdrt = new ReadDivisionRulesThread(this);
    this->rdrt->setFilePath(std::filesystem::path{this->path}.concat(DIVISIONRULES));
    connect(this->rdrt, SIGNAL(sendDivisionRules(QMap<std::string, QMap<std::string, QMap<std::string, QMap<std::string, std::string>>>>)), this, SLOT(setDivisionRules(QMap<std::string, QMap<std::string, QMap<std::string, QMap<std::string, std::string>>>>)));
    connect(this->rdrt, SIGNAL(finished()), this, SLOT(readDivisionRulesDone()));
    this->rdrt->start();

    connect(this->ui->analyzeButton, SIGNAL(clicked()), this, SLOT(analyzeStuff()));
}

DivisionDialog::~DivisionDialog() {
    delete this->ui;
    delete this->rdt;
    delete this->rdrt;
}

void DivisionDialog::addDivision(std::string division) {
    mutex.lock();
    //this->divisionList.push_back(division);
    QListWidgetItem *item = new QListWidgetItem(tr(division.c_str()), this->ui->divisionListWidget);
    //qDebug() << this->divisionList.size() << " : " << this->ui->divisionListWidget->count();
    //this->ui->divisionListWidget->setCurrentItem(item);
    mutex.unlock();
}

void DivisionDialog::setDivisions(QMap<std::string, QMap<std::string, std::vector<std::string>>> divisions) {
    this->mutex.lock();
    this->divisions = divisions;
    this->mutex.unlock();
    //this->divisions.detach();
}

void DivisionDialog::readDivisionsDone() {
    this->ui->divisionLabel->setText("Divisions done...");
}

void DivisionDialog::setDivisionRules(QMap<std::string, QMap<std::string, QMap<std::string, QMap<std::string, std::string>>>> divisionRules) {
    this->mutex.lock();
    this->divisionRules = divisionRules;
    this->mutex.unlock();
    //this->divisionRules.detach();
}

void DivisionDialog::readDivisionRulesDone() {
    this->ui->divisionRulesLabel->setText("DivisionsRules done...");
}

void DivisionDialog::analyzeStuff() {
    for(auto [divisionName, division]: this->divisions.asKeyValueRange()) {
        if(this->divisionRules.contains(divisionName)) {
            qDebug() << divisionName << " found";
        } else {
             qDebug() << divisionName << " not found";
        }
    }
    //for(auto [divisionName, division]: this->divisionRules.asKeyValueRange()) {
    //    qDebug() << divisionName;
    //}
}