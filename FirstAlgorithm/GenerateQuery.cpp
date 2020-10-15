//
// Created by liu1 on 9/10/20.
//
#include <iostream>
#include <cmath>
#include <fstream>
#include<time.h>

using namespace std;


int main() {
    int len = 0;
    cout << "please input the length of the array: ";
    cin >> len;
    ofstream tiny_file("/home/liu1/Desktop/tiny.txt");
    ofstream med_file("/home/liu1/Desktop/med.txt");
    ofstream huge_file("/home/liu1/Desktop/huge.txt");
    //    file_out << num <<"  ";
    /*int *tiny_query = new int[100];
    int *med_query = new int[100];
    int *huge_query = new int[100];*/
    int tiny_k = 100, med_k = 10, huge_k = 1;
    srand((unsigned)time(NULL));
    int first_point = 0;
    int second_point = 0;
    int buffer_point = 0;
    cout << "tiny part " << endl;
    for (int i = 0; i < 2000; i+=2) {
        first_point = (rand() % ((len - 1) + 1)) + 1;
        buffer_point = first_point + (len - first_point) / tiny_k;
        second_point = (rand() % (buffer_point - first_point + 1)) + first_point;
        /*cout << "first point: " << first_point << " buffer point:" << buffer_point<<
        "  second point: " << second_point << endl;
        tiny_query[i] = first_point;
        tiny_query[i + 1] = second_point;*/
        tiny_file << first_point <<"  " << second_point <<"  ";
    }
    cout << "med part " << endl;
    for (int i = 0; i < 2000; i+=2) {
        first_point = (rand() % ((len - 1) + 1)) + 1;
        buffer_point = first_point + (len - first_point) / med_k;
        second_point = (rand() % (buffer_point - first_point + 1)) + first_point;
        /*cout << "first point: " << first_point << " buffer point:" << buffer_point<<
        "  second point: " << second_point << endl;
        med_query[i] = first_point;
        med_query[i + 1] = second_point;*/
        med_file << first_point <<"  " << second_point <<"  ";
    }
    cout << "huge part: " << endl;
    for (int i = 0; i < 2000; i+=2) {
        first_point = (rand() % ((len - 1) + 1)) + 1;
        buffer_point = first_point + (len - first_point) / huge_k;
        second_point = (rand() % (buffer_point - first_point + 1)) + first_point;
        /*cout << "first point: " << first_point << " buffer point:" << buffer_point<<
        "  second point: " << second_point << endl;
        huge_query[i] = first_point;
        huge_query[i + 1] = second_point;*/
        huge_file << first_point <<"  " << second_point <<"  ";
    }
    if (tiny_file) {
        cout << "create tiny file successfully!" << endl;
    }
    if (med_file) {
        cout << "create med file successfully!" << endl;
    }
    if (huge_file) {
        cout << "create huge file successfully!" << endl;
    }
    tiny_file.close();
    tiny_file.clear();
    med_file.close();
    med_file.clear();
    huge_file.close();
    huge_file.clear();
    /*delete[] tiny_query;
    delete[] med_query;
    delete[] huge_query;*/
}
