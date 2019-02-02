#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <assert.h>

int req[1000];
int headPos;

void generateRandomly() {
    int i;
    for(i = 0; i < 1000; i++){
        int res = (int) ((5000) * ((double)rand() / RAND_MAX));
        if (res > 4999)
            res--;
        req[i] = res;
    }
}

int readFromFile(char* fileName) {
    FILE* fp;
    fp = fopen(fileName, "r");

    if (fp == NULL) {
        printf("%s\n", "invalid file");
        return -1;
    }

    int a, b;
    while ((fscanf(fp, "%d %d", &a, &b) != -1)) {
        assert(b < 5000 && a > 0 && a < 1001);
        req[a-1] = b;
    }
    return 0;
}

int findMax() {
    int max = req[0];
    int i;

    for (i = 1; i < 1000; i++){
        if (req[i] > max)
            max = req[i];
    }
    return max;
}

int findMin() {
    int min = req[0];
    int i;

    for (i = 1; i < 1000; i++){
        if (req[i] < min)
            min = req[i];
    }
    return min;
}

int findMaxLessThanHead() {
    int res = req[0];
    int i;

    for (i = 0; i < 1000; i++){
        if (req[i] > res && req[i] < headPos)
            res = req[i];
    }
    return res;
}

int findNearest(int base) {
    int res = req[0], i, pos = 0;

    for (i = 1; i < 1000; i++) {
        if(req[i] == -1)
            continue;
        if (abs(res - base) > abs(req[i] - base)) {
            res = req[i];
            pos = i;
        }
    }

    req[pos] = -1;
    return res;
}

void cLook() {
    int min = findMin();
    int max = findMax();
    int mv = 0;
    int lHeadPos = headPos;

    if (max <= lHeadPos) {
        mv = lHeadPos - min + (max - min);
        printf("%s:\t%d\n", "C-LOOK", mv);
        return;
    }

    if (min >= lHeadPos) {
        mv = max - lHeadPos;
        printf("%s:\t%d\n", "C-LOOK", mv);
        return;
    }

    mv = max - lHeadPos + (max - min) + findMaxLessThanHead();
    printf("%s:\t%d\n", "C-LOOK", mv);
}

void look() {
    int min = findMin();
    int max = findMax();
    int mv = 0;
    int lHeadPos = headPos;

    if (lHeadPos < min) {
        mv = max - lHeadPos;
        printf("%s:\t%d\n", "LOOK", mv);
        return;
    }

    if (max < headPos) {
        mv = headPos - min;
        printf("%s:\t%d\n", "LOOK", mv);
        return;
    }

    mv = max - lHeadPos + (max - min);
    printf("%s:\t%d\n", "LOOK", mv);
}

void cScan() {
    int min = findMin();
    int max = findMax();
    int mv = 0;
    int lHeadPos = headPos;

    if (lHeadPos <= min) {
        mv = max - lHeadPos;
    } else {
        int end = findMaxLessThanHead();
        mv = 4999 - headPos + 4999 + end;
    }

    printf("%s:\t%d\n", "C-SCAN", mv);
}

void scan() {
    int mv = 0;
    int min = findMin();
    int max = findMax();
    int lHeadPos = headPos;

    if (lHeadPos <= min){
        mv = max - lHeadPos;
    } else {
        mv = 4999 - headPos + (4999 - min);
    }

    printf("%s:\t%d\n", "SCAN", mv);
}

void sstf() {
    int i, mv, done, lHeadPos;
    mv = done = 0;
    lHeadPos = headPos;

    for (i = 0; i < 1000; i++){
        int tmp = findNearest(lHeadPos);
        mv = mv + abs(lHeadPos - tmp);
        lHeadPos = tmp;
    }

    printf("%s:\t%d\n", "SSTF", mv);
}

void fcfs() {
    int i;
    int mv = 0;
    int lHeadPos = headPos;
    for (i = 0; i < 1000; i++){
        int add = abs(lHeadPos - req[i]);
        lHeadPos = req[i];
        mv = mv + add;
    }

    printf("%s:\t%d\n", "FCFS", mv);
}

int main(int argc, char* argv[]) {
    srand(time(NULL));

    if (argc < 2 || argc > 3) {
       printf("%s", "Usage: diskscheduling <headpos> <inputfile>\t\t(inputfile is optional)");
        return -1;
    } else if (argc == 2) {
        headPos = atoi(argv[1]);
        generateRandomly();
    } else if (argc == 3) {
        headPos = atoi(argv[1]);
        int a = readFromFile(argv[2]);
        if (a == -1) {
            return -1;
        }
    }

    fcfs();
    scan();
    cScan();
    look();
    cLook();
    sstf();
    return 0;
}