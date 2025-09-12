
#ifndef WRITEPOINTSTHREAD_H 
#define WRITEPOINTSTHREAD_H

#include <string>
#include <QThread>


class WritePointsThread : public QThread {
    Q_OBJECT

    public:
    explicit WritePointsThread(QObject *parent);
    ~WritePointsThread();

    void run();
    void setModPath(const std::string &modPath);
    void setIsPointsModded(const bool isPointsModded);

    private:
    std::string modPath = "";
    bool isPointsModded = false;

    bool writePoints(const std::string &modPath, bool isPointsModded);

    signals:
    void pointsWritten(bool isPointsModded, bool running = false);

};


#endif // WRITEPOINTSTHREAD_H