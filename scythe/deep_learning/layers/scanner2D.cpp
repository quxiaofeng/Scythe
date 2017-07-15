/**
    scanner2D.cpp
    Multi-grained scanning

    @author Antoine Passemiers
    @version 1.0 10/06/2017
*/

#include "scanner2D.hpp"

namespace scythe {

Parameters parameters;

ScannedDataset2D::ScannedDataset2D(
    data_t* data, size_t N, size_t M, size_t P, size_t kc, size_t kr, int dtype) : 
    N(N),                // Number of instances
    M(M),                // Instance height
    P(P),                // Instance width
    kc(kc),              // Kernel width
    kr(kr),              // Kernel height
    sc(P - kc + 1),      // Number of kernel positions per column
    sr(M - kr + 1),      // Number of kernel positions per row
    Nprime(N * sr * sc), // Number of instances after scanning
    Mprime(kc * kr),     // Number of features after scanning
    data(data),          // Pointer to the raw data
    dtype(dtype) {       // Raw data type
    if (parameters.print_layer_info) {
        #pragma omp critical(scanned_dataset_2d_display_info)
        {
            std::cout << "\tKernel width  : " << kc << std::endl;
            std::cout << "\tKernel height : " << kr << std::endl;
            std::cout << "\tN prime       : " << Nprime << std::endl;
            std::cout << "\tM prime       : " << Mprime << std::endl;
        }
    }
}

data_t ScannedDataset2D::operator()(size_t i, size_t j) {
    size_t n = i / (sr * sc);
    size_t m = (i % sc) + (j % kc);
    size_t p = ((i % (sr * sc)) / sr) + (j / kr);
    return data[(M * P) * n + (P * m) + p];
}

void ScannedDataset2D::_iterator_begin(const size_t j) {
    _it_x = P * (j % kc) + (j / kr);
    _it_i = 0;
    _it_q = 0;
}

void ScannedDataset2D::_iterator_inc() {
    _it_i++;
    if (_it_i == sc) {
        ++_it_q;
        _it_i = 0;
        if (_it_q == sr) {
            _it_q = 0;
            _it_x += (M * P);
        }
    }
}

data_t ScannedDataset2D::_iterator_deref() {
    return data[_it_x + _it_i * P + _it_q / sr];
}

ScannedTargets2D::ScannedTargets2D(target_t* data, size_t n_instances, size_t sc, size_t sr) :
    VirtualTargets::VirtualTargets(), data(data), n_rows(n_instances * sc * sr), s(sc * sr) {}

target_t ScannedTargets2D::operator[](const size_t i) {
    return data[i / s];
}

MultiGrainedScanner2D::MultiGrainedScanner2D(
    LayerConfig lconfig, size_t kc, size_t kr) : Layer(lconfig), kc(kc), kr(kr) {}

vdataset_p MultiGrainedScanner2D::virtualize(MDDataset dataset) {
    assert(dataset.n_dims == 3);
    assert(dataset.dims[0] > 0);
    assert(dataset.dims[1] > 0);
    assert(dataset.dims[2] > 0);
    Layer::vdataset = std::shared_ptr<ScannedDataset2D>(
        new ScannedDataset2D(
            dataset.data,
            dataset.dims[0],
            dataset.dims[1],
            dataset.dims[2],
            this->kc,
            this->kr,
            dataset.dtype));
    return Layer::vdataset;
}

vtargets_p MultiGrainedScanner2D::virtualizeTargets(Labels* targets) {
    ScannedDataset2D* vdataset = dynamic_cast<ScannedDataset2D*>((this->vdataset).get());
    size_t sc = vdataset->getSc();
    size_t sr = vdataset->getSr();
    size_t n_rows = vdataset->getNumInstances();
    assert(sc > 0);
    assert(sr > 0);
    return std::shared_ptr<ScannedTargets2D>(
        new ScannedTargets2D(targets->data, n_rows, sc, sr));
}

size_t MultiGrainedScanner2D::getRequiredMemorySize() {
    size_t memory_size = vdataset->getNumInstances();
    assert(memory_size > 0);
    if (lconfig.fconfig.task == CLASSIFICATION_TASK) {
        memory_size *= lconfig.fconfig.n_classes;
    }
    return memory_size * lconfig.n_forests;
}

size_t MultiGrainedScanner2D::getNumVirtualFeatures() {
    ScannedDataset2D* sdataset = dynamic_cast<ScannedDataset2D*>(vdataset.get());
    size_t n_vfeatures = sdataset->getSc() * sdataset->getSr();
    assert(n_vfeatures > 0);
    if (lconfig.fconfig.task == CLASSIFICATION_TASK) {
        n_vfeatures *= lconfig.fconfig.n_classes;
    }
    return n_vfeatures * lconfig.n_forests;
}

}