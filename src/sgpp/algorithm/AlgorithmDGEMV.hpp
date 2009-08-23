/*****************************************************************************/
/* This file is part of sgpp, a program package making use of spatially      */
/* adaptive sparse grids to solve numerical problems                         */
/*                                                                           */
/* Copyright (C) 2008-2009 Dirk Pflueger (pflueged@in.tum.de)                */
/* Copyright (C) 2008 Jörg Blank (blankj@in.tum.de)                          */
/* Copyright (C) 2009 Alexander Heinecke (Alexander.Heinecke@mytum.de)       */
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

#ifndef ALGORTIHMDGEMV_HPP
#define ALGORTIHMDGEMV_HPP

#include "grid/GridStorage.hpp"
#include "data/DataVector.hpp"

#include "algorithm/GetAffectedBasisFunctions.hpp"

#include <vector>
#include <utility>
#include <iostream>

#ifdef USEOMP
#include <omp.h>
#endif

namespace sg {

/**
 * Basic multiplaction with B and B^T on grids with no boundaries.
 * If there are @f$N@f$ basis functions @f$\varphi(\vec{x})@f$ and @f$m@f$ data points, then B is a (Nxm) matrix, with
 * @f[ (B)_{i,j} = \varphi_i(x_j). @f]
 * (The common known name for this operation is the BLAS routine DGEMV.)
 *
 * @todo (blank) check if it is possible to have some functor for the BASIS type
 */
template<class BASIS>
class AlgorithmDGEMV
{
public:

	/**
	 * Performs the DGEMV Operation on the grid
	 *
	 * This operation can be executed in parallel by setting the USEOMP define
	 *
	 * @todo (heinecke, nice) add mathematical description
	 *
	 * @param storage GridStorage object that contains the grid's points information
	 * @param basis a reference to a class that implements a specific basis
	 * @param source the coefficients of the grid points
	 * @param x the d-dimensional vector with data points (row-wise)
	 * @param result the result vector of the matrix vector multiplication
	 */
	void mult(GridStorage* storage, BASIS& basis, DataVector& source, DataVector& x, DataVector& result)
	{
		typedef std::vector<std::pair<size_t, double> > IndexValVector;

		result.setAll(0.0);
#ifdef USEOMP
		#pragma omp parallel shared(result)
		{
			size_t source_size = source.getSize();

			std::vector<double> line;
			IndexValVector vec;

			GetAffectedBasisFunctions<BASIS> ga(storage);

			#pragma omp for schedule(static)
			for(size_t i = 0; i < source_size; i++)
			{
				//DataVector* temp = new DataVector(1);
				//double dbl_temp = 0.0;

				vec.clear();

				x.getLine(i, line);

				ga(basis, line, vec);

				#pragma omp critical
				{
					for(IndexValVector::iterator iter = vec.begin(); iter != vec.end(); iter++)
					{

						result[iter->first] += iter->second * source[i];
					}
				}
			}
		}
#else
		size_t source_size = source.getSize();

		std::vector<double> line;
		IndexValVector vec;

		GetAffectedBasisFunctions<BASIS> ga(storage);

		for(size_t i = 0; i < source_size; i++)
		{
			vec.clear();

			x.getLine(i, line);

			ga(basis, line, vec);

			for(IndexValVector::iterator iter = vec.begin(); iter != vec.end(); iter++)
			{
				result[iter->first] += iter->second * source[i];
			}
		}
#endif /* USEOMP */
	}

	/**
	 * Performs the DGEMV Operation on the grid having a transposed matrix
	 *
	 * This operation can be executed in parallel by setting the USEOMP define
	 *
	 * @todo (heinecke, nice) add mathematical description
	 *
	 * @param storage GridStorage object that contains the grid's points information
	 * @param basis a reference to a class that implements a specific basis
	 * @param source the coefficients of the grid points
	 * @param x the d-dimensional vector with data points (row-wise)
	 * @param result the result vector of the matrix vector multiplication
	 */
	void mult_transpose(GridStorage* storage, BASIS& basis, DataVector& source, DataVector& x, DataVector& result)
	{
		typedef std::vector<std::pair<size_t, double> > IndexValVector;

		result.setAll(0.0);

#ifdef USEOMP
		#pragma omp parallel shared (result)
		{
			size_t result_size = result.getSize();

			std::vector<double> line;
			IndexValVector vec;

			GetAffectedBasisFunctions<BASIS> ga(storage);

			#pragma omp for schedule (static)
			for(size_t i = 0; i < result_size; i++)
			{
				vec.clear();

				x.getLine(i, line);

				ga(basis, line, vec);

				for(IndexValVector::iterator iter = vec.begin(); iter != vec.end(); iter++)
				{
					result[i] += iter->second * source[iter->first];
				}
			}
		}
#else
		size_t result_size = result.getSize();

		std::vector<double> line;
		IndexValVector vec;

		GetAffectedBasisFunctions<BASIS> ga(storage);

		for(size_t i = 0; i < result_size; i++)
		{
			vec.clear();

			x.getLine(i, line);

			ga(basis, line, vec);

			for(IndexValVector::iterator iter = vec.begin(); iter != vec.end(); iter++)
			{
				result[i] += iter->second * source[iter->first];
			}
		}
#endif /* USEOMP */
	}
};

}

#endif /* ALGORTIHMDGEMV_HPP */
