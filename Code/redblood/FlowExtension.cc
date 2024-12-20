// This file is part of HemeLB and is Copyright (C)
// the HemeLB team and/or their institutions, as detailed in the
// file AUTHORS. This software is provided under the terms of the
// license in the file LICENSE.

#include "redblood/FlowExtension.h"
#include "util/Vector3D.h"
#include "Exception.h"
#include "units.h"

namespace hemelb
{
  namespace redblood
  {

    namespace {
      void check(FlowExtension const& flowExt)
      {
	if (std::abs(flowExt.normal.GetMagnitude() - 1e0) > 1e-8)
	  throw Exception() << "Flow extension normal not unit vector: " << flowExt.normal;
	if (flowExt.length <= 1e-12)
	  throw Exception() << "Flow extension too short, length = " << flowExt.length;
	if (flowExt.radius <= 1e-12)
	  throw Exception() << "Flow extension too narrow, radius = " << flowExt.radius;
      }
    }

    FlowExtension::FlowExtension(LatticePosition const &n0, LatticePosition const &gamma, LatticeDistance l,
				 LatticeDistance r, LatticeDistance fl) :
      Cylinder( { n0, gamma, r, l }), fadeLength(fl)
    {
      check(*this);
    }
    FlowExtension::FlowExtension() :
      Cylinder( { LatticePosition(1, 0, 0), LatticePosition(0, 0, 0), 1, 1 }), fadeLength(1)
    {
      check(*this);
    }

    //! Checks whether a cell is inside a flow extension
    bool contains(const Cylinder & flowExt, const LatticePosition& point)
    {
      // Vector from centre of start of cylinder to point
      LatticePosition const pd = point - flowExt.origin;

      // distance from pos to point
      LatticeDistance const dot = Dot(pd, flowExt.normal);

      // If the (squared) distance is less than 0 then the point is behind the
      // cylinder cap at pos.
      // If the (squared) distance is greater than the squared length of the
      // cylinder then the point is beyond the end of the cylinder.
      if (dot < 0.0 || dot > flowExt.length)
        return false;

      // Point lies between the parallel caps so calculate the (squared) distance
      // between the point and the line between a and b
      LatticeDistance const dist = pd.GetMagnitudeSquared() - dot * dot;
      return (dist <= flowExt.radius * flowExt.radius);
    }

    Dimensionless linearWeight(FlowExtension const& flowExt, LatticePosition const& position)
    {
      if (not contains(flowExt, position))
        return 0e0;

      auto const z = Dot(flowExt.normal, position - flowExt.origin);
      return std::max(0e0, 1e0 - z / flowExt.fadeLength);
    }
  } // namespace hemelb::redblood
} // namespace hemelb
