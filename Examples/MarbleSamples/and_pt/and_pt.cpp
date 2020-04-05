#include <vector>
#include <iostream>

using namespace std;


int and_pt(vector<int> v, vector<int> u) {
    int r = 0;
    for (size_t i = 0;  i < v.size(); i++) {
        r += v[i]*u[i];
    }
    return r;
}

int main() {
    vector<int> v = {0, 1, 1, 0, 0/*...*/};
    vector<int> u = {1, 0, 1, 0, 1/*...*/};

    std::cout << and_pt(v,u);
    return 0;
}