#include <cmath>
#include <iostream>
#include <random>
#include <ctime>
#include "murmur3.h"
#include "murmur3.c"
using namespace std;

static mt19937 rng(time(0));

struct Bucket {
	unsigned short int total_sum = 0;
	unsigned int fingerprint = 0;
	unsigned short int indicator = 0;
};

class MVSketch
{
private:
	int bucket_num;
    Bucket** bucketArray;

public:
	MVSketch(int _bucket_num)
	{
        
        bucket_num = _bucket_num/4;

        bucketArray = new Bucket*[4];
        for(int i=0;i<4;i++)
            bucketArray[i] = new Bucket[bucket_num];        
        
	}

	void Insert(unsigned int flow)
	{
		
        for (int i = 0; i < 4; i++) {
            uint32_t j;
            MurmurHash3_x86_32(&flow, 4, i, &j);
            int bucket_idx = j % bucket_num;
            bucketArray[i][bucket_idx].total_sum ++;
            if (bucketArray[i][bucket_idx].fingerprint == flow){
                bucketArray[i][bucket_idx].indicator ++;
            }
            else {
                bucketArray[i][bucket_idx].indicator --;
                if (bucketArray[i][bucket_idx].indicator < 0){
                    bucketArray[i][bucket_idx].fingerprint = flow;
                    bucketArray[i][bucket_idx].indicator = -bucketArray[i][bucket_idx].indicator;
                }
            }
        }
    }
};