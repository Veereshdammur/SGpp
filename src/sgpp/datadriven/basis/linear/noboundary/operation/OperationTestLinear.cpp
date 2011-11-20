/******************************************************************************
* Copyright (C) 2009 Technische Universitaet Muenchen                         *
* This file is part of the SG++ project. For conditions of distribution and   *
* use, please see the copyright notice at http://www5.in.tum.de/SGpp          *
******************************************************************************/
// @author Alexander Heinecke (Alexander.Heinecke@mytum.de), Dirk Pflueger (pflueged@in.tum.de)


#include "base/basis/linear/noboundary/LinearBasis.hpp"
#include "datadriven/algorithm/test_dataset.hpp"

#include "datadriven/basis/linear/noboundary/operation/OperationTestLinear.hpp"


namespace sg
{
namespace datadriven
{

  double OperationTestLinear::test(base::DataVector& alpha, base::DataMatrix& data, base::DataVector& classes)
  {
    base::LinearBasis<unsigned int, unsigned int> base;
    return test_dataset(this->storage, base, alpha, data, classes);
  }

  double OperationTestLinear::testMSE(base::DataVector& alpha, base::DataMatrix& data, base::DataVector& refValues)
  {
    base::LinearBasis<unsigned int, unsigned int> base;
    return test_dataset_mse(this->storage, base, alpha, data, refValues);
  }

  double OperationTestLinear::testWithCharacteristicNumber(base::DataVector& alpha, base::DataMatrix& data, base::DataVector& classes, base::DataVector& charaNumbers, base::DataMatrix& ROC_curve)
  {
    base::LinearBasis<unsigned int, unsigned int> base;
    return test_datasetWithCharacteristicNumber(this->storage, base, alpha, data, classes, charaNumbers, ROC_curve);
  }

}
}

