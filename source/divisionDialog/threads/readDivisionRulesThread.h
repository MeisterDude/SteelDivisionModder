
#ifndef READDIVISIONRULESTHREAD_H
#define READDIVISIONRULESTHREAD_H

#include <QThreaD>
#include <Qmutex>
#include <filesystem>


class ReadDivisionRulesThread : public QThread {
    Q_OBJECT

    public:
    explicit ReadDivisionRulesThread(QObject *parent);
    ~ReadDivisionRulesThread();

    void run();
    void setFilePath(std::filesystem::path &path);

    private:
    QMutex mutex;
    std::filesystem::path path;

    void readDivisionRules(std::filesystem::path &path);

    signals:
    void sendDivisionRules(QMap<std::string, QMap<std::string, QMap<std::string, QMap<std::string, std::string>>>> divisionRules);
};

#endif // READDIVISIONRULESTHREAD_H