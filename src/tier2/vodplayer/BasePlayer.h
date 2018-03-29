//
//Copyright (C) 2012 Tarbiat Modares University All Rights Reserved.
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
 * @file BasePlayer.h
 * @author Abdollah Ghaffari Sheshjavani(AGH),Behnam Ahmadifar, Yasser Seyyedi
 */

#ifndef BASEPLAYER_H_
#define BASEPLAYER_H_

#include <omnetpp.h>

#include <VideoFrame.h>
#include <VideoMessage_m.h>
#include <BaseVodStream.h>
#include <LocalVariables.h>

struct PlayerItem
{
	VideoFrame frame;
	bool status;
};
class BasePlayer: public BaseApp
{
public:
	/**
	 * base class-destructor
	 */
	~BasePlayer();
	/**
	 * function that play playerBuffer in 1/Fps time
	 */
	virtual void play();
	/**
	 * check dependency of frames base on dependency tree of specific code (like MPEG4)
	 */
	virtual void checkFrames ();
	/**
	 * update time substring on top of module
	 *
	 * @param frameNo current streaming frame number
	 */
	void updateTime(int long frameNo);
	/**
	 * base on the frame number and numberOfBFrame determine type
	 *
	 * @retrun character frame type
	 */
	char getFrameType(int FrameNumber);


	virtual void initializeApp(int stage);
	virtual void handleLowerMessage(cMessage* msg);
	virtual void handleTimerEvent(cMessage* msg); // called when we received a timer message
	virtual void finishApp();
	virtual void CheckDelay();
	BaseVodStream* bs;
	LocalVariables* LV;

protected:
	int Fps; /**< frame per second*/
	unsigned int numberOfBFrame; /**< number of B frames between I and P or between two P frames*/
	int gopSize; /**< size of GoP*/
	short videoAverageRate[10];//
	bool isPlaying; /**< whether the playing is start*/
	bool StartInit; /**< when first message from lower_tier received it become true*/
	bool discontinuityStart;
	bool InteractiveDiscontinuity;
//	int long totalBit;
	int frameCounter;
	static int nodeNumber;
	static int nodePlayed;
	short isplayed;
	static int recorded;
	double totalSize;
	double lossSize;
	double camSize;
	double playerLossSize;
    double measuringTime;
    double dependencyLoss_Size;
    double instance_dependencyLoss_Size;

	//self message
	cMessage* playTimer; /**< self message that call play() function*/

	std::vector<PlayerItem> playerBuffer; /**< vector in which acts as buffer*/

	//statistics
//	uint32_t stat_frameLoss; /**< number of frame that lost during simulation */
	uint32_t stat_totalBytesReceived; /**< number of bits that received */
	uint32_t stat_totalFrameReceived; /**< number of frames that received */
	uint32_t stat_frameDependancyLoss; /**< number of frame that is not playable due to dependency fail*/
	uint32_t stat_totalIframeReceived; /**< number of I frames that received */
	uint32_t stat_totalPframeReceived; /**< number of P frames that received */
	uint32_t stat_totalBframeReceived; /**< number of B frames that received */
	uint32_t stat_numOfPlayerTimer; /**< number of player timer self message that called*/
	double stat_startPlaying; /**< time in which the player start to play*/
	double stat_numberOfDiscontinuity;
	double stat_interactiveDiscontinuityTime;
	double stat_discontinuityTime;
	double stat_allPlayedTime;

};
#endif /* BASEPLAYER_H_ */
