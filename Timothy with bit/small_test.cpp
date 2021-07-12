//
// Created by liu1 on 9/16/20.
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

int main() {
    int *data_query = new int[109];
    int len = 100000;
    string tiny_file = "/home/liu1/Desktop/tiny.txt";
    string med_file = "/home/liu1/Desktop/med.txt";
    string huge_file = "/home/liu1/Desktop/huge.txt";
    string data_file = "/home/liu1/Desktop/data.txt";
    ifstream tiny_in(tiny_file);
    ifstream med_in(med_file);
    ifstream huge_in(huge_file);
    ifstream data_in(data_file);
    if (!data_in.is_open()) {
        cout << " cannot open the tiny file" << endl;
    }
    int count = 0;
    for (int i = 0; i < len; i++){
        if (i >= 63359 && i <= 63467) {
            //cout << "zaogao" << endl;
            data_in >> data_query[count];
            cout << data_query[count] << " ";
            count++;
        }
    }
    ofstream te2("/home/liu1/Desktop/te2.txt");
    for (int i = 0; i < 109; i++) {
        //cout << "i: " << i <<"  "<< result[i] << endl;
        //cout << result[i] << "  ";
        te2 << data_query[i] << " ";
        if ((i + 1) % 10 == 0) {
            te2 << endl;
        }
    }
    if (te2) {
        cout << endl;
        cout << "create normal result file successfully!" << endl;
    }
    te2.close();
    te2.clear();
    delete[] data_query;
    tiny_in.close();
    tiny_in.clear();
    med_in.close();
    med_in.clear();
    huge_in.close();
    huge_in.clear();
    data_in.close();
    data_in.clear();
}