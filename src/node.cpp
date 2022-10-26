#include "node.h"
#include <tuple>

RuleMatrixNode::RuleMatrixNode(const std::string &st_str, const double &cost):identifier(st_str),cost(cost){}

RuleMatrixNode::RuleMatrixNode(const std::string &st_str, const double &cost, const double &short_cost):identifier(st_str),cost(cost),short_cost(short_cost){}

RuleMatrixNode::RuleMatrixNode(const std::string &st_str, const std::pair<double, double> &costs):identifier(st_str), cost(costs.first), short_cost(costs.second){}

RuleMatrixNode::RuleMatrixNode(const std::string &st_str, const double &cost, const double &short_cost, unsigned int current_iterations):identifier(st_str),cost(cost),short_cost(short_cost),current_iterations(current_iterations){}
RuleMatrixNode::RuleMatrixNode(const std::string &st_str, const std::pair<double, double> &costs, unsigned int current_iterations):identifier(st_str), cost(costs.first), short_cost(costs.second),current_iterations(current_iterations){}

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




