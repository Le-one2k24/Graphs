#ifndef PRUFER_H
#define PRUFER_H

#include <vector>
#include <utility>

bool encode_prufer_with_weights(
    int** adj,
    int** weight,
    int N,
    std::vector<int>& code_nodes,
    std::vector<int>& code_weights,
    std::pair<std::pair<int,int>,int>& last_edge
);

int** decode_prufer_with_weights(
    const std::vector<int>& code_nodes,
    const std::vector<int>& code_weights,
    const std::pair<std::pair<int,int>,int>& last_edge,
    int N,
    int**& decoded_weights
);

#endif
