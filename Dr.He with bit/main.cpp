#include <iostream>


#include <list>
#include <fstream>
#include<time.h>
#include <sdsl/bit_vectors.hpp>
#include<unistd.h>
#include<ios>
#include<string>
#include <bits/stdc++.h>


using namespace std;
using namespace sdsl;


class MaxHeap{
private:
    void heapify() {
        for (int i = size / 2 - 1; i >= 0; i--) {
            percolateDown(i);
        }
    }
    void swap(pair<int, int> * array, int i, int j) {
        pair<int, int> tmp = array[i];
        array[i] = array[j];
        array[j] = tmp;
        //cout << "i: " << i << " array[i]: " << array[i]<< " j: " << j << " array[j]: " << array[j] << endl;
    }
public:
    pair<int, int> * array;
    int size;
    int capacity;
    MaxHeap(int capacity) {
        this->array = new pair<int, int>[capacity];
        size = 0;
        this->capacity = capacity;
        heapify();
    }
    bool isEmpty() {
        return size == 0;
    }
    bool isFull() {
        return size == capacity;
    }
    void percolateUp(int index) {
        while (index > 0) {
            int parentIndex = (index - 1) / 2;
            if (array[parentIndex].first > array[index].first) {
                break;
            } else {
                swap(array, parentIndex, index);
            }
            index = parentIndex;
        }
    }
    void percolateDown(int index) {
        while (index <= size / 2 - 1) {
            int leftChildIndex = index * 2 + 1;
            int rightChildIndex = index * 2 + 2;
            int swapCandidate = leftChildIndex;
            if (rightChildIndex <= size - 1 && array[leftChildIndex].first < array[rightChildIndex].first) {
                swapCandidate = rightChildIndex;
            }
            if (array[index].first > array[swapCandidate].first) {
                break;
            } else {
                swap(array, swapCandidate, index);
            }
            index = swapCandidate;
        }
    }
    pair<int, int> peek() {
        return array[0];
    }
    pair<int, int> poll() {
        pair<int, int> res = array[0];
        array[0] = array[size - 1];
        size--;
        percolateDown(0);
        return res;
    }
    void offer(pair<int, int> ele) {
        //array[size] = ele;
        array[size] = ele;
        size++;
        //cout << "ele: " << ele << " size: " << size << endl;
        percolateUp(size - 1);
    }
    pair<int, int> update(int index, int ele) {
        pair<int, int> res = array[index];
        array[index].first = ele;
        if (res.first > ele) {
            percolateDown(index);
        } else {
            percolateUp(index);
        }
        return res;
    }
    int find(int ele) {
        for (int i = 0; i < size; i++) {
            if (array[i].second == ele) {
                return i;
            }
        }
        return -1;
    }
    void print() {
        for (int i = 0; i < size; i++) {
            cout << array[i].first << " " << array[i].second << "    ";
        }
        cout << endl;
    }
    void discard() {
        delete[] array;
    }
};

class ToolBox{
public:
    double epsilon;
    int len;
    int** Mij;

    int Qk_len;
    bit_vector *Qk;
    int* bitSize;
    select_support_mcl<0>* Qk_select;
    rank_support_v5<0>* Qk_rank_0;
    rank_support_v5<1>* Qk_rank_1;
    int* ceil_f_2k_minus_1;
    int * ceil_f_2k;

    bit_vector** Kr;

    select_support_mcl<1> **Kr_select;
    rank_support_v5<0> **Kr_rank_0;
    rank_support_v5<1> **Kr_rank_1;
    int* upper_line_array;
    MaxHeap* maxHeap;


    int k_upper_bound;
    int delta;
    double* freq_2k;
    double* freq_2k_rj;
    double* freq_third_small;
    double* freq_third_large;

    long* countArray;
    double* bitTime;

};
ToolBox toolBox;

void read_file(int array[], string file, int len);
void generate_query(int *tiny_query, int *med_query, int *huge_query, int query);
void process_mem_usage(double& vm_usage, double& resident_set);
void fouthApproximate(double epsilon, int len);

int getQki(int k, int i);
int findLowFreq(int start, int end);
int findSi(int start, int k);
int findSj(int start, int k);

int getRi(int curK, int i);
int getRj(int curK, int j);
int findRiHeap(int si, int prevSi ,int prevRi, double freq_2k);
int findLargeMode(int start, int end);


