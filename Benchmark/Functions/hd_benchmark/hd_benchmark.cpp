#include <vector>
#include <functional>
#include "G.hpp"

using namespace std;
using namespace Granite;


void hd_batched(G v, G u) {
    G diff = (v != u);
    diff.fold(G::sum);
    output(diff);
}

void hd_enc(vector<G> v, vector<G> u) {
    G sum = 0;
    for (int i = 0; i < v.size(); ++i) {
        sum += (v[i] != u[i]);
    }
    output(sum);
}

int main() {
    vector<Library> libraries = {Library::SEALBFV}; //TODO: Add HELib and Palisade once they are working

    int start_size = 0;
    int end_size = 60;
    for (auto l: libraries){
        std::ofstream resfile;

        // Find files on the remote host under path: tmp/tmp.lyA63QWUHt/Benchmark/results
        resfile.open ("../../../../Benchmark/results/granite_hd_" + toString(l) + ".csv");
        resfile << "#n,#batched,#slow\n";

        for (size_t i = start_size; i < end_size ; i++) {
            vector<int> v;
            vector<int> u;
            for (size_t j = 0; j < i; j++){
                v.push_back(1); //maybe add randomness. But shouldn't matter for performance measuring
                u.push_back(1);
            }
            G v_enc_batched = batchEncrypt(v);
            G u_enc_batched = batchEncrypt(u);
            int n = v.size();
            resfile << to_string(n) + ",";
            try {
                double ms_batched = G::evaluate(bind(hd_batched, v_enc_batched, u_enc_batched), l);
                ms_batched = ms_batched / 10000; //ms to seconds
                resfile << to_string(ms_batched) + ",";
            }
            catch (const runtime_error &e){
                // skip here, because no batching support
                resfile << "NaN,";
            }
            vector<G> v_enc = encrypt(v);
            vector<G> u_enc = encrypt(u);

            try {
                double ms_enc = G::evaluate(bind(hd_enc, v_enc, u_enc), l);
                ms_enc = (ms_enc / 10000) / 60; // ms to minutes
                resfile << to_string(ms_enc) + "\n";
            }
            catch (const runtime_error &e){
                // skip here, because no suitable Q was found (multDepth too high for standard parameters)
                resfile << "NaN\n";
            }
        }
        resfile.close();
    }
    return 0;
}