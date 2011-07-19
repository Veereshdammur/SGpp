/******************************************************************************
* Copyright (C) 2009 Technische Universitaet Muenchen                         *
* This file is part of the SG++ project. For conditions of distribution and   *
* use, please see the copyright notice at http://www5.in.tum.de/SGpp          *
******************************************************************************/
// @author Jörg Blank (blankj@in.tum.de), Alexander Heinecke (Alexander.Heinecke@mytum.de)

#include "sgpp.hpp"

#include "basis/basis.hpp"
#include "basis/modpoly/operation/common/OperationHierarchisationModPoly.hpp"
#include "basis/modpoly/algorithm_sweep/HierarchisationModPoly.hpp"
#include "basis/modpoly/algorithm_sweep/DehierarchisationModPoly.hpp"

#include "data/DataVector.hpp"
#include "algorithm/common/sweep.hpp"

namespace sg
{
namespace base
{

void OperationHierarchisationModPoly::doHierarchisation(DataVector& node_values)
{
	HierarchisationModPoly func(this->storage, &this->base);
	sweep<HierarchisationModPoly> s(func, this->storage);

	// Execute hierarchisation in every dimension of the grid
	for (size_t i = 0; i < this->storage->dim(); i++)
	{
		s.sweep1D(node_values, node_values, i);
	}
}

void OperationHierarchisationModPoly::doDehierarchisation(DataVector& alpha)
{
	DehierarchisationModPoly func(this->storage, &this->base);
	sweep<DehierarchisationModPoly> s(func, this->storage);

	// Execute hierarchisation in every dimension of the grid
	for (size_t i = 0; i < this->storage->dim(); i++)
	{
		s.sweep1D(alpha, alpha, i);
	}
}

}
}