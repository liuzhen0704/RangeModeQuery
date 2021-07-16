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

class ToolBox{
public:
    int len;
    set<int> D_B0;
    int *B0_array;
    int B0_len;
    int B1_len;
    int *B1_array;

    int delta_B0;
    int delta;

    int **Qa_B1;
    int *A_Prime_B1;
    map<int,int> QaMap_B1;

    int *bitSize;
    int **Ci;

    int *upper_line_array;
    int *upper_line_array_B0;
    int *upper_line_array_B1;

    int* presumSize;

    rrr_vector<> rrrMergedBit;
    rrr_vector<>::select_1_type rrrb_select_1 ;
    rrr_vector<>::select_0_type rrrb_select_0 ;
    rrr_vector<>::rank_0_type rrrb_rank_0 ;
    rrr_vector<>::rank_1_type rrrb_rank_1;
};
ToolBox toolBox;

pair<int, int> Third_Method(int start_index, int end_index);
void update_Ci_Table(int numOfBlocks);
void finalMethod(int len, int s);
pair<int, int> bitMethod(int start_index, int end_index, int s, int t);
void process_mem_usage(double& vm_usage, double& resident_set);
void generate_query(int *tiny_query, int *med_query, int *huge_query, int query);
void read_file(int array[], string file, int len);
int getCandidateFreq(int len, int freq_c, int x, int i, int j);
int getSuffixCandidateFreq( int len, int freq_c, int x, int i, int j);