int main() {
    int len = 20;   // the length of input array
    cout << "please input the length of the array: ";
    cin >> len;
    double epsilon;
    cout << "please input the epsilon: ";
    cin >> epsilon;
    //int *array = new int[len];
    //string file_in = "/home/liu1/Desktop/data.txt";
    //read_file(array, file_in, len);
    clock_t program_start_time, program_end_time;
    program_start_time = clock();
    cout << endl;
    fouthApproximate(epsilon, len);
    program_end_time = clock();

    cout << "Program Total Time : " << (double)(program_end_time - program_start_time) / CLOCKS_PER_SEC <<" s " << endl;
    //delete[] array;
}
void fouthApproximate(double epsilon, int len) {
    clock_t constructStart, constructEnd;
    string file_in = "/users/grad/liu1/book100.txt";
    int *array = new int[len];
    read_file(array, file_in, len);
    int start_index = 0;
    int end_index = 0;
    int *tiny_query = new int[100000];
    int *med_query = new int[100000];
    int *huge_query = new int[100000];
    int *result = new int[300000];
    int *mode = new int[150000];
    generate_query(tiny_query, med_query, huge_query, 100000);
    toolBox.epsilon = epsilon;
    toolBox.len = len;
    double vm, rss;
    process_mem_usage(vm, rss);
    cout << "Start:" << "  " << "VM: " << vm << "; RSS: " << rss << endl;
    constructStart = clock();
    int Qk_len = ceil(1 / epsilon);
    int **Qi = new int*[Qk_len];
    //cout << "epsilon: " << epsilon << endl;

    //cout << "Qi_len: " << Qi_len << endl;
    for (int i = 0; i < Qk_len; i++) {
        Qi[i] = new int[len];
    }
    for (int i = 0; i < Qk_len; i++) {
        for (int j = 0; j < len; j++) {
            Qi[i][j] = -1;
        }
    }
    set<int> D;
    for (int i = 0; i < len; i++) {
        D.insert(array[i]);
    }
    /*set<int>::iterator iter = D.begin();
    while (iter != D.end()) {
        cout << *iter++ << ' ';
    }
    cout << endl;*/
    int delta = D.size();  // save the value of delta
    //cout << "delta: " << delta << endl;
    map<int, int> setMap;
    int count1 = 1;
    set<int>::iterator it;
    for(it=D.begin();it!=D.end();it++)  //get the upper_line_array  nlogΔ
    {
        setMap.insert(make_pair(*it, count1));
        count1++;
    }

    int *upper_line_array = new int[len];  // upper_line_array
    for (int i = 0; i < len; i++) {
        upper_line_array[i] = setMap[array[i]];
    }

    delete[] array;
    //cout << "delta: " << delta << endl;
    /* for (int i = 0; i < len; i++) {
         cout << i << " ";
     }
     cout << endl;
     for (int i = 0; i < len; i++) {
         cout << upper_line_array[i] << " ";
     }
     cout << endl;*/
    cout << endl;
    toolBox.upper_line_array = upper_line_array;
    clock_t Q_start, Q_end;
    Q_start = clock();
    int* freqArray = new int[delta + 1];
    int maxFreq = 0;
    for (int i = 0; i < delta + 1; i++) {
        freqArray[i] = 0;
    }


    int** Mij = new int*[len];
    for (int i = 0; i < len; i++) {
        Mij[i] = new int[Qk_len];
    }
    for (int i = 0; i < len; i++) {
        for (int k = 0; k < Qk_len; k++) {
            Mij[i][k] = -1;
        }
    }
    clock_t Original_Qk_start, Original_Qk_end, Qk_end;
    Original_Qk_start = clock();


    /*int** FreqQi = new int*[Qk_len];
    for (int i = 0; i < Qk_len; i++) {
        FreqQi[i] = new int[delta + 1];
    }
    int *f = new int[delta + 1];
    toolBox.f = f;
    toolBox.FreqQi = FreqQi;
    toolBox.Mij = Mij;
    toolBox.Qi = Qi;
    toolBox.Qk_len = Qk_len;
    for (int i = 0; i < len; i++) {
        for (int k = 0; k < Qk_len; k++) {

            updatedQi(i, len - 1, k, FreqQi);
            // cout << Qi[i][k] << " ";
        }
        // cout << endl;
    }

    for (int i = 0; i < Qk_len; i++) {
        delete[] FreqQi[i];
    }
    delete[] FreqQi;
    delete[] f;*/
    for (int i = 0; i < len; i++) {
        maxFreq = 0;
        int mode = -1;
        for (int j = i; j < len; j++) {
            freqArray[upper_line_array[j]]++;
            if (maxFreq < freqArray[upper_line_array[j]]) {
                maxFreq = freqArray[upper_line_array[j]];
                mode = upper_line_array[j];
                if (freqArray[upper_line_array[j]] - 1 < Qk_len) {
                    Qi[freqArray[upper_line_array[j]] - 1][i] = j;
                    Mij[i][freqArray[upper_line_array[j]] - 1] = mode;
                } else {
                    break;
                }
            } else if (maxFreq == freqArray[upper_line_array[j]]) {
                //maxFreq = freqArray[upper_line_array[j]];
                //mode = upper_line_array[j];
                if (freqArray[upper_line_array[j]] - 1 < Qk_len) {
                    Qi[freqArray[upper_line_array[j]] - 1][i] = j;
                    //Mij[i][freqArray[upper_line_array[j]] - 1] = mode;
                } else {
                    break;
                }
            } else {
                if (maxFreq - 1 < Qk_len) {
                    Qi[maxFreq - 1][i] = j;
                    //Mij[i][maxFreq - 1] = mode;
                } else {
                    break;
                }
            }

        }
        for (int i = 0; i < delta + 1; i++) {
            freqArray[i] = 0;
        }
    }
    delete []freqArray;

    Original_Qk_end = clock();
    cout << "Original QK Total Time : " << (double)(Original_Qk_end - Original_Qk_start) / CLOCKS_PER_SEC <<" s " << endl;


    /* for (int k = 0; k < Qk_len; k++) {
         cout << "k: " << k << "            ";
         for (int i = 0; i < len; i++) {
             cout << toolBox.Qi[k][i] << "  ";
         }
         cout << endl;
     }
     cout << endl;*/
    //cout << '
    int *bitSize = new int[Qk_len];
    for (int k = 0; k < Qk_len; k++) {
        bitSize[k] = 0;
        for (int j = len - 1; j >= 0; j--) {
            if (Qi[k][j] != -1) {
                bitSize[k] = Qi[k][j] + j + 1;
                break;
            }
        }
    }
    toolBox.bitSize = bitSize;
    /*for (int k = 0; k < Qk_len; k++) {
        cout << "bitSize[k]: " << bitSize[k] << endl;
    }*/

    bit_vector *Qk = new bit_vector[Qk_len];
    select_support_mcl<0>* Qk_select = new select_support_mcl<0>[Qk_len];
    rank_support_v5<1>* Qk_rank_1 = new rank_support_v5<1>[Qk_len];
    rank_support_v5<0>* Qk_rank_0 = new rank_support_v5<0>[Qk_len];

    for (int k = 0; k < Qk_len; k++) {
        bit_vector bk = bit_vector(bitSize[k], 0);
        int index = 0, pre = 0;
        //cout << "00k: " << k << endl;
        //cout <<"k: " << k << "            " << "size: " <<  bitSize[k] << endl;
        for (int i = 0; i < len; i++) {
            if (Qi[k][i] != -1) {
                int count = Qi[k][i] - pre;
                for (int c = 0; c < count; c++) {
                    bk[index++] = 1;
                }
                bk[index++] = 0;
                pre = Qi[k][i];
            }
        }
        //cout << "111k: " << k << endl;
        Qk[k] = bk;
        select_support_mcl<0> select_0_type(&Qk[k]);
        rank_support_v5<1> bit_rank_1 (&Qk[k]);
        rank_support_v5<0> bit_rank_0 (&Qk[k]);
        Qk_select[k] = select_0_type;
        Qk_rank_0[k] = bit_rank_0;
        Qk_rank_1[k] = bit_rank_1;

    }

    toolBox.Qk = Qk;
    toolBox.Qk_select = Qk_select;
    toolBox.Qk_rank_0 = Qk_rank_0;
    toolBox.Qk_rank_1 = Qk_rank_1;
    toolBox.Qk_len = Qk_len;
    cout << "QK done !!! " << endl;
    Q_end = clock();
    cout << " Q Total Time : " << (double) (Q_end - Original_Qk_end) / CLOCKS_PER_SEC << " s "
         << endl;
    //process_mem_usage(vm, rss);
    //cout << "before delete original Qk:" << "  " << "VM: " << vm << "; RSS: " << rss << endl;
    for (int i = 0; i < Qk_len; i++) {
        delete[] Qi[i];
    }
    delete[] Qi;
    //process_mem_usage(vm, rss);
    //cout << "after delete original Qk:" << "  " << "VM: " << vm << "; RSS: " << rss << endl;
    /*for (int k = 0; k < Qk_len; k++) {
        cout <<"k: " << k << "            " << Qk[k] << endl;
    }*/
    /*for (int k = 0; k < Qk_len; k++) {
        cout << "k: " << k << "            ";
        for (int i = 0; i < len; i++) {
            int res = getQki(Qk, k, i, bitSize);
            cout << res << "  ";
        }
        cout << endl;
    }*/
    /*int left = start_index - 1, right = end_index - 1;
    left = 0, right = 6;
    int res = findLowFreq(Qk, Qk_len, left, right, bitSize);
    cout << "res: " << res << endl;*/

    //Large Frequence
    int k_upper_bound = floor(log(epsilon * len) / log(1 + epsilon));
    //cout << "k_upper_bound: " << k_upper_bound << endl;

    bit_vector **Kr = new bit_vector*[k_upper_bound];
    for (int i = 0; i < k_upper_bound; i++) {
        Kr[i] = new bit_vector[2];
    }
    select_support_mcl<1> **Kr_select = new select_support_mcl<1>*[k_upper_bound];
    for (int i = 0; i < k_upper_bound; i++) {
        Kr_select[i] = new select_support_mcl<1>[2];
    }
    rank_support_v5<0> **Kr_rank_0 = new rank_support_v5<0>*[k_upper_bound];
    for (int i = 0; i < k_upper_bound; i++) {
        Kr_rank_0 [i] = new rank_support_v5<0>[2];
    }
    rank_support_v5<1> **Kr_rank_1 = new rank_support_v5<1>*[k_upper_bound];
    for (int i = 0; i < k_upper_bound; i++) {
        Kr_rank_1[i] = new rank_support_v5<1>[2];
    }



    clock_t testStart, testEnd, test2;
    cout << "t: " << k_upper_bound << endl;
    //k < k_upper_bound
    int* ceil_f_2k_minus_1 = new int[k_upper_bound];
    int* ceil_f_2k = new int[k_upper_bound];

    toolBox.ceil_f_2k = ceil_f_2k;
    toolBox.ceil_f_2k_minus_1 = ceil_f_2k_minus_1;
    toolBox.k_upper_bound = k_upper_bound;
    toolBox.delta = delta;

    //cout << "22222" << endl;
    testStart = clock();
    for (int k = 0; k < k_upper_bound; k++) {
        // cout << "  111111" <<endl;
        cout << " k:  " << k << "    " << endl;


        int real_k = k + 1;
        ceil_f_2k_minus_1[k] = ceil(((sqrt(1 + epsilon) - 1) / epsilon) * (pow(sqrt(1 + epsilon) , 2 * real_k - 1)));
        ceil_f_2k[k] = ceil(((sqrt(1 + epsilon) - 1) / epsilon) * (pow(sqrt(1 + epsilon) , 2 * real_k)));;
        //cout <<"k: " << real_k <<"  2k - 1 size: " << ceil_f_2k_minus_1 << " 2k size: " << ceil_f_2k << endl;
        //cout << "ehrherher" << endl;
        bit_vector bit_ri = bit_vector(len + len / ceil_f_2k_minus_1[k] + 1 , 0);
        double freq_2k = (pow(sqrt(1 + epsilon), 2 * real_k) / epsilon);
        // cout << len / ceil_f_2k_minus_1[k] << endl;
        MaxHeap *maxHeap = new MaxHeap(delta + 1);
        toolBox.maxHeap = maxHeap;
        int prevRi = 0;
        int index = 0;
        for (int i = 0; i <= len / ceil_f_2k_minus_1[k]; i++) {
            int si = i * ceil_f_2k_minus_1[k];
            if (si >= len) {
                break;
            }

            // cout << "si: " << si << " ";
            int prevSi = 0;

            if (i != 0) {
                prevSi = (i - 1) * ceil_f_2k_minus_1[k];
            }
            int ri = findRiHeap(si, prevSi, prevRi, freq_2k);

            if (ri != -1) {
                int diff = ri - prevRi;
                index += diff;
                bit_ri[index++] = 1;
            } else {
                break;
            }
            prevRi = ri;
        }

        Kr[k][0] = bit_ri;
        select_support_mcl<1> select_1_type(&Kr[k][0]);
        Kr_select[k][0] = select_1_type;
        rank_support_v5<0> bit_rank_0 (&Kr[k][0]);
        rank_support_v5<1> bit_rank_1 (&Kr[k][0]);
        Kr_rank_0[k][0] = bit_rank_0;
        Kr_rank_1[k][0] = bit_rank_1;

        maxHeap->discard();
        free(maxHeap);
        MaxHeap *maxHeap2 = new MaxHeap(delta + 1);
        toolBox.maxHeap = maxHeap2;
        int prevRj = 0;
        index = 0;
        //  cout << "333333333" << endl;
        //cout << "-----------------------------------------------" << endl;
        bit_vector bit_rj = bit_vector(len + len / ceil_f_2k[k] + 1 , 0);
        for (int i = 0; i <= len / ceil_f_2k[k]; i++) {
            int sj = i * ceil_f_2k[k];
            if (sj >= len) {
                break;
            }
            //cout << "sj: " << sj << " ";

            double freq_2k = (pow(sqrt(1 + epsilon), 2 * real_k + 1) / epsilon);
            int prevSj = 0;

            if (i != 0) {
                prevSj = (i - 1) * ceil_f_2k[k];
            }
            int rj = findRiHeap(sj, prevSj, prevRj, freq_2k);

            if (rj != -1) {
                int diff = rj - prevRj;
                index += diff;
                bit_rj[index++] = 1;
            } else {
                break;
            }
            prevRj = rj;

        }

        Kr[k][1] = bit_rj;
        select_support_mcl<1> select2_1_type(&Kr[k][1]);
        Kr_select[k][1] = select2_1_type;
        rank_support_v5<0> bit2_rank_0 (&Kr[k][1]);
        rank_support_v5<1> bit2_rank_1 (&Kr[k][1]);
        Kr_rank_0[k][1] = bit2_rank_0;
        Kr_rank_1[k][1] = bit2_rank_1;

        maxHeap2->discard();
        free(maxHeap2);

    }
    cout << "Kr done !!!! " << endl;
    double* freq_2k = new double[k_upper_bound];
    double* freq_2k_rj = new double[k_upper_bound];
    double* freq_third_small = new double[k_upper_bound];
    double* freq_third_large = new double[k_upper_bound];
    for (int k = 0; k < k_upper_bound; k++) {
        freq_2k[k] = (pow(sqrt(1 + epsilon), 2 * (k + 1)) / epsilon);
        freq_2k_rj[k] = (pow(sqrt(1 + epsilon), 2 * (k + 1) + 1) / epsilon);
        freq_third_small[k] = pow(1 + epsilon, (double)((k + 1) - (double) 1 / 2)) / epsilon;
        freq_third_large[k] = pow(1 + epsilon, (double)((k + 1) + (double) 1 / 2)) / epsilon;
    }
    constructEnd = clock();

    toolBox.Kr = Kr;
    toolBox.Kr_select = Kr_select;
    toolBox.Kr_rank_0 = Kr_rank_0;
    toolBox.Kr_rank_1 = Kr_rank_1;
    toolBox.freq_2k = freq_2k;
    toolBox.freq_2k_rj = freq_2k_rj;
    toolBox.freq_third_small = freq_third_small;
    toolBox.freq_third_large = freq_third_large;
    cout << "Construction Time : " << (double)(constructEnd - constructStart) / CLOCKS_PER_SEC <<" s " << endl;
    process_mem_usage(vm, rss);
    cout << "Construction memeory:" << "  " << "VM: " << vm << "; RSS: " << rss << endl;
    cout <<endl;

    long* countArray = new long[1];
    double* bitTime = new double[1];
    countArray[0] = 0, bitTime[0] = 0;

    toolBox.countArray = countArray;
    toolBox.bitTime = bitTime;

    clock_t tiny_start_time, tiny_end_time, med_start_time, med_end_time, huge_start_time, huge_end_time;
    for (int i = 0; i < 300000; i+=2) {            // tiny 0 - 999  med 1000 - 1999 huge 2000-2999
        // save mode
        int maxFreq = 0;   // save the freq of the mode
        if (i < 100000) {
            //continue;
            if (i == 0) {
                tiny_start_time = clock();
                cout << "Tiny Query is:" <<endl;
            }
            //cout << i <<" ";
            if (i == 99998) {
                tiny_end_time = clock();
                cout << " Tiny Query Total Time : " << (double)(tiny_end_time - tiny_start_time) / CLOCKS_PER_SEC <<" s " << endl;
            }
            start_index = tiny_query[i];
            end_index = tiny_query[i + 1];
            //cout << "Num: " << i / 2 << " start: " << start_index << " end: " << end_index;
        } else if (i < 200000) {
            // continue;
            if (i == 100000) {
                med_start_time = clock();
                cout << "Med Query is:" <<endl;
            }
            //cout << i <<" ";
            start_index = med_query[i - 100000];
            end_index = med_query[i + 1 - 100000];
            if (i == 199998) {
                med_end_time = clock();
                cout << " Med Query Total Time : " << (double)(med_end_time - med_start_time) / CLOCKS_PER_SEC <<" s " << endl;
            }
        } else {
            if (i == 200000) {
                cout << "Huge Query is:" <<endl;
                huge_start_time = clock();
            }
            // cout << i <<" ";
            start_index = huge_query[i - 200000];
            end_index = huge_query[i + 1 - 200000];
        }

        //start_index = 1, end_index = 18;
        int left = start_index - 1, right = end_index - 1;

        //cout << "i: "<< i << "  left: " << left << "  right: " << right << endl;

        int tmpResult = findLowFreq(left, right);


        double smallFreq = -1, largeFreq = -1;
        int finalMode = -1;

        if (tmpResult != -1) {
            finalMode = Mij[left][tmpResult - 1];

            //smallFreq = tmpResult;
            //largeFreq = tmpResult;
        } else {


            /*pair<double, double> res = findLargeFreq(left, right, Kr, upper_line_array, k_upper_bound, epsilon, delta, len, ceil_f_2k_minus_1, ceil_f_2k, freq_2k, freq_2k_rj,freq_third_small, freq_third_large);
            smallFreq = res.first;
            largeFreq = res.second;*/

            finalMode = findLargeMode(left, right);


        }
        //result[i] = smallFreq;
        //result[i + 1] = largeFreq;
        mode[(i / 2)] = finalMode;
        //cout << "finalMode: " << finalMode << endl;

    } // end of for query
    huge_end_time = clock();
    cout << " Huge Query Total Time : " << (double) (huge_end_time - huge_start_time) / CLOCKS_PER_SEC << " s "
         << endl;

    cout << "bit count: " << countArray[0] << endl;
    cout << "bit operation time: " << bitTime[0] << endl<<endl;

    // int left = 0, right = 19;
    for (int i = 0; i < 300000; i+=2) {            // tiny 0 - 999  med 1000 - 1999 huge 2000-2999
        if (i < 100000) {
            start_index = tiny_query[i];
            end_index = tiny_query[i + 1];
            //cout << "Num: " << i / 2 << " start: " << start_index << " end: " << end_index;
        } else if (i < 200000) {
            start_index = med_query[i - 100000];
            end_index = med_query[i + 1 - 100000];
        } else {
            start_index = huge_query[i - 200000];
            end_index = huge_query[i + 1 - 200000];
        }
        // cout << "mode:  " <<mode[i / 2] << "  ";
        int left = start_index - 1, right = end_index - 1;
        int freq = 0;
        for (int index = left; index <= right; index++) {
            if (mode[i / 2] == upper_line_array[index]) {
                freq++;
            }
        }

        //  cout << "  freq: " << freq << endl;
        mode[i / 2] = freq;
    }

    // cout << "smallFreq: " << smallFreq << "  largeFreq: " << largeFreq << endl;
    //cout << (pow(1 + epsilon, (2  - 1 + 1))) / epsilon << endl;
    //pair<double, double> res = findLargeFreq(left, right, Kr, upper_line_array, k_upper_bound, epsilon, delta, len);
    //cout << "small: " << res.first << "  large: " << res.second << endl;
    /*int si = findSi(19, 4, epsilon, len);
    cout << "si: " << si << endl;
    int sj = findSj(4, 4, epsilon, len);
    cout << "sj: " << sj << endl;*/


    cout << endl;
    //cout << "@22222222" <<endl;
    /*for (int i = 0; i < k_front_half_bound; i++) {
        delete[] Bl[i];
    }*/
    process_mem_usage(vm, rss);
    cout << "End(Large Frequency):" << "  " << "VM: " << vm << "; RSS: " << rss << endl;
    ofstream moderesult("/users/grad/liu1/HeplusBitMode.txt");
    for (int i = 0; i < 150000; i++) {
        //cout << "i: " << i <<"  "<< result[i] << endl;
        //cout << result[i] << "  ";
        moderesult << mode[i] << " ";
        if ((i + 1) % 10 == 0) {
            moderesult << endl;
        }
    }
    if (moderesult) {
        cout << endl;
        cout << "create first result file successfully!" << endl;
    }
    moderesult.close();
    moderesult.clear();
    delete[] mode;
    /*ofstream first_result("/users/grad/liu1/Heplus_result.txt");
    for (int i = 0; i < 300000; i++) {
        //cout << "i: " << i <<"  "<< result[i] << endl;
        //cout << result[i] << "  ";
        first_result << result[i] << " ";
        if ((i + 1) % 10 == 0) {
            first_result << endl;
        }
    }
    if (first_result) {
        cout << endl;
        cout << "create first result file successfully!" << endl;
    }
    first_result.close();
    first_result.clear();
    delete[] result;*/
    for (int i = 0; i < len; i++) {
        delete[] Mij[i];
    }
    delete[] Mij;
    delete[] bitSize;


    for (int i = 0; i < k_upper_bound; i++) {
        delete[] Kr[i];
    }
    delete[] Kr;
    for (int i = 0; i < k_upper_bound; i++) {
        delete[] Kr_select[i];
    }
    delete[] Kr_select;
    for (int i = 0; i < k_upper_bound; i++) {
        delete[] Kr_rank_0[i];
    }
    delete[] Kr_rank_0;
    for (int i = 0; i < k_upper_bound; i++) {
        delete[] Kr_rank_1[i];
    }
    delete[] Kr_rank_1;
    delete[] Qk;
    delete[] Qk_select;
    delete[] Qk_rank_0;
    delete[] Qk_rank_1;
    delete[] ceil_f_2k_minus_1;
    delete[] ceil_f_2k;
    delete[] freq_2k;
    delete[] freq_2k_rj;
    delete[] freq_third_small;
    delete[] freq_third_large;
    cout << endl;
}
int findLargeMode(int start, int end) {
    int len = toolBox.len;
    int mode = -1;
    int leftK = 0, rightK = toolBox.k_upper_bound - 1;
    while (leftK < rightK - 1) {
        int midK = leftK + (rightK - leftK) / 2;
        //cout << "midK: " << midK <<endl;
        int si = findSi(start, midK);
        int sj = findSj(start, midK);
        int ri = getRi(midK, si);
        int rj = getRj(midK, sj);
        //cout << "si: " << si << " sj: " << sj << endl;
        //cout << "ri: " << ri << " rj: " << rj << endl;
        if (end < ri || rj == -1) {
            rightK = midK;
        } else if (end >= rj) {
            leftK = midK;
        } else if (end >= ri && end < rj) {
            //  cout << "00000000" << endl;
            // cout << "herhere " << " midK: " << (double)((midK + 1) - (double) 1 / 2) << endl;
            mode = toolBox.upper_line_array[ri];

            return mode;
        }
    }

    //cout << "leftK: " << leftK << endl;
    //cout << "rightK: " << rightK << endl;
    int si_leftK = findSi(start, leftK);
    int sj_leftK = findSj(start, leftK);
    int ri_leftK = getRi(leftK, si_leftK);
    int rj_leftK = getRj(leftK, sj_leftK);
    if (ri_leftK == -1) {
        ri_leftK = len;
    }
    if (rj_leftK == -1) {
        rj_leftK = len;
    }
    //  cout << "si_leftK: " << si_leftK << " sj_leftK: " << sj_leftK << endl;
    //  cout << "ri_leftK: " << ri_leftK << " rj_leftK: " << rj_leftK << endl;
    if (end >= ri_leftK && (end < rj_leftK || rj_leftK == -1)) {
        //   cout << "111111 " << endl;
        mode = toolBox.upper_line_array[ri_leftK];

        return mode;
    } else if (end < ri_leftK) {
        if (leftK - 1 >= 0) {
            leftK = leftK - 1;
            int leftK_last_sj_new = findSj(start, leftK);
            //double freq_2k_rj_new = (pow(sqrt(1 + epsilon), 2 * (leftK + 1) + 1) / epsilon);
            int rj_leftK_new = getRj(leftK, leftK_last_sj_new);
            if (end >= rj_leftK_new) {
                //    cout << "22222222 " << endl;
                mode = toolBox.upper_line_array[rj_leftK_new];

                return mode;
            }
        }
    } else if (end >= rj_leftK) {
        //cout << "hererere " << endl;
        if (leftK + 1 <= toolBox.k_upper_bound - 1) {
            leftK = leftK + 1;
            int si_leftK_new = findSi(start, leftK);
            //double freq_2k_new = (pow(sqrt(1 + epsilon), 2 * (leftK + 1)) / epsilon);
            int ri_leftK_new = getRi(leftK, si_leftK_new);
            //cout << "ri_leftK_new " << ri_leftK_new << endl;
            if (end < ri_leftK_new || ri_leftK_new == -1) {
                //     cout << "3333 " << endl;
                mode = toolBox.upper_line_array[rj_leftK];

                return mode;
            }
        }
    }


    int si_rightK = findSi(start, rightK);
    int sj_rightK = findSj(start, rightK);
    int ri_rightK = getRi(rightK, si_rightK);
    int rj_rightK = getRj(rightK, sj_rightK);
    if (ri_rightK == -1) {
        ri_rightK = len;
    }
    if (rj_rightK == -1) {
        rj_rightK = len;
    }
    //cout << "ri_rightK: "<< ri_rightK << endl;
    //cout << "rj_rightK: "<< rj_rightK << endl;

    if (end >= ri_rightK && (end < rj_rightK || rj_rightK == -1)) {
        //  cout << "111111 " << endl;
        mode = toolBox.upper_line_array[ri_rightK];

        return mode;
    } else if (end < ri_rightK) {
        if (rightK - 1 >= 0) {
            rightK = rightK - 1;
            int rightK_last_sj_new = findSj(start, rightK);

            int rj_rightK_new = getRj(rightK, rightK_last_sj_new);
            if (end >= rj_rightK_new) {
                // cout << "2222222 " << endl;
                mode = toolBox.upper_line_array[rj_rightK_new];

                return mode;
            }
        }
    } else if (end >= rj_rightK) {
        if (rightK + 1 <= toolBox.k_upper_bound - 1) {
            rightK = rightK + 1;
            int si_rightK_new = findSi(start, rightK);

            int ri_rightK_new = getRi(rightK, si_rightK_new);
            if (end < ri_rightK_new || ri_rightK_new == -1) {
                //cout << "rj_rightK: "<< rj_rightK << endl;
                //     cout << "3333333" << endl;
                mode = toolBox.upper_line_array[rj_rightK];

                return mode;

            }
        }
    }



    return mode;
}

