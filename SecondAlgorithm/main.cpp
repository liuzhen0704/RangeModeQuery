#include <iostream>
#include <vector>
#include <sdsl/bit_vectors.hpp>
#include <bitset>
#include <iostream>
#include <set>
#include <map>
#include <cmath>
#include <list>
#include <fstream>
#include<time.h>
#include<unistd.h>
#include<ios>
#include<string>
#include <sdsl/suffix_trees.hpp>



using namespace std;
using namespace sdsl;

int Timothy_Second(int array[], int len, int s);
int getSize(int upper_line_array[], int *bitSize, int s, int t, int len);
bit_vector getBit(int bi, std::vector<unsigned int> word, int *bitSize);
bool check(int array[], int ax, int x, int bi, int t);
void process_mem_usage(double& vm_usage, double& resident_set);
void generate_query(int *tiny_query, int *med_query, int *huge_query, int query);
void read_file(int array[], string file, int len);
int getCandidateFreq(int upper_line_array[], int A_Prime[], map<int,int> QaMap, int len, int **Qa, int freq_c, int x, int i, int j);
int getSuffixCandidateFreq(int upper_line_array[], int A_Prime[], map<int,int> QaMap, int len, int **Qa, int freq_c, int x, int i, int j);


int main() {
    // int array[] = {3, 5, 10, 5, 10, 10, 2, 3};  // input array;
    //   int array[] = {40, 10, 30, 20, 40, 30, 20, 40, 20, 20, 10, 40, 40, 20, 10, 20, 30, 50, 50, 40, 10, 10, 40, 50};
    int len;   // the length of input array
    cout << "please input the length of the array: ";
    cin >> len;
    int s = ceil(sqrt(len * log(len)));     // fixed value : the number of blocks
    //cout << " s: " << s << endl;
    int *array = new int[len];
    string file_in = "/home/liu1/Desktop/data.txt";
    read_file(array, file_in, len);

    // std::cout << "Please input the value of s!" << std::endl;
    // cin >> s;
    /* int start_index = 0;
     int end_index = 0;
     cout << "please input the start point index: ";
     cin >> start_index;
     cout << "please input the end point index: ";
     cin >> end_index;*/
    clock_t program_start_time, program_end_time;
    program_start_time = clock();
    Timothy_Second(array, len, s);
    program_end_time = clock();
    cout << "Program Total Time : " << (double)(program_end_time - program_start_time) / CLOCKS_PER_SEC <<" s " << endl;
    delete[] array;
    return 0;
}

   