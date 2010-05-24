/*****************************************************************************/
/* This file is part of sgpp, a program package making use of spatially      */
/* adaptive sparse grids to solve numerical problems                         */
/*                                                                           */
/* Copyright (C) 2008 Jörg Blank (blankj@in.tum.de)                          */
/* Copyright (C) 2008-2010 Dirk Pflueger (pflueged@in.tum.de)                */
/* Copyright (C) 2009-2010 Alexander Heinecke (Alexander.Heinecke@mytum.de)  */
/*                                                                           */
/* sgpp is free software; you can redistribute it and/or modify              */
/* it under the terms of the GNU Lesser General Public License as published  */
/* by the Free Software Foundation; either version 3 of the License, or      */
/* (at your option) any later version.                                       */
/*                                                                           */
/* sgpp is distributed in the hope that it will be useful,                   */
/* but WITHOUT ANY WARRANTY; without even the implied warranty of            */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             */
/* GNU Lesser General Public License for more details.                       */
/*                                                                           */
/* You should have received a copy of the GNU Lesser General Public License  */
/* along with sgpp; if not, write to the Free Software                       */
/* Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA */
/* or see <http://www.gnu.org/licenses/>.                                    */
/*****************************************************************************/

#ifndef SGPP_HPP_
#define SGPP_HPP_

/**
 *  When using CrNic start with this
 *  number of ImEul steps
 *
 *  @todo (heinecke) should be removed when having config-file for BlackScholes Solver
 */
#define CRNIC_START_IMEUL 3

#include "grid/GridStorage.hpp"

#include "application/common/ScreenOutput.hpp"

#include "algorithm/datadriven/AlgorithmDGEMV.hpp"
#include "algorithm/common/GetAffectedBasisFunctions.hpp"

#include "algorithm/datadriven/test_dataset.hpp"
#include "algorithm/datadriven/DMSystemMatrix.hpp"

#include "algorithm/pde/BlackScholesODESolverSystem.hpp"
#include "algorithm/pde/HeatEquationODESolverSystem.hpp"

#include "application/pde/BlackScholesSolver.hpp"
#include "application/pde/HeatEquationSolver.hpp"

#include "basis/basis.hpp"

// @todo (heinecke) check if this can be removed
#include "basis/linear/noboundary/operation/pde/OperationLaplaceLinear.hpp"
#include "basis/linear/boundary/operation/pde/OperationLaplaceLinearBoundary.hpp"
#include "basis/modlinear/operation/pde/OperationLaplaceModLinear.hpp"

#include "data/DataVector.hpp"

#include "grid/Grid.hpp"
#include "grid/common/BoundingBox.hpp"
#include "grid/common/DirichletUpdateVector.hpp"
#include "grid/generation/RefinementFunctor.hpp"
#include "grid/generation/StandardGridGenerator.hpp"
#include "grid/generation/BoundaryGridGenerator.hpp"
#include "grid/generation/TrapezoidBoundaryGridGenerator.hpp"
#include "grid/generation/GridGenerator.hpp"
#include "grid/generation/hashmap/HashGenerator.hpp"
#include "grid/generation/hashmap/HashRefinement.hpp"
#include "grid/generation/hashmap/HashRefinementBoundaries.hpp"
#include "grid/generation/SurplusRefinementFunctor.hpp"

#include "solver/sle/ConjugateGradients.hpp"
#include "solver/sle/BiCGStab.hpp"
#include "solver/ode/Euler.hpp"
#include "solver/ode/CrankNicolson.hpp"

#include "tools/finance/IOToolBonnSG.hpp"
#include "tools/common/GridPrinter.hpp"
#include "tools/common/SGppStopwatch.hpp"

namespace sg
{

typedef linearboundaryBase<unsigned int, unsigned int> SLinearBoundaryBase;
typedef linear_base<unsigned int, unsigned int> SLinearBase;
typedef modified_linear_base<unsigned int, unsigned int> SModLinearBase;
typedef poly_base<unsigned int, unsigned int> SPolyBase;
typedef modified_poly_base<unsigned int, unsigned int> SModPolyBase;
typedef modified_wavelet_base<unsigned int, unsigned int> SModWaveletBase;
typedef modified_bspline_base<unsigned int, unsigned int> SModBsplineBase;

typedef AlgorithmDGEMV<SLinearBase> SGridOperationB;
typedef AlgorithmDGEMV<SLinearBoundaryBase> SGridBoundaryOperationB;
typedef AlgorithmDGEMV<SModLinearBase> SGridModOperationB;

}

#endif /*SGPP_HPP_*/
