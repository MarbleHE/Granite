#include <vector>
#include <functional>
#include "G.hpp"

using namespace std;
using namespace Granite;

void faces(G in, G db, int dim, int n) {
    output(in,"in");
    output(db,"db");
    G diff = in - db;
    output(diff,"diff");
    G sq = diff * diff;
    output(sq,"sq");

    // SIMD-summation over all dimensions of each face
    G res = sq.fold(sum, dim);
    output(res,"res");
    // SIMD-minimum-index
    G index;
    G min = res.fold(min_with_index, index, dim, 1);
    output(min, "min");
    output(index, "index");
}


int main() {
    // Database of faces templates
    vector<int> face1 = {20, 78, 15 /*...*/};
    vector<int> face2 = {67, 53, 49 /*...*/};
    vector<int> face3 = {76, 112, 40 /*...*/};
    vector<vector<int>> db /*= {face1,face2, ...}*/;
    db.push_back(face1); db.push_back(face2); db.push_back(face3);
    int n = db.size();
    int dim = db[0].size();

    // Input face representation
    vector<int> in = {76, 112, 40 /*...*/};

    // Batched Encryption
    vector<int> db_batched, in_batched;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < db[i].size(); ++j) {
            db_batched.push_back(db[i][j]);
            in_batched.push_back(in[j]);
        }
    }

    G db_enc = encrypt(batched, db_batched, /*bitSize=*/8);
    G in_enc = encrypt(batched, in_batched, /*bitSize=*/8);

    G::evaluate(bind(faces, in_enc, db_enc, dim, n));

    return 0;
}