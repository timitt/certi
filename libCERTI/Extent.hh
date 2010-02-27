// ----------------------------------------------------------------------------
// CERTI - HLA RunTime Infrastructure
// Copyright (C) 2003  ONERA
//
// This file is part of CERTI-libCERTI
//
// CERTI-libCERTI is free software ; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License
// as published by the Free Software Foundation ; either version 2 of
// the License, or (at your option) any later version.
//
// CERTI-libCERTI is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY ; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this program ; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
// USA
//
// $Id: Extent.hh,v 3.11 2010/02/27 16:53:35 erk Exp $
// ----------------------------------------------------------------------------

#ifndef CERTI_EXTENT_HH
#define CERTI_EXTENT_HH

#include "certi.hh"
#include "Exception.hh"

#include <vector>
#include <utility>
#include <map>

namespace certi {

typedef std::pair<ULong, ULong> Range ;
typedef std::vector<Range> RangeSet ;

/** 
 * An extent is a subspace in a routing space. It is made of ranges in
 * each dimension of the routing space. Routing regions are described
 * using a set of extents.
 * @sa RoutingSpace, Dimension, RegionImp
 */
class CERTI_EXPORT Extent {
public:

	Extent() {ranges.empty();};
	/**
	 * Extent constructor
	 * @param n Number of dimensions in the routing space
	 */
    Extent(size_t n);

    /** Get range lower bound */
    ULong getRangeLowerBound(DimensionHandle) const
        throw (ArrayIndexOutOfBounds);

    /** Get range upper bound */
    ULong getRangeUpperBound(DimensionHandle) const
        throw (ArrayIndexOutOfBounds);

    /** Set range upper bound */
    void setRangeUpperBound(DimensionHandle, ULong)
        throw (ArrayIndexOutOfBounds);

    /** Set range lower bound */
    void setRangeLowerBound(DimensionHandle, ULong)
        throw (ArrayIndexOutOfBounds);

    /** Check whether both extents overlap */
    bool overlaps(const Extent &) const ;

    /** Get the number of ranges in this Extent. */
    size_t size() const ;
    
private:
     RangeSet ranges ;
};

} // namespace certi

#endif // CERTI_EXTENT_HH

// $Id: Extent.hh,v 3.11 2010/02/27 16:53:35 erk Exp $
