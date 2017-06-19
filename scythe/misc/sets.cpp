/**
    sets.cpp
    Datasets' structures
    
    @author Antoine Passemiers
    @version 1.0 09/06/2017
*/

#include "sets.hpp"


DirectDataset::DirectDataset(Dataset dataset) :
    data(dataset.data), n_rows(dataset.n_rows), n_cols(dataset.n_cols) {}

DirectDataset::DirectDataset(data_t* data, size_t n_instances, size_t n_features) :
    data(data), n_rows(n_instances), n_cols(n_features) {}

DirectDataset::DirectDataset(const DirectDataset& other) :
    data(other.data), n_rows(other.n_rows), n_cols(other.n_cols) {}

DirectDataset& DirectDataset::operator=(const DirectDataset& other) {
    this->data = other.data;
    this->n_rows = other.n_rows;
    this->n_cols = other.n_cols;
    return *this;
}

data_t DirectDataset::operator()(size_t i, size_t j) {
    return this->data[i * this->n_cols + j];
}

DirectTargets::DirectTargets(target_t* data, size_t n_instances) :
    data(data), n_rows(n_instances) {}

DirectTargets::DirectTargets(const DirectTargets& other) :
    data(other.data), n_rows(other.n_rows) {}

DirectTargets& DirectTargets::operator=(const DirectTargets& other) {
    this->data = other.data;
    this->n_rows = other.n_rows;
    return *this;
}

data_t DirectTargets::operator[](const size_t i) {
    return this->data[i];
}