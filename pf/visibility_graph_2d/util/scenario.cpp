#include "scenario.h"

using namespace ues::pf::vg2d;

scenario::scenario() noexcept
    : points ( std::make_shared< point_vector >() ),
      segments ( std::make_shared< segment_vector >() ),
      polygons ( std::make_shared< polygon_vector >() )
{
}


scenario::scenario ( point_vector points, segment_vector segments, polygon_vector polygons ) noexcept
    : points ( std::make_shared< point_vector > ( points ) ),
      segments ( std::make_shared< segment_vector > ( segments ) ),
      polygons ( std::make_shared< polygon_vector > ( polygons ) )
{
}


scenario::scenario ( shared_point_vector points, shared_segment_vector segments, shared_polygon_vector polygons ) noexcept
    : points ( std::move ( points ) ),
      segments ( std::move ( segments ) ),
      polygons ( std::move ( polygons ) )
{
}
