#include "node.h"
#include <tuple>

RuleMatrixNode::RuleMatrixNode(const std::string &st_str, const double &cost):identifier(st_str),cost(cost){}

bool RuleMatrixNode::operator==(const RuleMatrixNode &tr) const {
    return (identifier == tr.identifier);
}

bool RuleMatrixNode::operator>=(const RuleMatrixNode& tr) const {
    return (std::tie(cost,identifier) >= std::tie(tr.cost,identifier));
}

bool RuleMatrixNode::operator<=(const RuleMatrixNode& tr) const {
    return (std::tie(cost,identifier) <= std::tie(tr.cost,identifier));
}

bool RuleMatrixNode::operator>(const RuleMatrixNode& tr) const {
    return (std::tie(cost,identifier) > std::tie(tr.cost,identifier));
}

// This is changed due to priority_queue
bool RuleMatrixNode::operator<(const RuleMatrixNode& tr) const {
    return (std::tie(cost,identifier) >= std::tie(tr.cost,tr.identifier));
}




