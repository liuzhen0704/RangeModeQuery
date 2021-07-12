//
// Created by liu1 on 9/2/20.
//

//
// Created by liu1 on 8/11/20.
//
#include <iostream>
#include <algorithm>
#include <fstream>
#include<time.h>
#include<unistd.h>
#include<ios>
#include<string>
#include "stdlib.h"
#include "stdio.h"
#include "string.h"

using namespace std;
void read_file(int pa[], string file, int len);
void process_mem_usage(double& vm_usage, double& resident_set);
int parseLine(char* line);
int getValue_VM();
int getValue_Rss();
int rand_point(int len);
void generate_query(int *tiny_query, int *med_query, int *huge_query, int len);

int main() {
    //   int array[] = { 40, 10, 30, 20, 40, 30, 20, 40, 20, 20, 10, 40, 40, 20, 10, 20, 30, 50, 50, 40, 10, 10, 40, 50};
    // int array[] = {3, 1, 0, 3, 10 ,2 ,5 ,9};

    int len = 0;
    cout << "please input the length of the array: ";
    cin >> len;
    int *pa = new int[len];
    string file_in = "/home/liu1/Desktop/data.txt";
    read_file(pa, file_in, len);

    clock_t start_time, end_time, tiny_start_time, tiny_end_time, med_start_time, med_end_time, huge_start_time, huge_end_time;
    start_time = clock();
    int start_index = 0;
    int end_index = 0;
    cout << "please input the start index: ";
    cin >> start_index;
    cout << "please input the end index: ";
    cin >> end_index;
    sort(pa + start_index - 1, pa + end_index);
    /* for (int i = 0; i < 10; i++) {
         cout << pa[i] << " ";
     }
     cout << endl;*/
    int mode = 0;    // save mode
    int maxFreq = 0;   // save the freq of the mode
    int count = 0;
    if (len == 1) {
        mode = pa[0];
        maxFreq = 1;
    }
    for (int i = start_index - 1; i < end_index; i++) {
        if (i + 1 < end_index && pa[i] == pa[i + 1]) {
            count++;
        } else {
            count = count + 1;
            if (count > maxFreq) {
                maxFreq = count;
                mode = pa[i];
            }
            count = 0;
        }
    }
    end_time = clock();
    // delete[] pa_restart;
    delete[] pa;
    cout << " Mode : " << mode <<" Freq : " << maxFreq << endl;
    cout << " Total Time : " << (double)(end_time - start_time) / CLOCKS_PER_SEC <<" s " << endl;
    cout << " Total Time : " << (double)((double)(end_time - start_time) / CLOCKS_PER_SEC) / 60 <<" min " << endl;
}

void read_file(int pa[], string file, int len) {
    ifstream file_in(file);
    int temp;
    if (!file_in.is_open()) {
        cout << " cannot open the file" << endl;
    }
    for (int i = 0; i < len; i++) {
        file_in >> pa[i];
    }
    /*  for (int i = 0; i < len; i++) {
          cout << pa[i] << " ";
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
int getValue_Rss(){ //Note: this value is in KB!
    FILE* file = fopen("/proc/self/status", "r");
    int result = -1;
    char line[128];

    while (fgets(line, 128, file) != NULL){
        if (strncmp(line, "VmRSS:", 6) == 0){
            result = parseLine(line);
            break;
        }
    }
    fclose(file);
    return result;
}
int getValue_VM(){ //Note: this value is in KB!
    FILE* file = fopen("/proc/self/status", "r");
    int result = -1;
    char line[128];

    while (fgets(line, 128, file) != NULL){
        if (strncmp(line, "VmSize:", 7) == 0){
            result = parseLine(line);
            break;
        }
    }
    fclose(file);
    return result;
}
int parseLine(char* line){
    // This assumes that a digit will be found and the line ends in " Kb".
    int i = strlen(line);
    const char* p = line;
    while (*p <'0' || *p > '9') p++;
    line[i-3] = '\0';
    i = atoi(p);
    return i;
}