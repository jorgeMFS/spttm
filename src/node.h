/**
    node.h
    Purpose: Node of Rule Matrix.

    @author Jorge Miguel Ferreira da Silva
    @version 0.1
*/

#include <string>

struct RuleMatrixNode{
    /* data */
    std::string identifier;
    double cost;
    double short_cost;

    RuleMatrixNode(const std::string &st_str, const double &cost);
    RuleMatrixNode(const std::string &st_str, const double &cost, const double &short_cost);
    RuleMatrixNode(const std::string &st_str, const std::pair<double, double> &costs);
    bool operator==(const RuleMatrixNode& rmn) const;
    bool operator>=(const RuleMatrixNode& rmn) const;
    bool operator<=(const RuleMatrixNode& tr) const;
    bool operator>(const RuleMatrixNode& tr) const;
    bool operator<(const RuleMatrixNode& tr) const;
};
