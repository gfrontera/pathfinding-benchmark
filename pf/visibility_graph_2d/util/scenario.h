#ifndef UES_PF_VG2D_SCENARIO_H
#define UES_PF_VG2D_SCENARIO_H

#include "definitions.h"

namespace ues
{
namespace pf
{
namespace vg2d
{

class scenario
{
public:
    
    scenario() noexcept;

    scenario ( point_vector points,
               segment_vector segments,
               polygon_vector polygons ) noexcept;

    scenario ( shared_point_vector points,
               shared_segment_vector segments,
               shared_polygon_vector polygons ) noexcept;

    /** \name Getter methods */
    /** \{ */
    inline const shared_point_vector & get_shared_points() const noexcept;
    inline const shared_segment_vector & get_shared_segments() const noexcept;
    inline const shared_polygon_vector & get_shared_polygons() const noexcept;

    inline const point_vector & get_points() const noexcept;
    inline const segment_vector & get_segments() const noexcept;
    inline const polygon_vector & get_polygons() const noexcept;
    /** \} */
private:
    shared_point_vector points;
    shared_segment_vector segments;
    shared_polygon_vector polygons;
};

// Inlined methods.

const shared_point_vector & scenario::get_shared_points() const noexcept
{
    return points;
}


const shared_segment_vector & scenario::get_shared_segments() const noexcept
{
    return segments;
}


const shared_polygon_vector & scenario::get_shared_polygons() const noexcept
{
    return polygons;
}


const point_vector & scenario::get_points() const noexcept
{
    return *points;
}


const segment_vector & scenario::get_segments() const noexcept
{
    return *segments;
}


const polygon_vector & scenario::get_polygons() const noexcept
{
    return *polygons;
}


}
}
}

#endif // UES_PF_VG2D_SCENARIO_H
