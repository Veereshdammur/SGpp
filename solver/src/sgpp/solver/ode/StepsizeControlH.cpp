// Copyright (C) 2008-today The SG++ project
// This file is part of the SG++ project. For conditions of distribution and
// use, please see the copyright notice provided with SG++ or at
// sgpp.sparsegrids.org

#include <sgpp/base/grid/common/DirichletUpdateVector.hpp>
#include <sgpp/solver/ode/StepsizeControlH.hpp>
#include <sgpp/base/operation/hash/OperationEval.hpp>
#include <sgpp/base/tools/GridPrinter.hpp>
#include <sgpp/base/exception/solver_exception.hpp>

#include <sgpp/globaldef.hpp>

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>

namespace sgpp {
namespace solver {

StepsizeControlH::StepsizeControlH(std::string odesolver, size_t imax, double timestepSize,
                                   double eps, sgpp::base::ScreenOutput* screen, double gamma)
    : StepsizeControl(imax, timestepSize, eps, 1.0, screen, gamma), _odesolver(odesolver) {
  this->residuum = 0.0;
  this->myEps = eps;
  std::stringstream fnsstream;
  fnsstream << "Time_"
            << "SCH" << this->myEps << ".gnuplot";
  filename = fnsstream.str();
}

StepsizeControlH::~StepsizeControlH() {}

void StepsizeControlH::predictor(SLESolver& LinearSystemSolver,
                                 sgpp::solver::OperationParabolicPDESolverSystem& System,
                                 double tmp_timestepsize, sgpp::base::DataVector& dv,
                                 sgpp::base::DataVector& corr, sgpp::base::DataVector* rhs) {
  System.setODESolver(_odesolver);
  System.setTimestepSize(tmp_timestepsize);

  // generate right hand side
  rhs = System.generateRHS();

  // solve the system of the current timesteps
  LinearSystemSolver.solve(System, *System.getGridCoefficientsForCG(), *rhs, true, false, -1.0);

  System.finishTimestep();

  dv.resize(System.getGridCoefficients()->getSize());
  System.getGridCoefficientsForSC(dv);

  System.abortTimestep();
}

void StepsizeControlH::corrector(SLESolver& LinearSystemSolver,
                                 sgpp::solver::OperationParabolicPDESolverSystem& System,
                                 double tmp_timestepsize, sgpp::base::DataVector& dv,
                                 sgpp::base::DataVector* rhs) {
  System.setODESolver(_odesolver);
  System.setTimestepSize(tmp_timestepsize / 2.0);

  // generate right hand side
  rhs = System.generateRHS();

  // solve the system of the current timesteps
  LinearSystemSolver.solve(System, *System.getGridCoefficientsForCG(), *rhs, true, false, -1.0);
  System.finishTimestep();

  rhs = System.generateRHS();

  LinearSystemSolver.solve(System, *System.getGridCoefficientsForCG(), *rhs, true, false, -1.0);
  System.finishTimestep();

  dv.resize(System.getGridCoefficients()->getSize());
  System.getGridCoefficientsForSC(dv);
}

double StepsizeControlH::nextTimestep(double tmp_timestepsize, double tmp_timestepsize_old,
                                       double norm, double epsilon) {
  double deltaY = 3.0 * norm / 4.0;

  return tmp_timestepsize * pow(epsilon / deltaY, 1.0 / 3.0);
}
}  // namespace solver
}  // namespace sgpp
