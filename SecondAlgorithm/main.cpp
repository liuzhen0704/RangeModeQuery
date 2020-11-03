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

    for (int i = 0; i < s; i++) {
        int index = 0;
        int size = bitSize[i];
        bit_vector b = bit_vector(size, 0);
        int countBlock = 0;
        map<int,int> currentmap;
        map<int,int> normalMap;
        int preZero = 0;
        if (i >= blocks) {
            continue;
        }
        for (int j = i * t ; j < len; j++) {
            int occurrence = 0;
            normalMap[upper_line_array[j]]++;
            if (normalMap.count(upper_line_array[j]) == 1) {
                occurrence = normalMap[upper_line_array[j]];
            }
            //   cout << "insert: " << occurrence <<"  " << arr[j] << endl;
            currentmap.insert(make_pair(occurrence, upper_line_array[j]));
            if ((countBlock + 1) % t == 0) {
                // cout << "zhen zheng: " <<currentmap.rbegin() -> first << " ";
                int numZero = currentmap.rbegin() -> first - preZero;
                //cout << " pre" << preZero << "  ";
                //cout << " numzero" << numZero << endl;
                for (int z = 0; z < numZero; z++) {
                    index++;
                }
                preZero += numZero;
                b[index++]=1;
            }
            countBlock++;
        } // every b already intialized

        //cout << "i: " << i << " qianmian: " << b << " size: " << size << endl;
//        for (int i = 0; i < size; i++) {
//            //cout << "i: " << i << endl;
//            if (b[i] == 1) {
//                //cout << "currentIndex " << currentIndex << endl;
//
//                int r = ((currentIndex - 1) % 32) + 1;
//                //word[floor(currentIndex / 32.0)] = word[floor(currentIndex / 32.0)] + pow(2, 32 - r -1);
//                if (currentIndex % 32 == 0) {
//                    //cout << " okk111" <<endl;
//                    word[floor(currentIndex / 32.0) - 1] = word[floor(currentIndex / 32.0) - 1] + pow(2, 32 - r);
//                } else {
//                    word[floor(currentIndex / 32.0)] = word[floor(currentIndex / 32.0)] + pow(2, 32 - r);
//                }
//            }
//            currentIndex++;
//        }

        for (int i = 0; i < size; i++) {
            //cout << "i: " << i << endl;
            if (b[i] == 1) {
                //cout << "currentIndex " << currentIndex << endl;
                word[newMethod / 32] |= 1 << (newMethod % 32);
            } else {
                word[newMethod / 32] &= ~(1 << (newMethod % 32));
            }
            newMethod++;
        }
        currentSize += size;

        //cout << b.size() <<" ";
        //cout << b << endl;

    }  // the end of update bit*/
    process_mem_usage(vm, rss);
    cout << "Bit peak usage =:" << "  " << "VM: " << vm << "; RSS: " << rss << endl;
