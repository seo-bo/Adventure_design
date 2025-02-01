#include "tspthread.h"

TSPThread::TSPThread(QObject* parent) : QThread(parent), running(true), paused(false), algorithmType(0), bf_counter(0) {}

void TSPThread::run()
{
	running = true;
	paused = false;
	QElapsedTimer timer;
	timer.start();

	const qint64 timeLimit = 180000; // 3분 = 180,000 밀리초

	if (algorithmType == 1) // 브루트 포스 알고리즘
	{
		QElapsedTimer timer;
		timer.start();
		init_bf();
		bf_mini = LLONG_MAX;
		bf_counter = 0;
		bool newFound = false; // 새로운 최소 거리 발견 여부
		do 
		{
			// 실행 시간 제한 체크
			if (timer.elapsed() >= timeLimit) // 시간 초과
			{
				break;
			}
			mutex.lock();
			if (!running) 
			{
				mutex.unlock();
				break;
			}
			if (paused)
			{
				pauseCondition.wait(&mutex);
			}
			mutex.unlock();
			newFound = cal_bf_step();
			bf_counter++;
			if (newFound)// 새로운 최소 거리를 발견
			{		
				emit newMinimum(bf_mini, path_bf, bf_head);
			}
			// 현재 경로를 업데이트
			emit updatePath(bf_mini, permutation, count);
			QThread::msleep(2);
		} while (bf_head != -1);
		qint64 elapsedTime = timer.elapsed();
		emit algorithmFinished(elapsedTime);
	}
	else if (algorithmType == 2) // 비트 필드 DP(20 이하의 적은 수에서만 동작)
	{		
		if (count > 20) 
		{
			emit newMinimum(-1, nullptr, 0); // 에러
			return;
		}
		QElapsedTimer timer;
		timer.start();
		init_dp();
		dp_algorithm();
		dp_mini = 1e9;
		int last_city = -1;
		int full_mask = (1 << count) - 1;
		for (int i = 0; i < count; ++i) 
		{
			long long temp = dp[full_mask][i] + distance[i][0];
			if (dp_mini > temp) 
			{
				dp_mini = temp;
				last_city = i;
			}
		}
		if (last_city == -1) 
		{
			emit newMinimum(-1, nullptr, 0); // 경로 없음
			return;
		}
		int* path = get_dp_path(last_city);
		emit newMinimum(dp_mini, path, dp_head);
		qint64 elapsedTime = timer.elapsed();
		emit algorithmFinished(elapsedTime);
	}
	else if (algorithmType == 3) // 담금질 기법
	{	
		QElapsedTimer timer;
		timer.start();
		init_sa();
		long long sa_pre = 1e9;
		while (running && temper > 1e-3 && timer.elapsed() <= 180000) 
		{
			mutex.lock();
			if (!running) 
			{
				mutex.unlock();
				break;
			}
			if (paused) 
			{
				pauseCondition.wait(&mutex);
			}
			mutex.unlock();
			cal_sa_step();
			emit updatePath(current_dis, current, count);
			if (sa_mini < sa_pre) 
			{
				sa_pre = sa_mini;
				emit newMinimum(sa_mini, path_sa, count);
			}
			QThread::msleep(2);
		}
		qint64 elapsedTime = timer.elapsed();
		emit algorithmFinished(elapsedTime);
	}
}

void TSPThread::stop() 
{
	QMutexLocker locker(&mutex);
	running = false;
	paused = false;
	pauseCondition.wakeAll();
}

void TSPThread::pause() 
{
	QMutexLocker locker(&mutex);
	paused = true;
}

void TSPThread::resume() 
{
	QMutexLocker locker(&mutex);
	paused = false;
	pauseCondition.wakeAll();
}

void TSPThread::setAlgorithm(int algoType)
{
	algorithmType = algoType;
}
