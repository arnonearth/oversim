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


// @file VodApp.ned
// @author Abdollah Ghaffari Sheshjavani(AGH)
//

package oversim.applications.vodapp;
import oversim.common.BaseApp;

//
// Application layer for DenaCast protocol
//
// @author Abdollah Ghaffari Sheshjavani(AGH)
//

simple VodApp extends BaseApp
{
    parameters:
        @class(VodApp);
        int gopSize;		 //Number of frames per Group of Picture
        int numOfBFrame;	//Number of B frames between 'I' and 'P' or between two 'P' frames;
        bool limitedBuffer;// if it is true = buffer is windowOfIntrest else buffer is film length
        int windowOfIntrest @unit(s);  //equal to peer-side buffer
       	int videoAverageRate1 @unit(Kbps);
       	int videoAverageRate2 @unit(Kbps);
       	int videoAverageRate3 @unit(Kbps);
       	int videoAverageRate4 @unit(Kbps);
       	int videoAverageRate5 @unit(Kbps);
       	int videoAverageRate6 @unit(Kbps);
       	int videoAverageRate7 @unit(Kbps);
       	int videoAverageRate8 @unit(Kbps);
       	int videoAverageRate9 @unit(Kbps);
       	int videoAverageRate10 @unit(Kbps);
      	bool adaptiveBufferMapXchange;// AGH
     	int epsilon;// for adaptive Buffer XchangeSize 
		int	firstAdaptiveEpsilon;// for adaptive Buffer XchangeSize 
		int redundantEpsilon;
		bool useRequestBigBufferMap;//AGH
		double bufferMapExchangePeriod @unit(s);  //it means how often send bufferMap to our neighbors
        int Fps;					// Frame per second of the video
        bool rateControl;		// to enable and disable rateControl
        int maxFrameRequestPerBufferMapExchangePeriod; // for rate control
        double startUpBuffering @unit(s);	// How much time of video should buffer until playing
        int maxDownloadFurtherPlayPoint @unit(s);	// how much time of video can download Further PlayPoint-AGH	
        double measuringTime @unit(s);
        int receiverSideSchedulingNumber;
        int senderSideSchedulingNumber;
        int chunkSize @unit(Frames);
        double averageChunkLength @unit(Kb);
        bool checkNeighborsTimeOut; //when we have peers that Not Gracefull Exit- it must be true-AGH
       	int discontinuityTimeParam @unit(s);
       	// improvment-AGH
        bool improvmentStructure;
        double improvmentLineGradientTreshold;//AGH
        double improvmentTradeoffParam;//AGH- trade off parameter between bandwidth difference and playback difference
        int improvementMinTreshold @unit(Kbps);//AGH
        double improvementPeriod @unit(s);//AGH 
       //Emergency-AGH
      	bool useEmergencyConnection;
      	int emergencyMinVideoInBuffer @unit(s);
      	int emergencyVideolengthBuffer @unit(s);
      	//
      	int deadLineFrame @unit(s);
      	int retryframeReqTime  @unit(s);
      	bool retryReq;
      	bool retryReqChunksIfNeighborLeave;
      	//
        bool meshTestwithoutStreaming;// for test mesh structiure-if it is true streaming dont work-AGH
       	bool limitSeekForwardForTransiantState;//AGH
       	int limitTimeFromSimTime @unit(s);// for seek fwd-AGH
        @display("i=block/transport");	
}