//    cout << "--------------------" <<endl;
//    cout << "word output: " << endl;
//    for (int i = 0; i < ceil(totalBitSize / 32.0); i++) {
//        cout << word[i] << endl;
//    }
//    /*for (int tmp : word) {
//        cout << tmp << endl;
//    }*/
//    cout << "--------------------" <<endl;
    clock_t start_time, end_time;
    clock_t tiny_start_time, tiny_end_time, med_start_time, med_end_time, huge_start_time, huge_end_time;
    for (int i = 0; i < 6000; i+=2) {            // tiny 0 - 999  med 1000 - 1999 huge 2000-2999

        if (i < 2000) {
            //continue;
            if (i == 0) {
                tiny_start_time = clock();
                cout << "Tiny Query is:" << endl;
            }
            // cout << i <<" ";
            if (i == 1998) {
                tiny_end_time = clock();
                cout << " Tiny Query Total Time : " << (double) (tiny_end_time - tiny_start_time) / CLOCKS_PER_SEC
                     << " s " << endl;
            }
            start_index = tiny_query[i];
            end_index = tiny_query[i + 1];
        } else if (i < 4000) {
            // continue;
            if (i == 2000) {
                med_start_time = clock();
                cout << "Med Query is:" << endl;
            }
            // cout << i <<" ";
            start_index = med_query[i - 2000];
            end_index = med_query[i + 1 - 2000];
            if (i == 3998) {
                med_end_time = clock();
                cout << " Med Query Total Time : " << (double) (med_end_time - med_start_time) / CLOCKS_PER_SEC << " s "
                     << endl;
            }
        } else {
            if (i == 4000) {
                cout << "Huge Query is:" << endl;
                huge_start_time = clock();
            }
            // cout << i <<" ";
            start_index = huge_query[i - 4000];
            end_index = huge_query[i + 1 - 4000];
        }
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
        int c = 0, freq_c = 0;
        if (bi >= bj) {
            c = 0;  // candidate
            freq_c = 0;

            for (int x = start_index; x <= end_index; x++) {       // start to traverse the prefix to find the candidate
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
            // cout << "need special address!!!" << endl;
        } else {
            bit_vector bi_bit = getBit(bi, word, bitSize);
            rrr_vector<> rrrb(bi_bit);
            //cout << bi_bit <<endl;
            //cout << rrrb <<endl;
            /*cout << " s: " << s << " t: " << t <<" start_index: " << start_index << "  end_index: " << end_index
                 <<"  bi: "<< bi <<"  bj:  " << bj <<"  span_start: "<< span_start <<"  span_end: "
                 << span_end<<"  prefix_start: "<< prefix_start <<"  prefix_end: " << prefix_end
                 <<"  suffix_start: "<< suffix_start <<"  suffix_end: " << suffix_end << endl;*/
            rrr_vector<>::select_1_type rrrb_select_1 (&rrrb);
            rrr_vector<>::select_0_type rrrb_select_0 (&rrrb);
            rrr_vector<>::rank_0_type rrrb_rank_0 (&rrrb);
            rrr_vector<>::rank_1_type rrrb_rank_1 (&rrrb);
            int pos_bj = rrrb_select_1(bj - bi + 1);
            int span_mode = 0;
            int span_freq = rrrb_rank_0(pos_bj);
            int pos_last = rrrb_select_0(span_freq);
            //cout << "33333" << endl;
           // cout << "pos_bj: " << pos_bj << " span_freq: " << span_freq << " pos_last: " << pos_last << endl;
            int b_last = rrrb_rank_1(pos_last) + bi;
           // cout << "44444" << endl;
            int b_last_start = b_last * t + 1, b_last_end = b_last_start + t - 1;
            //cout << "55555" << endl;
            for (int i = b_last_start; i <= b_last_end; i++) {
                int tmp = upper_line_array[i - 1];
                int index_start = bi * t + 1;
                int index_end = i;
                //cout << tmp << endl;
                //cout << "Qa: " << tmp << " " << A_Prime[i - 1] << endl;
                //cout << index_start << " " << index_end << endl;
                if (A_Prime[i - 1] - span_freq + 1 >= 0) {
                    int value = Qa[tmp - 1][A_Prime[i - 1] - span_freq + 1];
                    //cout << "value :" << value << endl;
                    if (value >= index_start) {
                        span_mode = array[i - 1];
                    }
                } else {
                    continue;
                }

            }
            //cout << "span mode: " << span_mode << endl;
            //cout << "span freq: " << span_freq << endl;
            c = span_mode;  // candidate
            freq_c = span_freq;
            for (int x = prefix_start; x <= prefix_end; x++) {       // start to traverse the prefix to find the candidate

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

                if (A_Prime[x-1] + 1 < QaMap[upper_line_array[x-1]] && Qa[upper_line_array[x-1] - 1][A_Prime[x-1] + 1] >= suffix_start &&Qa[upper_line_array[x-1] - 1][A_Prime[x-1] + 1] <= end_index) {
                    //cout<< " jinlaile" <<endl;
                    continue;
                } else {
                    if (A_Prime[x-1] - freq_c + 1 >= 0 && Qa[upper_line_array[x-1] - 1][A_Prime[x-1] - freq_c + 1] >= start_index) {

                        c = array[x-1];
                        freq_c = getSuffixCandidateFreq(upper_line_array, A_Prime, QaMap, len, Qa, freq_c, x, start_index ,end_index);
                        //     cout << "c: " << c <<"  " << " freq_c: " << freq_c << endl;
                    } else {
                        continue;
                    }
                }
            }// the end of traversing the prefix
        }
        result[(i / 2)] = freq_c;
    }  // the end of query
    huge_end_time = clock();
    cout << " Huge Query Total Time : " << (double)(huge_end_time - huge_start_time) / CLOCKS_PER_SEC <<" s " << endl;

    //cout << "c: " << c <<"  " << " freq_c: " << freq_c << endl;

    delete[] bitSize;
    //delete []array;
    for (int i = 0; i < delta; i++) {
        delete[] Qa[i];
    }
    delete[] Qa;
//    delete[] result;
//    delete[] tiny_query;
//    delete[] med_query;
//    delete[] huge_query;
    delete[] tiny_query;
    delete[] med_query;
    delete[] huge_query;
    delete[] upper_line_array;
    delete[] A_Prime;
    ofstream second_result("/home/liu1/Desktop/second_result.txt");
    for (int i = 0; i < 3000; i++) {
        //cout << "i: " << i <<"  "<< result[i] << endl;
        //cout << result[i] << "  ";
        second_result << result[i] << " ";
        if ((i + 1) % 10 == 0) {
            second_result << endl;
        }
    }
    if (second_result) {
        cout << endl;
        cout << "create second result file successfully!" << endl;
    }
    second_result.close();
    second_result.clear();
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
bit_vector getBit(int bi, std::vector<unsigned int> word, int *bitSize) {
    int size = bitSize[bi];
    //cout << "size: " << size << endl;
    bit_vector bi_bit = bit_vector(size, 0);
    int j = 0, j_prime = 0;
    for (int i = 0; i < bi; i++) {
        j_prime += bitSize[i];
    }
    j = j_prime + bitSize[bi] - 1;
    for (int i = 0; i < size; i++) {
        bi_bit[i] = (word[j_prime / 32] >> (j_prime % 32)) & 1;
        j_prime++;
    }

    return bi_bit;
}

int getSize(int upper_line_array[], int *bitSize, int s, int t, int len) {
    int totalSize = 0;
    int blocks = ceil(len / t + 0.0);
    int diff = s - blocks;
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
            count++;
            //cout <<  "s: " <<s  <<" t: " << t <<" j: " << j <<endl;
        }

        if (i >= blocks) {
            bitSize[i] = 0;
            continue;
        }
        //cout <<  "i: " <<i  <<endl;
        /*cout << "-------------------------------------" <<endl;
        for(auto it = currentmap.cbegin(); it != currentmap.cend(); ++it)
        {
            std::cout << it->first << " " << it->second<< "\n";
        }
        cout << "-------------------------------------" <<endl;*/
        bitSize[i] = currentmap.rbegin()->first + s - i - diff;
        totalSize += (currentmap.rbegin()->first + s - i);
        // cout << "ben lun: " << (currentmap.rbegin()->first + s - i) <<" ";

    }
    return totalSize;
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