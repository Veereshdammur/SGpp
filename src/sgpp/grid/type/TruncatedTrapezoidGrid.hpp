/*
 * TruncatedTrapezoidGrid.hpp
 *
 *  Created on: Aug 4, 2010
 *      Author: aliz
 */

#ifndef TRUNCATEDTRAPEZOIDGRID_HPP_
#define TRUNCATEDTRAPEZOIDGRID_HPP_
#include "grid/Grid.hpp"

#include <iostream>

namespace sg
{
namespace base
{

/**
 * grid with linear base functions with boundaries, pentagon cut
 * Generalization of the LinearBoundary and LinearTrapezoidBoundary Grids
 * The sparse grid does contain all fullgrids with |l|<a given level, and l_min>l_user
 * For l_user=0 we obtain the LinearBoundaryGrid and for l_user=1 we obtain the linear trapezoid boundary grid
 */
class TruncatedTrapezoidGrid : public Grid
{
protected:
	TruncatedTrapezoidGrid(std::istream& istr);

public:
	/**
	 * Constructor Linear Trapezoid Boundary Grid
	 *
	 * @param dim the dimension of the grid
	 */
	TruncatedTrapezoidGrid(size_t dim);

	/**
	 * Constructor Linear Trapezoid Boundary Grid
	 *
	 * @param BB the BoundingBox of the grid
	 */
	TruncatedTrapezoidGrid(BoundingBox& BB);

	/**
	 * Destructor
	 */
	virtual ~TruncatedTrapezoidGrid();

	virtual const char* getType();

	virtual GridGenerator* createGridGenerator();

	static Grid* unserialize(std::istream& istr);
};

}
}

#endif /* TRUNCATEDTRAPEZOIDGRID_HPP_ */
