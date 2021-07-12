//
// Created by liu1 on 9/5/20.
//

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

void Timothy_First(int array[], int len, int s);
int getCandidateFreq(int upper_line_array[], int A_Prime[], map<int,int> QaMap, int len, int **Qa, int freq_c, int x, int i, int j);
int getSuffixCandidateFreq(int upper_line_array[], int A_Prime[], map<int,int> QaMap, int len, int **Qa, int freq_c, int x, int i, int j);
void read_file(int array[], string file, int len);
void getTheSTable(int upper_line_array[], int **S, int **S_freq, int s, int t, int len);
void process_mem_usage(double& vm_usage, double& resident_set);
void generate_query(int *tiny_query, int *med_query, int *huge_query, int len);
void updateTheSTable(int **S, int len, set<int> D, int s);
int realMode(int **S, set<int> D, int mode);

int main() {
    // int array[] = {3, 5, 10, 5, 10, 10, 2, 3};  // input array;
    //   int array[] = {40, 10, 30, 20, 40, 30, 20, 40, 20, 20, 10, 40, 40, 20, 10, 20, 30, 50, 50, 40, 10, 10, 40, 50};
    int len;   // the length of input array
    cout << "please input the length of the array: ";
    cin >> len;
    int s = ceil(sqrt(len));     // fixed value : the number of blocks
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
    Timothy_First(array, len, s);
    program_end_time = clock();
    cout << "Program Total Time : " << (double)(program_end_time - program_start_time) / CLOCKS_PER_SEC <<" s " << endl;
    return 0;
}

