//参考：https://blog.csdn.net/cai0538/article/details/7061922

#include <iostream>
#include <vector>
#include <cmath>
#include <random>
#include <iomanip>

#define k 3  //k：簇的个数
#define c 1E-3  //c:偏差值
#define s 30  //s:数据个数

//控制数据大小
#define range1 0
#define range2 20

int number = 0;

using namespace std;

struct Tuple {  //Tuple结构体：存储attr1（横坐标）和attr2（纵坐标）
	float attr1;
	float attr2;
};

float getDistXY(Tuple t1, Tuple t2) {  //计算两个元组间的欧几里得距离
	return sqrt((t1.attr1 - t2.attr1) * (t1.attr1 - t2.attr1) + (t1.attr2 - t2.attr2) * (t1.attr2 - t2.attr2));
}

int clusterOfTuple(Tuple means[], Tuple tuple) {  //根据质心，决定当前元组属于哪个簇
	float dist = getDistXY(means[0], tuple);
	float tmp;
	int label = 0;
	for (int i = 1; i < k; i++) {  //找到该元组最短欧几里得距离的簇
		tmp = getDistXY(means[i], tuple);
		if (tmp < dist) {
            dist = tmp;
            label = i;
        }
	}
	return label;
}

float getVar(vector<Tuple> clusters[], Tuple means[]) {  //获得给定簇集的平方误差
	float var = 0;
	for (int i = 0; i < k; i++) {
		vector<Tuple> t = clusters[i];
        /*for (int j = 0; j < t.size(); j++) {
            var += getDistXY(t[j], means[i]);
        }*/
		for (auto & j : t) {
			var += getDistXY(j, means[i]);
		}
	}
	return var;
}

Tuple getMeans(vector<Tuple> cluster) {  //获得当前簇的质心
	unsigned long num = cluster.size();
	float meansX = 0, meansY = 0;
	Tuple t{};
	for (int i = 0; i < num; i++) {
		meansX += cluster[i].attr1;
		meansY += cluster[i].attr2;
	}
	t.attr1 = float(meansX / float(num));
	t.attr2 = float(meansY / float(num));
	return t;
}

vector<Tuple> clusters[k];
Tuple means[k];

void Draw() {
    int num = abs(range1 - range2);
    cout << setw(4) << "";
    for (int i = 0; i < num; i++) {
        cout << setw(4) << i;
    }
    cout << endl;
    int temp[num + 3][num + 3];
    for (int i = 0; i < num; i++) {
        for(int j = 0; j < num; j++) {
            temp[i][j] = 0;
        }
    }
    for (int i = 0; i < k; i++) {
        vector<Tuple> t = clusters[i];
        int t1, t2;
        /*for (int j = 0; j < t.size(); j++) {
            t1 = int(t[j].attr1);
            t2 = int(t[j].attr2);
            temp[t1][t2] = i + 1;
        }*/
        for (auto & j : t) {
            t1 = int(j.attr1);
            t2 = int(j.attr2);
            temp[t1][t2] = i + 1;
        }
        t1 = int(round(means[i].attr1));
        t2 = int(round(means[i].attr2));
        temp[t1][t2] = -1 * (i + 1);
    }
    for (int i = 0; i < num; i++) {
        cout << setw(4) << i;
        for (int j = 0; j < num; j++) {
            if (temp[i][j] < 0) {
                cout << setw(4) << "&" << temp[i][j] * -1;
            }
            else if (temp[i][j] == 0) {
                cout << setw(4) << "";
            }
            else {
                cout << setw(4) << temp[i][j];
            }
        }
        cout << endl;
    }
}

void KMeans(vector<Tuple> tuples) {
    int i;
	for (i = 0; i < k; i++) {  //默认一开始将前k个元组的值作为k个簇的质心
		means[i].attr1 = tuples[i].attr1;
		means[i].attr2 = tuples[i].attr2;
	}
	int label;
	for (i = 0; i != tuples.size(); ++i) {  //根据默认的质心给簇赋值
        label = clusterOfTuple(means, tuples[i]);
		clusters[label].push_back(tuples[i]);
	}
    number++;
	for (label = 0; label < k; label++) {  //第一次输出
        if (label == 0) {
            cout << "第" << number << "次变换" << endl;
        }
		cout << "第" << label + 1 << "个簇：" << endl;
		vector<Tuple> t = clusters[label];
        cout << "质心位置:" << "(" << means[label].attr1 << "," << means[label].attr2 << ")" << endl;
		for (i = 0; i< t.size(); i++) {
			cout << "(" << t[i].attr1 << "," << t[i].attr2 << ")" << "   ";
		}	
		cout << endl;
	}
    cout << endl;
    Draw();
	float oldVar = -1;
	float newVar = getVar(clusters, means);
	while (abs(newVar - oldVar) >= c) {  //当新旧函数值相差不到c即准则函数值不发生明显变化时，算法终止
		for (i = 0; i < k; i++) {  //清空每个簇
			clusters[i].clear();
		}
		for (i = 0; i != tuples.size(); ++i) {  //根据新的质心获得新的簇
            label = clusterOfTuple(means, tuples[i]);
			clusters[label].push_back(tuples[i]);
		}
        for (i = 0; i < k; i++) {  //更新每个簇的中心点
            means[i] = getMeans(clusters[i]);
        }
        oldVar = newVar;
        newVar = getVar(clusters, means); //计算新的准则函数值，接下来要输出的
        if (abs(newVar - oldVar) < c) {
            break;
        }
        number++;
		for (label = 0; label < k; label++) {  //输出当前的簇
            if (label == 0) {
                cout << "第" << number << "次变换" << endl;
            }
			cout << "第" << label + 1 << "个簇：" << endl;
			vector<Tuple> t = clusters[label];
            cout << "质心位置:" << "(" << means[label].attr1 << "," << means[label].attr2 << ")" << endl;
			for (i = 0; i < t.size(); i++) {
				cout << "(" << t[i].attr1 << "," << t[i].attr2 << ")" << "   ";
			}	
			cout << endl;
		}
        cout << endl;
        Draw();
	}
}

int main() {
    default_random_engine random(time(nullptr) * time(nullptr));
    uniform_int_distribution<float> u(range1, range2);
    int count = 0;
    vector<Tuple> tuples;
    Tuple tuple{};
    while (count < s * 2) {
        count++;
        if (count % 2 == 1) {
            tuple.attr1 = u(random);
        }
        else {
            tuple.attr2 = u(random);
            tuples.push_back(tuple);
        }
    }
	//输出文件中的元组信息
    cout << "原始数据:";
    /*for (vector<Tuple>::size_type ix = 0; ix != tuples.size(); ++ix) {
        cout << "(" << tuples[ix].attr1 << "," << tuples[ix].attr2 << ")" << "\t";
    }*/
    for (auto & tuple : tuples) {
        cout << "(" << tuple.attr1 << "," << tuple.attr2 << ")" << " ";
    }
    cout << endl << endl;
	KMeans(tuples);
	return 0;
}