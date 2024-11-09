#include <iostream>
#include <ctime>
#include "base1.h"
using namespace std;

int main() {

    /*
    Here are the parameters you could modify
    N = number of random elements generated
    min = minimum number of the elements
    max = maximum number of the elements
    sorttype = type of sorts:{
        1: Bubble Sort
        2: Insertion Sort
        3: Selection Sort
        4: Merge Sort
        5: Quick Sort
        6: Shell Sort
        7: Iquick Sort 
    }
    */
    int N = 20000;
    int min = 1;
    int max = 10000;
    int sorttype = 4;

/*---------------------------------------------------------------------------------*/
    clock_t start = clock();
    vector<int> v = rand_vec(N, min, max);
    string result;
    if(sorttype == 1){
        bubble_sort(v);
    }else if(sorttype == 2){
        insertion_sort(v);
    }else if(sorttype == 3){
        selection_sort(v);
    }else if(sorttype == 4){
        merge_sort(v);
    }else if(sorttype == 5){
        quick_sort(v);
    }else if(sorttype == 6){
        shell_sort(v);
    }else if(sorttype == 7){
        iquick_sort(v);
    }else{
        cout << "Error Sort_type" << endl;
        return 0;
    }
    
    clock_t end = clock();
    double elapsed_cpu_time_sec = double(end - start) / CLOCKS_PER_SEC;
    cout << "CPU time: " << elapsed_cpu_time_sec << " seconds" << endl;
    return 0;
}