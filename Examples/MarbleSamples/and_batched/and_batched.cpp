#include <vector>
#include <functional>
#include "G.hpp"

using namespace std;
using namespace Granite;


void and_batched(G v, G u) {
    G ands = (v * u);
    ands.fold(G::sum);
    output(ands);
}

int main() {
    vector<int> v = {0, 1, 1, 0, 0/*...*/};
    vector<int> u = {1, 0, 1, 0, 1/*...*/};

    G v_enc = batchEncrypt(v);
    G u_enc = batchEncrypt(u);

    // Simulates the execution and
    // reports e.g. multiplicative depth
    int md = G::analyse(bind(and_batched, v_enc, u_enc));

    // Benchmarks the application,
    // using the most appropriate settings
    double ms = G::evaluate(bind(and_batched, v_enc, u_enc), Wool::Library::SEALBFV);

    cout << md << endl;
    cout << ms << endl;

    cout << G::result(bind(and_batched, v_enc, u_enc), Wool::Library::Plaintext) << endl;
    cout << G::result(bind(and_batched, v_enc, u_enc), Wool::Library::SEALBFV) << endl;
    return 0;
}