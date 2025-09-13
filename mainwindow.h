
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>

#include "source/mainThreads/readSupplyThread.h"
#include "source/mainThreads/readRepairThread.h"
#include "source/mainThreads/readPointsThread.h"
#include "source/mainThreads/writeSupplyThread.h"
#include "source/mainThreads/writeRepairThread.h"
#include "source/mainThreads/writePointsThread.h"


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
    bool isRepairModded, isPointsModded;
    int supplyMultiplier;
    ReadSupplyThread *rst;
    ReadRepairThread *rrt;
    ReadPointsThread *rpt;
    WriteSupplyThread *wst;
    WriteRepairThread *wrt;
    WritePointsThread *wpt;

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
    void switchPoints();
    void setSupplyLabel(const int supplyMultiplier, const bool running = false);
    void setRepairLabel(const bool isRepairModded, const bool running = false);
    void setPointsLabel(const bool isPointsModded, const bool running = false);
    void openDivisionDialog();
};

#endif // MAINWINDOW_H
