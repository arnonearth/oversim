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
 * @file VodApp.h
 * @author Abdollah Ghaffari Sheshjavani (AGH),Behnam Ahmadifar, Yasser Seyyedi
 */

#ifndef VODAPP_H_
#define VODAPP_H_
#include "BufferMap.h"
#include "VideoMessage_m.h"
#include <BaseApp.h>
#include <BaseVodStream.h>
#include <VideoBuffer.h>
#include "LocalVariables.h"

struct requesterNode
{
	TransportAddress tAddress; /**< Transport address of the node that request a chunk*/
	short int chunkNo; /**< the chunk number that the node is requested*/
	int filmId; // for vod application (AGH)
};

struct nodeBufferMap
{
	TransportAddress tAddress; /**< Transport address of a neighbor */
	BufferMap buffermap; /**< the BufferMap object of the node (AGH)*/
	int cascadeNumber;
	int videoId;
	bool isServer;
	bool pleaseSendToMEBigbufferMap;//AGH
	bool requestBigbufferMap;//AGH
	bool watchInPast;//(AGH)
	int recivePoint;// point that neighbor receiving chunks
	int playbackPoint;// its use only for Structure Improving-AGH
	int bufferPoint;// point of buffer map that  current node send to neighbor
	int videoBufferlastSetChunk;//(AGH)
	double totalBandwidth; /**< the total bandwidth capacity of a node*/
	int downBandwidth;
	double freeBandwidth; /**< the free bandwidth of neighbor in a defined time slot*/
	int requestCounter; /**< a number that shows number of requested - number of response chunk*/
	double delay;
	short hopCount;//AGH
	short redundantSize;//AGH
	
};
struct chunkPopulation
{
	int chunkNum; /**< chunk number */
	std::vector <int>supplierIndex; /**< vector that keeps supplier index of neighborsBufferMaps vector*/
};

class VodApp : public BaseApp
{
public:
    /**
     * initializes base class-attributes
     *
     * @param stage the init stage
     */
    virtual void initializeApp(int stage);
    virtual void finishApp();
    virtual void handleTimerEvent(cMessage* msg);
    virtual void handleLowerMessage(cMessage* msg);
    virtual void handleUpperMessage(cMessage* msg);

	/**
	 * delete a frame number from sendframes vector (after timeout)
	 * @param framenum frame Number to delete
	 * @param sendFrames vector of frame that is already send
	 */
	virtual void deleteElement(int frameNum, std::vector <requestedFrames>& sendFrames);
	/**
	 * broadcast buffermap to all the node neighbor
	 */
	virtual void bufferMapExchange();
	/**
	 * check if we whether enough frames eqaul to startup buffering to play
	 */
	virtual void checkForPlaying();

protected:
	//10 is for if we want to study effects and relations between chunk size and video length
    unsigned short int numOfBFrame; /**<Number of B frames between 'I' and 'P' or between two 'P' frames */
   	unsigned short int chunkSize; /**< Number of frames in a chunk - */
	unsigned short int gopSize; /**< number of frame available in one GoP (Group of picture)*/
	int bufferSize[10]; /**<number of chunk in buffer */
	int videolenght[10];// length of videos that peer watching them hold in this variable (AGH)
	double bufferMapExchangePeriod; /**<Period in which we push our BufferMap to our neighbors */
    bool isVideoServer; /**<store parameter isVideoServer, if true it is video server */
    bool limitedBuffer;// if it is true = buffer is windowOfIntrest else buffer is film length (AGH)
    unsigned short int Fps; /**< Store parameter Fps, Frame per second of the video*/
    int discontinuityTimeParam;
    short videoAverageRate[10];//
    bool rateControl; /**< if true use rate control mechanism*/
    int maxFrameRequestPerBufferMapExchangePeriod; // for rate control
    int numberOfFrameRequested;//for rate control
    int playbackPoint; /**< store the frame number of the frame that recently send to player module*/
    int recivePoint;
    PlayingState playingState; /**< when we send first message to player module it become true*/
    bool bufferMapExchangeStart; /**< as first bufferMap received we start to exchange our bufferMap too and it become ture*/
    double startUpBuffering; /** < How much video (second) should buffer in order to play*/
    double measuringTime; /**< the SimTime that we start collecting statistics*/
    unsigned short int receiverSideSchedulingNumber; /**< The number for selecting receiver side scheduling*/
    unsigned short int senderSideSchedulingNumber; /**< The number for selecting sender side scheduling*/
    double averageChunkLength; /**< average of one chunk length*/
    bool schedulingSatisfaction; /**< true if condition is OK for start scheduling*/
    //int currentFilmId; // selected film in vod app. (AGH);
    int windowOfIntrest;
    int FirstbufferExchangeWindowInterest;//-AGH
    int  bufferExchangeWindowInterest;//AGH
    bool adaptiveBufferMapXchange;//AGH
    short epsilon;
    int firstAdaptiveEpsilon;//AGH
    int redundantEpsilon;
    bool useRequestBigBufferMap;//AGH
    int endTimeWatchVideo;
    bool haveInteractive;//AGH
    int interactiveTimeStart;
    int seekTime;// how many second video seek forward when peer interactive to video-AGH
    int interactiveChunkStart;
    int seekChuncknumbers;
    int maxDownloadFurtherPlayPoint;
    int minHopCount;
    bool  ready;
    bool discontinuityStart;
    bool meshTestwithoutStreaming;// for test mesh structiure-if it is true streaming dont work-AGH
    bool limitSeekForwardForTransiantState; // AGH
    int limitTimeFromSimTime;// for seek fwd-AGH
    bool improvmentStructure;
    double improvmentLineGradientTreshold;//AGH
    double improvmentTradeoffParam;//AGH- trade off parameter between bandwidth difference and playback difference
    int improvementMinTreshold;//AGH
    double  improvementPeriod;//AGH
    short improvementScoreindex;//AGH
    bool useEmergencyConnection;//AGH
    int emergencyMinVideoInBuffer ;//AGH
    int emergencyVideolengthBuffer ;//AGH
	int deadLineFrame;
    int retryframeReqTime;
    bool retryReq;
    bool retryReqChunksIfNeighborLeave;
    bool setStatisticsPosition;
    bool checkNeighborsTimeOut; //when we have peers that Not Gracefull Exit- it must be true-AGH
    int FreeRideUpload;
	bool firstRequest;
	double firstChunkCreation;
  std::vector <nodeBufferMap> neighborsBufferMaps;



