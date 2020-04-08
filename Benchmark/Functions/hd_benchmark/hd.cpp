#include <vector>
#include <fstream>
#include <chrono>
#include <iostream>
#include <omp.h>

using namespace std;
using namespace chrono;

int hd_plaintext(vector<int> v, vector<int> u) {
    vector<int> t;
    for (size_t i = 0; i < v.size(); i++){
        t.push_back(1 - (v[i] - u[i])*(v[i] - u[i]));
    }
    while(t.size() > 1) {
        if (t.size() % 2 == 1) { // just put last element on top to make equal again.
            t[0] += t.back();
            t.pop_back();
        }
        else {
            int rotationF = t.size() / 2;
            vector<int> tv;
            for (size_t i = 0; i < rotationF; i++){
                int ri = i + rotationF;
                tv.push_back(t[i] + t[ri]);
            }
            t = tv;
        }
    }

    return t[0];
}

int hd_plaintext_batched(vector<int> v, vector<int> u) {
    vector<int> t;
    #pragma omp simd
    for (size_t i = 0; i < v.size(); i++){
        t.push_back(1 - (v[i] - u[i])*(v[i] - u[i]));
    }
    while(t.size() > 1) {
        if (t.size() % 2 == 1) { // just put last element on top to make equal again.
            t[0] += t.back();
            t.pop_back();
        }
        else {
            int rotationF = t.size() / 2;
            vector<int> tv;
            #pragma omp simd
            for (size_t i = 0; i < rotationF; i++){
                tv.push_back(t[i] + t[i + rotationF]);
            }
            t = tv;
        }
    }

    return t[0];
}

int main() {
    ofstream resfile;
    resfile.open ("../../../../Benchmark/results/granite_hd_plainC++.csv");
    resfile << "#n,#batched,#slow\n";

    int n_runs = 10;
    int start_size = 1;
    int end_size = 180;
    for (size_t i = start_size; i < end_size ; i++) {
        vector<int> v;
        vector<int> u;
        for (size_t j = 0; j < i; j++){
            v.push_back(1); //maybe add randomness. But shouldn't matter for performance measuring
            u.push_back(j % 2);
        }
        int n = v.size();
        duration<double, std::milli> time_span;
        for (auto k = 0; k < n_runs; k++) {
            high_resolution_clock::time_point t1 = high_resolution_clock::now();
            hd_plaintext(v, u);
            high_resolution_clock::time_point t2 = high_resolution_clock::now();
            time_span += t2 - t1;
        }
        double avg = time_span.count() / n_runs;
        duration<double, std::milli> time_span_batched;
        for (auto k = 0; k < n_runs; k++) {
            high_resolution_clock::time_point t1 = high_resolution_clock::now();
            hd_plaintext_batched(v, u);
            high_resolution_clock::time_point t2 = high_resolution_clock::now();
            time_span_batched += t2 - t1;
        }
        double avg_batched = time_span_batched.count() / n_runs;
        resfile << to_string(n) << "," << to_string(avg_batched) << "," << to_string(avg) << endl;
        resfile.flush();
    }
    resfile.close();

}