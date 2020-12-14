//
// Created by liu1 on 11/18/20.
//

#include <iostream>
#include <cstdlib>
#include <random>
#include <fstream>
#include <map>
using namespace std;

void read_file(string array[], string file, int len);

int main() {
    int len;
    cout << "the length of randomized array : ";
    cin >> len;
    string *array = new string[len];
    string file_in = "/home/liu1/Desktop/book.txt";
    read_file(array, file_in, len);
    ofstream file_out("/home/liu1/Desktop/bookdata.txt");
    map<string,int> map;
    int num = 0;
    for (int i = 0; i < len; i++) {
        auto sit = map.find(array[i]);
        if (sit != map.end()) {
            int count = map[array[i]];
            num = count;
        } else {
            map.insert(make_pair(array[i], ++num));
        }
        //cout << num << endl;
        file_out << num <<"  ";
    }
    if (file_out) {
        cout << "create file successfully!";
    }
    file_out.close();
    file_out.clear();
    delete[] array;
}
//
// Created by liu1 on 11/18/20.
//

void read_file(string array[], string file, int len) {
    ifstream file_in(file);
    int temp;
    if (!file_in.is_open()) {
        cout << " cannot open the file" << endl;
    }
    for (int i = 0; i < len; i++) {
        file_in >> array[i];
        //cout << array[i] << endl;
    }
    /*for (int i = 0; i < len; i++) {
        cout << array[i] << endl;
    }*/
    file_in.close();
    file_in.clear();
}//
// Created by liu1 on 2020-12-02.
//

