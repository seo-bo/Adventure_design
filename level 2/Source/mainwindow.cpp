#define _CRT_SECURE_NO_WARNINGS
#include "mainwindow.h"
#include "sub.h"
#include <QInputDialog>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGraphicsEllipseItem>
#include <QGraphicsLineItem>
#include <QMessageBox>
#include <QPen>
#include <QBrush>
#include <QSplitter>
#include <QFont>
#include <QDebug>
#include <QTextStream>
#include <QRegularExpression>

city cities[1000];
int count = 0;
long long distance[1000][1000];
int path_dp[1000];
int dp_head = 0;
long long dp_mini = 1e9;
int path_sa[1000];
int sa_head = 0;
long long sa_mini = 1e9;
long long dp[1 << 20][20];
int dp_parent[1 << 20][20];
int parent[1 << 20][20];
int current[1000];
long long current_dis = 0;
long double temper = 0.0;
long double cooling = 0.995;
int permutation[1000];
long long bf_mini = 1e9;
int path_bf[1000];
int bf_head = 0;

MainWindow::MainWindow(QWidget* parent)
	: QMainWindow(parent), running(false), algorithmType(0), tspThread(nullptr) 
{
	setupUI();
	QString appDirPath = QCoreApplication::applicationDirPath();
	QString filePath = appDirPath + "/tsp_data.txt"; // 도시 데이터 파일 읽기
	// 파일 존재 여부 확인
	QFile file(filePath);
	if (!file.exists()) 
	{
		QMessageBox::critical(this, tr("오류"), tr("데이터 파일이 존재하지 않습니다."));
		close();
		return;
	}
	// 사용자로부터 도시의 개수 입력받기
	bool ok;
	int cityCount = QInputDialog::getInt(this, tr("도시의 개수 입력"), tr("도시의 개수를 입력하세요:"), 1, 1, 1000, 1, &ok);
	if (!ok) 
	{
		QMessageBox::critical(this, tr("오류"), tr("도시의 개수를 입력하지 않았습니다."));
		close();
		exit(0);
	}
	read_data(filePath, cityCount);
	if (count > 0)
	{
		init_mat();
		drawCities();
	}
	else 
	{
		QMessageBox::critical(this, tr("오류"), tr("데이터를 읽어올 수 없습니다."));
		close();
	}
}

MainWindow::~MainWindow()
{
	if (tspThread) {
		tspThread->stop();
		tspThread->wait();
		delete tspThread;
	}
}

void MainWindow::read_data(const QString& filename, int cityCount) {
	QFile file(filename);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		QMessageBox::critical(this, tr("오류"), tr("파일을 열 수 없습니다."));
		return;
	}

	count = 0;
	QTextStream in(&file);
	for (int i = 0; i < cityCount; ++i) 
	{
		if (in.atEnd()) 
		{
			QMessageBox::critical(this, tr("오류"), tr("파일의 데이터가 부족합니다. 필요한 도시의 개수: %1, 파일에서 읽은 도시의 개수: %2").arg(cityCount).arg(count));
			break;
		}
		QString line = in.readLine().trimmed(); 
		QStringList tokens = line.split(QRegularExpression("\\s+"), Qt::SkipEmptyParts);
		if (tokens.size() < 3)
		{
			QMessageBox::critical(this, tr("오류"), tr("데이터 형식이 잘못되었습니다. 읽은 줄: %1").arg(line));
			break;
		}
		bool ok1, ok2, ok3;
		int id = tokens[0].toInt(&ok1);
		long long x = tokens[1].toLongLong(&ok2);
		long long y = tokens[2].toLongLong(&ok3);
		if (!ok1 || !ok2 || !ok3) 
		{
			QMessageBox::critical(this, tr("오류"), tr("숫자 변환에 실패했습니다. 읽은 줄: %1").arg(line));
			break;
		}
		cities[count++] = create_city(id, x, y);
	}
	file.close();
	if (count != cityCount) 
	{
		QMessageBox::warning(this, tr("경고"), tr("실제로 읽은 도시의 개수(%1)가 입력된 개수(%2)와 일치하지 않습니다.").arg(count).arg(cityCount));
	}
}

