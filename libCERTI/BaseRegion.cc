// ----------------------------------------------------------------------------
// CERTI - HLA RunTime Infrastructure
// Copyright (C) 2002-2005  ONERA
//
// This file is part of CERTI-libRTI
//
// CERTI-libRTI is free software ; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License
// as published by the Free Software Foundation ; either version 2 of
// the License, or (at your option) any later version.
//
// CERTI-libRTI is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY ; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this program ; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
// USA
//
// $Id: BaseRegion.cc,v 3.0 2005/03/25 16:55:25 breholee Exp $
// ----------------------------------------------------------------------------

#include <config.h>
#include "BaseRegion.hh"

using std::vector ;

namespace certi {

// ----------------------------------------------------------------------------
BaseRegion::BaseRegion(RegionHandle h)
    : Handled<RegionHandle>(h)
{
}

// ----------------------------------------------------------------------------
BaseRegion::BaseRegion(RegionHandle h, const vector<Extent> &ext)
    : Handled<RegionHandle>(h), extents(ext)
{
}

// ----------------------------------------------------------------------------
BaseRegion::~BaseRegion()
{
}

// ----------------------------------------------------------------------------
ULong
BaseRegion::getNumberOfExtents() const
    throw ()
{
    return extents.size();
}

// ----------------------------------------------------------------------------
ULong
BaseRegion::getRangeLowerBound(ExtentIndex index,
			       DimensionHandle dimension) const
    throw (ArrayIndexOutOfBounds)
{
    if (index >= extents.size()) {
        throw ArrayIndexOutOfBounds();
    }
    else {
        return extents[index].getRangeLowerBound(dimension);
    }
}

// ----------------------------------------------------------------------------
ULong
BaseRegion::getRangeUpperBound(ExtentIndex index,
			       DimensionHandle dimension) const
    throw (ArrayIndexOutOfBounds)
{
    if (index >= extents.size()) {
        throw ArrayIndexOutOfBounds();
    }
    else {
        return extents[index].getRangeUpperBound(dimension);
    }
}

// ----------------------------------------------------------------------------
void
BaseRegion::setRangeLowerBound(ExtentIndex index,
			       DimensionHandle dimension,
			       ULong val)
    throw (ArrayIndexOutOfBounds)
{
    if (index >= extents.size()) {
        throw ArrayIndexOutOfBounds();
    }
    else {
        extents[index].setRangeLowerBound(dimension, val);
    }
}

// ----------------------------------------------------------------------------
void
BaseRegion::setRangeUpperBound(ExtentIndex index,
			       DimensionHandle dimension,
			       ULong val)
    throw (ArrayIndexOutOfBounds)
{
    if (index >= extents.size()) {
        throw ArrayIndexOutOfBounds();
    }
    else {
        extents[index].setRangeUpperBound(dimension, val);
    }
}

// ----------------------------------------------------------------------------
/** Get the region's extents
    \return The extents, as a vector
 */
const vector<Extent> &
BaseRegion::getExtents() const
{
    return extents ;
}

// ----------------------------------------------------------------------------
void
BaseRegion::setExtents(const vector<Extent> &e)
{
    extents = e ;
}

// ----------------------------------------------------------------------------
// Replace extents
void
BaseRegion::replaceExtents(const vector<Extent> &e)
    throw (InvalidExtents)
{
    if (e.size() != extents.size())
	throw InvalidExtents();

    extents = e ;
}

// ----------------------------------------------------------------------------
/** Check if regions overlap
 */
bool
BaseRegion::overlaps(const BaseRegion &region) const
{
    if (getSpaceHandle() != region.getSpaceHandle()) {
	return false ;
    }

    for (unsigned int i = 0 ; i < getNumberOfExtents() ; ++i) {
	for (unsigned int j = 0 ; j < region.getNumberOfExtents(); ++j) {
	    if (extents[i].overlaps(region.extents[j]))
		return true ;
	}
    }
    return false ;
}

} // namespace certi

// $Id: BaseRegion.cc,v 3.0 2005/03/25 16:55:25 breholee Exp $