// Copyright (C) 2008-today The SG++ project
// This file is part of the SG++ project. For conditions of distribution and
// use, please see the copyright notice provided with SG++ or at
// sgpp.sparsegrids.org

#include <sgpp/globaldef.hpp>
#include <sgpp/optimization/test_problems/unconstrained/Schwefel26.hpp>

#include <cmath>

namespace sgpp {
namespace optimization {
namespace test_problems {

Schwefel26::Schwefel26(size_t d) : UnconstrainedTestProblem(d), f(d) {}

Schwefel26::~Schwefel26() {}

TestScalarFunction& Schwefel26::getObjectiveFunction() { return f; }

double Schwefel26::getOptimalPointUndisplaced(base::DataVector& x) {
  x.resize(d);
  x.setAll(0.920968746359982027311844365);
  return -418.9828872724337 * static_cast<double>(d);
}

Schwefel26Objective::Schwefel26Objective(size_t d) : TestScalarFunction(d) {}

Schwefel26Objective::~Schwefel26Objective() {}

double Schwefel26Objective::evalUndisplaced(const base::DataVector& x) {
  double result = 0.0;

  for (size_t t = 0; t < d; t++) {
    const double xt = 1000.0 * x[t] - 500.0;
    result -= xt * std::sin(std::sqrt(std::abs(xt)));
  }

  return result;
}

void Schwefel26Objective::clone(std::unique_ptr<base::ScalarFunction>& clone) const {
  clone = std::unique_ptr<base::ScalarFunction>(new Schwefel26Objective(*this));
}
}  // namespace test_problems
}  // namespace optimization
}  // namespace sgpp
