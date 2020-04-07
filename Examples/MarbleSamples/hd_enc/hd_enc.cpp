#include <vector>
#include <functional>
#include "G.hpp"

using namespace std;
using namespace Granite;

void hd_enc(vector<G> v, vector<G> u) {
    vector<G> diff = (1 - (v - u)*(v - u)); //TODO instead of this workaround, use a library supporting !=
    G sum = fold(diff,G::sum);
    output(sum);
}

int main() {
    vector<int> v = {0, 1, 1, 0, 0/*...*/};
    vector<int> u = {1, 0, 1, 0, 1/*...*/};

    vector<G> v_enc = encrypt(v);
    vector<G> u_enc = encrypt(u);

    // Simulates the execution and
    // reports e.g. multiplicative depth
    //int mult_depth = G::analyse(bind(hd_enc, v_enc, u_enc));

    // Benchmarks the application,
    // using the most appropriate settings
    double ms = G::evaluate(bind(hd_enc, v_enc, u_enc), Wool::Library::SEALBFV); //TODO: change to HElib

    long res = G::result(bind(hd_enc, v_enc, u_enc), Wool::Library ::SEALBFV);
    cout << res << endl;
    //cout << mult_depth << endl;
    cout << ms << endl;
    return 0;
}