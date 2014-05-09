#ifndef SGPP_OPT_OPTIMIZER_OPTIMIZER_HPP
#define SGPP_OPT_OPTIMIZER_OPTIMIZER_HPP

#include "opt/function/Objective.hpp"

#include <vector>
#include <cstddef>

#include <iostream>

namespace sg
{
namespace opt
{
namespace optimizer
{

class Optimizer
{
public:
    static const size_t DEFAULT_MAX_IT_COUNT = 200;
    
    Optimizer(function::Objective &f) :
            Optimizer(f, DEFAULT_MAX_IT_COUNT) {}
    Optimizer(function::Objective &f, size_t max_it_count) :
            f(f), N(max_it_count), x0(std::vector<double>(f.getDimension(), 0.5)) {}
    virtual ~Optimizer() {}
    
    virtual void optimize(std::vector<double> &xopt) = 0;
    
    function::Objective &getObjectiveFunction() const { return f; }
    
    size_t getMaxItCount() const { return N; }
    void setMaxItCount(size_t max_it_count) { N = max_it_count; }
    
    const std::vector<double> &getStartingPoint() const { return x0; }
    void setStartingPoint(const std::vector<double> &x0) { this->x0 = x0; }
    
protected:
    function::Objective &f;
    size_t N;
    std::vector<double> x0;
};

}
}
}

#endif
