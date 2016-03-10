/* Copyright (C) 2008-today The SG++ project
 * This file is part of the SG++ project. For conditions of distribution and
 * use, please see the copyright notice provided with SG++ or at
 * sgpp.sparsegrids.org
 *
 * FunctionWrapper.cpp
 *
 *  Created on: Feb 8, 2016
 *      Author: perun
 */

#include <sgpp/datadriven/datamining/dataSource/FunctionWrapper.hpp>

#include <sgpp/globaldef.hpp>

namespace sgpp {
namespace datadriven {

FunctionWrapper::FunctionWrapper(datadriven::DataMiningConfigJsonParser& config)
    : SampleProvider(config) {}

FunctionWrapper::~FunctionWrapper() {
  // TODO(lettrich): Auto-generated destructor stub
}

} /* namespace datadriven */
} /* namespace sgpp */
