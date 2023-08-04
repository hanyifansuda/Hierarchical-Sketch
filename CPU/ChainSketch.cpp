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

class ChainSketch
{
private:
	int bucket_num;
    Bucket** bucketArray;
    double theta;
    float threshold;
public:
	ChainSketch(int _bucket_num)
	{
        
        bucket_num = _bucket_num/4;

        bucketArray = new Bucket*[4];
        for(int i=0;i<4;i++)
            bucketArray[i] = new Bucket[bucket_num];        
        
        theta = 0.000004;
        threshold = theta * 32000000;
	}

	void Insert(unsigned int flow)
	{
        bool flag = false;
        int m = 0xffffffff;
        int hash_idx = -1;
        for (int i = 0; i < 4; i++) {
            if (flag == false) {
                uint32_t j;
                MurmurHash3_x86_32(&flow, 4, i, &j);
                int bucket_idx = j % bucket_num;
                
                if (bucketArray[i][bucket_idx].fingerprint == flow || bucketArray[i][bucket_idx].counter == 0 ){
                    bucketArray[i][bucket_idx].fingerprint = flow;
                    bucketArray[i][bucket_idx].counter ++;
                    flag = true;
                }
                else {
                    if (bucketArray[i][bucket_idx].counter < m) {
                        hash_idx = i;
                    }
                }
            }
        }
        if (flag == false) {
            bool* visit = new bool[bucket_num];
            int count = 0;
            uint32_t j;
            MurmurHash3_x86_32(&flow, 32, hash_idx, &j);
            int p1 = j % bucket_num;
            double a = bucketArray[hash_idx][p1].counter;
            double randomVal = (double)rng()/4294967296;
            if (randomVal < 1/(a+1)){
                if (a > threshold) {
                    uint32_t hash_f;
                    MurmurHash3_x86_32(&flow, 32, 4+1, &hash_f);
                    int p2 = (p1 + hash_f) % bucket_num;
                    visit[p1] = true;
                    count ++;
                    int l = 2;
                    double b = bucketArray[hash_idx][p2].counter;

                    while (visit[p2] == false && a > b && count <= 3){
                        visit[p2] = true;
                        count ++;
                        b = bucketArray[hash_idx][p2].counter;
                        double randomVal = (double)rng()/4294967296;

                        if (randomVal <= pow(a/(a+b), l) ) {
                            bucketArray[hash_idx][p2].fingerprint = bucketArray[hash_idx][p1].fingerprint;
                            bucketArray[hash_idx][p2].counter = bucketArray[hash_idx][p1].counter;
                        }
                        else {
                            p2 = ( p2 + hash_f ) % bucket_num;
                            l += 1;
                        }
                    }


                }
            }
            MurmurHash3_x86_32(&flow, 32, hash_idx, &j);
            int p = j % bucket_num;
            bucketArray[hash_idx][p].fingerprint = flow;
            bucketArray[hash_idx][p].counter += 1;
        }

    }
};