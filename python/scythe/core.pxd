# -*- coding: utf-8 -*-
# distutils: language=c++

from libcpp cimport bool
from libcpp.limits cimport numeric_limits

cimport numpy as cnp

ctypedef double data_t

cdef extern from "../../src/misc/sets.hpp" namespace "scythe":
    ctypedef unsigned int uint
    ctypedef double       data_t
    ctypedef float        fast_data_t
    ctypedef double       target_t
    ctypedef float        proba_t
    ctypedef int          label_t

    struct Dataset:
        void* data
        size_t n_rows
        size_t n_cols

    struct Labels:
        target_t* data
        size_t n_rows

cdef extern from "../../src/tree/cart.hpp" namespace "scythe":
    struct TreeConfig:
        int    task
        bool   is_incremental
        double min_threshold
        size_t max_height
        size_t max_n_features
        size_t n_classes
        size_t max_nodes
        int    partitioning
        data_t nan_value
        bool   is_complete_random

cdef extern from "../../src/forest/forest.hpp" namespace "scythe":
    struct ForestConfig:
        int       type                 
        int       task                 
        size_t    n_classes            
        int       score_metric         
        size_t    n_iter               
        size_t    max_n_trees          
        size_t    max_n_nodes          
        size_t    max_n_features       
        float     learning_rate        
        size_t    n_leaves             
        size_t    n_jobs               
        size_t    n_samples_per_leaf   
        int       regularization       
        size_t    bag_size             
        size_t    early_stopping_round 
        int       boosting_method      
        int       max_depth            
        float     l1_lambda            
        float     l2_lambda            
        float     seed                 
        int       verbose              
        data_t    nan_value            
        double    min_threshold        

cdef extern from "../../src/scythe.cpp":
    void* fit_classification_tree(Dataset*, Labels*, TreeConfig*)
    void* fit_regression_tree(Dataset*, Labels*, TreeConfig*)
    float* tree_classify(Dataset*, void*, TreeConfig*)
    data_t* tree_predict(Dataset*, void*, TreeConfig*)
    void* fit_classification_forest(Dataset*, Labels*, ForestConfig*)
    void api_test(Dataset*)


cdef Dataset to_dataset(object X)
cdef Labels to_labels(object y)
cdef cnp.ndarray ptr_to_cls_predictions(float* predictions, size_t n_rows, size_t n_classes)
cdef cnp.ndarray ptr_to_reg_predictions(data_t* predictions, size_t n_rows)
