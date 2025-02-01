#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <memory.h>
#include <math.h>
#include <float.h>
#include <time.h>
#include <QtCore>
#include <QMessageBox>

typedef struct city {
    int id;
    long long x;
    long long y;
} city;

extern city cities[1000];
extern int count;
extern long long distance[1000][1000];
extern int path_dp[1000];
extern int dp_head;
extern long long dp_mini;
extern int path_sa[1000];
extern int sa_head;
extern long long sa_mini;
extern long long dp[1 << 20][20];
extern int dp_parent[1 << 20][20];
extern int current[1000];
extern long long current_dis;
extern long double temper;
extern long double cooling;
extern int permutation[1000];
extern long long bf_mini;
extern int path_bf[1000];
extern int bf_head;

city create_city(int id, long long x, long long y);
long long calculate(city a, city b);
void init_mat();
void init_dp();
void dp_algorithm();
int* get_dp_path(int last_city);
void init_sa();
void cal_sa_step();
int* get_sa_path();
int* make_permutation();
void swap(int pre, int next);
void inversion(int pre, int next);
void init_bf();
bool cal_bf_step();
int* get_bf_path();
bool next_permutation(int* begin, int* end);
