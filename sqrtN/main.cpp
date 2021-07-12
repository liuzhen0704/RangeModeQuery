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

class ToolBox{
public:
    int* upper_line_array;
    int* A_Prime;
    map<int,int> QaMap;
    int **Qa;
    int len;
    int **S;
    int **S_freq;
    set<int> D;
    int s;
    int t;
};
ToolBox toolBox;

void Timothy_First(int array[], int len, int s);
int getCandidateFreq(int freq_c, int x, int i, int j);
int getSuffixCandidateFreq( int freq_c, int x, int i, int j);
void read_file(int array[], string file, int len);
void getTheSTable();
void process_mem_usage(double& vm_usage, double& resident_set);
void generate_query(int *tiny_query, int *med_query, int *huge_query, int query);
void updateTheSTable();
int realMode(int mode);

int main() {
    // int array[] = {3, 5, 10, 5, 10, 10, 2, 3};  // input array;
    //   int array[] = {40, 10, 30, 20, 40, 30, 20, 40, 20, 20, 10, 40, 40, 20, 10, 20, 30, 50, 50, 40, 10, 10, 40, 50};
    int len;   // the length of input array
    cout << "please input the length of the array: ";
    cin >> len;
    int s = ceil(sqrt(len));     // fixed value : the number of blocks
    cout << " s: " << s << endl;
    int *array = new int[len];
    toolBox.s = s;
    toolBox.len = len;
    string file_in = "/users/grad/liu1/data.txt";
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
    delete[] array;

    return 0;
}

