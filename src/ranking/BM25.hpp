#pragma once

#include <cmath>

struct BM25Params {
    double k1 = 1.2;
    double b  = 0.75;
    double delta = 0.0;

    BM25Params(double k = 1.2, double bb = 0.75) : k1(k), b(bb) {}
};

class BM25 {
public:
    BM25(const BM25Params& params = BM25Params())
        : k1(params.k1), b(params.b), delta(params.delta) {}

    double score( size_t freq, size_t docLength, size_t totalDocs, size_t docsWithTerm, double avgDocLength) const {
        if (docsWithTerm == 0) return 0.0;
        double idf = std::log((totalDocs - docsWithTerm + 0.5) / (docsWithTerm + 0.5) + 1.0);
        double tf_part = freq * (k1 + 1.0) /
                         (freq + k1 * (1.0 - b + b * docLength / avgDocLength));
        return idf * tf_part;
    }

private:
    double k1;
    double b;
    double delta;
};