int getRi(int curK, int i){
    int res = -1;

    //bit_vector bit = toolBox.Kr[curK][0];
    clock_t s1, e1;
    /*select_support_mcl<1> select_1_type(&bit);
    rank_support_v5<0> bit_rank_0 (&bit);
    rank_support_v5<1> bit_rank_1 (&bit);*/
    //cout << "curK: " << curK << endl;
    //cout << "bit: " <<bit.size() << endl;
    //rank_support_v5<1> bit_rank_1 (&bit);
    //cout << "2222222" << bit_rank_1(bit.size()) << endl;
    // cout << "Kr_rank_1[curK][0](bit.size()): " <<Kr_rank_1[curK][0](bit.size() - 1) << endl;
    toolBox.countArray[0]++;

    s1 = clock();
    int tmp = toolBox.Kr_rank_1[curK][0](toolBox.Kr[curK][0].size());
    e1 = clock();
    toolBox.bitTime[0] += (double) (e1 - s1) / CLOCKS_PER_SEC;
    if (tmp < i + 1) {
        return res;
    }

    s1 = clock();
    int pos = toolBox.Kr_select[curK][0](i + 1);
    e1 = clock();
    toolBox.countArray[0]++;
    toolBox.bitTime[0] += (double) (e1 - s1) / CLOCKS_PER_SEC;

    if (pos == toolBox.Kr[curK][0].size()) {
        return res;
    }

    s1 = clock();
    res = toolBox.Kr_rank_0[curK][0](pos);
    e1 = clock();
    toolBox.countArray[0]++;
    toolBox.bitTime[0] += (double) (e1 - s1) / CLOCKS_PER_SEC;

    return res;
}
int getRj(int curK, int j){
    int res = -1;
    //bit_vector bit  = toolBox.Kr[curK][1];

    /*select_support_mcl<1> select_1_type(&bit);
    rank_support_v5<0> bit_rank_0 (&bit);
    rank_support_v5<1> bit_rank_1 (&bit);*/
    //cout << "bit2: " <<bit << endl;
    clock_t s1, e1;
    s1 = clock();
    int tmp = toolBox.Kr_rank_1[curK][1](toolBox.Kr[curK][1].size());
    e1 = clock();
    toolBox.countArray[0]++;

    toolBox.bitTime[0] += (double) (e1 - s1) / CLOCKS_PER_SEC;
    if (tmp < j + 1) {
        return res;
    }

    s1 = clock();
    int pos = toolBox.Kr_select[curK][1](j + 1);
    e1 = clock();
    toolBox.countArray[0]++;
    toolBox.bitTime[0] += (double) (e1 - s1) / CLOCKS_PER_SEC;

    if (pos == toolBox.Kr[curK][1].size()) {
        return res;
    }

    s1 = clock();
    res = toolBox.Kr_rank_0[curK][1](pos);
    e1 = clock();
    toolBox.countArray[0]++;
    toolBox.bitTime[0] += (double) (e1 - s1) / CLOCKS_PER_SEC;

    return res;
}
int findSi(int start,  int k) {
    int res = -1;
    int time = start / toolBox.ceil_f_2k_minus_1[k];
    if (time == 0) {
        res = 0;
    } else {
        res = time;
    }
    return res;
}
int findSj(int start,  int k) { // curK = 0, 1, 2, 3...
    int res = -1;
    int time = start / toolBox.ceil_f_2k[k];
    if (time == 0) {
        res = 0;
    } else {
        res = time;
    }
    return res;
}

