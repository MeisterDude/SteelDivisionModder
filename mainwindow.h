#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>

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

    const QString countryText[3], countryStuff[3];

    void fillCCB();
    void readRavitaillement();
    void setRepairLabel();
    void readUniteDescriptor();
    void setSupplyLabel();

private slots:
    //void on_countryComboBox_currentIndexChanged();
    //void changeLabels();
    int createNewMod();
    void openMod();
    void switchRepair();
    void setSupply();
};

#endif // MAINWINDOW_H
