/* Copyright (C) 2008-today The SG++ project
 * This file is part of the SG++ project. For conditions of distribution and
 * use, please see the copyright notice provided with SG++ or at
 * sgpp.sparsegrids.org
 *
 * CombiConfigurator.hpp
 *
 *  Created on: Jan 29, 2019
 *      Author: nico
 */

#include <python3.7/Python.h>
#include <iostream>
#include <vector>

#pragma once
namespace sgpp {
namespace datadriven {

using std::vector;

struct combiConfig {
  double coef;
  vector<size_t> levels;
};

class CombiConfigurator {
 public:
  CombiConfigurator() = default;

  /**
   * This returns the set of level vectors with the corresponding coefficients
   * for the standard combination technique
   * @param vec reference where to store the combiConfig
   * @oaram d dimension of the sparse grid
   * @param l level of the sparse grid
   * @return vector of combiConfig
   */
  const int getStandardCombi(vector<combiConfig> &vec, size_t d, size_t l);
};
} /* namespace datadriven */
} /* namespace sgpp */