void Timothy_First(int array[], int len, int s) {
    int start_index = 8;
    int end_index = 27;
    int *tiny_query = new int[200000];
    int *med_query = new int[200000];
    int *huge_query = new int[200000];
    // int second_point = rand_point(len);
    generate_query(tiny_query, med_query, huge_query, len);
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
    cout << "D:" << "  " << "VM: " << vm << "; RSS: " << rss << endl;
    int delta = D.size();  // save the value of delta
    int upper_line_array[len];  // upper_line_array
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
    int A_Prime[len];
    for (int i = 0; i < len; i++) {  //traverse the upper_line_array for getting the length of every row in Qa
        QaMap[upper_line_array[i]]++;
        A_Prime[i] = QaMap[upper_line_array[i]] - 1;
    } //the end of getting the length of every row in Qa
    /* for (int x : A_Prime) {
         cout << x <<"  ";
     }*/

    process_mem_usage(vm, rss);
    cout << "QaMap:" << "  " << "VM: " << vm << "; RSS: " << rss << endl;

    //int Qa[delta][currentMax];
    int **Qa = new int*[delta];            // initialize 2-D dynamic array Qa
    for (int i = 0; i < delta; i++) {
        Qa[i] = new int[QaMap[i + 1]];
    }
    std::map<int,std::list<int>> index_upper_line_array;    // create the map for creating the Qa
    for (int i = 0; i < len; i++) {
        index_upper_line_array[upper_line_array[i]].push_back(i + 1);
    }
    /*   for (auto itr : index_upper_line_array){
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
    cout << "Qa:" << "  " << "VM: " << vm << "; RSS: " << rss << endl;
    /*for (auto itr : index_upper_line_array){
        cout << itr.first << ":\t" ;
        for (auto innerItr : itr.second)
            cout << innerItr << " ";
        cout << endl;
    }*/
    /* for (int i = 0; i < delta; i++) {
         for (int j = 0; j < QaMap[i + 1]; j++) {
             cout<<  Qa[i][j] << " ";
         }
         cout << endl;
     }*/

    // start to build table S and S'
    int  t = (int)ceil((float)len/(float)s); // t: the size of every block, s : the fixed number of blocks
    int **S = new int*[s];
    int **S_freq = new int*[s];
    for (int i = 0; i < s; i++) {
        S[i] = new int[s];
    }
    for (int i = 0; i < s; i++) {
        S_freq[i] = new int[s];
    }
    getTheSTable(upper_line_array, S, S_freq, s, t, len);  // create the table S and S_freq
    updateTheSTable(S, len, D, s);
    for (int i = 0; i < s; i++) {
        for (int j = i; j < s; j++) {
            cout << S[i][j] <<" ";
        }
        cout << endl;
    }
    cout << endl;
    for (int i = 0; i < s; i++) {
        for (int j = i; j < s; j++) {
            cout << S_freq[i][j] <<" ";
        }
        cout << endl;
    }
    cout << endl;
    //start_index = 63359, end_index = 63467;
    //start_index = 9, end_index = 10;
    process_mem_usage(vm, rss);
    cout << "S and S' table:" << "  " << "VM: " << vm << "; RSS: " << rss << endl;
    clock_t start_time, end_time;
    clock_t tiny_start_time, tiny_end_time, med_start_time, med_end_time, huge_start_time, huge_end_time;

        start_time = clock();
        int bi = ceil((float)(start_index-1)/t), bj = floor(end_index/t) - 1;
        int span_start = bi * t + 1;
        int span_end = (bj + 1) * t;
        int prefix_start = start_index;
        int prefix_end = min(bi * t, end_index);
        int suffix_start = max((bj + 1) * t + 1, start_index);
        int suffix_end = end_index;  //s : 317  t:316
         /*cout << " s: " << s << " t: " << t <<" start_index: " << start_index << "  end_index: " << end_index
         <<"  bi: "<< bi <<"  bj:  " << bj <<"  span_start: "<< span_start <<"  span_end: "
         << span_end<<"  prefix_start: "<< prefix_start <<"  prefix_end: " << prefix_end
         <<"  suffix_start: "<< suffix_start <<"  suffix_end: " << suffix_end << endl;*/
        int c = 0;  // candidate
        int freq_c = 0;
        if (bi <= bj) {
            int span_mode = S[(span_start - 1) / t][(span_end / t) - 1];
            // cout << endl;
            c = span_mode;  // candidate
            freq_c = S_freq[(span_start - 1) / t][(span_end / t) - 1];
            for (int x = prefix_start; x <= prefix_end; x++) {       // start to traverse the prefix to find the candidate
                /* cout << " x: " << x << " -A " << upper_line_array[x-1]
                      <<" "<< A_Prime[x-1]<< endl;*/
                if (Qa[upper_line_array[x-1] - 1][A_Prime[x-1] - 1] >= start_index) {
                    continue;
                } else {
                    if (A_Prime[x-1] + freq_c - 1 < QaMap[upper_line_array[x-1]] && Qa[upper_line_array[x-1] - 1][A_Prime[x-1] + freq_c - 1] <= end_index) {

                        c = array[x-1];
                        freq_c = getCandidateFreq(upper_line_array, A_Prime, QaMap, len, Qa, freq_c, x, start_index ,end_index);
                        // cout << "c: " << c <<"  " << " freq_c: " << freq_c << endl;
                    } else {
                        continue;
                    }
                }
            }// the end of traversing the prefix
            for (int x = suffix_end; x >= suffix_start; x--) {       // start to traverse the prefix to find the candidate
                /* cout << " x: " << x << " -A " << upper_line_array[x-1]
                      <<" "<< A_Prime[x-1]<< endl;*/
                if (A_Prime[x-1] + 1 < QaMap[upper_line_array[x-1]] && Qa[upper_line_array[x-1] - 1][A_Prime[x-1] + 1] >= suffix_start &&Qa[upper_line_array[x-1] - 1][A_Prime[x-1] + 1] <= end_index) {
                    //cout<< " jinlaile" <<endl;
                    continue;
                } else {
                    if (A_Prime[x-1] - freq_c + 1 >= 0 && Qa[upper_line_array[x-1] - 1][A_Prime[x-1] - freq_c + 1] >= start_index) {
                        /* cout << " x: " << x << " -A " << upper_line_array[x-1]
                              <<" "<< A_Prime[x-1]<< endl;
                         cout << "c: " << c <<"  " << " freq_c: " << freq_c << endl;*/
                        c = array[x-1];
                        freq_c = getSuffixCandidateFreq(upper_line_array, A_Prime, QaMap, len, Qa, freq_c, x, start_index ,end_index);
                        //     cout << "c: " << c <<"  " << " freq_c: " << freq_c << endl;
                    } else {
                        continue;
                    }
                }
            }// the end of traversing the prefix
        } else {
            c = 0;  // candidate
            freq_c = 1;
            for (int x = start_index; x <= end_index; x++) {
                if (upper_line_array[x-1] - 1 >= 0 && Qa[upper_line_array[x-1] - 1][A_Prime[x-1] - 1] >= start_index) {
                    continue;
                } else {
                    if (A_Prime[x-1] + freq_c - 1 < QaMap[upper_line_array[x-1]] && Qa[upper_line_array[x-1] - 1][A_Prime[x-1] + freq_c - 1] <= end_index) {
                        c = array[x-1];
                        freq_c = getCandidateFreq(upper_line_array, A_Prime, QaMap, len, Qa, freq_c, x, start_index ,end_index);
                        //freq_c = getSpecialFreq(start_index, end_index, );
                        // cout << "c: " << c <<"  " << " freq_c: " << freq_c << endl;
                    } else {
                        continue;
                    }
                }
            }
        }
         // the frequency of the candidate
         // cout << "c: " << c <<"  " << " freq_c: " << freq_c << endl;

           cout << "c: " << c <<"  " << " freq_c: " << freq_c << endl;
     // end of for query

    end_time = clock();
    cout << "Total Time : " << (double)(end_time - start_time) / CLOCKS_PER_SEC <<" s " << endl;
    delete []array;
    for (int i = 0; i < delta; i++) {
        delete[] Qa[i];
    }
    delete[] Qa;
    for (int i = 0; i < s; i++) {
        delete[] S[i];
    }
    delete[] S;
    for (int i = 0; i < s; i++) {
        delete[] S_freq[i];
    }
    delete[] S_freq;
    delete[] tiny_query;
    delete[] med_query;
    delete[] huge_query;
    //  cout << "c: " << c <<"  " << " freq_c: " << freq_c << endl;

}
int getSpecialFreq(int upper_line_array[],int A_Prime[], map<int,int> QaMap, int len, int **Qa, int freq_c, int x, int i, int j) {
    int currentKey = 0;
    //  cout << "x " << x << " QaMap: " << QaMap[upper_line_array[x-1]] << endl;
    for (int q = A_Prime[x-1] + freq_c - 1; q  < QaMap[upper_line_array[x-1]]; q++) {
        if (Qa[upper_line_array[x-1] - 1][q] > j) {
            break;
        }
        currentKey = q;
    }
    //  cout << "currentKey : " << currentKey << endl;
    int freq = currentKey - A_Prime[x-1] + 1;

    return freq;
}
void generate_query(int *tiny_query, int *med_query, int *huge_query, int len){
    // cout << "second point: " << second_point << endl;
    int tiny_k = 100, med_k = 10, huge_k = 1;
    srand((unsigned)time(NULL));
    int second_point = 0;
    for (int i = 0; i < 200000; i+=2) {
        second_point = (rand() % (len - 1 + 1)) + 1;
        /*cout << "THis is Tiny" <<endl;
        cout << "second point: " << second_point << " " << second_point + (len - second_point) / tiny_k << endl;*/
        tiny_query[i] = second_point;
        tiny_query[i + 1] = second_point + (len - second_point) / tiny_k;
        if (tiny_query[i] > tiny_query[i + 1]) {
            cout<<"zaogao!!!!"<<endl;
        }
    }
    for (int i = 0; i < 200000; i+=2) {
        second_point = (rand() % (len - 1 + 1)) + 1;
        /*cout << "THis is MED" <<endl;
        cout << "second point: " << second_point << " " << second_point + (len - second_point) / med_k << endl;*/
        med_query[i] = second_point;
        med_query[i + 1] = second_point + (len - second_point) / med_k;
    }
    for (int i = 0; i < 200000; i+=2) {
        second_point = (rand() % (len - 1 + 1)) + 1;
        /*cout << "THis is huge" <<endl;
        cout << "second point: " << second_point << " " << second_point + (len - second_point) / huge_k << endl;*/
        huge_query[i] = second_point;
        huge_query[i + 1] = second_point + (len - second_point) / huge_k;
    }
    /*for (int i = 0; i < 30; i++) {
        cout << med_query[i] << "  ";
    }
    cout << endl;
    for (int i = 0; i < 30; i++) {
        cout << huge_query[i] << "  ";
    }*/
}
void getTheSTable(int upper_line_array[], int **S, int **S_freq, int s, int t, int len) {
    for (int i = 0; i < s; i++) {
        int count = 0;
        map<int,int> currentmap;
        map<int,int> normalMap;
        for (int j = i * t ; j < len; j++) {
            int occurrence = 0;
            normalMap[upper_line_array[j]]++;
            if (normalMap.count(upper_line_array[j]) == 1) {
                occurrence = normalMap[upper_line_array[j]];
            }
            //   cout << "insert: " << occurrence <<"  " << arr[j] << endl;
            currentmap.insert(make_pair(occurrence, upper_line_array[j]));
            if ((count + 1) % t == 0) {
                S[i][i + ((count + 1) / t) - 1] = currentmap.rbegin() -> second;
                S_freq[i][i + ((count + 1) / t) - 1] = currentmap.rbegin() -> first;
            }
            count++;
        }
    }
}
void updateTheSTable(int **S, int len, set<int> D, int s) {
    for (int i = 0; i < s; i++) {
        for (int j = i; j < s; j++) {
            S[i][j] = realMode(S, D, S[i][j]);
        }
    }
}
int realMode(int **S, set<int> D, int mode) {
    int real_mode = 0;
    int countD = 0;
    set<int>::iterator iter = D.begin();
    while (iter != D.end()) {
        if (countD == mode - 1) {
            real_mode = *iter;
            break;
        }
        countD++;
        *iter++;
    }
    return real_mode;
}
int getCandidateFreq(int upper_line_array[],int A_Prime[], map<int,int> QaMap, int len, int **Qa, int freq_c, int x, int i, int j) {
    /*  for (int i = 0; i < row; i++) {
          for (int j = 0; j < QaMap[i + 1]; j++) {
            //  cout<<*(Qa+i*col+j)<<" ";
            cout << Qa[i][j] <<" ";
          }
          cout << endl;
      }*/

    //   cout << *(Qa+1*col+2)<<endl;
    //Qa[upper_line_array[x-1] - 1][q]

    int currentKey = 0;

    //  cout << "x " << x << " QaMap: " << QaMap[upper_line_array[x-1]] << endl;
    for (int q = A_Prime[x-1] + freq_c - 1; q  < QaMap[upper_line_array[x-1]]; q++) {
        if (Qa[upper_line_array[x-1] - 1][q] > j) {
            break;
        }
        currentKey = q;
    }
    //  cout << "currentKey : " << currentKey << endl;
    int freq = currentKey - A_Prime[x-1] + 1;

    return freq;
}
int getSuffixCandidateFreq(int upper_line_array[],int A_Prime[], map<int,int> QaMap, int len, int **Qa, int freq_c, int x, int i, int j) {
    /* for (int i = 0; i < row; i++) {
         for (int j = 0; j < QaMap[i + 1]; j++) {
             //  cout<<*(Qa+i*col+j)<<" ";
             cout << Qa[i][j] <<" ";
         }
         cout << endl;
     }*/

    //   cout << *(Qa+1*col+2)<<endl;
    //Qa[upper_line_array[x-1] - 1][q]

    int currentKey = 0;
    for (int q = A_Prime[x-1]; q  >= 0; q--) {
        if (Qa[upper_line_array[x-1] - 1][q]  < i) {
            //  cout << q   <<"  "  << Qa[upper_line_array[x-1] - 1][q] <<" chu lai le";
            break;
        }
        /*cout << "x " << x  << "  A_Prime[x-1]  " << A_Prime[x-1] << " Qa[" << upper_line_array[x-1] - 1 <<
             "][" << q << "]" <<endl;*/
        currentKey = q;
    }

    int freq = A_Prime[x-1] - currentKey + 1;

    return freq;
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