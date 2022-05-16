#include "HAC.h"
#include <cmath>
#include <algorithm>
#include <iostream>
#include <random>

#define NUM 20
#define MIN_DIST 100

HAC::HAC() {
    //default_random_engine random(0);
    default_random_engine random(time(nullptr) * time(nullptr));
    uniform_int_distribution<int> u(1, 100);
    int a, b, c, d, e;
    double sum;
    Point point_temp{};
    Cluster cluster_temp;
    for (int i = 0; i < NUM; i++) {
        //产生随机数
        a = u(random);
        b = u(random);
        c = u(random);
        d = u(random);
        e = u(random);
        cout << "产生的随机数据: (" << a << ", " << b << ", "  << c << ", "  << d << ", " << e << ")"<< endl;

        //进行归一化处理
        sum = a + b + c + d + e;
        point_temp.pointID = i;
        point_temp.position.a = a / sum;
        point_temp.position.b = b / sum;
        point_temp.position.c = c / sum;
        point_temp.position.d = d / sum;
        point_temp.position.e = e / sum;
        point.push_back(point_temp);

        //簇初始化
        cluster_temp.pointindex.clear();
        cluster_temp.clusterID = i;
        cluster_temp.pointindex.push_back(i);
        cluster.push_back(cluster_temp);

        cout << i + 1 << ": ("  << point[i].position.a << ", "
        << point[i].position.b << ", "  << point[i].position.c << ", "  << point[i].position.d << ", "
        << point[i].position.e << ")"<< endl;
    }

    //计算距离矩阵
    for (int i = 0; i < 20; i++) {
        distance[i][i] = 0;
        for (int j = i + 1; j < 20; j++) {
            distance[i][j] = sqrt((point[i].position.a - point[j].position.a) *
            (point[i].position.a - point[j].position.a) + (point[i].position.b - point[j].position.b) *
            (point[i].position.b - point[j].position.b) + (point[i].position.c - point[j].position.c) *
            (point[i].position.c - point[j].position.c) + (point[i].position.d - point[j].position.d) *
            (point[i].position.d - point[j].position.d) + (point[i].position.e - point[j].position.e) *
            (point[i].position.e - point[j].position.e));
            distance[j][i] = distance[i][j];
        }
    }
}

//计算组平均
void HAC::calculate_ave() {
    int m, n;
    vector<double> dist;
    double dist_sum;
    for (int i = 0; i < cluster.size(); i++) {  //簇i
        cluster[i].avedistance.clear();  //清空上一次的结果
        for (int j = i + 1; j < cluster.size(); j++) {  //簇j
            dist_sum = 0;
            for (int x = 0; x < cluster[i].pointindex.size(); x++) {  //簇i的第x个点
                m = cluster[i].pointindex[x];
                /*for (int y = 0; y < cluster[j].pointindex.size(); y++) {  //簇j的第y个点
                    n = cluster[j].pointindex[y];
                    dist.push_back(distance[m][n]);  //簇i和簇j的点距离存入dist
                }*/
                for (int y : cluster[j].pointindex) {  //簇j的第y个点
                    n = y;
                    dist.push_back(distance[m][n]);  //簇i和簇j的点距离存入dist
                }
                for (int y = x + 1; y < cluster[i].pointindex.size(); y++) {
                    n = cluster[i].pointindex[y];
                    dist.push_back(distance[m][n]);  //将簇i与自己簇内的点距离存入dist
                }
            }
            //求簇i和簇j的组平均
            /*
            for (int p = 0; p < dist.size(); p++) {
                dist_sum += dist[p];
            }
            */
            for (double p : dist) {
                dist_sum += p;
            }
            dist_sum /= double(cluster[i].pointindex.size() * cluster[j].pointindex.size());
            cluster[i].avedistance.push_back(dist_sum);  //簇i和簇j的组平均放入簇i的avedistance，最后一个簇的avedistance为空
            dist.clear();
        }
    }
}

//计算最小组平均
void HAC::merge() {
    double min_dist = MIN_DIST;
    for (int i = 0; i < cluster.size(); i++) {
        for (int j = 0; j < cluster[i].avedistance.size(); j++) {
            if (cluster[i].avedistance[j] < min_dist) {
                min_dist = cluster[i].avedistance[j];
                minindex[0] = i;
                minindex[1] = i + j + 1;
            }
        }
    }
    print();
    //merge
    for (int i = 0; i < cluster[minindex[1]].pointindex.size(); i++) {
        cluster[minindex[0]].pointindex.push_back(cluster[minindex[1]].pointindex[i]);
        //将最小组平均簇对的其中一个簇的点索引添加至另一个簇
    }
    cluster.erase(cluster.begin() + minindex[1]);
}

void HAC::print() {
    cout << endl << "簇" << minindex[0] + 1 << "  簇内点为：";
    for (int i = 0; i < cluster[minindex[0]].pointindex.size(); i++) {
        cout << cluster[minindex[0]].pointindex[i] + 1;
        if (i != cluster[minindex[0]].pointindex.size() - 1) {
            cout << ", ";
        }
    }
    cout << endl << "簇" << minindex[1] + 1 << "  簇内点为：";
    for (int i = 0; i < cluster[minindex[1]].pointindex.size(); i++) {
        cout << cluster[minindex[1]].pointindex[i] + 1;
        if (i != cluster[minindex[1]].pointindex.size() - 1) {
            cout << ", ";
        }
    }
    cout << endl;
}