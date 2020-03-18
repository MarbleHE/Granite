#include <vector>
#include <functional>
#include "M.hpp"

using namespace std;
using namespace Marble;

void hd_enc(vector<M> v, vector<M> u) {
    M sum = 0;
    for (int i = 0; i < v.size(); ++i) {
        sum += (v[i] != u[i]);
    }
    output(sum);
}

int main() {
    vector<int> v = {0, 1, 1, 0, 0/*...*/};
    vector<int> u = {1, 0, 1, 0, 1/*...*/};

    vector<M> v_enc = encrypt(v);
    vector<M> u_enc = encrypt(u);

    // Simulates the execution and
    // reports e.g. multiplicative depth
    //int mult_depth = M::analyse(bind(hd_enc, v_enc, u_enc));

    // Benchmarks the application,
    // using the most appropriate settings
    double ms = M::evaluate(bind(hd_enc, v_enc, u_enc), Wool::Library::SEALBFV);

    long res = M::result(bind(hd_enc, v_enc, u_enc));
    cout << res << endl;
    //cout << mult_depth << endl;
    cout << ms << endl;
    return 0;
}