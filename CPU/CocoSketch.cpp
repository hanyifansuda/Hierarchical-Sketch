#include <cmath>
#include <iostream>
#include <random>
#include <ctime>
#include "murmur3.h"
#include "murmur3.c"
using namespace std;

static mt19937 rng(time(0));

struct Bucket {
	unsigned int fingerprint = 0;
	unsigned int counter = 0;
};

class CocoSketch
{
private:
	int bucket_num;
    Bucket** bucketArray;

public:
	CocoSketch(int _bucket_num)
	{
        
        bucket_num = _bucket_num/2;

        bucketArray = new Bucket*[2];
        for(int i=0;i<2;i++)
            bucketArray[i] = new Bucket[bucket_num];        
        
	}

	void Insert(unsigned int flow)
	{
		bool flag = false;
        int a[2] = {-1, -1};
        for (int i = 0; i < 2; i++) {
            if (flag == false) {
                uint32_t j;
                MurmurHash3_x86_32(&flow, 4, i, &j);
                int bucket_idx = j % bucket_num;
                a[i] = bucket_idx;
                if (bucketArray[i][bucket_idx].fingerprint == 0){
                    bucketArray[i][bucket_idx].fingerprint = flow;
                    bucketArray[i][bucket_idx].counter = 1;
                    flag = true;
                }
                else if (bucketArray[i][bucket_idx].fingerprint == flow) {
                    bucketArray[i][bucket_idx].counter ++;
                    flag = true;
                }
            }
        }
        if (flag == false) {
            int value1 = bucketArray[0][a[0]].counter;
            int value2 = bucketArray[1][a[1]].counter;
            if (value1 <= value2) {
                bucketArray[0][a[0]].counter ++;
                double randomVal = (double)rng()/4294967296;
                if (randomVal < 1/bucketArray[0][a[0]].counter){
                    bucketArray[0][a[0]].fingerprint = flow;
                }
            }
            else {
                bucketArray[1][a[1]].counter ++;
                double randomVal = (double)rng()/4294967296;
                if (randomVal < 1/bucketArray[1][a[1]].counter){
                    bucketArray[1][a[1]].fingerprint = flow;
                }
            }
        }
    }
};