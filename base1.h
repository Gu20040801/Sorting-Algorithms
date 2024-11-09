#pragma once
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <string>
#include <vector>

using namespace std;
template <typename T>
ostream &operator<<(ostream &os, const vector<T> &v)
{
    int n = v.size();
    os << "{";
    for (int i = 0; i < v.size(); i++)
    {
        os << v[i];
        if (i < n - 1)
        {
            os << ", ";
        }
    }
    os << "}";
    return os;
}

//
// SortStats stores statistics about one run of a sorting routine.
//

// "ulong" is easier to type than "unsigned long"
typedef unsigned long ulong;

struct SortStats
{
    string sort_name;
    size_t vector_size = 0;
    ulong num_comparisons = 0;
    double cpu_running_time_sec = 0.0;

    string to_csv() const
    {
        return sort_name + ", " + to_string(vector_size) + ", " + to_string(num_comparisons) + ", " + to_string(cpu_running_time_sec);
    }
}; // struct SortStats

//
// For conveniently printing SortStats objects.
//
ostream &operator<<(ostream &os, const SortStats &ss)
{
    os << "SortStats{" << ss.sort_name
       << ", size=" << ss.vector_size
       << ", num_comparisons=" << ss.num_comparisons
       << ", cpu_running_time_sec=" << ss.cpu_running_time_sec
       << "}";
    return os;
}

template <typename T>
struct merged{
    vector<T> merged_array;
    ulong num_compar = 0;
};

template <typename T>
bool is_sorted(vector<T> &v){
    if(v.size()==0 || v.size()==1){
        return true;
    }else{
        for(int i = 0; i < (v.size()-1); i++){
            if(v[i]>v[i+1]){
                return false;
            }
        }
        return true;
    }
}

template <typename T>
SortStats bubble_sort(vector<T> &v)
{
    ulong num_comps = 0;
    clock_t start = clock(); 

    for (int i = 0; i < v.size(); i++)
    {
        for (int j = 0; j < v.size() - 1; j++)
        {
            num_comps++;
            if (v[j] > v[j + 1])
            {
                T temp = v[j];
                v[j] = v[j + 1];
                v[j + 1] = temp;
            }
        }
    }
    
    clock_t end = clock();
    double elapsed_cpu_time_sec = double(end - start) / CLOCKS_PER_SEC;
    
    return SortStats{"Bubble sort",
                     v.size(),
                     num_comps,
                     elapsed_cpu_time_sec};
}

template <typename T>
SortStats insertion_sort(vector<T> &v){
    ulong num_of_compar = 0;
    clock_t start = clock();
    if(v.size() <= 1){
        clock_t end = clock();
        double time = double(end - start) / CLOCKS_PER_SEC;
        return SortStats{"Insertion Sort", v.size(), num_of_compar, time};
    }
    for(int i = 1; i < v.size(); i++){
        if(v[i] >= v[i-1]){
            num_of_compar++;
        } else {
            T element = v[i];
            int j = i;
            while(j != 0 && element < v[j-1]){
                num_of_compar++;
                v[j] = v[j-1];
                j--;
            }
            v[j] = element;
        }
    }
    clock_t end = clock();
    double time = double(end - start) / CLOCKS_PER_SEC;
    return SortStats{"Insertion Sort", v.size(), num_of_compar, time};
}

template <typename T>
SortStats selection_sort(vector<T> &v){
    ulong num_of_compar = 0;
    clock_t start = clock();
    if(v.size()==0 || v.size()==1){
        clock_t end = clock();
        double time = double(end-start)/ CLOCKS_PER_SEC;
        return SortStats{"Selection Sort", v.size(), num_of_compar, time};
    }
    for(int i = 0; i < v.size(); i++){
        int min_element_index = i;
        for(int j = i; j < v.size(); j++){
            num_of_compar++;
            if(v[j]<v[min_element_index]){
                min_element_index = j;
            }
        }
        T min_element = v[min_element_index];
        v[min_element_index]=v[i];
        v[i] = min_element;
    }
    clock_t end = clock();
    double time = double(end-start) / CLOCKS_PER_SEC;
    return SortStats{"Selection Sort", v.size(), num_of_compar, time};
}

template <typename T>
merged<T> merge(vector<T> &left, vector<T> &right){
    vector<T> array;
    ulong num = 0;
    int leftindex = 0;
    int rightindex = 0;
    while(leftindex<left.size() && rightindex<right.size()){
        if(left[leftindex]<=right[rightindex]){
            array.push_back(left[leftindex]);
            leftindex++;
        }else{
            array.push_back(right[rightindex]);
            rightindex++;
        }
        num++;       
    }
    while (leftindex < left.size()) {
        array.push_back(left[leftindex]);
        leftindex++;
    }
    while (rightindex < right.size()) {
        array.push_back(right[rightindex]);
        rightindex++;
    }

    return merged<T>{array, num};

}

template <typename T>
merged<T> mergesort1(vector<T> &v){
    if(v.size()<=1){
        return merged<T>{v, 0};
    }else{
        ulong num_compar = 0;
        int mid_index = v.size()/2;
        vector<T> left;
        vector<T> right;
        for(int i = 0; i < mid_index; i++){
            left.push_back(v[i]);
        }
        for(int i = mid_index; i < v.size(); i++){
            right.push_back(v[i]);
        }
        merged<T> left_result = mergesort1(left);
        merged<T> right_result = mergesort1(right);
        num_compar+=left_result.num_compar;
        num_compar+=right_result.num_compar;
        merged<T> result = merge(left_result.merged_array, right_result.merged_array);
        return merged<T>{result.merged_array, num_compar};
    }
}


