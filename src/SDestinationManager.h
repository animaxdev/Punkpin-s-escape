#pragma once

#include "defines.h"

//  Helper struct used to store destination
//  of consequent moves
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
        if( sqrt( pow(( cords.first - destinationList[dstNr].first ), 2) +
          pow(( cords.second - destinationList[dstNr].second ), 2) ) < ( velocity/2 + 1 ) )
        {
          if(++dstNr == destinationList.size())
              dstNr = 0;
        }

    return destinationList[dstNr];
    }
};
