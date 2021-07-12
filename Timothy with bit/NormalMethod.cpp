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
void generate_query(int *tiny_query, int *med_query, int *huge_query, int query);

int main() {
 //   int array[] = { 40, 10, 30, 20, 40, 30, 20, 40, 20, 20, 10, 40, 40, 20, 10, 20, 30, 50, 50, 40, 10, 10, 40, 50};
  // int array[] = {3, 1, 0, 3, 10 ,2 ,5 ,9};

    int len = 0;
    cout << "please input the length of the array: ";
    cin >> len;
    int *pa = new int[len];
    int *pa_copy = new int[len];
    string file_in = "/home/liu1/Desktop/data.txt";
    read_file(pa, file_in, len);
    int *tiny_query = new int[2000];
    int *med_query = new int[2000];
    int *huge_query = new int[2000];
    int *result = new int[3000];
    // int second_point = rand_point(len);
    generate_query(tiny_query, med_query, huge_query, 2000);
    clock_t start_time, end_time, tiny_start_time, tiny_end_time, med_start_time, med_end_time, huge_start_time, huge_end_time;
    start_time = clock();
    int start_index = 0;
    int end_index = 0;
    int *pa_restart = new int[len];
    for (int i = 0; i < 6000; i+=2) {            // tiny 0 - 999  med 1000 - 1999 huge 2000-2999
        int mode = 0;    // save mode
        int maxFreq = 0;   // save the freq of the mode
        if (i < 2000) {
            //continue;
            if (i == 0) {
                tiny_start_time = clock();
                cout << "Tiny Query is:" <<endl;
            }
            //cout << i <<" ";
            if (i == 1998) {
                tiny_end_time = clock();
                cout << " Tiny Query Total Time : " << (double)(tiny_end_time - tiny_start_time) / CLOCKS_PER_SEC <<" s " << endl;
            }
            start_index = tiny_query[i];
            end_index = tiny_query[i + 1];
            //cout << "Num: " << i / 2 << " start: " << start_index << " end: " << end_index;
        } else if (i < 4000) {
            // continue;
            if (i == 2000) {
                med_start_time = clock();
                cout << "Med Query is:" <<endl;
            }
            //cout << i <<" ";
            start_index = med_query[i - 2000];
            end_index = med_query[i + 1 - 2000];
            if (i == 3998) {
                med_end_time = clock();
                cout << " Med Query Total Time : " << (double)(med_end_time - med_start_time) / CLOCKS_PER_SEC <<" s " << endl;
            }
        } else {
            if (i == 4000) {
                cout << "Huge Query is:" <<endl;
                huge_start_time = clock();
            }
            // cout << i <<" ";
            start_index = huge_query[i - 4000];
            end_index = huge_query[i + 1 - 4000];
        }
        for (int i = 0; i < len; i++) {
            pa_copy[i] = pa[i];
        }
        sort(pa_copy + start_index - 1, pa_copy + end_index);
         /* for (int i = 0; i < 10; i++) {
              cout << pa[i] << " ";
          }
          cout << endl;*/

        int count = 0;
        if (len == 1) {
            mode = pa_copy[0];
            maxFreq = 1;
        }
        for (int i = start_index - 1; i < end_index; i++) {
            if (i + 1 < end_index && pa_copy[i] == pa_copy[i + 1]) {
                count++;
            } else {
                count = count + 1;
                if (count > maxFreq) {
                    maxFreq = count;
                    mode = pa_copy[i];
                }
                count = 0;
            }
        }

        /* cout << " VM:" << getValue_VM() << " ";
         cout << " RSS:" << getValue_Rss() << endl;
         double vm3, rss3;
         process_mem_usage(vm3, rss3);
         cout << "VM: " << vm3 << "; RSS: " << rss3 << endl;*/
      //  cout << "Query is : " << start_index << " - " << end_index << "  ";
       // cout << "  Mode is : " << mode << "  , Frequency is : " << maxFreq << endl;
        result[(i / 2)] = maxFreq;
    }
    huge_end_time = clock();
    cout << " Huge Query Total Time : " << (double)(huge_end_time - huge_start_time) / CLOCKS_PER_SEC <<" s " << endl;
    end_time = clock();
    cout << " Total Time : " << (double)(end_time - start_time) / CLOCKS_PER_SEC <<" s " << endl;
    cout << " Total Time : " << (double)((double)(end_time - start_time) / CLOCKS_PER_SEC) / 60 <<" min " << endl;
    delete[] pa;
    delete[] pa_copy;
    delete[] tiny_query;
    delete[] med_query;
    delete[] huge_query;
    ofstream normal_result("/home/liu1/Desktop/normal_result.txt");
    for (int i = 0; i < 3000; i++) {
        //cout << "i: " << i <<"  "<< result[i] << endl;
        //cout << result[i] << "  ";
        normal_result << result[i] << " ";
        if ((i + 1) % 10 == 0) {
            normal_result << endl;
        }
    }
    if (normal_result) {
        cout << endl;
        cout << "create normal result file successfully!" << endl;
    }
    normal_result.close();
    normal_result.clear();
    delete[] result;
}
void generate_query(int *tiny_query, int *med_query, int *huge_query, int query){
    int len = query;
    string tiny_file = "/home/liu1/Desktop/tiny.txt";
    string med_file = "/home/liu1/Desktop/med.txt";
    string huge_file = "/home/liu1/Desktop/huge.txt";
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
int rand_point(int len) {
    int a = 1, b = len;
    srand((unsigned)time(NULL));
    int second = (rand() % (b - a + 1)) + a;
    //cout << second <<endl;
    return second;
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