template <typename T>
SortStats merge_sort(vector<T> &v){
    clock_t start = clock();
    merged<T> result = mergesort1(v);
    v = result.merged_array;
    clock_t end = clock();
    double time = double(end-start)/CLOCKS_PER_SEC;
    return SortStats{"Merge Sort", v.size(), result.num_compar, time};
}

template <typename T>
merged<T> quicksort1(vector<T> &v){
    if(v.size()<=1){
        return merged<T>{v, 0};
    }
    ulong num_compar = 0;
    vector<T> left;
    vector<T> right;
    T pivot = v[0];    
    for(int i = 1; i < v.size(); i++){
        if(v[i]<=pivot){
            num_compar++;
            left.push_back(v[i]);
        }else{
            num_compar++;
            right.push_back(v[i]);
        }
    }   
    merged<T> left1 = quicksort1(left);
    merged<T> right1 = quicksort1(right);
    vector<T> result;
    
    num_compar+=left1.num_compar;
    num_compar+=right1.num_compar;
    
    if(left1.merged_array.size()==0){
        result.push_back(pivot);
        for(int i = 0; i < right1.merged_array.size(); i++){
            result.push_back(right1.merged_array[i]);
        }
        return merged<T>{result, num_compar};
    }else if(right1.merged_array.size()==0){
    
        for(int i = 0; i < left1.merged_array.size(); i++){
            result.push_back(left1.merged_array[i]);
        }
        result.push_back(pivot);
        return merged<T>{result, num_compar};
    }else{
    
        for(int i = 0; i < left1.merged_array.size(); i++){
            result.push_back(left1.merged_array[i]);
        }
        result.push_back(pivot);
        for(int i = 0; i < right1.merged_array.size(); i++){
            result.push_back(right1.merged_array[i]);
        }
        return merged<T>{result, num_compar};
    }
}


template <typename T>
SortStats quick_sort(vector<T> &v){
    
    clock_t start = clock();
    merged<T> result = quicksort1(v);
    v = result.merged_array;
    clock_t end = clock();
    double time = double(end-start)/CLOCKS_PER_SEC;
    return SortStats{"Quick Sort", v.size(), result.num_compar, time};
}



template <typename T>
SortStats shell_sort(vector<T> &v){
    clock_t start = clock();
    if(v.size()<=1){
        clock_t end = clock();
        double time = double(end-start)/ CLOCKS_PER_SEC;
        return SortStats{"Shell Sort", v.size(), 0, time};
    }
    int n = v.size();
    ulong num_compar = 0;
    for (int gap = n / 2; gap > 0; gap /= 2) {
        
        for (int i = gap; i < n; i++) {
            T temp = v[i];
            int j = i;
            
        
            while (j >= gap && v[j - gap] > temp) {
                num_compar++;
                v[j] = v[j - gap];
                j -= gap;
            }
            v[j] = temp;
        }
    }
    clock_t end = clock();
    double time = double(end-start)/ CLOCKS_PER_SEC;
    return SortStats{"Shell Sort", v.size(), num_compar, time};
}


int determine_threshold(int n){
    if(n<=10){
        return 2;
    }else{
        return 20;
    }
}



template <typename T>
merged<T> iquicksort1(vector<T> &v, int k){
    if (v.size() <= 1) {
        return merged<T>{v, 0};
    }
    ulong num_compar = 0;
    if(v.size()>=k){  
        T pivot= v[0];
        vector<T> left;
        vector<T> right;
        for(int i = 1; i < v.size(); i++){
            if(v[i]<=pivot){
                left.push_back(v[i]);
                num_compar++;
            }else{
                right.push_back(v[i]);
                num_compar++;
            }
        }
        merged<T> left1 = iquicksort1(left, k);
        merged<T> right1 = iquicksort1(right, k);
        num_compar+= left1.num_compar;
        num_compar+= right1.num_compar;
        vector<T> result;
        if(left1.merged_array.size()==0){
            result.push_back(pivot);
            for(int i = 0; i < right1.merged_array.size(); i++){
                result.push_back(right1.merged_array[i]);
            }
            return merged<T>{result, num_compar};
        }else if(right1.merged_array.size()==0){
            for(int i = 0; i < left1.merged_array.size(); i++){
                result.push_back(left1.merged_array[i]);
            }
            result.push_back(pivot);
            return merged<T>{result, num_compar};
        }else{
            for(int i = 0; i < left1.merged_array.size(); i++){
                result.push_back(left1.merged_array[i]);
            }
            result.push_back(pivot);
            for(int i = 0; i < right1.merged_array.size(); i++){
                result.push_back(right1.merged_array[i]);
            }
            return merged<T>{result, num_compar};
        }
    }  
    else
    {
        for(int i = 1; i < (v.size()); i++){
            if(v[i]>=v[i-1]){
                num_compar++;
            }else{
                T element = v[i];
                int j = i;
                while(j!=0 && element<v[j-1]){
                    num_compar++;
                    j--;
                }
                v[i] = v[j];
                v[j] = element;
            }
        }
        return merged<T>{v, num_compar};

    }
}

template <typename T>
SortStats iquick_sort(vector<T> &v) {
    clock_t start = clock();
    int k = determine_threshold(v.size());
    merged<T> result = iquicksort1(v, k);
    v = result.merged_array;
    clock_t end = clock();
    double time = double(end - start) / CLOCKS_PER_SEC;
    return SortStats{"IQuick Sort", v.size(), result.num_compar, time};
}

vector<int> rand_vec(int n, int min, int max) {
    srand(time(nullptr));
    vector<int> result;
    result.reserve(n);
    for (int i = 0; i < n; i++) {
        int num = min + rand() % (max - min + 1);
        result.push_back(num);
    }
    return result;
}
