
#ifndef READPACKSTHREAD_H
#define READPACKSTHREAD_H

#include <QThread>
#include <Qmutex>
#include <filesystem>
#include <map>
#include <vector>


class ReadPacksThread : public QThread {
    Q_OBJECT

    public:
    explicit ReadPacksThread(QObject *parent);
    ~ReadPacksThread();

    void run();
    void setFilePath(std::filesystem::path &path);

    private:
    QMutex mutex;
    std::filesystem::path path;
    std::map<std::string, std::map<std::string, std::vector<std::string>>> packsContent;

    void readPacks(std::filesystem::path &path);
    //void readDivisionsHelper(std::ifstream &file, const std::string &name, const std::string &tmp);
    //void analizeContent();

    signals:
    void sendDivision(std::string division);
    

};

#endif // READPACKSTHREAD_H