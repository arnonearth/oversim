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


// @file CdnServer.ned
// @author Abdollah Ghaffari Sheshjavani(AGH)
//

package oversim.overlay.cdnserver;
import oversim.overlay.meshoverlay.MeshOverlay;

simple CdnServer extends MeshOverlay
{
    parameters:
        @class(CdnServer);
        int activeNeighbors;
        int passiveNeighbors;
        int cdnClients;// because in vod cdn must service allfilm at least one- for example if we have 10 film it must be 10 or larger (AGH)-
     	int meshStructure;// 0 is simple mesh// 1 is CTMVOD//  2 is TMVOD...(AGH)
		int directConnectionToServers;
       	int meshType; // 0 is randomly and 1 is ORDINAL -AGH
       	//int percentofInteractive;//its between 0 to 1 and if it is -1 then interactive is base on result of paper"can vod profitable" AGH
       // int simpleMeshType;//AGH
        double neighborsTimeOutLimit @unit(s);// for check not graceful exit
        bool checkNeighborsTimeOut; //when we have peers that Not Gracefull Exit- it must be true-AGH
        double neighborNotificationPeriod @unit(s);
        double sourceBandwidth @unit(Mbps);
      // double changeProbability;// Probability of changing video is p, and Probability of exit is 1-p (AGH)
       // bool adaptiveNeighboring;
        bool serverGradualNeighboring;
        int serverGradualNeighboringAverageTime;
       // bool usePreVideo;
        
}