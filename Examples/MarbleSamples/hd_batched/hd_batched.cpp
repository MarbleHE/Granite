#include <vector>
#include <functional>
#include "M.hpp"

using namespace std;
using namespace Marble;


void hd_batched(M v, M u) {
    M diff = (v != u);
    diff.fold(M::sum);
    output(diff);
}

int main() {
    vector<int> v = {0, 1, 1, 0, 0/*...*/};
    vector<int> u = {1, 0, 1, 0, 1/*...*/};

    M v_enc = batchEncrypt(v);
    M u_enc = batchEncrypt(u);

    // Simulates the execution and
    // reports e.g. multiplicative depth
    int md = M::analyse(bind(hd_batched, v_enc, u_enc));

    // Benchmarks the application,
    // using the most appropriate settings
    double ms = M::evaluate(bind(hd_batched, v_enc, u_enc), Wool::Library::SEALBFV);

    cout << md << endl;
    cout << ms << endl;

    cout << M::result(bind(hd_batched, v_enc, u_enc), Wool::Library::Plaintext) << endl;
    cout << M::result(bind(hd_batched, v_enc, u_enc), Wool::Library::SEALBFV) << endl;
    cout << M::result(bind(hd_batched, v_enc, u_enc), Wool::Library::HElib) << endl;
    return 0;
}