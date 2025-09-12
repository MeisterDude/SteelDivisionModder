
#ifndef WRITESUPPLYTHREAD_H 
#define WRITESUPPLYTHREAD_H

#include <string>
#include <QThread>


class WriteSupplyThread : public QThread {
    Q_OBJECT

    public:
    explicit WriteSupplyThread(QObject *parent);
    ~WriteSupplyThread();

    void run();
    void setModPath(const std::string &modPath);
    void setsupplyMultiplier(const int supplyMultiplier, const int oldMultiplier);
    

    private:
    std::string modPath = "";
    int supplyMultiplier = 1, oldMultiplier = 1;

    void writeSupply(const std::string &modPath, const int multiplier, const int old);

    signals:
    void supplyWritten(int newMultiplier, bool running = false);

};


#endif // WRITESUPPLYTHREAD_H