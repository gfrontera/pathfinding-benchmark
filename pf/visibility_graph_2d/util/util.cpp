#include "util.h"

using namespace ues::pf::vg2d;


ues::pf::vg2d::point_index ues::pf::vg2d::add_if_not_present ( ues::pf::vg2d::point_vector & points,
                                                point_map & added_points,
                                                ues::geom::point<2> input_point ) noexcept
{
    auto added_points_it = added_points.find ( input_point );
    if ( added_points_it != added_points.end() )
    {
        return added_points_it->second;
    }
    else
    {
        // If the point is not in the point vector,
        // then it is added.
        ues::pf::vg2d::point_index point_index = points.size();
        added_points.insert ( { input_point, point_index } );
        points.push_back ( std::move( input_point ) );
        return point_index;
    }
}