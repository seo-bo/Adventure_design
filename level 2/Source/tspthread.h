#pragma once
#include <QThread>
#include <QElapsedTimer>
#include <QWaitCondition>
#include <QMutex>
#include "sub.h"

class TSPThread : public QThread 
{
    Q_OBJECT

public:
    explicit TSPThread(QObject* parent = nullptr);
    void run() override;
    void stop();
    void pause();
    void resume();
    void setAlgorithm(int algoType);

signals:
    void newMinimum(long long distance, int* path, int path_length);
    void updatePath(long long distance, int* path, int path_length);
    void algorithmFinished(qint64 elapsedTime);

private:
    volatile bool running;
    volatile bool paused;
    int algorithmType; // 1: ���Ʈ ����, 2: DP, 3: ����� ���
    int bf_counter; // ���Ʈ ���� ���� ��Ȳ
    QWaitCondition pauseCondition;
    QMutex mutex;
};
