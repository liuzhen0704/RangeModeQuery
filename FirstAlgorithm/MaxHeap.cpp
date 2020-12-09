//
// Created by liu1 on 8/27/20.
//

#include <iostream>
using namespace std;



template <class T>
class MaxHeap{

private:

    T* array;
    T* count;
    int mCapacity;  // total size
    int mSize;   // real size

private:

    void heapify();
    void percolateUp(int index);
    void percolateDown(int index);
    void swap(T* array, int a, int b);

public:

    MaxHeap();
    MaxHeap(int cap);
    MaxHeap(T* array, int len);
    int size();
    bool isEmpty();
    //bool isFull();
    int peek();
    int offer(T ele);
    int poll();
    int update(int index, T ele);
    void deleteArray();
    void print();

};

template <class T>
void MaxHeap<T>::deleteArray() {
    delete[] array;
    delete[] count;
}

template <class T>
MaxHeap<T>::MaxHeap() {
    new (this)MaxHeap(10);
}

template <class T>
MaxHeap<T>::MaxHeap(int cap) {
    mSize = 0;
    mCapacity = cap;
    array = new T[mCapacity];
    count = new T[mCapacity];
    heapify();
}

template <class T>
MaxHeap<T>::MaxHeap(T* arr, int len) {
    array = arr;
    mSize = len;
    mCapacity = len;
    count = new T[mCapacity];
    heapify();
}

template <class T>
void MaxHeap<T>::heapify() {
    for (int i = mSize / 2 - 1; i >= 0; i--) {
        percolateUp(i);
    }
}

template <class T>
int MaxHeap<T>::size() {
    return size;
}

template <class T>
bool MaxHeap<T>::isEmpty() {
    return mSize == 0;
}

template <class T>
void MaxHeap<T>::percolateUp(int index) {
    while (index > 0) {
        int parentIndex = (index - 1) / 2;
        if (array[parentIndex] < array[index]) {
            swap(array, parentIndex, index);
        } else {
            break;
        }
        index = parentIndex;
    }
}

template <class T>
void MaxHeap<T>::percolateDown(int index) {
    while (index <= mSize / 2 - 1) {
        int leftChildIndex = index * 2 + 1;
        int rightChildIndex = index * 2 + 2;
        int swapCandidate = leftChildIndex;
        if (rightChildIndex <= mSize - 1 && array[leftChildIndex] <= array[rightChildIndex]) {
            swapCandidate = rightChildIndex;
        }
        if (array[index] < array[swapCandidate]) {
            swap(array, index, swapCandidate);
        } else {
            break;
        }
        index = swapCandidate;
    }
}

template <class T>
int MaxHeap<T>::peek() {
    return array[0];
}

template <class T>  // 0: success -1: fall
int MaxHeap<T>::offer(T ele) {  //modify here
    if (mSize == mCapacity) {
        return -1;
    }
    array[mSize] = ele;
    mSize++;
    percolateUp(mSize - 1);
}

template <class T>
int MaxHeap<T>::poll() {
    if (mSize == 0) {
        return -1;
    }
   // int res = array[0];
    array[0] = array[mSize - 1];
    mSize--;
    percolateDown(0);
    return 0;
}

template <class T>
void MaxHeap<T>::swap(T* array, int a, int b) {
    T temp = array[a];
    array[a] = array[b];
    array[b] = temp;
}

template <class T>
void MaxHeap<T>::print() {
    for (int i = 0; i < mSize; i++) {
        cout << array[i] << "  ";
    }
    cout << endl;
}

int main() {
    int arr[] = {4, 1, 3, 2, 4, 3, 2, 4, 2, 2, 1, 4, 4, 2, 1, 2, 3, 5, 5, 4, 1, 1, 4, 5};
    int a[] = {1, 5, 7, 2, 3};
    int a2[] = {6, 3, 11, 13, 8, 5, 19, 1, 4, 7};
    int s = 6, t = 4;
    int len = 24;
    int len2 = 5;
    int len3 = 10;
    MaxHeap<int>* tree = new MaxHeap<int>(a2, len3);
    for (int i = 0; i < len3; i++) {
    //    cout << a2[i] << " ";
        tree->offer(a2[i]);
    }
    cout << "start the print" << endl;
    tree->print();
    cout <<"peek: " << tree->peek() << endl;
    tree->poll();
    tree->print();
    tree->offer(20);
    tree->print();
    tree->deleteArray();
}