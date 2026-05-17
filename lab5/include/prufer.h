#ifndef PRUFER_H
#define PRUFER_H

#include <vector>

bool encode_prufer_with_weights(
    int** adj,
    int** weight,
    int N,
    std::vector<int>& code_nodes,
    std::vector<int>& code_weights,
    int& last_u,
    int& last_v
);

int** decode_prufer_with_weights(
    const std::vector<int>& code_nodes,
    const std::vector<int>& code_weights,
    int N,
    int**& decoded_weights
);

#endif