void Timothy_First(int array[], int len, int s) {
    int start_index = 0;
    int end_index = 0;
    int *tiny_query = new int[100000];
    int *med_query = new int[100000];
    int *huge_query = new int[100000];
    int *result = new int[150000];
    // int second_point = rand_point(len);
    generate_query(tiny_query, med_query, huge_query, 100000);
    clock_t constructStart, constructEnd;
    constructStart = clock();
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
    int delta = D.size();  // save the value of delta
    //int *upper_line_array = new int[len];  // upper_line_array
    //int *upper_line_array;
    map<int, int> setMap;
    int count1 = 1;
    set<int>::iterator it;

    for(it=D.begin();it!=D.end();it++)  //get the upper_line_array  nlogΔ
    {
        setMap.insert(make_pair(*it, count1));
        count1++;
    }
    int *upper_line_array = new int[len];
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
    toolBox.D = D;
    toolBox.upper_line_array = upper_line_array;
    map<int,int> QaMap;
    int *A_Prime = new int[len];

    for (int i = 0; i < len; i++) {  //traverse the upper_line_array for getting the length of every row in Qa
        QaMap[upper_line_array[i]]++;
        A_Prime[i] = QaMap[upper_line_array[i]] - 1;
    } //the end of getting the length of every row in Qa
    /* for (int x : A_Prime) {
         cout << x <<"  ";
     }*/
    toolBox.QaMap = QaMap;

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
    toolBox.A_Prime = A_Prime;
    toolBox.Qa = Qa;
    int  t = (int)ceil((float)len/(float)s); // t: the size of every block, s : the fixed number of blocks
    int **S = new int*[s];
    int **S_freq = new int*[s];
    for (int i = 0; i < s; i++) {
        S[i] = new int[s];
    }
    for (int i = 0; i < s; i++) {
        S_freq[i] = new int[s];
    }
    toolBox.t = t;
    toolBox.S = S;
    toolBox.S_freq = S_freq;

    getTheSTable();  // create the table S and S_freq
    updateTheSTable();
    /*for (int i = 0; i < s; i++) {
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
    cout << endl;*/
    // start_index = 1, end_index = 100000;
    //clock_t start_time, end_time;
    constructEnd = clock();
    cout << "Construction Time : " << (double)(constructEnd - constructStart) / CLOCKS_PER_SEC <<" s " << endl;

    process_mem_usage(vm, rss);
    cout << "Construction memeory:" << "  " << "VM: " << vm << "; RSS: " << rss << endl;
    cout << endl;
    int tiny = 0, med = 0, huge = 0;
    int tinyLen1 = 0, tinyLen2 = 0, medLen1 = 0, medLen2 = 0, hugeLen1 = 0, hugeLen2 = 0;
    double tiny1 = 0, tiny2 = 0, med1 = 0, med2 = 0, huge1 = 0, huge2 = 0;

    int tinySecondCase1 = 0, tinySecondCase2 = 0, medSecondCase1 = 0, medSecondCase2 = 0, hugeSecondCase1 = 0, hugeSecondCase2 = 0;
    double tinySecondCaseTime1 = 0, tinySecondCaseTime2 = 0, medSecondCaseTime1 = 0, medSecondCaseTime2 = 0,
            hugeSecondCaseTime1 = 0, hugeSecondCaseTime2 = 0;

    clock_t firstStart, firstEnd, secStart, secEnd, s1, e1;
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
        //start_time = clock();
        int bi = ceil((float)(start_index-1)/t), bj = floor(end_index/t) - 1;
        int span_start = bi * t + 1;
        int span_end = (bj + 1) * t;
        int prefix_start = start_index;
        int prefix_end = min(bi * t, end_index);
        int suffix_start = max((bj + 1) * t + 1, start_index);
        int suffix_end = end_index;  //s : 317  t:316
        /* cout << " s: " << s << " t: " << t <<" start_index: " << start_index << "  end_index: " << end_index
         <<"  bi: "<< bi <<"  bj:  " << bj <<"  span_start: "<< span_start <<"  span_end: "
         << span_end<<"  prefix_start: "<< prefix_start <<"  prefix_end: " << prefix_end
         <<"  suffix_start: "<< suffix_start <<"  suffix_end: " << suffix_end << endl;*/
        int c = 0;  // candidate
        int freq_c = 0;
        if (bi < bj) {    // not in one span
            firstStart = clock();
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
                        s1 = clock();
                        c = array[x-1];
                        freq_c = getCandidateFreq( freq_c, x, start_index ,end_index);
                        e1 = clock();
                        if (i < 100000) {
                            tinySecondCase1++;
                            tinySecondCaseTime1 += ((double)(e1 - s1) / CLOCKS_PER_SEC);
                        } else if (i < 200000) {
                            medSecondCase1++;
                            medSecondCaseTime1 += ((double)(e1 - s1) / CLOCKS_PER_SEC);
                        } else {
                            hugeSecondCase1++;
                            hugeSecondCaseTime1 += ((double)(e1 - s1) / CLOCKS_PER_SEC);
                        }
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
                        s1 = clock();
                        c = array[x-1];
                        freq_c = getSuffixCandidateFreq( freq_c, x, start_index ,end_index);
                        e1 = clock();
                        if (i < 100000) {
                            tinySecondCase1++;
                            tinySecondCaseTime1 += ((double)(e1 - s1) / CLOCKS_PER_SEC);
                        } else if (i < 200000) {
                            medSecondCase1++;
                            medSecondCaseTime1 += ((double)(e1 - s1) / CLOCKS_PER_SEC);
                        } else {
                            hugeSecondCase1++;
                            hugeSecondCaseTime1 += ((double)(e1 - s1) / CLOCKS_PER_SEC);
                        }
                        //     cout << "c: " << c <<"  " << " freq_c: " << freq_c << endl;
                    } else {
                        continue;
                    }
                }
            }// the end of traversing the prefix
            firstEnd = clock();
            double time = (double)(firstEnd - firstStart) / CLOCKS_PER_SEC;
            if (i < 100000) {
                tiny1 += time;
                tinyLen1 += (prefix_end - prefix_start);
                tinyLen1 += (suffix_end - suffix_start);
            } else if (i < 200000) {
                med1 += time;
                medLen1 += (prefix_end - prefix_start);
                medLen1 += (suffix_end - suffix_start);
            } else {
                huge1 += time;
                hugeLen1 += (prefix_end - prefix_start);
                hugeLen1 += (suffix_end - suffix_start);
            }
        } else {
            secStart = clock();
            c = 0;  // candidate
            freq_c = 0;
            for (int x = start_index; x <= end_index; x++) {       // start to traverse the prefix to find the candidate
                /* cout << " x: " << x << " -A " << upper_line_array[x-1]
                      <<" "<< A_Prime[x-1]<< endl;*/
                if (Qa[upper_line_array[x-1] - 1][A_Prime[x-1] - 1] >= start_index) {
                    continue;
                } else {
                    if (A_Prime[x-1] + freq_c - 1 < QaMap[upper_line_array[x-1]] && Qa[upper_line_array[x-1] - 1][A_Prime[x-1] + freq_c - 1] <= end_index) {
                        s1 = clock();
                        c = array[x-1];
                        freq_c = getCandidateFreq(freq_c, x, start_index ,end_index);
                        e1 = clock();
                        if (i < 100000) {
                            tinySecondCase2++;
                            tinySecondCaseTime2 += ((double)(e1 - s1) / CLOCKS_PER_SEC);
                        } else if (i < 200000) {
                            medSecondCase2++;
                            medSecondCaseTime2 += ((double)(e1 - s1) / CLOCKS_PER_SEC);
                        } else {
                            hugeSecondCase2++;
                            hugeSecondCaseTime2 += ((double)(e1 - s1) / CLOCKS_PER_SEC);
                        }
                        // cout << "c: " << c <<"  " << " freq_c: " << freq_c << endl;
                    } else {
                        continue;
                    }
                }
            }// the end of traversing the prefix
            secEnd = clock();
            double time = (double)(secEnd - secStart) / CLOCKS_PER_SEC;
            if (i < 100000) {
                tiny2 += time;
                tinyLen2 += (end_index - start_index);
            } else if (i < 200000) {
                med2 += time;
                medLen2 += (end_index - start_index);
            } else {
                huge2 += time;
                hugeLen2 += (end_index - start_index);
            }
        }
        //  cout << "c: " << c <<"  " << " freq_c: " << freq_c << endl;
        if (i < 100000) {
            if (bi >= bj) {
                tiny++;
            }
        } else if (i < 200000) {
            if (bi >= bj) {
                med++;
            }
        } else {
            if (bi >= bj) {
                huge++;
            }
        }
        //   cout << "c: " << c <<"  " << " freq_c: " << freq_c << endl;
        result[(i / 2)] = freq_c;
    } // end of for query
    huge_end_time = clock();
    cout << " Huge Query Total Time : " << (double)(huge_end_time - huge_start_time) / CLOCKS_PER_SEC <<" s " << endl;
    cout << "tiny: " << tiny << "  med:  " << med << "  huge: " << huge << endl;


    cout << "tiny bi >= bj scan len : " << tinyLen2 <<"  time: " << tiny2 << endl;
    cout << "tiny bi <  bj scan len : " << tinyLen1 <<"  time: " << tiny1 << endl;


    cout << endl;
    cout << "med bi >= bj scan len : " << medLen2 <<"  time: " << med2 << endl;
    cout << "med bi <  bj scan len : " << medLen1 <<"  time: " << med1 << endl;

    cout << endl;

    cout << "huge bi >= bj scan len : " << hugeLen2 <<"  time: " << huge2 << endl;
    cout << "huge bi <  bj scan len : " << hugeLen1 <<"  time: " << huge1 << endl;

    cout << endl;

    cout << "tiny bi >= bj second case count: " << tinySecondCase2 << " time: " << tinySecondCaseTime2 << endl;
    cout << "tiny bi < bj second case count: " << tinySecondCase1 << " time: " << tinySecondCaseTime1 << endl;

    cout << "med bi >= bj second case count: " << medSecondCase2 << " time: " << medSecondCaseTime2 << endl;
    cout << "med bi < bj second case count: " << medSecondCase1 << " time: " << medSecondCaseTime1 << endl;

    cout << "huge bi >= bj second case count: " << hugeSecondCase2 << " time: " << hugeSecondCaseTime2 << endl;
    cout << "huge bi < bj second case count: " << hugeSecondCase1 << " time: " << hugeSecondCaseTime1 << endl;

    //end_time = clock();
    //cout << "Total Time : " << (double)(end_time - start_time) / CLOCKS_PER_SEC <<" s " << endl;
    //delete []array;
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
    delete[] upper_line_array;
    delete[] A_Prime;
    ofstream first_result("/users/grad/liu1/first_result.txt");
    for (int i = 0; i < 150000; i++) {
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
    delete[] result;
    //  cout << "c: " << c <<"  " << " freq_c: " << freq_c << endl;

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
void getTheSTable() {
    int t = toolBox.t;
    for (int i = 0; i < toolBox.s; i++) {
        int count = 0;
        map<int,int> currentmap;
        map<int,int> normalMap;
        for (int j = i * t ; j < toolBox.len; j++) {
            int occurrence = 0;
            normalMap[toolBox.upper_line_array[j]]++;
            if (normalMap.count(toolBox.upper_line_array[j]) == 1) {
                occurrence = normalMap[toolBox.upper_line_array[j]];
            }
            //   cout << "insert: " << occurrence <<"  " << arr[j] << endl;
            currentmap.insert(make_pair(occurrence, toolBox.upper_line_array[j]));
            if ((count + 1) % t == 0) {
                toolBox.S[i][i + ((count + 1) / t) - 1] = currentmap.rbegin() -> second;
                toolBox.S_freq[i][i + ((count + 1) / t) - 1] = currentmap.rbegin() -> first;
            }
            count++;
        }
    }
}
void updateTheSTable() {
    for (int i = 0; i < toolBox.s; i++) {
        for (int j = i; j < toolBox.s; j++) {
            toolBox.S[i][j] = realMode(toolBox.S[i][j]);
        }
    }
}
int realMode(int mode) {
    int real_mode = 0;
    int countD = 0;
    set<int>::iterator iter = toolBox.D.begin();
    while (iter != toolBox.D.end()) {
        if (countD == mode - 1) {
            real_mode = *iter;
            break;
        }
        countD++;
        *iter++;
    }
    return real_mode;
}
int getCandidateFreq(int freq_c, int x, int i, int j) {
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
    for (int q = toolBox.A_Prime[x-1] + freq_c - 1; q  < toolBox.QaMap[toolBox.upper_line_array[x-1]]; q++) {
        if (toolBox.Qa[toolBox.upper_line_array[x-1] - 1][q] > j) {
            break;
        }
        currentKey = q;
    }
    //  cout << "currentKey : " << currentKey << endl;
    int freq = currentKey - toolBox.A_Prime[x-1] + 1;

    return freq;
}
int getSuffixCandidateFreq(int freq_c, int x, int i, int j) {
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
    for (int q = toolBox.A_Prime[x-1]; q  >= 0; q--) {
        if (toolBox.Qa[toolBox.upper_line_array[x-1] - 1][q]  < i) {
            //  cout << q   <<"  "  << Qa[upper_line_array[x-1] - 1][q] <<" chu lai le";
            break;
        }
        /*cout << "x " << x  << "  A_Prime[x-1]  " << A_Prime[x-1] << " Qa[" << upper_line_array[x-1] - 1 <<
             "][" << q << "]" <<endl;*/
        currentKey = q;
    }

    int freq = toolBox.A_Prime[x-1] - currentKey + 1;

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