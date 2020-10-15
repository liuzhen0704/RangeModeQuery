//
// Created by liu1 on 9/10/20.
//

#include <iostream>
#include <cmath>
#include <fstream>
#include<time.h>

using namespace std;

int main() {
    int len = 100;
    int *tiny = new int[len];
    int *med = new int[len];
    int *huge = new int[len];
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
        tiny_in >> tiny[i];
    }
    if (!med_in.is_open()) {
        cout << " cannot open the med file" << endl;
    }
    for (int i = 0; i < len; i++) {
        med_in >> med[i];
    }
    if (!huge_in.is_open()) {
        cout << " cannot open the huge file" << endl;
    }
    for (int i = 0; i < len; i++) {
        huge_in >> huge[i];
    }
    tiny_in.close();
    tiny_in.clear();
    med_in.close();
    med_in.clear();
    huge_in.close();
    huge_in.clear();
    delete[] tiny;
    delete[] med;
    delete[] huge;
}