/*
 * OCLOperatorFactory.hpp
 *
 *  Created on: Mar 25, 2015
 *      Author: pfandedd
 */

#pragma once

#include <sgpp/base/operation/hash/OperationMultipleEval.hpp>
#include <sgpp/base/exception/factory_exception.hpp>
#include <sgpp/globaldef.hpp>
#include <sgpp/datadriven/operation/hash/simple/DatadrivenOperationCommon.hpp>
#include <sgpp/datadriven/operation/hash/OperationMultipleEvalStreamingBSplineOCL/OperationMultipleEvalStreamingBSplineOCL.hpp>

namespace SGPP {
  namespace datadriven {

    base::OperationMultipleEval* createStreamingBSplineOCLConfigured(base::Grid& grid, base::DataMatrix& dataset,
        SGPP::datadriven::OperationMultipleEvalConfiguration& configuration);

  }
}
