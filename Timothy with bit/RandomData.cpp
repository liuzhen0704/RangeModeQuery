//
// Created by liu1 on 8/12/20.
//
#include <iostream>
#include <cstdlib>
#include <time.h>
#include <fstream>

using namespace std;

int main() {
    srand((unsigned)time(NULL));
    ofstream file_out("/home/liu1/Desktop/data.txt");
    int length;
    cout << "the length of randomized array : ";
    cin >> length;
    int num = 0;
    for (int i = 0; i < length; i++) {
        num = rand() % 10000;
        file_out << num <<"  ";
    }
    if (file_out) {
        cout << "create file successfully!";
    }
    file_out.close();
    file_out.clear();

}
