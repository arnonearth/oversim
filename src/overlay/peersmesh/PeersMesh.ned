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


// @file PeersMesh.ned
// @author Abdollah Ghaffari Sheshjavani(AGH)
//

package oversim.overlay.peersmesh;
import oversim.overlay.meshoverlay.MeshOverlay;

simple PeersMesh extends MeshOverlay
{
    parameters:
        @class(PeersMesh);
        int activeNeighbors;
        int passiveNeighbors;
     	int meshStructure;// 0 is simple mesh 1 is my structure 2 ...(AGH)
      	int isStable;// if is 1 peer dont exit from system- 2 it is can seek forward and 0 is real world-AGH 
        double percentofExit;
        double percentOfNotGracefullExit;// its between 0 and 1-AGH
        bool checkNeighborsTimeOut; //when we have peers that Not Gracefull Exit- it must be true-AGH
		double neighborsTimeOutLimit @unit(s);// for check not graceful exit-AGH
		int startExitSimTime;
       	double percentofInteractive; //if is 1 peer dont exit from system -2 it can seek forward-3 can exit with exit prob-4 can exit and seek forward  -0 (real world)is base on result of paper"can vod profitable"-AGH 
       	int startIntractiveSimTime;  //sim time that after this time peer can interactive-AGH
        int simpleMeshType;//AGH
        double improvementProcessTimeLenght @unit(s);
        double neighborNotificationPeriod @unit(s);
        double sourceBandwidth @unit(Mbps);
        double changeProbability;// Probability of changing video is p, and Probability of exit is 1-p (AGH)
        bool adaptiveNeighboring;
        bool usePreVideo;//AGH
        bool usingSpecialInteractiveNeghborReq;
        
}