#ifndef UES_PF_VG2D_DEFINITIONS_H
#define UES_PF_VG2D_DEFINITIONS_H

#include <vector>

#include <geom/point.h>

namespace ues
{
namespace pf
{
namespace vg2d
{

typedef std::vector< geom::point<2> > point_vector; /**< Vector containing 2D point objects. */
typedef point_vector::size_type point_index; /**< An index to a point. */
typedef std::vector< point_index > point_index_vector; /**< A vector of indices to points. */

typedef std::pair< point_index, point_index > segment; /**< Segments are pairs of indices to points in a point vector. */
typedef std::vector< segment > segment_vector; /**< Vector containing segments. */
typedef segment_vector::size_type segment_index; /**< Index to a segment */
typedef std::vector< segment_index > segment_index_vector; /**< A vector of indices to points. */

typedef std::vector< segment_index > polygon; /**< Polygons are vectors of indices to a segment vector. */
typedef std::vector< polygon > polygon_vector; /**< Vector containing polygons. */
typedef polygon_vector::size_type polygon_index; /**< Index to a polygon */

typedef std::vector< ues::math::numeric_type > angle_vector; /**< Vector containing the angles of a sequence of points from a different point. */
typedef angle_vector::size_type point_rank; /**< Rank of a point */
typedef std::vector< point_rank > rank_vector; /**< Vector containing the rank of a sequence of points from a different point. */

typedef std::shared_ptr< const point_vector > shared_point_vector;
typedef std::shared_ptr< const segment_vector > shared_segment_vector;
typedef std::shared_ptr< const polygon_vector > shared_polygon_vector;

static const segment_index NULL_SEGMENT_INDEX = std::numeric_limits<segment_index>::max();

}
}
}

#endif // UES_PF_VG2D_DEFINITIONS_H
