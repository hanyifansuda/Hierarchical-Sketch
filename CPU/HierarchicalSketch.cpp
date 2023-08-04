#include <cmath>
#include <iostream>
#include <random>
#include <ctime>
// #include "murmur3.h"
#include "murmur3.c"
using namespace std;

static mt19937 rng(time(0));

struct Bucket {
	uint8_t fingerprint1 = 0;
	uint8_t counter1 = 0;
	uint8_t fingerprint2 = 0;
	uint8_t counter2 = 0;
	uint8_t fingerprint3 = 0;
	uint8_t counter3 = 0;
    uint8_t l_slot1_2 = 0;
    uint8_t l_slot3_4 = 0;
};

class HierarchicalSketch
{
private:
	Bucket* bucketArray;  
	int bucket_num;

public:
	HierarchicalSketch(int _bucket_num)
	{
		bucket_num = _bucket_num;

		bucketArray = new Bucket[bucket_num];

	}

	void Insert(unsigned int flow)
	{
		uint32_t j;
        uint32_t seed = 1;
        MurmurHash3_x86_32(&flow, 4, seed, &j);
        uint8_t fingerprint = j % 256;
        int bucket_idx = j % bucket_num;
        int cell_idx = j % 4;

        // heavy part
        bool flag1 = false; // indicate fingerprint match
        int max_count = 0xffffffff;
        int max_slot_idx = -1;
        if (bucketArray[bucket_idx].fingerprint1 == fingerprint) {
            bucketArray[bucket_idx].counter1 += 1;
            flag1 = true;
            if (bucketArray[bucket_idx].counter1 < max_count){
                max_count = bucketArray[bucket_idx].counter1;
                max_slot_idx = 0;
            }
        }
        else if (bucketArray[bucket_idx].fingerprint1 == 0) {
            bucketArray[bucket_idx].fingerprint1 = fingerprint;
            bucketArray[bucket_idx].counter1 = 1;
            flag1 = true;
            if (bucketArray[bucket_idx].counter1 < max_count){
                max_count = bucketArray[bucket_idx].counter1;
                max_slot_idx = 0;
            }
        }
        if (flag1 == false){
            if (bucketArray[bucket_idx].fingerprint2 == fingerprint) {
                bucketArray[bucket_idx].counter2 += 1;
                flag1 = true;
                if (bucketArray[bucket_idx].fingerprint2 < max_count){
                    max_count = bucketArray[bucket_idx].fingerprint2;
                    max_slot_idx = 1;
                }
            }
            else if (bucketArray[bucket_idx].fingerprint2 == 0) {
                bucketArray[bucket_idx].fingerprint2 = fingerprint;
                bucketArray[bucket_idx].counter2 = 1;
                flag1 = true;
                if (bucketArray[bucket_idx].counter2 < max_count){
                    max_count = bucketArray[bucket_idx].counter2;
                    max_slot_idx = 1;
                }
            }
        }
        if (flag1 == false){
            if (bucketArray[bucket_idx].fingerprint3 == fingerprint) {
                bucketArray[bucket_idx].counter3 += 1;
                flag1 = true;
                if (bucketArray[bucket_idx].counter3 < max_count){
                    max_count = bucketArray[bucket_idx].counter3;
                    max_slot_idx = 2;
                }
            }
            else if (bucketArray[bucket_idx].fingerprint3 == 0) {
                bucketArray[bucket_idx].fingerprint3 = fingerprint;
                bucketArray[bucket_idx].counter3 = 1;
                flag1 = true;
                if (bucketArray[bucket_idx].counter3 < max_count){
                    max_count = bucketArray[bucket_idx].counter3;
                    max_slot_idx = 2;
                }
            }
        }
        
        // light part
        if (flag1 == false){
            double randomVal = (double)rng()/4294967296;

            if (cell_idx == 0) {
                int exp = bucketArray[bucket_idx].l_slot1_2 >> 6;
                int co = (bucketArray[bucket_idx].l_slot1_2&0b00110000) >> 4;
                if (randomVal < (1/pow(exp, 2))){
                    co ++;
                    if (co == 3){
                        co = 0;
                        exp ++;
                        if (exp == 3){
                            exp = 0;
                            if (max_slot_idx == 0) {
                                bucketArray[bucket_idx].counter1 = 32;
                                bucketArray[bucket_idx].fingerprint1 = fingerprint;
                            }
                            if (max_slot_idx == 1) {
                                bucketArray[bucket_idx].counter2 = 32;
                                bucketArray[bucket_idx].fingerprint2 = fingerprint;
                            }
                            if (max_slot_idx == 2) {
                                bucketArray[bucket_idx].counter3 = 32;
                                bucketArray[bucket_idx].fingerprint3 = fingerprint;
                            }
                        }
                    }
                }
                bucketArray[bucket_idx].l_slot1_2 = (exp << 6) | (co << 4) | (bucketArray[bucket_idx].l_slot1_2&0b00001111);
            }
            else if (cell_idx == 1) {
                int exp = (bucketArray[bucket_idx].l_slot1_2&0b00001100) >> 2;
                int co = (bucketArray[bucket_idx].l_slot1_2&0b00000011);
                if (randomVal < (1/pow(exp, 2))){
                    co ++;
                    if (co == 3){
                        co = 0;
                        exp ++;
                        if (exp == 3){
                            exp = 0;
                            if (max_slot_idx == 0) {
                                bucketArray[bucket_idx].counter1 = 32;
                                bucketArray[bucket_idx].fingerprint1 = fingerprint;
                            }
                            if (max_slot_idx == 1) {
                                bucketArray[bucket_idx].counter2 = 32;
                                bucketArray[bucket_idx].fingerprint2 = fingerprint;
                            }
                            if (max_slot_idx == 2) {
                                bucketArray[bucket_idx].counter3 = 32;
                                bucketArray[bucket_idx].fingerprint3 = fingerprint;
                            }
                        }
                    }
                }
                bucketArray[bucket_idx].l_slot1_2 = (exp << 2) | (co) | (bucketArray[bucket_idx].l_slot1_2&0b11110000);
            }        
            else if (cell_idx == 2) {
                int exp = bucketArray[bucket_idx].l_slot3_4 >> 6;
                int co = (bucketArray[bucket_idx].l_slot3_4&0b00110000) >> 4;
                if (randomVal < (1/pow(exp, 2))){
                    co ++;
                    if (co == 3){
                        co = 0;
                        exp ++;
                        if (exp == 3){
                            exp = 0;
                            if (max_slot_idx == 0) {
                                bucketArray[bucket_idx].counter1 = 32;
                                bucketArray[bucket_idx].fingerprint1 = fingerprint;
                            }
                            if (max_slot_idx == 1) {
                                bucketArray[bucket_idx].counter2 = 32;
                                bucketArray[bucket_idx].fingerprint2 = fingerprint;
                            }
                            if (max_slot_idx == 2) {
                                bucketArray[bucket_idx].counter3 = 32;
                                bucketArray[bucket_idx].fingerprint3 = fingerprint;
                            }
                        }
                    }
                }
                bucketArray[bucket_idx].l_slot3_4 = (exp << 6) | (co << 4) | (bucketArray[bucket_idx].l_slot3_4&0b00001111);
            }
            else if (cell_idx == 3) {
                int exp = (bucketArray[bucket_idx].l_slot3_4&0b00001100) >> 2;
                int co = (bucketArray[bucket_idx].l_slot3_4&0b00000011);
                if (randomVal < (1/pow(exp, 2))){
                    co ++;
                    if (co == 3){
                        co = 0;
                        exp ++;
                        if (exp == 3){
                            exp = 0;
                            if (max_slot_idx == 0) {
                                bucketArray[bucket_idx].counter1 = 32;
                                bucketArray[bucket_idx].fingerprint1 = fingerprint;
                            }
                            if (max_slot_idx == 1) {
                                bucketArray[bucket_idx].counter2 = 32;
                                bucketArray[bucket_idx].fingerprint2 = fingerprint;
                            }
                            if (max_slot_idx == 2) {
                                bucketArray[bucket_idx].counter3 = 32;
                                bucketArray[bucket_idx].fingerprint3 = fingerprint;
                            }
                        }
                    }
                }
                bucketArray[bucket_idx].l_slot3_4 = (exp << 2) | (co) | (bucketArray[bucket_idx].l_slot3_4&0b11110000);
            }
        }
    
    }
};