int main() {
    // int array[] = {3, 5, 10, 5, 10, 10, 2, 3};  // input array;
    //   int array[] = {40, 10, 30, 20, 40, 30, 20, 40, 20, 20, 10, 40, 40, 20, 10, 20, 30, 50, 50, 40, 10, 10, 40, 50};
    int len;   // the length of input array
    cout << "please input the length of the array: ";
    cin >> len;
    int s = ceil(sqrt(len * log(len)));     // fixed value : the number of blocks
    cout << " s: " << s << endl;

    // int *array = new int[len];


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
    finalMethod(len, s);
    program_end_time = clock();
    cout << "Program Total Time : " << (double)(program_end_time - program_start_time) / CLOCKS_PER_SEC <<" s " << endl;
    //delete[] array;
    return 0;
}
void finalMethod(int len, int s) {
    int *array = new int[len];
    string file_in = "/users/grad/liu1/data.txt";
    read_file(array, file_in, len);
    clock_t constructStart, constructEnd;
    int start_index = 0;
    int end_index = 0;
    int *tiny_query = new int[100000];
    int *med_query = new int[100000];
    int *huge_query = new int[100000];
    int *result = new int[150000];
    int *mode = new int[150000];
    toolBox.len = len;
    pair<int, int> *range = new pair<int, int>[150000];
    // int second_point = rand_point(len);
    generate_query(tiny_query, med_query, huge_query, 100000);
    int t = (int) ceil((float) len / (float) s);
    //cout << "t: " << t << endl;
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
    process_mem_usage(vm, rss);
    cout << "D peak usage =:" << "  " << "VM: " << vm << "; RSS: " << rss << endl;
    int delta = D.size();  // save the value of delta
    int *upper_line_array = new int[len];  // upper_line_array
    map<int, int> setMap;
    int count1 = 1;
    set<int>::iterator it;
    for (it = D.begin(); it != D.end(); it++)  //get the upper_line_array  nlogΔ
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
    toolBox.upper_line_array = upper_line_array;
    map<int, int> QaMap;
    int *A_Prime = new int[len];
    for (int i = 0; i < len; i++) {  //traverse the upper_line_array for getting the length of every row in Qa
        QaMap[upper_line_array[i]]++;
        A_Prime[i] = QaMap[upper_line_array[i]] - 1;
    } //the end of getting the length of every row in Qa
    /* for (int x : A_Prime) {
         cout << x <<"  ";
     }*/
    /*for(auto it = QaMap.cbegin(); it != QaMap.cend(); ++it)
    {
        std::cout << it->first << " " << it->second << "\n";
    }*/
    process_mem_usage(vm, rss);
    cout << "QaMap peak usage =:" << "  " << "VM: " << vm << "; RSS: " << rss << endl;
    toolBox.delta = delta;
    //int Qa[delta][currentMax];
    //cout <<"delta: " << delta << " currentMax: " << currentMax;
    int **Qa = new int *[delta];            // initialize 2-D dynamic array Qa
    for (int i = 0; i < delta; i++) {
        Qa[i] = new int[QaMap[i + 1]];
    }
    std::map<int, std::list<int>> index_upper_line_array;    // create the map for creating the Qa
    for (int i = 0; i < len; i++) {
        index_upper_line_array[upper_line_array[i]].push_back(i + 1);
    }
    /*for (auto itr : index_upper_line_array){
        cout << itr.first << ":\t" ;
        for (auto innerItr : itr.second)
            cout << innerItr << " ";
        cout << endl;
    }*/
    for (auto itr : index_upper_line_array) {      // update the value into the 2-D array
        int j = 0;
        for (auto innerItr : itr.second)
            Qa[itr.first - 1][j++] = innerItr;
    }
    process_mem_usage(vm, rss);
    cout << "Qa peak usage =:" << "  " << "VM: " << vm << "; RSS: " << rss << endl;
    int B0_len = 0;  // large freq
    int B1_len = 0;  // small freq
    set<int> highFreq;
    for(auto it = QaMap.cbegin(); it != QaMap.cend(); ++it)
    {
        //std::cout << it->first << " " << it->second << "\n";
        if (it->second > s) {
            B0_len += it->second;
            highFreq.insert(it->first);
        } else {
            B1_len += it->second;
        }
    }
    cout << "B0_len: " << B0_len << endl;
    cout << "B1_len: " << B1_len << endl;
    toolBox.B0_len = B0_len;
    toolBox.B1_len = B1_len;
    int **B = new int*[2];
    B[0] = new int[B0_len];
    B[1] = new int[B1_len];
    int **I = new int*[2];
    I[0] = new int[len];
    I[1] = new int[len];
    int **J = new int*[2];
    J[0] = new int[len];
    J[1] = new int[len];
    /*for(auto it = highFreq.cbegin(); it != highFreq.cend(); ++it)
    {
        cout<<*it<<" occurs "<<endl;
    }*/
    int B0_index = 0, B1_index = 0;
    for (int i = 0; i < len; i++) {
        if (highFreq.count(upper_line_array[i]) == 1) {
            B[0][B0_index++] = array[i];
        } else {
            B[1][B1_index++] = array[i];
        }
    }
    /*for (int i = 0; i < B0_len; i++) {
        cout << B[0][i] << " ";
    }
    cout << endl;
    for (int i = 0; i < B1_len; i++) {
        cout << B[1][i] << " ";
    }*/

    B0_index = 0, B1_index = 0;
    for (int i = 0; i < len; i++) {
        if (highFreq.count(upper_line_array[i]) == 1) {
            I[0][i] = B0_index;
            B0_index++;
        } else {
            I[0][i] = B0_index;
        }
    }
    for (int i = 0; i < len; i++) {
        if (highFreq.count(upper_line_array[i]) != 1) {
            I[1][i] = B1_index;
            B1_index++;
        } else {
            I[1][i] = B1_index;
        }
    }
    /*for (int i = 0; i < len; i++) {
        cout << I[0][i] << " ";
    }
    cout << endl;
    for (int i = 0; i < len; i++) {
        cout << I[1][i] << " ";
    }*/
    B0_index = -1, B1_index = -1;
    for (int i = 0; i < len; i++) {
        if (highFreq.count(upper_line_array[i]) == 1) {
            B0_index++;
            J[0][i] = B0_index;
        } else {
            J[0][i] = B0_index;
        }
    }
    for (int i = 0; i < len; i++) {
        if (highFreq.count(upper_line_array[i]) != 1) {
            B1_index++;
            J[1][i] = B1_index;
        } else {
            J[1][i] = B1_index;
        }
    }
    /*for (int i = 0; i < len; i++) {
        cout << J[0][i] << " ";
    }
    cout << endl;
    for (int i = 0; i < len; i++) {
        cout << J[1][i] << " ";
    }*/

    process_mem_usage(vm, rss);
    cout << "B,I,J peak usage =:" << "  " << "VM: " << vm << "; RSS: " << rss << endl;
    int *B0_array = B[0];
    int *B1_array = B[1];
    /*cout << "B1 array: ";
    for (int i = 0; i < B1_len; i++) {
        cout << B1_array[i] <<" ";
    }
    cout << endl;*/
    toolBox.B0_array = B0_array;
    toolBox.B1_array = B1_array;
    set<int> D_B1;
    for (int i = 0; i < B1_len; i++) {
        D_B1.insert(B1_array[i]);
    }
    set<int>::iterator iter = D_B1.begin();
    /*cout << "D: " ;
    while (iter != D_B1.end()) {
        cout << *iter++ << ' ';
    }
    cout << endl;*/
    process_mem_usage(vm, rss);
    //cout << "D peak usage =:" << "  " << "VM: " << vm << "; RSS: " << rss << endl;
    int delta_B1 = D_B1.size();  // save the value of delta
    int *upper_line_array_B1 = new int[B1_len];  // upper_line_array
    map<int, int> setMap_B1;
    int count1_B1 = 1;
    set<int>::iterator it_B1;
    for(it_B1=D_B1.begin();it_B1!=D_B1.end();it_B1++)  //get the upper_line_array  nlogΔ
    {
        setMap_B1.insert(make_pair(*it_B1, count1_B1));
        count1_B1++;
    }
    for (int i = 0; i < B1_len; i++) {
        upper_line_array_B1[i] = setMap_B1[B1_array[i]];
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
    /*for(int i = 0; i < B1_len; i++) {
        cout << upper_line_array_B1[i] << "  ";
    }
    cout << endl;*/
    map<int,int> QaMap_B1;
    int *A_Prime_B1 = new int[B1_len];
    for (int i = 0; i < B1_len; i++) {  //traverse the upper_line_array for getting the length of every row in Qa
        QaMap_B1[upper_line_array_B1[i]]++;
        A_Prime_B1[i] = QaMap_B1[upper_line_array_B1[i]] - 1;
    } //the end of getting the length of every row in Qa
    /* for (int x : A_Prime) {
         cout << x <<"  ";
     }*/


    //cout << "QaMap peak usage =:" << "  " << "VM: " << vm << "; RSS: " << rss << endl;

    //int Qa[delta][currentMax];
    //cout <<"delta: " << delta << " currentMax: " << currentMax;
    int **Qa_B1 = new int*[delta_B1];            // initialize 2-D dynamic array Qa
    for (int i = 0; i < delta_B1; i++) {
        Qa_B1[i] = new int[QaMap_B1[i + 1]];
    }
    std::map<int,std::list<int>> index_upper_line_array_B1;    // create the map for creating the Qa
    for (int i = 0; i < B1_len; i++) {
        index_upper_line_array_B1[upper_line_array_B1[i]].push_back(i + 1);
    }
    /*for (auto itr : index_upper_line_array){
        cout << itr.first << ":\t" ;
        for (auto innerItr : itr.second)
            cout << innerItr << " ";
        cout << endl;
    }*/
    for (auto itr : index_upper_line_array_B1){      // update the value into the 2-D array
        int j = 0;
        for (auto innerItr : itr.second)
            Qa_B1[itr.first-1][j++] = innerItr;
    }
    toolBox.Qa_B1 = Qa_B1;
    toolBox.A_Prime_B1 = A_Prime_B1;
    toolBox.QaMap_B1 = QaMap_B1;
    toolBox.upper_line_array_B1 = upper_line_array_B1;
    //cout << "Qa peak usage =:" << "  " << "VM: " << vm << "; RSS: " << rss << endl;
    //int s_B1 = ceil(sqrt(B1_len * log(B1_len)));
    //int t_B1 = (int) ceil((float) B1_len / (float) s_B1);
    process_mem_usage(vm, rss);
    cout << "Before Bit peak usage =:" << "  " << "VM: " << vm << "; RSS: " << rss << endl;
    int *bitSize = new int[s];
    /*int count_0 = 0, count_1 = 0, count_2 = 0, count_3 = 0, count_4 = 0;
    for (int i = 0; i < len; i++) {
        if (array[i] == 0) {
            count_0++;
        } else if (array[i] == 1) {
            count_1++;
        } else if (array[i] == 2) {
            count_2++;
        } else if (array[i] == 3) {
            count_3++;
        } else {
            count_4++;
        }
    }
    cout << "0: " << count_0 << " "<< "1: " << count_1 << " "<< "2: " << count_2  << " "<< "3: " << count_3
            << " "<< "4: " << count_4 << endl;*/

    //cout << "s_B1: " << s_B1 << endl;
    //int totalBitSize = getSize(upper_line_array, bitSize, s, t, len);
    //std::vector<unsigned int> word(ceil(totalBitSize / 32.0) + 1);
    unsigned int** word = new unsigned int*[s];
    int currentIndex = 1, rowIndex = 0;;
    int blocks = ceil(B1_len / t + 0.0);
    int diff = s - blocks;
    int* bit = new int[2 * s + 1];

    bit_vector * realBitArray = new bit_vector[s];
    int mergedSize = 0;
    for (int i = 0; i < s; i++) {
        bitSize[i] = 0;
    }
    for (int i = 0; i < s; i++) {
        //cout << "i: " << i << endl;
        int newMethod = 0;
        int index = 0;
        //int size = 0;
        for (int i = 0; i < 2 * s; i++) {
            bit[i] = 0;
        }

        int countBlock = 0;
        map<int,int> currentmap;
        map<int,int> normalMap;
        int preZero = 0;
        if (i >= blocks) {
            continue;
        }

        for (int j = i * t ; j < B1_len; j++) {
            int occurrence = 0;
            normalMap[upper_line_array_B1[j]]++;
            //cout << "j: "<< j << endl;
            if (normalMap.count(upper_line_array_B1[j]) == 1) {
                occurrence = normalMap[upper_line_array_B1[j]];
            }
            // cout << "insert: " << occurrence <<"  "  << endl;
            currentmap.insert(make_pair(occurrence, upper_line_array_B1[j]));
            if ((countBlock + 1) % t == 0) {
                // cout << "zhen zheng: " <<currentmap.rbegin() -> first << " ";
                int numZero = currentmap.rbegin() -> first - preZero;
                index += numZero;
                preZero += numZero;
                //cout << "index: " << index << endl;
                bit[index++] = 1;

                //b[index++]=1;
                //word[newMethod / 32] |= 1 << (newMethod % 32);
            }
            countBlock++;
        } // every b already intialized
        int size = index;
//        bit_vector b = bit_vector(size, 0);
//        cout << "i: " << i << " size: " << size << endl;
//        for (int i = 0; i < size; i++) {
//            cout << bit[i] << " ";
//        }
//        cout << endl;
        bit_vector curBit =  bit_vector (size, 0);
        mergedSize += size;
        for (int z = 0; z < size; z++) {
            if (bit[z] == 1) {
                //word[rowIndex][newMethod / 32] |= 1 << (newMethod % 32);
                curBit[z] = 1;
            } else {
                //word[rowIndex][newMethod / 32] &= ~(1 << (newMethod % 32));
            }
        }

        //cout << "curBit: " << curBit << endl <<endl;
        //currentSize += size;
        realBitArray[i] = curBit;
        //cout << "curBit: " << realBitArray[i] << endl <<endl;
        bitSize[i] = size;
        //cout << "i: " << i << " BitSize[i]: " << bitSize[i] << endl;
    }  // the end of update bit*/

    bit_vector mergedBit = bit_vector(mergedSize, 0);
    int* presumSize = new int[s];
    for (int i = 0; i < s; i++) {
        presumSize[i] = 0;
    }
    for (int i = 1; i < s; i++) {
        presumSize[i] += (presumSize[i - 1] + bitSize[i - 1]);
    }
    int index = 0;
    for (int i = 0; i < s; i++) {
        for (int j = 0; j < bitSize[i]; j++) {
            if (realBitArray[i][j] == 1) {
                mergedBit[index++] = 1;
            } else {
                mergedBit[index++] = 0;
            }
        }
    }
    rrr_vector<> rrrMergedBit(mergedBit);
    rrr_vector<>::select_1_type rrrb_select_1 (&rrrMergedBit);
    rrr_vector<>::select_0_type rrrb_select_0 (&rrrMergedBit);
    rrr_vector<>::rank_0_type rrrb_rank_0 (&rrrMergedBit);
    rrr_vector<>::rank_1_type rrrb_rank_1 (&rrrMergedBit);
    /*select_support_mcl<0> select_0_type(&mergedBit);
    select_support_mcl<1> select_1_type(&mergedBit);
    rank_support_v5<0> bit_rank_0 (&mergedBit);
    rank_support_v5<1> bit_rank_1 (&mergedBit);*/

    toolBox.bitSize = bitSize;
    toolBox.presumSize = presumSize;
    toolBox.rrrMergedBit = rrrMergedBit;
    toolBox.rrrb_select_0 = rrrb_select_0;
    toolBox.rrrb_select_1 = rrrb_select_1;
    toolBox.rrrb_rank_0 = rrrb_rank_0;
    toolBox.rrrb_rank_1 = rrrb_rank_1;

    delete [] bit;
    delete[] realBitArray;
    process_mem_usage(vm, rss);
    cout << "Bit Method peak usage =:" << "  " << "VM: " << vm << "; RSS: " << rss << endl;
    set<int> D_B0;
    for (int i = 0; i < B0_len; i++) {
        D_B0.insert(B0_array[i]);
    }
    /*set<int>::iterator iter = D.begin();
    while (iter != D.end()) {
       // cout << *iter++ << ' ';
    }
    cout << endl;*/
    int delta_B0 = D_B0.size();  // save the value of delta
    int *upper_line_array_B0 = new int[B0_len];  // upper_line_array
    map<int, int> setMap_B0;
    int count1_B0 = 1;
    set<int>::iterator it_B0;
    for(it_B0=D_B0.begin();it_B0!=D_B0.end();it_B0++)  //get the upper_line_array  nlogΔ
    {
        setMap_B0.insert(make_pair(*it_B0, count1_B0));
        count1_B0++;
    }
    for (int i = 0; i < B0_len; i++) {
        upper_line_array_B0[i] = setMap_B0[B0_array[i]];
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
    int numOfBlocks = 0;
    if (delta_B0 != 0) {
        numOfBlocks = floor(B0_len / delta_B0 + 0.0);
    }
    int **Ci = new int*[numOfBlocks];

    for (int i = 0; i < numOfBlocks; i++) {
        Ci[i] = new int[delta_B0 + 1];
    }
    toolBox.D_B0 = D_B0;
    toolBox.delta_B0 = delta_B0;
    toolBox.upper_line_array_B0 = upper_line_array_B0;
    toolBox.Ci = Ci;

    update_Ci_Table(numOfBlocks);
    process_mem_usage(vm, rss);
    cout << "Third Method peak usage =:" << "  " << "VM: " << vm << "; RSS: " << rss << endl;
    //start_index = 7, end_index = 12;
    //cout << "I1: " << I[0][start_index - 1] << " J1: " << J[0][end_index - 1] << endl;
    //cout << "I2: " << I[1][start_index - 1] << " J2: " << J[1][end_index - 1] << endl;
    cout << "B0_len: " << B0_len << " B1_len: " << B1_len << endl;
    cout << endl;
    constructEnd = clock();
    cout << "Construction Time : " << (double)(constructEnd - constructStart) / CLOCKS_PER_SEC <<" s " << endl;

    process_mem_usage(vm, rss);
    cout << "Construction memeory:" << "  " << "VM: " << vm << "; RSS: " << rss << endl;
    cout << endl;


    clock_t tiny_start_time, tiny_end_time, med_start_time, med_end_time, huge_start_time, huge_end_time;
    for (int i = 0; i < 300000; i+=2) {            // tiny 0 - 999  med 1000 - 1999 huge 2000-2999

        if (i < 100000) {
            //continue;
            if (i == 0) {
                tiny_start_time = clock();
                cout << "Tiny Query is:" << endl;
            }
            // cout << i <<" ";
            if (i == 99998) {
                tiny_end_time = clock();
                cout << " Tiny Query Total Time : " << (double) (tiny_end_time - tiny_start_time) / CLOCKS_PER_SEC
                     << " s " << endl;
            }
            start_index = tiny_query[i];
            end_index = tiny_query[i + 1];
        } else if (i < 200000) {
            // continue;
            if (i == 100000) {
                med_start_time = clock();
                cout << "Med Query is:" << endl;
            }
            // cout << i <<" ";
            start_index = med_query[i - 100000];
            end_index = med_query[i + 1 - 100000];
            if (i == 199998) {
                med_end_time = clock();
                cout << " Med Query Total Time : " << (double) (med_end_time - med_start_time) / CLOCKS_PER_SEC << " s "
                     << endl;
            }
        } else {
            if (i == 200000) {
                cout << "Huge Query is:" << endl;
                huge_start_time = clock();
            }
            start_index = huge_query[i - 200000];
            end_index = huge_query[i + 1 - 200000];
        }

        //cout << "start index: " << start_index << " end index: " << end_index << endl;
        // start_index = 6, end_index = 6;
        int freq_b0 = 0, freq_b1 = 0;
        int mode_b0 = 0, mode_b1 = 0;
        if (B0_len != 0) {
            //   cout << "Third method: " << endl;
            pair<int, int> resB0 = Third_Method( I[0][start_index - 1] + 1, J[0][end_index - 1] + 1);
            freq_b0 = resB0.second;
            mode_b0 = resB0.first;

        }

        // cout << "kai!!!" << endl;
        if (B1_len != 0) {

            // cout << "Bit method: " << endl;
            pair<int, int> resB1 = bitMethod(I[1][start_index - 1] + 1, J[1][end_index - 1] + 1, s , t);
            freq_b1 = resB1.second;
            mode_b1 = resB1.first;
        }

        // cout << "freq_b0: " << freq_b0 << " freq_b1: " << freq_b1 << endl;
        int final_freq = 0;
        int final_mode = 0;
        if (freq_b0 >= freq_b1) {
            final_freq = freq_b0;
            final_mode = mode_b0;
            // range[i / 2] = make_pair(I[0][start_index - 1] + 1, J[0][end_index - 1] + 1);
            // cout << "00000000000000000" << endl;
        } else {
            // cout << "22222222222222222" << endl;
            final_freq = freq_b1;
            final_mode = mode_b1;
            //   range[i / 2] = make_pair(I[1][start_index - 1] + 1, J[1][end_index - 1] + 1);
        }


        /*if (freq == final_freq) {
            continue;
        } else {
            cout << "diff    i: " << i << "  left: " << left << "  right: " << right << endl;

        }*/
        // cout << "final freq: " << final_freq << endl;
        result[(i / 2)] = final_freq;
        //mode[(i / 2)] = final_mode;
        //result[(i / 2)] = final_freq;
    }  // the end of query
    huge_end_time = clock();
    cout << " Huge Query Total Time : " << (double)(huge_end_time - huge_start_time) / CLOCKS_PER_SEC <<" s " << endl;


    for (int i = 0; i < delta; i++) {
        delete[] Qa[i];
    }
    delete[] Qa;

    for (int i = 0; i < 2; i++) {
        delete[] B[i];
    }
    delete[] B;
    delete[] range;
    delete[] mode;

    for (int i = 0; i < 2; i++) {
        delete[] I[i];
    }
    delete[] I;

    for (int i = 0; i < 2; i++) {
        delete[] J[i];
    }
    delete[] J;

//    delete[] result;
//    delete[] tiny_query;
//    delete[] med_query;
//    delete[] huge_query;
    delete[] tiny_query;
    delete[] med_query;
    delete[] huge_query;
    delete[] upper_line_array;
    delete[] upper_line_array_B1;
    delete[] upper_line_array_B0;
    delete[] A_Prime;
    delete[] A_Prime_B1;

    for (int i = 0; i < delta_B1; i++) {
        delete[] Qa_B1[i];
    }
    delete[] Qa_B1;
//    delete[] result;
//    delete[] tiny_query;
//    delete[] med_query;
//    delete[] huge_query;

    /*for (int i = 0; i < s; i++) {
        delete[] word[i];
    }*/
    delete[] word;

    delete[] bitSize;




    for (int i = 0; i < numOfBlocks; i++) {
        delete[] Ci[i];
    }
    delete[] Ci;

    ofstream integrated_result("/users/grad/liu1/TimothyFreq.txt");
    for (int i = 0; i < 150000; i++) {
        //cout << "i: " << i <<"  "<< result[i] << endl;
        //cout << result[i] << "  ";
        integrated_result << result[i] << " ";
        if ((i + 1) % 10 == 0) {
            integrated_result << endl;
        }
    }
    if (integrated_result) {
        cout << endl;
        cout << "create integrated result file successfully!" << endl;
    }
    integrated_result.close();
    integrated_result.clear();
    delete[] result;
    delete[] array;
}
pair<int, int> Third_Method(int start_index, int end_index) {
    pair<int, int> res;

    // int second_point = rand_point(len);
    //generate_query(tiny_query, med_query, huge_query, 100000);
    //cout << "66666" << endl;
    //cout << "delta B0: " << delta_B0 << " B0_len: " << B0_len << endl;
    int delta_B0 = toolBox.delta_B0;
    int *prefix_freq = new int[delta_B0 + 1];
    int *suffix_freq = new int[delta_B0 + 1];
    int *final_freq = new int[delta_B0 + 1];
    int *one_to_j = new int[delta_B0 + 1];
    int *one_to_i_minus_one = new int[delta_B0 + 1];
    for (int i = 0; i <= delta_B0; i++) {
        final_freq[i] = 0;
        suffix_freq[i] = 0;
        prefix_freq[i] = 0;
        one_to_j[i] = 0;
        one_to_i_minus_one[i] = 0;
    }

    int bi = floor((start_index - 1) / delta_B0 + 0.0) - 1;
    int bj = floor((end_index) / delta_B0 + 0.0) - 1;
    int prefix_start = (bi + 1) * delta_B0 + 1;
    int prefix_end = start_index - 1;
    int suffix_start = (bj + 1) * delta_B0 + 1;
    int suffix_end = end_index;
    //cout << "start index: " << start_index << " end index: " << end_index << endl;
    //cout << "delta: " << delta_B0 << endl;
    //cout << "bi: " << bi   <<" prefix_start: " << prefix_start << " preffix_end: " << prefix_end << endl;
    //cout << "bj: " << bj   <<" suffix_start: " << suffix_start << " suffix_end: " << suffix_end << endl;

    ///cout << "777777" << endl;
    if (bi < 0 && bj <= 0) {
        for (int i = start_index; i <= end_index; i++) {
            //cout << "upper: " << upper_line_array_B0[i - 1] << " ";
            final_freq[toolBox.upper_line_array_B0[i - 1]]++;
            //cout << final_freq[upper_line_array_B0[i - 1]] << endl;
        }
    } else if (bi < 0 && bj > 0) {
        for (int i = suffix_start; i <= suffix_end; i++) {
            suffix_freq[toolBox.upper_line_array_B0[i - 1]]++;
        }
        for (int i = 1; i <= delta_B0; i++) {
            one_to_j[i] = suffix_freq[i] + toolBox.Ci[bj][i];
        }
        for (int i = prefix_start; i <= prefix_end; i++) {
            prefix_freq[toolBox.upper_line_array_B0[i - 1]]++;
        }
        for (int i = 1; i <= delta_B0; i++) {
            one_to_i_minus_one[i] = prefix_freq[i];
        }
        for (int i = 0; i <= delta_B0; i++) {
            final_freq[i] = one_to_j[i] - one_to_i_minus_one[i];
            //cout << final_freq[i] << " ";
        }
    } else {
        for (int i = suffix_start; i <= suffix_end; i++) {
            suffix_freq[toolBox.upper_line_array_B0[i - 1]]++;
        }
        for (int i = 1; i <= delta_B0; i++) {
            one_to_j[i] = suffix_freq[i] + toolBox.Ci[bj][i];
        }
        for (int i = prefix_start; i <= prefix_end; i++) {
            prefix_freq[toolBox.upper_line_array_B0[i - 1]]++;
        }
        for (int i = 1; i <= delta_B0; i++) {
            one_to_i_minus_one[i] = prefix_freq[i] + toolBox.Ci[bi][i];
        }
        for (int i = 0; i <= delta_B0; i++) {
            final_freq[i] = one_to_j[i] - one_to_i_minus_one[i];
            //cout << final_freq[i] << " ";
        }
    }
    //cout << "888888" << endl;

    int c = 0, freq_c = 0;
    for (int i = 1; i <= delta_B0; i++) {
        if (final_freq[i] > freq_c) {
            freq_c = final_freq[i];
            c = i;
            //cout <<"mode: " << c << " freq: " << freq_c << endl;
        }
    }
    int count = c;
    set<int>::iterator iter = toolBox.D_B0.begin();
    if (count == 1) {
        c = *toolBox.D_B0.begin();
    }
    while (iter != toolBox.D_B0.end() && count > 1) {
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
    // cout <<"mode: " << c << " freq: " << freq_c << endl;
    res = make_pair(c, freq_c);
    delete[] prefix_freq;
    delete[] suffix_freq;
    delete [] final_freq;
    delete[] one_to_j;
    delete[] one_to_i_minus_one;
    return res;

}
pair<int, int> bitMethod( int start_index, int end_index, int s, int t) {

    pair<int, int> res;
    //cout << "s: " << s << " t: " << t << endl;

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
            if (toolBox.Qa_B1[toolBox.upper_line_array_B1[x-1] - 1][toolBox.A_Prime_B1[x-1] - 1] >= start_index) {
                continue;
            } else {
                if (toolBox.A_Prime_B1[x-1] + freq_c - 1 < toolBox.QaMap_B1[toolBox.upper_line_array_B1[x-1]] && toolBox.Qa_B1[toolBox.upper_line_array_B1[x-1] - 1][toolBox.A_Prime_B1[x-1] + freq_c - 1] <= end_index) {

                    c = toolBox.B1_array[x-1];
                    freq_c = getCandidateFreq(toolBox.B1_len, freq_c, x, start_index ,end_index);

                    // cout << "c: " << c <<"  " << " freq_c: " << freq_c << endl;
                } else {
                    continue;
                }
            }
        }// the end of traversing the prefix

        // cout << "need special address!!!" << endl;
    } else {

        int pos_bj = toolBox.rrrb_select_1(bj - bi + 1 + toolBox.rrrb_rank_1(toolBox.presumSize[bi])) - toolBox.presumSize[bi];
        int span_mode = 0;
        int span_freq = toolBox.rrrb_rank_0(pos_bj + toolBox.presumSize[bi]) - toolBox.rrrb_rank_0(toolBox.presumSize[bi]);

        int pos_last = toolBox.rrrb_select_0(span_freq + toolBox.rrrb_rank_0(toolBox.presumSize[bi])) - toolBox.presumSize[bi];
        int b_last = toolBox.rrrb_rank_1(pos_last + toolBox.presumSize[bi]) - toolBox.rrrb_rank_1(toolBox.presumSize[bi]) + bi;



        int b_last_start = b_last * t + 1, b_last_end = b_last_start + t - 1;
        //cout << "b_last: " << b_last_start << " " << b_last_end << endl;


        for (int i = b_last_start; i <= b_last_end; i++) {
            int tmp = toolBox.upper_line_array_B1[i - 1];
            int index_start = bi * t + 1;
            int index_end = i;
            /*cout << "tmp:" << tmp << endl;
            cout << "Qa: " << tmp - 1 << " " << A_Prime_B1[i - 1] << endl;
            cout << index_start << " " << index_end << endl;
            cout << " span freq: " << span_freq << endl;
            cout << "sec value: " << A_Prime_B1[i - 1] - span_freq + 1 << endl;*/
            if (toolBox.A_Prime_B1[i - 1] - span_freq + 1 >= 0) {
                int value = toolBox.Qa_B1[tmp - 1][toolBox.A_Prime_B1[i - 1] - span_freq + 1];
                if (value >= index_start) {
                    span_mode = toolBox.B1_array[i - 1];
                    //cout << "span mode: " << span_mode << endl;
                }
            } else {
                continue;
            }
        }

        // cout << "span mode: " << span_mode << endl;
        // cout << "span freq: " << span_freq << endl;
        c = span_mode;  // candidate
        freq_c = span_freq;
        for (int x = prefix_start; x <= prefix_end; x++) {       // start to traverse the prefix to find the candidate

            if (toolBox.Qa_B1[toolBox.upper_line_array_B1[x-1] - 1][toolBox.A_Prime_B1[x-1] - 1] >= start_index) {
                continue;
            } else {
                if (toolBox.A_Prime_B1[x-1] + freq_c - 1 < toolBox.QaMap_B1[toolBox.upper_line_array_B1[x-1]] && toolBox.Qa_B1[toolBox.upper_line_array_B1[x-1] - 1][toolBox.A_Prime_B1[x-1] + freq_c - 1] <= end_index) {

                    c = toolBox.B1_array[x-1];
                    freq_c = getCandidateFreq(toolBox.B1_len, freq_c, x, start_index ,end_index);

                    //cout << "inner c: " << c <<"  " << " freq_c: " << freq_c << endl;
                } else {
                    continue;
                }
            }
        }// the end of traversing the prefix
        for (int x = suffix_end; x >= suffix_start; x--) {       // start to traverse the prefix to find the candidate

            if (toolBox.A_Prime_B1[x-1] + 1 < toolBox.QaMap_B1[toolBox.upper_line_array_B1[x-1]] && toolBox.Qa_B1[toolBox.upper_line_array_B1[x-1] - 1][toolBox.A_Prime_B1[x-1] + 1] >= suffix_start && toolBox.Qa_B1[toolBox.upper_line_array_B1[x-1] - 1][toolBox.A_Prime_B1[x-1] + 1] <= end_index) {
                //cout<< " jinlaile" <<endl;
                continue;
            } else {
                if (toolBox.A_Prime_B1[x-1] - freq_c + 1 >= 0 && toolBox.Qa_B1[toolBox.upper_line_array_B1[x-1] - 1][toolBox.A_Prime_B1[x-1] - freq_c + 1] >= start_index) {

                    c = toolBox.B1_array[x-1];
                    freq_c = getSuffixCandidateFreq(toolBox.B1_len, freq_c, x, start_index ,end_index);

                    //cout << "inner: c: " << c <<"  " << " freq_c: " << freq_c << endl;
                } else {
                    continue;
                }
            }
        }// the end of traversing the prefix


    }


    //cout << "c: " << c <<"  " << " freq_c: " << freq_c << endl;
    res = make_pair(c, freq_c);

    return res;
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
int getCandidateFreq( int len, int freq_c, int x, int i, int j) {
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
    for (int q = toolBox.A_Prime_B1[x-1] + freq_c - 1; q  < toolBox.QaMap_B1[toolBox.upper_line_array_B1[x-1]]; q++) {
        if (toolBox.Qa_B1[toolBox.upper_line_array_B1[x-1] - 1][q] > j) {
            break;
        }
        currentKey = q;
    }
    //  cout << "currentKey : " << currentKey << endl;
    int freq = currentKey - toolBox.A_Prime_B1[x-1] + 1;

    return freq;
}
int getSuffixCandidateFreq(  int len, int freq_c, int x, int i, int j) {
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
    for (int q = toolBox.A_Prime_B1[x-1]; q  >= 0; q--) {
        if (toolBox.Qa_B1[toolBox.upper_line_array_B1[x-1] - 1][q]  < i) {
            //  cout << q   <<"  "  << Qa[upper_line_array[x-1] - 1][q] <<" chu lai le";
            break;
        }
        /*cout << "x " << x  << "  A_Prime[x-1]  " << A_Prime[x-1] << " Qa[" << upper_line_array[x-1] - 1 <<
             "][" << q << "]" <<endl;*/
        currentKey = q;
    }

    int freq = toolBox.A_Prime_B1[x-1] - currentKey + 1;

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
void update_Ci_Table( int numOfBlocks) {
    for (int i = 0; i < numOfBlocks; i++) {
        int count = 0;
        map<int,int> normalMap;
        set<int> set;
        for (int j = 0; j < toolBox.delta_B0 * (i + 1) ; j++) {
            normalMap[toolBox.upper_line_array_B0[j]]++;
        }
        for (int j = 1; j <= toolBox.delta_B0; j++) {
            if (set.count(j) != 1) {
                // cout << "i: " << i << " j: " << j <<endl;
                normalMap.insert(make_pair(j, 0));
            }
        }
        for(auto it = normalMap.cbegin(); it != normalMap.cend(); ++it)
        {
            toolBox.Ci[i][it->first] = it->second;
        }
    }
    /*for (int i = 0; i < numOfBlocks; i++) {
        for (int j = 0; j < delta + 1; j++) {
            cout << Ci[i][j] << " ";
        }
        cout << endl;
    }*/

}