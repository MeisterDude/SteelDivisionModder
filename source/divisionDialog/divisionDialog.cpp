
#include <QListWidgetItem>

#include "divisionDialog.h"
#include "ui_divisionDialog.h"
#include "../../constants.h"


divisionDialog::divisionDialog(QWidget *parent, std::filesystem::path path): QDialog(parent), path{path}, ui(new Ui::divisionDialog) {
    this->ui->setupUi(this);

    this->rdt = new ReadDivisionsThread(this);
    this->rdt->setFilePath(std::filesystem::path{this->path}.concat(DIVISIONS));
    connect(this->rdt, SIGNAL(sendDivision(std::string)), this, SLOT(addDivision(std::string)));
    connect(this->rdt, SIGNAL(divisionsDone(std::map<std::string, std::map<std::string, std::vector<std::string>>>)), this, SLOT(setDivisions(std::map<std::string, std::map<std::string, std::vector<std::string>>>)));
    this->rdt->start();

    this->rdrt = new ReadDivisionRulesThread(this);
    this->rdrt->setFilePath(std::filesystem::path{this->path}.concat(DIVISIONRULES));
    connect(this->rdrt, SIGNAL(sendDivisionRules(QMap<std::string, QMap<std::string, QMap<std::string, QMap<std::string, std::string>>>>)), this, SLOT(setDivisionRules(QMap<std::string, QMap<std::string, QMap<std::string, QMap<std::string, std::string>>>>)));
    this->rdrt->start();
}

divisionDialog::~divisionDialog() {
    delete this->ui;
    delete this->rdt;
}

void divisionDialog::addDivision(std::string division) {
    mutex.lock();
    //this->divisionList.push_back(division);
    QListWidgetItem *item = new QListWidgetItem(tr(division.c_str()), this->ui->divisionListWidget);
    //qDebug() << this->divisionList.size() << " : " << this->ui->divisionListWidget->count();
    //this->ui->divisionListWidget->setCurrentItem(item);
    mutex.unlock();
}

void divisionDialog::setDivisions(QMap<std::string, QMap<std::string, std::vector<std::string>>> divisions) {
    this->mutex.lock();
    this->divisions = divisions;
    this->mutex.unlock();
}

void divisionDialog::setDivisionRules(QMap<std::string, QMap<std::string, QMap<std::string, QMap<std::string, std::string>>>> divisionRules) {
    this->mutex.lock();
    this->divisionRules = divisionRules;
    this->mutex.unlock();
}