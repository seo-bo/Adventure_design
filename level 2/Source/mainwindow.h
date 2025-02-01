#pragma once
#include <QMainWindow>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QPushButton>
#include <QLabel>
#include "tspthread.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private slots:
    void startAlgorithm();
    void stopAlgorithm();
    void onNewMinimum(long long distance, int* path, int path_length);
    void onUpdatePath(long long distance, int* path, int path_length);
    void onAlgorithmFinished(qint64 elapsedTime);

private:
    void setupUI();
    void drawCities();
    void drawPath(int* path, int path_length);
    void updateDistanceAndPath(long long distance, int* path, int path_length);
    void read_data(const QString& filename, int cityCount);
    QGraphicsView* view;
    QGraphicsScene* scene;
    QPushButton* startButton;
    QPushButton* stopButton;
    QPushButton* bruteForceButton;
    QPushButton* dpButton;
    QPushButton* saButton;
    QLabel* distanceLabel;
    QLabel* pathLabel;
    QLabel* statusLabel;
    QLabel* timeLabel;
    bool running;
    int algorithmType; // 1: 브루트 포스, 2: DP, 3: 담금질 기법
    TSPThread* tspThread;
};
