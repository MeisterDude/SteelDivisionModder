
#ifndef READPOINTSTHREAD_H
#define READPOINTSTHREAD_H

#include <QThread>


class ReadPointsThread : public QThread {
    Q_OBJECT

    public:
    explicit ReadPointsThread(QObject *parent);
    ~ReadPointsThread();

    void run();
    void setModPath(const std::string &modPath);

    private:
    std::string modPath = "";

    bool readPoints(const std::string &modPath);

    signals:
    void pointsRead(bool pointsModded, bool running = false);

};

#endif // READPOINTSTHREAD_H