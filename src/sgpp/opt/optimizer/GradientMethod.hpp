#ifndef SGPP_OPT_OPTIMIZER_GRADIENTMETHOD_HPP
#define SGPP_OPT_OPTIMIZER_GRADIENTMETHOD_HPP

#include "opt/optimizer/Optimizer.hpp"
#include "opt/function/ObjectiveGradient.hpp"

namespace sg
{
namespace opt
{
namespace optimizer
{

class GradientMethod : public Optimizer
{
public:
    static const size_t DEFAULT_MAX_IT_COUNT = 2000;
    static const double DEFAULT_BETA;
    static const double DEFAULT_GAMMA;
    static const double DEFAULT_TOLERANCE;
    
    GradientMethod(function::Objective &f,
                   function::ObjectiveGradient &f_gradient);
    
    GradientMethod(function::Objective &f,
                   function::ObjectiveGradient &f_gradient,
                   size_t max_it_count, double beta, double gamma, double tolerance);
    
    void optimize(std::vector<double> &xopt);
    
    function::ObjectiveGradient &getObjectiveGradient() const;
    
    double getBeta() const;
    void setBeta(double beta);
    
    double getGamma() const;
    void setGamma(double gamma);
    
    double getTolerance() const;
    void setTolerance(double tolerance);
    
protected:
    function::ObjectiveGradient &f_gradient;
    double beta;
    double gamma;
    double tol;
};

}
}
}

#endif
