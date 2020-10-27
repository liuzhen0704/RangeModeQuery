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
int Timothy_Second(int array[], int len, int s)
{
    int start_index = 0;
    int end_index = 0;
    int *tiny_query = new int[2000];
    int *med_query = new int[2000];
    int *huge_query = new int[2000];
    int *result = new int[3000];
    // int second_point = rand_point(len);
    generate_query(tiny_query, med_query, huge_query, 2000);
    int  t = (int)ceil((float)len/(float)s);
    double vm, rss;
    process_mem_usage(vm, rss);
    cout << "Start:" << "  " << "VM: " << vm << "; RSS: " << rss << endl;
    set<int> D;
    for (int i = 0; i < len; i++) {
        D.insert(array[i]);
    }
    /*set<int>::iterator iter = D.begin();
    while (iter != D.end()) {
       // cout << *iter++ << ' ';
    }
    cout << endl;*/
    process_mem_usage(vm, rss);
    cout << "D peak usage =:" << "  " << "VM: " << vm << "; RSS: " << rss << endl;
    int delta = D.size();  // save the value of delta
    int *upper_line_array = new int[len];  // upper_line_array
    map<int, int> setMap;
    int count1 = 1;
    set<int>::iterator it;
    for(it=D.begin();it!=D.end();it++)  //get the upper_line_array  nlogΔ
    {
        setMap.insert(make_pair(*it, count1));
        count1++;
    }
    for (int i = 0; i < len; i++) {
        upper_line_array[i] = setMap[array[i]];
    }  // the end of getting upper_line_array
    /* for (int i = 0; i < len; i++) {      // get the upper_line_array
         int count = 1;
         set<int>::iterator iter = D.begin();
         while (iter != D.end()) {
             if (*iter == array[i]) {
                 upper_line_array[i] = count;
                 break;
             }
             *iter++;
             count++;
         }
     }   // the end of getting the upper_line_array
     */
    /*  for(int i : upper_line_array) {
          cout << i << "  ";
      }
      cout << endl;*/
    map<int,int> QaMap;
    int *A_Prime = new int[len];
    for (int i = 0; i < len; i++) {  //traverse the upper_line_array for getting the length of every row in Qa
        QaMap[upper_line_array[i]]++;
        A_Prime[i] = QaMap[upper_line_array[i]] - 1;
    } //the end of getting the length of every row in Qa
    /* for (int x : A_Prime) {
         cout << x <<"  ";
     }*/

    process_mem_usage(vm, rss);
    cout << "QaMap peak usage =:" << "  " << "VM: " << vm << "; RSS: " << rss << endl;

    //int Qa[delta][currentMax];
    //cout <<"delta: " << delta << " currentMax: " << currentMax;
    int **Qa = new int*[delta];            // initialize 2-D dynamic array Qa
    for (int i = 0; i < delta; i++) {
        Qa[i] = new int[QaMap[i + 1]];
    }
    std::map<int,std::list<int>> index_upper_line_array;    // create the map for creating the Qa
    for (int i = 0; i < len; i++) {
        index_upper_line_array[upper_line_array[i]].push_back(i + 1);
    }
       /*for (auto itr : index_upper_line_array){
           cout << itr.first << ":\t" ;
           for (auto innerItr : itr.second)
               cout << innerItr << " ";
           cout << endl;
       }*/
    for (auto itr : index_upper_line_array){      // update the value into the 2-D array
        int j = 0;
        for (auto innerItr : itr.second)
            Qa[itr.first-1][j++] = innerItr;
    }
    process_mem_usage(vm, rss);
    cout << "Qa peak usage =:" << "  " << "VM: " << vm << "; RSS: " << rss << endl;

    int *bitSize = new int[s];
    int totalBitSize = getSize(upper_line_array, bitSize, s, t, len);
    //int_vector<> word(ceil(totalBitSize / 32.0) + 1, 0, 32);
    std::vector<unsigned int> word(ceil(totalBitSize / 32.0) + 1);
    //vector<unsigned int> word;
    int currentSize = 0, currentIndex = 1, newMethod = 0;
    int blocks = ceil(len / t + 0.0);
    int diff = s - blocks;
}
void read_file(int array[], string file, int len) {
    ifstream file_in(file);
    int temp;
    if (!file_in.is_open()) {
        cout << " cannot open the file" << endl;
    }
    for (int i = 0; i < len; i++) {
        file_in >> array[i];
    }
    /*     for (int i = 0; i < len; i++) {
             cout << array[i] << " ";
         }*/
    file_in.close();
    file_in.clear();
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
   