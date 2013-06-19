// 
// Copyright (C) University College London, 2007-2012, all rights reserved.
// 
// This file is part of HemeLB and is CONFIDENTIAL. You may not work 
// with, install, use, duplicate, modify, redistribute or share this
// file, or any part thereof, other than as allowed by any agreement
// specifically made by you with University College London.
// 

#ifndef HEMELB_LB_STREAMERS_LADDIOLET_H
#define HEMELB_LB_STREAMERS_LADDIOLET_H

#include "lb/streamers/StreamerTypeFactory.h"
#include "lb/streamers/LaddIoletDelegate.h"
#include "lb/streamers/SimpleBounceBackDelegate.h"
#include "lb/streamers/BouzidiFirdaousLallemandDelegate.h"
#include "lb/streamers/GuoZhengShiDelegate.h"

namespace hemelb
{
  namespace lb
  {
    namespace streamers
    {

      template<class CollisionType>
      struct LaddIolet
      {
          typedef IoletStreamerTypeFactory<CollisionType, LaddIoletDelegate<CollisionType> > Type;
      };

      template<class CollisionType>
      struct LaddIoletSBB
      {
          typedef WallIoletStreamerTypeFactory<CollisionType,
              SimpleBounceBackDelegate<CollisionType>, LaddIoletDelegate<CollisionType> > Type;
      };

      template<class CollisionType>
      struct LaddIoletBFL
      {
          typedef WallIoletStreamerTypeFactory<CollisionType,
              BouzidiFirdaousLallemandDelegate<CollisionType>, LaddIoletDelegate<CollisionType> > Type;
      };

      template<class CollisionType>
      struct LaddIoletGZS
      {
          typedef WallIoletStreamerTypeFactory<CollisionType, GuoZhengShiDelegate<CollisionType>,
              LaddIoletDelegate<CollisionType> > Type;
      };

      template<class CollisionType>
      struct LaddIoletJY
      {
          typedef JunkYangFactory<CollisionType, LaddIoletDelegate<CollisionType> > Type;
      };
    }
  }
}

#endif /* HEMELB_LB_STREAMERS_LADDIOLET_H */
