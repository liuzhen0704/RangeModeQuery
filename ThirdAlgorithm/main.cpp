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
using namespace std;

void read_file(int array[], string file, int len);
void process_mem_usage(double& vm_usage, double& resident_set);
void Third_Method(int *array, int len);
void update_Ci_Table(int **Ci, int *upper_line_array, int len, int delta, int numOfBlocks);
void generate_query(int *tiny_query, int *med_query, int *huge_query, int query);

int main() {
    // int array[] = {3, 5, 10, 5, 10, 10, 2, 3};  // input array;
    //   int array[] = {40, 10, 30, 20, 40, 30, 20, 40, 20, 20, 10, 40, 40, 20, 10, 20, 30, 50, 50, 40, 10, 10, 40, 50};
    int len;   // the length of input array
    cout << "please input the length of the array: ";
    cin >> len;
    int s = ceil(sqrt(len));     // fixed value : the number of blocks
    //cout << " s: " << s << endl;
    int *array = new int[len];
    string file_in = "/users/grad/liu1/data.txt";
    read_file(array, file_in, len);
    //int array[] = {4, 1, 3, 2, 4, 3, 2, 4, 2, 2, 1, 4, 4, 2, 1, 2, 3, 5, 5, 4, 1, 1, 4, 5};
    //int array[] = {4, 1, 3, 2, 4, 3, 2, 4, 2, 2, 1, 4, 4, 2, 1, 2, 3, 5, 5, 4, 1, 1, 4, 4};
    //int array[] = {40, 10, 30, 20, 40, 30, 20, 40, 20, 20, 10, 40, 40, 20, 10, 20, 30, 50, 50, 40, 10, 10, 40, 50};
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
    Third_Method(array, len);
    program_end_time = clock();
    cout << "Program Total Time : " << (double)(program_end_time - program_start_time) / CLOCKS_PER_SEC <<" s " << endl;
    delete[] array;
    return 0;
}
void Third_Method(int *array, int len) {
    int start_index = 0 ;
    int end_index = 0;
    int *tiny_query = new int[100000];
    int *med_query = new int[100000];
    int *huge_query = new int[100000];
    int *result = new int[150000];
    // int second_point = rand_point(len);
    generate_query(tiny_query, med_query, huge_query, 100000);
    // int second_point = rand_point(len);
    //generate_query(tiny_query, med_query, huge_query, 100000);
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
    /* for(int i =0; i < len; i++) {
         cout << upper_line_array[i] << "  ";
     }*/
    int numOfBlocks = floor(len / delta + 0.0);
    //cout << "delta: " << delta << " numOfBlocks: " << numOfBlocks << endl;
    int **Ci = new int*[numOfBlocks];
    for (int i = 0; i < numOfBlocks; i++) {
        Ci[i] = new int[delta + 1];
    }
    update_Ci_Table(Ci, upper_line_array, len, delta, numOfBlocks);
    int *prefix_freq = new int[delta + 1];
    int *suffix_freq = new int[delta + 1];
    int *final_freq = new int[delta + 1];
    int *one_to_j = new int[delta + 1];
    int *one_to_i_minus_one = new int[delta + 1];

    clock_t start_time, end_time;
    clock_t tiny_start_time, tiny_end_time, med_start_time, med_end_time, huge_start_time, huge_end_time;
    for (int i = 0; i < 300000; i+=2) {            // tiny 0 - 999  med 1000 - 1999 huge 2000-2999

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
        } else if (i < 200000) {
            // continue;
            if (i == 100000) {
                med_start_time = clock();
                cout << "Med Query is:" <<endl;
            }
            // cout << i <<" ";
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
        for (int i = 0; i <= delta; i++) {
            final_freq[i] = 0;
            suffix_freq[i] = 0;
            prefix_freq[i] = 0;
            one_to_j[i] = 0;
            one_to_i_minus_one[i] = 0;
        }
        int bi = floor((start_index - 1) / delta + 0.0) - 1;
        int bj = floor((end_index) / delta + 0.0) - 1;
        int prefix_start = (bi + 1) * delta + 1;
        int prefix_end = start_index - 1;
        int suffix_start = (bj + 1) * delta + 1;
        int suffix_end = end_index;
//    cout << "start index: " << start_index << " end index: " << end_index << endl;
//    cout << "delta: " << delta << endl;
//    cout << "bi: " << bi   <<" prefix_start: " << prefix_start << " preffix_end: " << prefix_end << endl;
//    cout << "bj: " << bj   <<" suffix_start: " << suffix_start << " suffix_end: " << suffix_end << endl;


        if (bi < 0 && bj <= 0) {
            for (int i = start_index; i <= end_index; i++) {
                //cout << "upper: " << upper_line_array[i - 1] << " ";
                final_freq[upper_line_array[i - 1]]++;
                //cout << final_freq[upper_line_array[i - 1]] << endl;
            }
        } else if (bi < 0 && bj > 0) {
            for (int i = suffix_start; i <= suffix_end; i++) {
                suffix_freq[upper_line_array[i - 1]]++;
            }
            for (int i = 1; i <= delta; i++) {
                one_to_j[i] = suffix_freq[i] + Ci[bj][i];
            }
            for (int i = prefix_start; i <= prefix_end; i++) {
                prefix_freq[upper_line_array[i - 1]]++;
            }
            for (int i = 1; i <= delta; i++) {
                one_to_i_minus_one[i] = prefix_freq[i];
            }
            for (int i = 0; i <= delta; i++) {
                final_freq[i] = one_to_j[i] - one_to_i_minus_one[i];
                //cout << final_freq[i] << " ";
            }
        } else {
            for (int i = suffix_start; i <= suffix_end; i++) {
                suffix_freq[upper_line_array[i - 1]]++;
            }
            for (int i = 1; i <= delta; i++) {
                one_to_j[i] = suffix_freq[i] + Ci[bj][i];
            }
            for (int i = prefix_start; i <= prefix_end; i++) {
                prefix_freq[upper_line_array[i - 1]]++;
            }
            for (int i = 1; i <= delta; i++) {
                one_to_i_minus_one[i] = prefix_freq[i] + Ci[bi][i];
            }
            for (int i = 0; i <= delta; i++) {
                final_freq[i] = one_to_j[i] - one_to_i_minus_one[i];
                //cout << final_freq[i] << " ";
            }
        }
//    if (bj < 0) {
//        for (int i = 1; i <= delta; i++) {
//            one_to_j[i] = suffix_freq[i];
//        }
//    } else {
//        for (int i = 1; i <= delta; i++) {
//            one_to_j[i] = suffix_freq[i] + Ci[bj][i];
//        }
//    }


        // cout << " ------------------" << endl;
//    for (int i = prefix_start; i <= prefix_end; i++) {
//        prefix_freq[upper_line_array[i - 1]]++;
//    }
//    if (bi < 0) {
//        for (int i = 1; i <= delta; i++) {
//            one_to_i_minus_one[i] = prefix_freq[i];
//        }
//    } else {
//        for (int i = 1; i <= delta; i++) {
//            one_to_i_minus_one[i] = prefix_freq[i] + Ci[bi][i];
//        }
//    }


        int c = 0, freq_c = 0;
        for (int i = 1; i <= delta; i++) {
            if (final_freq[i] > freq_c) {
                freq_c = final_freq[i];
                c = i;
                //cout <<"mode: " << c << " freq: " << freq_c << endl;
            }
        }
        int count = c;
        set<int>::iterator iter = D.begin();
        if (count == 1) {
            c = *D.begin();
        }
        while (iter != D.end() && count > 1) {
            // cout << *iter++ << ' ';
            iter++;
            count--;
            if (count == 1) {
                c = *iter;
            }
        }
        //cout <<"mode: " << c << " freq: " << freq_c << endl;
        /*if ((i / 2) == 2997) {
            cout << "start index: " << start_index << " end index: " << end_index << endl;
            cout << "delta: " << delta << endl;
            cout << "bi: " << bi   <<" prefix_start: " << prefix_start << " preffix_end: " << prefix_end << endl;
            cout << "bj: " << bj   <<" suffix_start: " << suffix_start << " suffix_end: " << suffix_end << endl;
            cout <<"mode: " << c << " freq: " << freq_c << endl;
        }*/
        //cout <<"mode: " << c << " freq: " << freq_c << endl;
        result[(i / 2)] = freq_c;
    } // end of for query
    huge_end_time = clock();
    cout << " Huge Query Total Time : " << (double)(huge_end_time - huge_start_time) / CLOCKS_PER_SEC <<" s " << endl;
    process_mem_usage(vm, rss);
    cout << " Query memory peak usage =:" << "  " << "VM: " << vm << "; RSS: " << rss << endl;
    delete[] prefix_freq;
    delete[] suffix_freq;
    delete [] final_freq;
    delete[] one_to_j;
    delete[] one_to_i_minus_one;

    for (int i = 0; i < numOfBlocks; i++) {
        delete[] Ci[i];
    }
    delete[] Ci;

    ofstream third_result("/users/grad/liu1/third_result.txt");
    for (int i = 0; i < 150000; i++) {
        //cout << "i: " << i <<"  "<< result[i] << endl;
        //      cout << result[2997] << "  ";
//        if (result[i] == 13) {
//            cout << "i: " << i << endl;
//        }
        third_result << result[i] << " ";
        if ((i + 1) % 10 == 0) {
            third_result << endl;
        }
    }
    if (third_result) {
        cout << endl;
        cout << "create third result file successfully!" << endl;
    }
    third_result.close();
    third_result.clear();
    delete[] result;
    process_mem_usage(vm, rss);
    cout << "End peak usage =:" << "  " << "VM: " << vm << "; RSS: " << rss << endl;
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
void update_Ci_Table(int **Ci, int *upper_line_array, int len, int delta, int numOfBlocks) {
    for (int i = 0; i < numOfBlocks; i++) {
        int count = 0;
        map<int,int> normalMap;
        for (int j = 0; j < delta * (i + 1) ; j++) {
            normalMap[upper_line_array[j]]++;
        }
        for(auto it = normalMap.cbegin(); it != normalMap.cend(); ++it)
        {
            Ci[i][it->first] = it->second;
        }
    }
    /*for (int i = 0; i < numOfBlocks; i++) {
        for (int j = 0; j < delta + 1; j++) {
            cout << Ci[i][j] << " ";
        }
        cout << endl;
    }*/

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