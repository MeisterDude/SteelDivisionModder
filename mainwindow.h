#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>

//#include "supply.h"
#include "readSupplyThread.h"
#include "readRepairThread.h"
#include "writeSupplyThread.h"
#include "writeRepairThread.h"


QT_BEGIN_NAMESPACE
namespace Ui { class SDStuffWidget; }
QT_END_NAMESPACE

class SDStuffWidget : public QWidget {
    Q_OBJECT

public:
    explicit SDStuffWidget(QWidget *parent = nullptr);
    ~SDStuffWidget();

private:
    Ui::SDStuffWidget *ui;

    QString modPath;
    bool isRepairModded;
    int supplyMultiplier;
    //Supply supply;
    ReadSupplyThread *rst;
    ReadRepairThread *rrt;
    WriteSupplyThread *wst;
    WriteRepairThread *wrt;

    //const QString countryText[3], countryStuff[3];

    //void fillCCB();
    //void readRavitaillement();
    //void setRepairLabel();
    //void readUniteDescriptor();
    

private slots:
    //void on_countryComboBox_currentIndexChanged();
    //void changeLabels();
    //int createNewMod();
    void openMod();
    void switchRepair();
    void setSupply();
    void setSupplyLabel(const int supplyMultiplier, const bool running = false);
    void setRepairLabel(const bool isRepairModded, const bool running = false);
};

#endif // MAINWINDOW_H
