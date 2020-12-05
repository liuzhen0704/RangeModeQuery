//
// Created by liu1 on 2020-12-02.
//

//
// Created by liu1 on 11/27/20.
//


#include<iostream>
#include<string>
#include <fstream>

using namespace std;
void read_file(int pa[], string file, int len);

int main() {
    int len;   // the length of input array
    cout << "please input the length of the array: ";
    cin >> len;
    double alpha;   // the length of input array
    cout << "please input the alpha: ";
    cin >> alpha;
    //cout << " s: " << s << endl;
    int *originalArray = new int[3000];
    string file_in = "/users/grad/liu1/normal_result.txt";
    read_file(originalArray, file_in, len);
    int *approximateArray = new int[3000];
    string file_in_2 = "/users/grad/liu1/approximate_result.txt";
    read_file(approximateArray, file_in_2, len);
    for (int i = 0; i < 3000; i++) {
        if (originalArray[i] * alpha <= approximateArray[i]) {
            //cout << "check " << i << endl;
            continue;
        }  else {
            cout << "False!!!!" << endl;
        }
    }
    cout << "all good!!!" << endl;
}
void read_file(int pa[], string file, int len) {
    ifstream file_in(file);
    int temp;
    if (!file_in.is_open()) {
        cout << " cannot open the file" << endl;
    }
    for (int i = 0; i < len; i++) {
        file_in >> pa[i];
    }
    /*  for (int i = 0; i < len; i++) {
          cout << pa[i] << " ";
      }*/
    file_in.close();
    file_in.clear();
}