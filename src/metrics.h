/**
    metrics.h
    Purpose: Mesure Behaviour of Turing Machines.
    @author Jorge Miguel Ferreira da Silva
    @version 0.1
*/

#pragma once
#include <vector>
#include "turingMachine.h"

struct Metrics{
  unsigned int k;
  unsigned int amplitude;
  double selfCompression;
  double normalizedCompression;
  void print();
  
};

Metrics averageMetrics(std::vector<Metrics> metricsVec);
Metrics get_min_nc(std::vector<Metrics> metricsVec);
Metrics get_max_nc(std::vector<Metrics> metricsVec);
Metrics set_default();

