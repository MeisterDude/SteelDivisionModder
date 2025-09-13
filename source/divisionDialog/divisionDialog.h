
#ifndef DIVISIONDIALOG_H
#define DIVISIONDIALOG_H

#include <QDialog>
#include <QMutex>
#include <vector>
#include <filesystem>

#include "threads/readDivisionsThread.h"
#include "threads/readDivisionRulesThread.h"


QT_BEGIN_NAMESPACE
namespace Ui { class divisionDialog; }
QT_END_NAMESPACE

class divisionDialog : public QDialog {
    Q_OBJECT

    public:
    explicit divisionDialog(QWidget *parent = nullptr, std::filesystem::path path = "");
    ~divisionDialog();

    Ui::divisionDialog *ui;

    
    private:
    QMutex mutex;
    std::filesystem::path &path;
    //std::vector<std::string> divisionList;
    QMap<std::string, QMap<std::string, std::vector<std::string>>> divisions;
    QMap<std::string, QMap<std::string, QMap<std::string, QMap<std::string, std::string>>>> divisionRules;
    ReadDivisionsThread *rdt;
    ReadDivisionRulesThread *rdrt;

    

    private slots:
    void addDivision(std::string division);
    void setDivisions(QMap<std::string, QMap<std::string, std::vector<std::string>>> divisions);
    void setDivisionRules(QMap<std::string, QMap<std::string, QMap<std::string, QMap<std::string, std::string>>>> divisionRules);
};

#endif // DIVISIONDIALOG_H