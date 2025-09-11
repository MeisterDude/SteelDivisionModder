
#ifndef READREPAIRTHREAD_H 
#define READREPAIRTHREAD_H

#include <string>
#include <QThread>


class ReadRepairThread : public QThread {
    Q_OBJECT

    public:
    explicit ReadRepairThread(QObject *parent, const std::string modPath = "");
    ~ReadRepairThread();

    void run();
    void setModPath(const std::string &modPath);
    

    private:
    std::string modPath;


    bool readRepair(const std::string &modPath);

    signals:
    void repairRead(bool isRepairModded, bool running = false);

};


#endif // READREPAIRTHREAD_H