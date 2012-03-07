#include "extraction/PlaneGeometrySelector.h"

namespace hemelb
{
  namespace extraction
  {
    PlaneGeometrySelector::PlaneGeometrySelector(const util::Vector3D<float>& point,
                                                 const util::Vector3D<float>& normal) :
      planePoint(point), normal(normal.GetNormalised()), radius(0.)
    {

    }

    /**
     * Constructor makes a plane geometry object with given normal, about a given
     * point, with given radius.
     * @param point
     * @param normal
     * @param radius
     */
    PlaneGeometrySelector::PlaneGeometrySelector(const util::Vector3D<float>& point,
                                                 const util::Vector3D<float>& normal,
                                                 float radius) :
      planePoint(point), normal(normal.GetNormalised()), radius(radius)
    {

    }

    const util::Vector3D<float>& PlaneGeometrySelector::GetPoint() const
    {
      return planePoint;
    }

    const util::Vector3D<float>& PlaneGeometrySelector::GetNormal() const
    {
      return normal;
    }

    float PlaneGeometrySelector::GetRadius() const
    {
      return radius;
    }

    bool PlaneGeometrySelector::IsWithinGeometry(const extraction::IterableDataSource& data,
                                                 const util::Vector3D<float>& location)
    {
      const float perpendicularDistance = (location - planePoint).Dot(normal);

      if (std::abs(perpendicularDistance) > (0.5 * data.GetVoxelSize()))
      {
        return false;
      }

      // Return true if using infinite radius of the plane.
      if (radius <= 0.)
      {
        return true;
      }

      const float radiusOfPointSquared =
          ( (location - normal * perpendicularDistance) - planePoint).GetMagnitudeSquared();

      return radiusOfPointSquared <= radius * radius;
    }
  }
}