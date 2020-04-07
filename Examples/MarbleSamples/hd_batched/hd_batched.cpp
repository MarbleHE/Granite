#include <vector>
#include <functional>
#include "G.hpp"

using namespace std;
using namespace Granite;


void hd_batched(G v, G u) {
    G diff = (1 - (v - u)*(v - u)); //TODO instead of this workaround, use a library supporting !=
    diff.fold(G::sum);
    output(diff);
}

int main() {
    vector<int> v = {0, 1, 1, 0, 0/*...*/};
    vector<int> u = {1, 0, 1, 0, 1/*...*/};

    G v_enc = batchEncrypt(v);
    G u_enc = batchEncrypt(u);

    // Simulates the execution and
    // reports e.g. multiplicative depth
    int md = G::analyse(bind(hd_batched, v_enc, u_enc));

    // Benchmarks the application,
    // using the most appropriate settings
    //double ms = G::evaluate(bind(hd_batched, v_enc, u_enc), Wool::Library::HElib);

    cout << md << endl;
    //cout << ms << endl;

    cout << G::result(bind(hd_batched, v_enc, u_enc), Wool::Library::Plaintext) << endl;
    //cout << G::result(bind(hd_batched, v_enc, u_enc), Wool::Library::HElib) << endl;
    return 0;
}