//
// Created by liu1 on 2020-12-02.
//

//
// Created by liu1 on 11/18/20.
//

#include <iostream>
#include <cstdlib>
#include <random>
#include <fstream>

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
    ofstream file_out("/users/grad/liu1/data.txt");
    int length;
    cout << "the length of randomized array : ";
    cin >> length;
    int num = 0;
    Generator g(0, 500);
    for (int i = 0; i < length; i++) {
        num = g();
        //cout << num << endl;
        file_out << num <<"  ";
    }
    if (file_out) {
        cout << "create file successfully!";
    }
    file_out.close();
    file_out.clear();

}
