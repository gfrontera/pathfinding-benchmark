#ifndef UES_PF_VG2D_UTIL_H
#define UES_PF_VG2D_UTIL_H

#include <unordered_map>

#include "definitions.h"

namespace ues
{
namespace pf
{
namespace vg2d
{

/** Auxiliary type for some functions. */
typedef std::unordered_map< ues::geom::point<2>, ues::pf::vg2d::point_index > point_map;

/** Auxiliary function that adds a point to a vector if it is not in there. It uses an auxiliary
 * data structure to determine in constant time whether a point is already in the vector. */
ues::pf::vg2d::point_index add_if_not_present ( ues::pf::vg2d::point_vector & points,
                                                point_map & added_points,
                                                ues::geom::point<2> input_point ) noexcept;

}
}
}

#endif // UES_PF_VG2D_UTIL_H
