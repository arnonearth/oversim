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

/**
 * @file MeshOverlay.h
 * @author Abdollah Ghaffari Sheshjavani (AGH),Behnam Ahmadifar, Yasser Seyyedi
 */

#ifndef MeshOVERLAY_H_
#define MeshOVERLAY_H_


#include <BaseOverlay.h>
#include "MeshOverlayMessage_m.h"
#include "VideoMessage_m.h"
#include "ChunkPackets.h"
#include "VodApp.h"


#define nCr(n,r) (factorial(n) / factorial(n-r)/factorial(r))

class MeshOverlay : public BaseOverlay
{
protected:
	int packetUnit; /**< unit of packet for packetization process*/
	double firstBufferMapSending; /**< First time that bufferMap is sent to neighbors*/
	bool bufferMapExchangeStart; /**< Whether bufferMap is start to sent*/
    double downBandwidth; /**< The download bandwidth that node initially has*/
    double upBandwidth; /**< The upload bandwidth that node initially has*/
    double sourceBandwidth; /**< upload and download Bandwidth of source node*/
    double videoAverageRate[10]; /**< average video bit rate for calculating neighbor num*/
    double percentOfFreeRiders;
    bool isSource;/**<whether this node is server*/
	double pMax; /**< */
	bool FEC; /**< true if Forward Error Correct enable*/
	bool ARQ; /**< true if Automatic Repeat reQuest (ARQ) enable*/
	bool limitedBuffer;
	LocalVariables* LV; /**< pointer to LocalVariables module */
    TransportAddress trackerAddress; /**< Transport address of tracker node */
    double stat_PeerEntranceTime; /**< Entrance Time of  peer-AGH*/
	double stat_TotalEndtoEndDelay;
	int stat_EndtoEndCount;
	//double stat_endtoendDelay; /** Delay for all packets */
	// statistics
	uint32_t stat_TotalReceived; /**< total message received*/
	uint32_t stat_TotalErrorReceived; /**< Total message that received*/
	uint32_t stat_TotalByte; /**<Total Byte received*/
	uint32_t stat_FECRedundent; /**< Total FEC redundant packet that received*/
//	uint32_t stat_UpBandwidthUtilization; /**< percent of upload bandwidth that uses by peer-AGH*/
	uint32_t stat_TotalSendByte; /**< Total byte send by peer-AGH*/
	uint32_t stat_TotalDataSendByte; /**< Total Data byte send by peer-AGH*/
	uint32_t stat_TotalControlSendByte; /**< Total Control byte send by peer-AGH*/




public:
	virtual void initializeOverlay(int stage);
	virtual void handleAppMessage(cMessage* msg);
	virtual void handleUDPMessage(BaseOverlayMessage* msg);
	virtual void finishOverlay();
	/**
	 * base class destructor
	 */
	~MeshOverlay();
	/**
	 * Segments large message and send to UDP
	 *
	 * @param msg message to be segmented
	 */
	void packetizeAndSendToUDP(VideoMessage* videoMsgApp);
	/**
	 * Buffer received packets from UDP and when it completed send it
	 * to application layer
	 *
	 * @param msg message to buffer
	 */
	void bufferAndSendToApp(VideoMessage* videoMsgUDP, bool hasBitError, int length, int seq, int redundant);
	/**
	 * handle messeges that have error if ARQ become true
	 * @param videoMsgUDP video message (packet) with error from UDP
	 * @param length size of packet
	 * @param seq sequence number of packet
	 */
	void handleARQ(VideoMessage* videoMsgUDP, int length, int seq);
	/**
	 * Retransmit packet that has error
	 * @param ARQMsg packet with retransmission
	 */
	void retransmitPAcket(ErrorRecoveryMessage* ARQMsg);
	/**
	 * calculate number of FEC redundant packet need for a message that is going to send
	 * @param fragmentNo number of fragmentation of this message based on packetSize
	 * @return integer  FEC redundant
	 */
	int getNumFECRedundantPackets(int fragmentNo);
	/**
	 * gives out factorial of x
	 * @param x a variable to calculate its factorial
	 * @return integer factorial of x
	 */
	int factorial (int x);
    /**
     * gives out current node download bandwidth
     * @return double download bandwidth
     */
    double getDownBandwidth(){return downBandwidth;}
    /**
     * gives out current node upload bandwidth
     * @return upload bandwidth
     */
    double getUpBandwidth(){return upBandwidth;}
    /**
     * set bandwidth variables
     */
    virtual void setBandwidth();

	std::vector <ChunkPackets> chunkBuffer; /**< Vector in which keeps chunk packets for buffering.*/


};
#endif /* MESHOVERLAY_H_ */
