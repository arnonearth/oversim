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
 * @file BasePlayer.cc
 * @author Behnam Ahmadifar, Yasser Seyyedi,Abdollah Ghaffari Sheshjavani(AGH)
 */

#include "BasePlayer.h"
#include <GlobalStatistics.h>


Define_Module(BasePlayer);

BasePlayer::~BasePlayer()
{
}

void BasePlayer::initializeApp(int stage)
{
	if (stage != MIN_STAGE_APP)
		return;
	Fps = par("Fps");
	numberOfBFrame = par("numOfBFrame");
	gopSize = par("gopSize");
	measuringTime = par("measuringTime");
	discontinuityStart=false;
	InteractiveDiscontinuity=false;
	isPlaying = false;
	StartInit = false;
	totalSize = 0;
	lossSize = 0;
	camSize = 0;
	playerLossSize = 0;
	dependencyLoss_Size = 0;
	instance_dependencyLoss_Size = 0;
	nodeNumber += 1;
	isplayed=0;
	videoAverageRate[0] = par("videoAverageRate1");
	videoAverageRate[1] = par("videoAverageRate2");
	videoAverageRate[2] = par("videoAverageRate3");
	videoAverageRate[3] = par("videoAverageRate4");
	videoAverageRate[4] = par("videoAverageRate5");
	videoAverageRate[5] = par("videoAverageRate6");
	videoAverageRate[6] = par("videoAverageRate7");
	videoAverageRate[7] = par("videoAverageRate8");
	videoAverageRate[8] = par("videoAverageRate9");
	videoAverageRate[9] = par("videoAverageRate10");

	playTimer = new cMessage();
	bs = check_and_cast<BaseVodStream*>(simulation.getModuleByPath("CDN-Server[1].tier2.mpeg4vodstream"));

    LV = check_and_cast<LocalVariables*>(getParentModule()->getParentModule()->getSubmodule("tier1")->getSubmodule("localvariables"));

    // statistics
	stat_totalBytesReceived = 0;
	stat_totalFrameReceived = 0;
	stat_frameDependancyLoss = 0;
	stat_totalIframeReceived = 0;
	stat_totalBframeReceived = 0;
	stat_totalPframeReceived = 0;
	stat_numOfPlayerTimer = 0;
	stat_startPlaying = 0;
	stat_numberOfDiscontinuity=0;
	stat_discontinuityTime=0;
	stat_interactiveDiscontinuityTime=0;
	stat_allPlayedTime=0;
}
void BasePlayer::handleTimerEvent(cMessage* msg)
{
	if(msg == playTimer )
	{
		stat_allPlayedTime++;// 1/fps-AGH
		if(InteractiveDiscontinuity)
		{
			stat_interactiveDiscontinuityTime++;
		}
		else if(discontinuityStart)
		{
			stat_discontinuityTime++;// 1/fps-AGH
		}
		else
		{
			if(isPlaying)
				if(playerBuffer.size() >= numberOfBFrame+2)
					play();
				else
					isPlaying = false;
			else
			{
				if(playerBuffer.size() >= numberOfBFrame+2)
				{
					isPlaying = true;
					frameCounter = playerBuffer.begin()->frame.getFrameNumber();
					CheckDelay();
					stat_startPlaying = simTime().dbl();
				}
			}
		}
		scheduleAt(simTime()+1/(double)Fps, playTimer);
		stat_numOfPlayerTimer +=1;
	}
	else
		delete msg;
}

void BasePlayer::handleLowerMessage(cMessage* msg)
{
	if (dynamic_cast<VideoMessage*>(msg) != NULL)
	{
		VideoMessage* playermessage=dynamic_cast<VideoMessage*>(msg);
		 if(playermessage->getType()==6)
		 {
			 discontinuityStart=true;
			 stat_numberOfDiscontinuity++;
			 delete playermessage;
			 return;
		 }
		 else if(playermessage->getType()==7)
		 {
			 cancelEvent(playTimer);
			 scheduleAt(simTime()+2/(double)Fps, playTimer);
			 discontinuityStart=false;
			 delete playermessage;
			 return;
		 }
		 else if(playermessage->getType()==8)
		 {
			 InteractiveDiscontinuity=true;
			 delete playermessage;
			 return;
		 }
		 else if(playermessage->getType()==9)
		 {
			 cancelEvent(playTimer);
			 scheduleAt(simTime()+2/(double)Fps, playTimer);
			 InteractiveDiscontinuity=false;
			 delete playermessage;
			 return;
		 }

		if(!StartInit)
		{
			cancelEvent(playTimer);
			scheduleAt(simTime()+1/(double)Fps, playTimer);
			StartInit = true;
			nodePlayed += 1;
			isplayed=100;
		}
		if(playermessage->getVFrame().isSet())
		{
			if(playermessage->getVFrame().getFrameType() == 'I')
				stat_totalIframeReceived +=1;
			if(playermessage->getVFrame().getFrameType() == 'P')
				stat_totalPframeReceived +=1;
			if(playermessage->getVFrame().getFrameType() == 'B')
				stat_totalBframeReceived +=1;
			stat_totalFrameReceived++;
			stat_totalBytesReceived+= playermessage->getVFrame().getFrameLength();
		}
		PlayerItem receivedFrame;
		receivedFrame.frame.setFrame(playermessage->getVFrame());
		receivedFrame.status=false;
		playerBuffer.push_back(receivedFrame);
		checkFrames();
		delete playermessage;
	}
	else
		delete msg;
}
void BasePlayer::updateTime(int long frameNo)
{
	int hour=0,min=0,sec=0;
	if (frameNo > Fps-1)
	{
		sec=frameNo/Fps;
		frameNo= frameNo%Fps;
	}
	if(sec >59)
	{
		min=sec/60;
		sec=sec%60;
	}
	if(min >59)
	{
		hour=min/60;
		min=min%60;
	}
	std::stringstream buf;
	buf << "elapsed Time:" << hour << ":" << min << ":" << sec << ":" << frameNo ;
	std::string s = buf.str();
	getParentModule()->getDisplayString().setTagArg("t", 0, s.c_str());
	getDisplayString().setTagArg("t", 0, s.c_str());

}

