
#ifndef WRITEREPAIRTHREAD_H 
#define WRITEREPAIRTHREAD_H

#include <string>
#include <QThread>


class WriteRepairThread : public QThread {
    Q_OBJECT

    public:
    explicit WriteRepairThread(QObject *parent);
    ~WriteRepairThread();

    void run();
    void setModPath(const std::string &modPath);
    void setIsRepairModded(const bool isRepairModded);
    

    private:
    std::string modPath = "";
    bool isRepairmodded = true;


    bool writeRepair(const std::string &modPath, bool isRepairModded);

    signals:
    void repairWritten(bool isRepairmodded, bool running = false);

};


#endif // WRITEREPAIRTHREAD_H