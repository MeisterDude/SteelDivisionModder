
#ifndef READSUPPLYTHREAD_H 
#define READSUPPLYTHREAD_H

#include <string>
#include <QThread>


class ReadSupplyThread : public QThread {
    Q_OBJECT

    public:
    explicit ReadSupplyThread(QObject *parent, const std::string modPath = "");
    ~ReadSupplyThread();

    void run();
    void setModPath(const std::string &modPath);
    

    private:
    std::string modPath;


    int readSupply(const std::string &modPath);

    signals:
    void supplyRead(int supplyMultiplier, bool running = false);

};


#endif // READSUPPLYTHREAD_H