char BasePlayer::getFrameType(int FrameNumber)
{
	if (FrameNumber % (gopSize)==0)
		return 'I';
	if (FrameNumber %(numberOfBFrame + 1)==0)
		return 'P';
	return 'B';
}

void BasePlayer::play()
{
}
void BasePlayer::checkFrames()
{
}
void BasePlayer::CheckDelay()
{
    globalStatistics->addStdDev("BasePlayer: Playback Delay", (double)(bs->getLastFrameStreamed() - frameCounter)/(double)Fps);
}
void BasePlayer::finishApp()
{
	cancelAndDelete(playTimer);

	// statistics
	if(totalSize!=0)
		globalStatistics->addStdDev("BasePlayer: Total frame loss", (lossSize/totalSize)*100);
    globalStatistics->addStdDev("BasePlayer: Total received bytes", stat_totalBytesReceived);
    globalStatistics->addStdDev("BasePlayer: Total received frames", stat_totalFrameReceived);
    globalStatistics->addStdDev("BasePlayer: Total frames dependency loss", stat_frameDependancyLoss);
    globalStatistics->addStdDev("BasePlayer: Total I frames received ", stat_totalIframeReceived);
    globalStatistics->addStdDev("BasePlayer: Total P frames received ", stat_totalPframeReceived);
    globalStatistics->addStdDev("BasePlayer: Total B frames received ", stat_totalBframeReceived);
    globalStatistics->addStdDev("BasePlayer: Total playerTimer self message called", stat_numOfPlayerTimer);
    if(stat_allPlayedTime!=0)
    {
    	globalStatistics->addStdDev("BasePlayer:Average percent of Discontinuity ", (stat_discontinuityTime*100)/stat_allPlayedTime);
    	if(stat_discontinuityTime!=0)
    		globalStatistics->addStdDev("BasePlayer:Average percent of Discontinuity in Discontinuity peers ", (stat_discontinuityTime*100)/stat_allPlayedTime);
    }
    globalStatistics->addStdDev("BasePlayer: Average Number of Discontinuity", stat_numberOfDiscontinuity);
    if(stat_interactiveDiscontinuityTime>0)
    {
    	globalStatistics->addStdDev("BasePlayer: Average Time of Interactive Discontinuity", stat_interactiveDiscontinuityTime/(double)Fps);
    	globalStatistics->recordOutVector("BasePlayer:vector of Time of Interactive Discontinuity",stat_interactiveDiscontinuityTime/(double)Fps);
	}
    if(stat_startPlaying != 0&& totalSize!=0)
    {
    	globalStatistics->addStdDev("BasePlayer: Start playing time", stat_startPlaying);
    	globalStatistics->addStdDev("BasePlayer: Late Arrival Loss Distortion", (LV->getLateArivalLoss()/totalSize)*100);
    	//globalStatistics->addStdDev("BasePlayer: Availability-RateControl Loss Distortion", (LV->getAvailability_RateControlLoss()/totalSize)*100);
    	globalStatistics->addStdDev("BasePlayer: BER(Bit Error Rate) Loss Distortion", ((lossSize - LV->getAvailability_RateControlLoss()-LV->getLateArivalLoss()-dependencyLoss_Size)/totalSize)*100);
    }
    if(simTime() >= atof(ev.getConfig()->getConfigValue("sim-time-limit")) - 0.01 && !recorded)
	{
    	globalStatistics->addStdDev("BasePlayer: Total Participated Peers", nodeNumber);
    	globalStatistics->addStdDev("BasePlayer: Total Peers that played", nodePlayed);
    	globalStatistics->addStdDev("BasePlayer: Percentage of peers that played",(double)nodePlayed*100/ (double)nodeNumber );
    	recorded = true;
	}
    	int up= LV->getUpBandwidth()/1024;
    	if(up <=100)// it is free rider
    	{
    		globalStatistics->addStdDev("BasePlayer: Percentage played in free riders",isplayed);
    	}
    	if(up <=(videoAverageRate[LV->currentFilmId])&& up>100 )// it is low bandwidth
    	{
    		globalStatistics->addStdDev("BasePlayer: Percentage played in low up bandwidth",isplayed);
    	}
    	if(up <=(videoAverageRate[LV->currentFilmId]))// it is low bandwidth
    	{
    		globalStatistics->addStdDev("BasePlayer: Percentage played low-free up bandwidth",isplayed);
    	}
    	if(up >=(videoAverageRate[LV->currentFilmId])&&up <(2*videoAverageRate[LV->currentFilmId]) )// it is medium band width
    	{
    		globalStatistics->addStdDev("BasePlayer: Percentage played in medium up bandwidth",isplayed);
    	}
    	if(up >=(2*videoAverageRate[LV->currentFilmId]))// it is high bandwidth
    	{
    		globalStatistics->addStdDev("BasePlayer: Percentage played in high up bandwidth",isplayed);
    	}
}

int BasePlayer::nodeNumber = 0;
int BasePlayer::nodePlayed = 0;
int BasePlayer::recorded = false;

