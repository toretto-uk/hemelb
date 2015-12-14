// 
// Copyright (C) University College London, 2007-2012, all rights reserved.
// 
// This file is part of HemeLB and is CONFIDENTIAL. You may not work 
// with, install, use, duplicate, modify, redistribute or share this
// file, or any part thereof, other than as allowed by any agreement
// specifically made by you with University College London.
// 

#ifndef HEMELB_GEOMETRY_SITEDATA_H
#define HEMELB_GEOMETRY_SITEDATA_H

#include "units.h"
#include "geometry/GeometrySite.h"
#include "geometry/SiteType.h"
#include "net/MpiDataType.h"

namespace hemelb
{
  namespace net
  {
    template<>
    MPI_Datatype MpiDataTypeTraits<geometry::SiteType>::RegisterMpiDataType();
  }

  namespace geometry
  {
    class SiteData
    {
      public:
        SiteData(const GeometrySite& siteReadResult);
        SiteData(const SiteData& other);
        SiteData(); //default constructor allows one to use operator[] for maps
        ~SiteData();

        bool IsWall() const;
        bool IsSolid() const;
        unsigned GetCollisionType() const;

        SiteType GetSiteType() const
        {
          return type;
        }
        SiteType& GetSiteType()
        {
          return type;
        }

        int GetIoletId() const
        {
          return ioletId;
        }
        int& GetIoletId()
        {
          return ioletId;
        }

        bool HasWall(Direction direction) const;
        bool HasIolet(Direction direction) const;

        /**
         * These functions return internal representations and should only be used for debugging.
         */
        uint32_t GetIoletIntersectionData() const;
        uint32_t &GetIoletIntersectionData()
        {
          return ioletIntersection;
        }
        uint32_t GetWallIntersectionData() const;
        uint32_t &GetWallIntersectionData()
        {
          return wallIntersection;
        }

      protected:
        /**
         * This is a bit mask for whether a wall is hit by links in each direction.
         */
        uint32_t wallIntersection;

        /**
         * This is a bit mask for whether an iolet is hit by links in each direction.
         */
        uint32_t ioletIntersection;

        SiteType type;
        int ioletId;
    };
  }
}

#endif /* HEMELB_GEOMETRY_SITEDATA_H */
