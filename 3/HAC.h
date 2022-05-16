#ifndef HAC_H
#define HAC_H

#include <vector>
using namespace std;

struct Position {
    __attribute__((unused)) double a;
    __attribute__((unused)) double b;
    __attribute__((unused)) double c;
    __attribute__((unused)) double d;
    __attribute__((unused)) double e;
};

struct Cluster {
    __attribute__((unused)) vector<int> pointindex;
    __attribute__((unused)) int clusterID;
    __attribute__((unused)) vector<double> avedistance;
};

struct Point {
    __attribute__((unused)) Position position;
    __attribute__((unused)) int pointID;
};

class HAC {
private:
    __attribute__((unused)) vector<Point> point;
    __attribute__((unused)) vector<Cluster> cluster;
    __attribute__((unused)) double distance[20][20]{};	//点对距离矩阵
    __attribute__((unused)) int minindex[2]{};		//最小组平均的两个簇序号

public:
    HAC();
    ~HAC() = default;
    void calculate_ave();		//计算组平均距离
    void merge();		//求最小组平均，合并簇
    __attribute__((unused)) void print();		//显示每次合并的情况
};

#endif