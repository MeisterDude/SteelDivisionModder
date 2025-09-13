
#ifndef READDIVISIONSTHREAD_H
#define READDIVISIONSTHREAD_H

#include <QThread>
#include <Qmutex>
//#include <QMap>
#include <filesystem>
//#include <map>
#include <vector>


class ReadDivisionsThread : public QThread {
    Q_OBJECT

    public:
    explicit ReadDivisionsThread(QObject *parent);
    ~ReadDivisionsThread();

    void run();
    void setFilePath(std::filesystem::path &path);

    private:
    QMutex mutex;
    std::filesystem::path path;
    //std::map<std::string, std::map<std::string, std::vector<std::string>>> divisionContent;
    QMap<std::string, QMap<std::string, std::vector<std::string>>> divisionContent;

    void readDivisions(std::filesystem::path &path);
    void readDivisionsHelper(std::ifstream &file, const std::string &name, const std::string &tmp);
    void analizeContent();

    signals:
    void sendDivision(std::string division);
    void divisionsDone(QMap<std::string, QMap<std::string, std::vector<std::string>>> divisions);

};

#endif // READDIVISIONSTHREAD_H