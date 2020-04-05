#include <vector>
#include <functional>
#include "G.hpp"

using namespace std;
using namespace Granite;


void and_enc(vector<G> v, vector<G> u) {
    vector<G> ands = v * u;
    G result = fold(ands, G::sum);
    output(result);
}

int main() {
    vector<int> v = {0, 1, 1, 0, 0, 1, 1, 0, 1, 1/*...*/};
    vector<int> u = {1, 0, 1, 0, 1, 0, 1, 0, 1, 1/*...*/};

    vector<G> v_enc = encrypt(v);
    vector<G> u_enc = encrypt(u);

    // Simulates the execution and
    // reports e.g. multiplicative depth
    int md = G::analyse(bind(and_enc, v_enc, u_enc));

    // Benchmarks the application,
    // using the most appropriate settings
    double ms = G::evaluate(bind(and_enc, v_enc, u_enc), Wool::Library::SEALBFV);

    cout << md << endl;
    cout << ms << endl;

    cout << G::result(bind(and_enc, v_enc, u_enc), Wool::Library::Plaintext) << endl;
    cout << G::result(bind(and_enc, v_enc, u_enc), Wool::Library::SEALBFV) << endl;
    cout << G::result(bind(and_enc, v_enc, u_enc), Wool::Library::TFHEInteger) << endl;
    return 0;
}