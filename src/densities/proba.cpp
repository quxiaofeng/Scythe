/**
    proba.cpp
    Compute standard probability densities

    @author Antoine Passemiers
    @version 1.0 25/06/2017
*/

#include "proba.hpp"


namespace scythe {

Density* getArbitraryProbaDensities(size_t n_features, size_t n_classes) {
    assert(n_features > 0);
    Density* densities = new Density[n_features];
    densities[0].counters_left = new size_t[n_classes];
    densities[0].counters_right = new size_t[n_classes];
    densities[0].counters_nan = new size_t[n_classes];
    densities[0].values = new data_t[100];
    densities[0].n_values = 100;

    for (unsigned int i = 1; i <= 100; i++) {
        densities[0].values[i - 1] = i / 100.0;
    }

    for (unsigned int f = 1; f < n_features; f++) {
        densities[f] = densities[0];
    }
    return densities;
}

}