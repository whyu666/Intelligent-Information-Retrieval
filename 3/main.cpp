//https://blog.csdn.net/zero_only/article/details/106461526

#include"HAC.h"
#include <iostream>

#define NUM 20

int main() {
    HAC hac;
    for (int i = 0; i < NUM - 1; i++) {
        cout << endl << "第" << i + 1 << "次合并的两个簇为：";
        hac.calculate_ave();
        hac.merge();
    }
    return 0;
}