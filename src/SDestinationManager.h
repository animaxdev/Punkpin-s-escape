#pragma once

#include "defines.h"

//	Helper struct used to store destination
//	of consequent moves
//

struct destinationManager
{
    std::vector<position> destinationList;
    unsigned int dstNr;
    bool active;

    destinationManager(std::vector<position> dstLst)
    {
        destinationList = dstLst;
        dstNr = 0;
        active = true;
    }

    position move(const position cords, int velocity)
    {
        //	couldnt't assign simple == , because doubles aren't
        //  accurate enough, so i made sections...      - TODO (cause its ugly)
        //
//        if (    (cords.first / destinationList[dstNr].first) > 1.f - velocity/200.f
//            && (cords.second / destinationList[dstNr].second) > 1.f - velocity/200.f
//            && (cords.first / destinationList[dstNr].first) < 1.f + velocity/200.f
//            && (cords.second / destinationList[dstNr].second) < 1.f + velocity/200.f  ) {
//                if (++dstNr == destinationList.size())
//                    dstNr = 0;

//        }

          if( sqrt( pow(( cords.first - destinationList[dstNr].first ), 2) +
              pow(( cords.second - destinationList[dstNr].second ), 2) ) < ( velocity/2 + 1 ) )
          {
              if (++dstNr == destinationList.size())
                  dstNr = 0;
          }

        return destinationList[dstNr];
    }
};