void MainWindow::setupUI() 
{
	setWindowTitle(tr("외판원 순회 문제"));
	resize(1200, 800);
	QSplitter* splitter = new QSplitter(Qt::Horizontal, this);
	QFrame* leftFrame = new QFrame();
	QVBoxLayout* leftLayout = new QVBoxLayout(leftFrame);
	view = new QGraphicsView(this);
	scene = new QGraphicsScene(this);
	view->setScene(scene);
	view->setRenderHint(QPainter::Antialiasing);
	leftLayout->addWidget(view);
	splitter->addWidget(leftFrame);
	splitter->setStretchFactor(0, 6);
	QFrame* rightFrame = new QFrame();
	QVBoxLayout* rightLayout = new QVBoxLayout(rightFrame);
	QFrame* buttonFrame = new QFrame();
	QVBoxLayout* buttonLayout = new QVBoxLayout(buttonFrame);
	startButton = new QPushButton(tr("시작/재개"), this);
	stopButton = new QPushButton(tr("중지"), this);
	bruteForceButton = new QPushButton(tr("브루트 포스"), this);
	dpButton = new QPushButton(tr("동적 계획법"), this);
	saButton = new QPushButton(tr("담금질 기법"), this);
	QFont buttonFont;
	buttonFont.setPointSize(12);
	startButton->setFont(buttonFont);
	stopButton->setFont(buttonFont);
	bruteForceButton->setFont(buttonFont);
	dpButton->setFont(buttonFont);
	saButton->setFont(buttonFont);
	buttonLayout->addWidget(startButton);
	buttonLayout->addWidget(stopButton);
	buttonLayout->addWidget(bruteForceButton);
	buttonLayout->addWidget(dpButton);
	buttonLayout->addWidget(saButton);
	QFrame* infoFrame = new QFrame();
	QVBoxLayout* infoLayout = new QVBoxLayout(infoFrame);
	distanceLabel = new QLabel(tr("최소 거리: "), this);
	pathLabel = new QLabel(tr("경로: "), this);
	statusLabel = new QLabel(tr("상태: 대기 중"), this);
	QFont infoFont;
	infoFont.setPointSize(12);
	distanceLabel->setFont(infoFont);
	pathLabel->setFont(infoFont);
	statusLabel->setFont(infoFont);
	infoLayout->addWidget(distanceLabel);
	infoLayout->addWidget(pathLabel);
	infoLayout->addWidget(statusLabel);
	rightLayout->addWidget(buttonFrame);
	rightLayout->addWidget(infoFrame);
	splitter->addWidget(rightFrame);
	splitter->setStretchFactor(1, 4);
	timeLabel = new QLabel(tr("소요 시간: "), this);
	timeLabel->setFont(infoFont);
	infoLayout->addWidget(timeLabel);
	setCentralWidget(splitter);
	connect(startButton, &QPushButton::clicked, this, &MainWindow::startAlgorithm);
	connect(stopButton, &QPushButton::clicked, this, &MainWindow::stopAlgorithm);
	connect(bruteForceButton, &QPushButton::clicked, [this]() { algorithmType = 1; });
	connect(dpButton, &QPushButton::clicked, [this]() { algorithmType = 2; });
	connect(saButton, &QPushButton::clicked, [this]() { algorithmType = 3; });
}

void MainWindow::drawCities() 
{
	scene->clear();
	QPen pen(Qt::black);
	QBrush brush(Qt::white);
	for (int i = 0; i < count; ++i) 
	{
		QGraphicsEllipseItem* cityItem = scene->addEllipse(cities[i].x - 5, cities[i].y - 5, 10, 10, pen, brush);
		QGraphicsTextItem* textItem = scene->addText(QString::number(cities[i].id));
		textItem->setPos(cities[i].x + 5, cities[i].y + 5);
	}
}

void MainWindow::drawPath(int* path, int path_length) 
{
	if (path_length <= 0)
	{
		return;
	}
	drawCities();
	QPen pen(Qt::red, 2);
	for (int i = 0; i < path_length - 1; ++i)
	{
		scene->addLine(cities[path[i]].x, cities[path[i]].y,
		cities[path[i + 1]].x, cities[path[i + 1]].y, pen);
	}
	scene->addLine(cities[path[path_length - 1]].x, cities[path[path_length - 1]].y,
	cities[path[0]].x, cities[path[0]].y, pen);
}

void MainWindow::startAlgorithm()
{
	if (running)
	{
		if (tspThread && tspThread->isRunning())
		{
			tspThread->resume();
			statusLabel->setText(tr("상태: 실행 중"));
		}
		return;
	}
	running = true;
	if (algorithmType == 0)
	{
		QMessageBox::warning(this, tr("알고리즘 선택"), tr("알고리즘을 선택하세요."));
		running = false;
		return;
	}
	if (algorithmType == 2 && count > 20) 
	{
		QMessageBox::information(this, tr("알림"), tr("동적 계획법은 도시 수가 20 이하일 때만 가능합니다."));
		running = false;
		return;
	}
	if (tspThread && tspThread->isRunning()) 
	{
		tspThread->resume();
	}
	else
	{
		tspThread = new TSPThread(this);
		tspThread->setAlgorithm(algorithmType);
		connect(tspThread, &TSPThread::newMinimum, this, &MainWindow::onNewMinimum);
		connect(tspThread, &TSPThread::updatePath, this, &MainWindow::onUpdatePath);
		connect(tspThread, &TSPThread::algorithmFinished, this, &MainWindow::onAlgorithmFinished);
		tspThread->start();
	}
	statusLabel->setText(tr("상태: 실행 중"));
}

void MainWindow::stopAlgorithm() 
{
	if (!running)
	{
		return;
	}
	running = false;
	if (tspThread && tspThread->isRunning()) {
		tspThread->pause();
		statusLabel->setText(tr("상태: 일시정지됨"));
	}
}

void MainWindow::onNewMinimum(long long distance, int* path, int path_length) 
{
	if (distance < 0) 
	{
		QMessageBox::warning(this, tr("오류"), tr("도시 수가 너무 많습니다."));
		stopAlgorithm();
		return;
	}
	updateDistanceAndPath(distance, path, path_length);
}

void MainWindow::onUpdatePath(long long distance, int* path, int path_length)
{
	drawPath(path, path_length);
}

void MainWindow::updateDistanceAndPath(long long distance, int* path, int path_length) 
{
	distanceLabel->setText(tr("최소 거리: %1").arg(QString::number(distance)));
	QString pathStr = "경로:\n";
	for (int i = 0; i < path_length; ++i)
	{
		pathStr += QString::number(path[i]);
		if (i != path_length - 1)
		{
			pathStr += " -> ";
		}	
		if ((i + 1) % 10 == 0) // 10개마다 줄바꿈 추가
		{
			pathStr += "\n";
		}
	}
	pathLabel->setText(pathStr);
}

void MainWindow::onAlgorithmFinished(qint64 elapsedTime)
{
	running = false;
	if (tspThread)
	{
		tspThread->wait();
		delete tspThread;
		tspThread = nullptr;
	}
	statusLabel->setText(tr("상태: 완료되었습니다."));
	timeLabel->setText(tr("소요 시간: %1 ms").arg(elapsedTime));
}
