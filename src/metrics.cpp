/**
    metrics.cpp
    Purpose: Mesure Behaviour of Turing Machines.

    @author Jorge Miguel Ferreira da Silva
    @version 0.1
*/

#include <iomanip>
#include <cassert>

#include "metrics.h"




void Metrics::print(){
    std::cout << "amplitude = " << this->amplitude << std::endl;
    std::cout << "k = " << this->k << std::endl;
    std::cout << "normalized Compression = " << this->normalizedCompression << std::endl;
    std::cout << "Self Compression = " << this->selfCompression << std::endl;
}

Metrics set_default(){
    Metrics averageOfMetrics;
    averageOfMetrics.amplitude=100;
    averageOfMetrics.k=2;
    averageOfMetrics.normalizedCompression=0.5;
    averageOfMetrics.selfCompression=154;
}

Metrics averageMetrics(std::vector<Metrics> metricsVec){
    Metrics averageOfMetrics;
    for(auto&M:metricsVec){
        averageOfMetrics.amplitude+=M.amplitude;
        averageOfMetrics.k+=M.k;
        averageOfMetrics.normalizedCompression+=M.normalizedCompression;
        averageOfMetrics.selfCompression+=M.selfCompression;
    }
    auto const count = static_cast<float>(metricsVec.size());
    averageOfMetrics.amplitude/=count;
    averageOfMetrics.k/=count;
    averageOfMetrics.normalizedCompression/=count;
    averageOfMetrics.selfCompression/=count;
    return averageOfMetrics;
}

Metrics get_min_nc(std::vector<Metrics> metricsVec){
    auto nc=10.0;
    auto index = 0;
    for(auto i=0u; i<metricsVec.size();++i){
        auto M = metricsVec[i];
        if(M.normalizedCompression<nc){
            nc=M.normalizedCompression;
            index=i;
        }
    }
    return metricsVec[index];
}

Metrics get_max_nc(std::vector<Metrics> metricsVec){
    auto nc=10.0;
    auto index = 0;
    for(auto i=0u; i<metricsVec.size();++i){
        auto M = metricsVec[i];
        if(M.normalizedCompression>nc){
            nc=M.normalizedCompression;
            index=i;
        }
    }
    return metricsVec[index];
}


