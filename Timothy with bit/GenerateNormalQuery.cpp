//
// Created by liu1 on 11/17/20.
//

//
// Created by liu1 on 9/10/20.
//
#include <iostream>
#include <cmath>
#include <fstream>
#include<time.h>
#include<random>

using namespace std;


class Generator {
    std::default_random_engine generator;
    std::normal_distribution<double> distribution;
    double min;
    double max;
public:
    Generator(double mean, double stddev, double min, double max):
            distribution(mean, stddev), min(min), max(max)
    {}
    Generator(double min, double max):
            distribution((min + max) / 2, (max - min) / 6), min(min), max(max)
    {}

    double operator ()() {
        while (true) {
            double number = this->distribution(generator);
            if (number >= this->min && number <= this->max)
                return number;
        }
    }
};

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
    //srand((unsigned)time(NULL));
    int first_point = 0;
    int second_point = 0;
    int buffer_point = 0;
    Generator tiny_g(1, len);
    Generator med_g(1, len);
    Generator huge_g(1, len);
    cout << "tiny part " << endl;
    for (int i = 0; i < 2000; i+=2) {

        first_point = tiny_g();
        buffer_point = first_point + (len - first_point) / tiny_k;
        int scope_end = (buffer_point - first_point + 1) + first_point;
        Generator tiny_g1(first_point, scope_end);
        second_point = tiny_g1();
        /*cout << "first point: " << first_point << " buffer point:" << buffer_point<<
        "  second point: " << second_point << endl;*/

        tiny_file << first_point <<"  " << second_point <<"  ";
    }
    //cout << "med part " << endl;
    for (int i = 0; i < 2000; i+=2) {
        first_point = med_g();
        buffer_point = first_point + (len - first_point) / med_k;
        int scope_end = (buffer_point - first_point + 1) + first_point;
        Generator med_g1(first_point, scope_end);
        second_point = med_g1();
        /*cout << "first point: " << first_point << " buffer point:" << buffer_point<<
        "  second point: " << second_point << endl;*/

        med_file << first_point <<"  " << second_point <<"  ";
    }
    //cout << "huge part: " << endl;
    for (int i = 0; i < 2000; i+=2) {
        first_point = huge_g();
        buffer_point = first_point + (len - first_point) / huge_k;
        int scope_end = (buffer_point - first_point + 1) + first_point;
        Generator huge_g1(first_point, scope_end);
        second_point = huge_g1();
        /*cout << "first point: " << first_point << " buffer point:" << buffer_point<<
        "  second point: " << second_point << endl;*/

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


}