//smallest larger or equal than end
int findRiHeap( int si, int prevSi , int prevRi, double freq_2k) {
    int len = toolBox.len;
    int left = si;
    int maxFreq = 0;
    if (left == 0) {
        for (int i = left; i < len; i++) {
            int count = 0;
            int index = toolBox.maxHeap->find(toolBox.upper_line_array[i]);
            if (index != -1) {
                count = toolBox.maxHeap->array[index].first;
                toolBox.maxHeap->update(index, count + 1);
                count = count + 1;
            } else { // first time appear
                toolBox.maxHeap->offer(make_pair(1, toolBox.upper_line_array[i]));
                count = 1;
            }
            if (maxFreq < count) {
                maxFreq = count;
                if (maxFreq >= freq_2k) {
                    //cout << "freq_2k: " << freq_2k << endl;
                    //cout << "left: " << i << endl;
                    return i;
                }
            }
            if (i == len - 1) {
                return -1;
            }
        }
    } else {
        //cout << "  hererer   " << maxHeap->peek().first << endl;
        // cout << "left: " << left  << "   prevSI: " << prevSi << endl;
        if (prevRi == -1) {
            return -1;
        }
        for (int i = prevSi; i < left; i++) {
            int index = toolBox.maxHeap->find(toolBox.upper_line_array[i]);
            int count = 0;
            if (index != -1) {
                count = toolBox.maxHeap->array[index].first;
                toolBox.maxHeap->update(index, count - 1);
            }
        }
        int maxFreq = toolBox.maxHeap->peek().first;
        if (maxFreq >= freq_2k) {
            return prevRi;
        } else {
            for (int start = prevRi + 1; start < len; start++) {
                int count = 0;
                int index = toolBox.maxHeap->find(toolBox.upper_line_array[start]);
                if (index != -1) {
                    count = toolBox.maxHeap->array[index].first;
                    toolBox.maxHeap->update(index, count + 1);
                    count = count + 1;
                } else { // first time appear
                    toolBox.maxHeap->offer(make_pair(1, toolBox.upper_line_array[start]));
                    count = 1;
                }
                if (maxFreq < count) {
                    maxFreq = count;
                    if (maxFreq >= freq_2k) {
                        //cout << "freq_2k: " << freq_2k << endl;
                        //cout << "left: " << i << endl;
                        return start;
                    }
                }
                if (start == len - 1) {
                    return -1;
                }
            }
        }
        return 0;
    }
}
int findLowFreq(int start, int end ) {

    int res = -1;
    int leftK = 0, rightK = toolBox.Qk_len - 1;
    while (leftK < rightK - 1) {
        int midK = leftK + (rightK - leftK) / 2;
        int tmp = getQki(midK, start);
        if (tmp >= end  || tmp == -1) {
            rightK = midK;
        } else if (tmp < end) {
            leftK = midK + 1;
        }
    }

    if (getQki(leftK, start) >= end) {
        res = leftK + 1;

        return res;
    }
    if (getQki(rightK, start) >= end) {
        res = rightK + 1;

        return res;
    }

    return res;
}
int getQki(int k, int i) { //k: 0

    //bit_vector bit = toolBox.Qk[k];
    clock_t s1, e1;

    //select_support_mcl<0> select_0_type(&bit);
    //rank_support_v5<1> bit_rank_1 (&bit);
    s1 = clock();
    int tmp = toolBox.Qk_rank_0[k](toolBox.bitSize[k]);
    e1 = clock();
    toolBox.countArray[0]++;

    toolBox.bitTime[0] += (double) (e1 - s1) / CLOCKS_PER_SEC;

    if (tmp < i + 1) {
        return -1;
    }

    s1 = clock();
    int pos_i_0 = toolBox.Qk_select[k](i + 1); // i + 1
    e1 = clock();
    toolBox.countArray[0]++;
    toolBox.bitTime[0] += (double) (e1 - s1) / CLOCKS_PER_SEC;

    if (pos_i_0 == toolBox.bitSize[k]) {
        return -1;
    }

    s1 = clock();
    int num_1 = toolBox.Qk_rank_1[k](pos_i_0);
    e1 = clock();
    toolBox.countArray[0]++;
    toolBox.bitTime[0] += (double) (e1 - s1) / CLOCKS_PER_SEC;

    return num_1;
}
void read_file(int array[], string file, int len) {
    ifstream file_in(file);
    int temp;
    if (!file_in.is_open()) {
        cout << " cannot open the file" << endl ;
    }
    for (int i = 0; i < len; i++) {
        file_in >> array[i];
    }
    /*for (int i = 0; i < len; i++) {
        cout << array[i] << endl;
    }*/
    file_in.close();
    file_in.clear();
}
void generate_query(int *tiny_query, int *med_query, int *huge_query, int query){
    int len = query;
    string tiny_file = "/users/grad/liu1/tiny.txt";
    string med_file = "/users/grad/liu1/med.txt";
    string huge_file = "/users/grad/liu1/huge.txt";
    ifstream tiny_in(tiny_file);
    ifstream med_in(med_file);
    ifstream huge_in(huge_file);
    if (!tiny_in.is_open()) {
        cout << " cannot open the tiny file" << endl;
    }
    for (int i = 0; i < len; i++) {
        tiny_in >> tiny_query[i];
    }
    if (!med_in.is_open()) {
        cout << " cannot open the med file" << endl;
    }
    for (int i = 0; i < len; i++) {
        med_in >> med_query[i];
    }
    if (!huge_in.is_open()) {
        cout << " cannot open the huge file" << endl;
    }
    for (int i = 0; i < len; i++) {
        huge_in >> huge_query[i];
    }
    tiny_in.close();
    tiny_in.clear();
    med_in.close();
    med_in.clear();
    huge_in.close();
    huge_in.clear();
}
void process_mem_usage(double& vm_usage, double& resident_set)
{
    using std::ios_base;
    using std::ifstream;
    using std::string;

    vm_usage     = 0.0;
    resident_set = 0.0;

    // 'file' stat seems to give the most reliable results
    //
    ifstream stat_stream("/proc/self/stat",ios_base::in);

    // dummy vars for leading entries in stat that we don't care about
    //
    string pid, comm, state, ppid, pgrp, session, tty_nr;
    string tpgid, flags, minflt, cminflt, majflt, cmajflt;
    string utime, stime, cutime, cstime, priority, nice;
    string O, itrealvalue, starttime;

    // the two fields we want
    //
    unsigned long vsize;
    long rss;

    stat_stream >> pid >> comm >> state >> ppid >> pgrp >> session >> tty_nr
                >> tpgid >> flags >> minflt >> cminflt >> majflt >> cmajflt
                >> utime >> stime >> cutime >> cstime >> priority >> nice
                >> O >> itrealvalue >> starttime >> vsize >> rss; // don't care about the rest

    stat_stream.close();

    long page_size_kb = sysconf(_SC_PAGE_SIZE) / 1024; // in case x86-64 is configured to use 2MB pages
    vm_usage     = vsize / 1024.0;
    resident_set = rss * page_size_kb;
}