	// self-messages
	cMessage* structureImprovementTimer;//AGH
    cMessage* bufferMapTimer;/**< timer message to send buffermap to the neighbors */
	cMessage* requestChunkTimer; /**< time message to request chunk(s) from neighbors */
	cMessage* sendFrameTimer; /**< timer that is used in order to call sender side scheduling */
	cMessage* playingTimer; /**< timer in which send self message in 1/fps to send buffered frames to the player */

	BaseVodStream* bc; /**< a pointer to base camera*/
	LocalVariables* LV; /** < a pointer to LocalVariables module*/

	//Scheduling
	virtual void coolStreamingScheduling();
	virtual void recieverSideScheduling2();
	virtual void recieverSideScheduling3();
	virtual double getRequestFramePeriod();
	//virtual int requestRateState();
	virtual void senderSideScheduling1();
	virtual void senderSideScheduling2();
	virtual void selectRecieverSideScheduling();
	virtual void selectSenderSideScheduling();
	virtual void handleChunkRequest(TransportAddress& SrcNode,int FilmId,int chunkNo, bool push);
	virtual void countRequest(TransportAddress& node);
	virtual void sendFrameToPlayer();
	virtual void deleteNeighbor(TransportAddress& node);
	virtual void sendframeCleanUp();
	virtual bool isMeasuring(int frameNo);
	virtual void resetFreeBandwidth();
	virtual void updateNeighborBMList(BufferMapMessage* BufferMap_Recieved);
	bool isInVector(TransportAddress& Node,  std::vector <neighbor> &neighbors);
	void checkAvailability_RateControlLoss();
	int getNextChunk();
	int minimum(int A,int B);
	virtual void setStatistics();


	/**<
	 *  Vector in which keeps the sending frames
	 * to prevent duplicate request.
	 */
	std::vector <requestedFrames> sendFrames;
	/**<
	 * Vector in which keeps the frames that is next frame for requesting
	 * but they are not in neighbors (for exclusion). When the BufferMap message
	 * received it will be cleared
	 */
	std::vector < int > notInNeighbors;
	/**<
	 * Vector for keep bufferMap of neighbors
	 */

	std::vector <int> requestedChunks;
	std::vector <double> neighborHops;
	std::map <double,requesterNode> senderqueue;

	//statistics

	double stat_startupDelay;/**<
	 * Time (a random variable) that elapse between
	 * start to buffering (selecting a streaming channel)
	 * and start to playing
	 */
	double stat_startSendToPlayer; /**< Time to start to send frames to player module*/
	double stat_startBuffering; /**< Time that we start to buffering (select channel)*/
	double stat_startBufferMapExchange; /**< The time that node start to exchange it bufferMap*/
	double stat_TotalReceivedSize; /** < */
	double stat_RedundentSize; /** < */
	double stat_totalBufferExchange;//AGH
	double stat_lossBufferExchange;//AGH
	double stat_lossBufferExchange2;//AGH-where big buffer requested
	double stat_videoDownloadBigerThanBufferXchange;
	//double stat_peresentTimeInSystem;
	double stat_RedunduntReceiveBufferExchange;//AGH
	double stat_BandwidthForFirstBufferExchange;//AGH
	double stat_BandwidthForBufferExchange;//AGH
	double stat_BandwidthForBufferExchangePerSending;//AGH
	double stat_BandwidthSavedForNotSendingBuffermapWhenPeerNotHaveUsefullData;//AGH
	double stat_BandwidthSavedForNotSendingBuffermapBiggerLastsendChunk;//AGH
	double stat_BandwidthOverheadForsendingRedunduntInBuffermap;//AGH
	double stat_VideoFramesDownloadedButNotPlayedForInteractive;//AGH
	double stat_VideoFramesDownloadedButNotPlayedForExit;//AGH
	int stat_continuityShow;//AGH 0is not contiguous
	double stat_discontinuityTime;//AGH
	double peerAllWatchingTime;//AGH- all time from user start to watching to end watching
	double stat_NumberOfDiscontinuity;//AGH
	double stat_startSeekforward;//AGH
	double stat_seekforwardDelay;//AGH
	double stat_seekforwardNewJoinDelay;//AGH
	double stat_jointoMeshStart;
	double stat_recivefirstStreamPacket;
	double stat_allFramePlayed;
	double stat_lossFrame;
	double stat_lossFrame2;
	uint32_t stat_allRedundantRecivedBits;
	uint32_t stat_usefullRedundantRecivedBits;
	uint32_t stat_allExistInBufferRedundantRecivedbits;
	uint32_t stat_allRedundantRecivedMsg;

};

#endif /* VodAPP_H_ */
