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
    RuleMatrixNode(const std::string &st_str, const double &cost);
    bool operator==(const RuleMatrixNode& rmn) const;
    bool operator>=(const RuleMatrixNode& rmn) const;
    bool operator<=(const RuleMatrixNode& tr) const;
    bool operator>(const RuleMatrixNode& tr) const;
    bool operator<(const RuleMatrixNode& tr) const;
};
