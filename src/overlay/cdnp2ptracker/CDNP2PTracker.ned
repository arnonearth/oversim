//
// Copyright (C) 2012 Tarbiat Modares University All Rights Reserved.
// http://ece.modares.ac.ir/mnl/
// This system was designed and developed at the DML(Digital Media Lab http://dml.ir/)
// under supervision of Dr. Behzad Akbari (http://www.modares.ac.ir/ece/b.akbari)
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
//


// @file CDNP2PTracker.ned
// @author Abdollah Ghaffari Sheshjavani (AGH)
//

package oversim.overlay.cdnp2ptracker;
import oversim.common.BaseOverlay;

simple CDNP2PTracker extends BaseOverlay
{
    parameters:
	    int selectPolicy;
        bool connectedMesh;
        bool usePreVideo;// if its true peer that watch a video in past can serve this video when he dont wach this(AGH). 
    	int meshStructure;//0 is simple mesh//  1 is CTMVOD//  2 is TMVOD...(AGH)
    	int directConnectionToServers;
    	int meshType;//0 is randomly and 1 is ORDINAL -AGH
        int cdnClients;
        int varianceTime;// this is used for connect watchPast peer to other peer(AGH)
        int minTimeLenghtWP;// this is min time that peer must watch film in past to can serv that film in future (AGH)
  		double neighborsTimeOutLimit @unit(s);// for check not graceful exit-AGH
      	int passiveNeighbors;//AGH
       	int cascadeMinTime;//AGH
       	int cascadeMaxTime;//AGH
       	int cascadeMinNodes;//AGH
       	int cascadeMaxNodes;//AGH
       	int maxCascadeNumber;//AGH
       	int reserveConnectionTime;//AGH
       	double diferenceForCascadeMinesforFWD;// its between 0 and 1 and for seek forward 
        @class(CDNP2PTracker);
}
