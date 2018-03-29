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
 * @file VodApp.cc
 * @author Abdollah Ghaffari Sheshjavani(AGH)
 */


#include "VodApp.h"
#include <GlobalStatistics.h>
#include <GlobalNodeList.h>
#include <PeerInfo.h>
#include <LocalVariables.h>
#include "MeshMessage_m.h"

Define_Module(VodApp);

void VodApp::initializeApp(int stage)
{
//	cConfiguration *cfg = getConfig();
//	cSimulation::getActiveSimulation()->getEnvir()->getConfigEx()->getAsDouble();
//	std::cout << toDouble(ev.getConfig()->getConfigValue("sim-time-limit")) << std::endl;
//	std::cout << atof(ev.getConfig()->getConfigValue("sim-time-limit")) << std::endl;
	if (stage != MIN_STAGE_APP)
		return;
	//initialize parameters

	bufferMapExchangePeriod = par("bufferMapExchangePeriod");

    if(globalNodeList->getPeerInfo(thisNode.getAddress())->getTypeID() == 2)
    	isVideoServer = true;
    else
    	isVideoServer = false;
    //initialize global variables
	firstChunkCreation = 0;
    playbackPoint = -1;
    recivePoint=0;
    playingState = BUFFERING;
    endTimeWatchVideo=10;
    discontinuityStart=false;
    haveInteractive=false;
    interactiveTimeStart=0;
    seekTime=0;
    interactiveChunkStart=-1;
    seekChuncknumbers=0;
    bufferMapExchangeStart = false;
    schedulingSatisfaction = false;
    setStatisticsPosition=false;
    startUpBuffering = par ("startUpBuffering");
    limitedBuffer=par("limitedBuffer");
    windowOfIntrest = par("windowOfIntrest");
    adaptiveBufferMapXchange=par("adaptiveBufferMapXchange");
    FirstbufferExchangeWindowInterest=0;
    bufferExchangeWindowInterest=0;
    maxDownloadFurtherPlayPoint=par("maxDownloadFurtherPlayPoint");
    epsilon=par("epsilon");
    firstAdaptiveEpsilon=par("firstAdaptiveEpsilon");
    redundantEpsilon=par("redundantEpsilon");
    useRequestBigBufferMap=par("useRequestBigBufferMap");
    meshTestwithoutStreaming=par("meshTestwithoutStreaming");
    limitSeekForwardForTransiantState=par("limitSeekForwardForTransiantState");
    limitTimeFromSimTime=par("limitTimeFromSimTime");
    improvmentLineGradientTreshold=par("improvmentLineGradientTreshold");//AGH
    improvmentTradeoffParam = par("improvmentTradeoffParam");//AGH
    improvementMinTreshold=par("improvementMinTreshold");//AGH
    improvementScoreindex=0;
    useEmergencyConnection=par("useEmergencyConnection");//AGH
    emergencyMinVideoInBuffer=par("emergencyMinVideoInBuffer") ;//AGH
    emergencyVideolengthBuffer=par("emergencyVideolengthBuffer") ;//AGH
	deadLineFrame=par("deadLineFrame");
	retryframeReqTime=par("retryframeReqTime");
    retryReq=par("retryReq");
    retryReqChunksIfNeighborLeave=par("retryReqChunksIfNeighborLeave");
    checkNeighborsTimeOut=par("checkNeighborsTimeOut");
    discontinuityTimeParam=par("discontinuityTimeParam");
    if(!isVideoServer)
    	bc = check_and_cast<BaseVodStream*>(simulation.getModuleByPath("CDN-Server[1].tier2.mpeg4vodstream"));
    LV = check_and_cast<LocalVariables*>(getParentModule()->getSubmodule("localvariables"));
    LV->WatchExpired=false;
    LV->StructureImproving=false;
    LV->improveScore=-1000;
    LV->playbackpoint=-1;
    LV->needNewneighbor=false;
    LV->interactive=false;
    LV->seekTime=0;
    LV->hopCount=-1;
    LV->emergency=false;
    if(isVideoServer)
    	LV->hopCount=0;
    minHopCount=-1;
    LV->improveLock=false;
    LV->improveDeny=false;
    Fps = par("Fps");
    gopSize = par("gopSize");
    chunkSize = par ("chunkSize");
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
    // bufferSize[i] = windowOfIntrest*Fps[i]/chunkSize[i];
    // if(bufferSize[i]%chunkSize[i] > 0)
    //	bufferSize[i] += chunkSize[i] - (bufferSize[i]%chunkSize[i]);
   numOfBFrame = par("numOfBFrame");
    for (int i=0;i<10;i++)
	{
		bufferSize[i] =0;
		videolenght[i]=0;
		if (isVideoServer==true)
			LV->watchFilm[i]=true;
	}
    //LV->
    rateControl = par("rateControl");
    maxFrameRequestPerBufferMapExchangePeriod=par("maxFrameRequestPerBufferMapExchangePeriod");
    numberOfFrameRequested=0;
    measuringTime = par("measuringTime");
    receiverSideSchedulingNumber = par("receiverSideSchedulingNumber");
    senderSideSchedulingNumber = par("senderSideSchedulingNumber");
    averageChunkLength = par("averageChunkLength");
	//initialize self messages
    structureImprovementTimer=new cMessage("structureImprovementTimer");
    improvmentStructure=par("improvmentStructure");
    improvementPeriod=par("improvementPeriod");
    bufferMapTimer = new cMessage("bufferMapTimer");
	requestChunkTimer = new cMessage("requestChunkTimer");
	playingTimer = new cMessage("playingTimer");
	sendFrameTimer = new cMessage("sendFrameTimer");
	//statistics
	stat_startupDelay = 0;
	stat_startSendToPlayer = 0;
	stat_startBuffering = 0;
	stat_startBufferMapExchange = 0;
	stat_TotalReceivedSize = 0;
	stat_RedundentSize = 0;
	stat_totalBufferExchange=0;
	stat_lossBufferExchange=0;
	stat_lossBufferExchange2=0;
	stat_BandwidthForBufferExchange=0;
	stat_videoDownloadBigerThanBufferXchange=0;
	stat_BandwidthForBufferExchangePerSending=0;
	stat_RedunduntReceiveBufferExchange=0;//AGH
	stat_BandwidthSavedForNotSendingBuffermapWhenPeerNotHaveUsefullData=0;//AGH
	stat_BandwidthSavedForNotSendingBuffermapBiggerLastsendChunk=0;//AGH
	stat_BandwidthOverheadForsendingRedunduntInBuffermap=0;//AGH
	stat_BandwidthForFirstBufferExchange=0;//AGH
	stat_VideoFramesDownloadedButNotPlayedForInteractive=0;//AGH
	stat_VideoFramesDownloadedButNotPlayedForExit=0;//AGH
	stat_continuityShow=1;//AGH 0 is not contiguous
	stat_discontinuityTime=0;//AGH
	stat_NumberOfDiscontinuity=0;//AGH
	peerAllWatchingTime=0;
	stat_startSeekforward=0;
	stat_seekforwardDelay=0;//AGH
	stat_seekforwardNewJoinDelay=0;
	stat_jointoMeshStart=0;
	stat_recivefirstStreamPacket=0;
	stat_allFramePlayed=0;
	stat_lossFrame=0;
	stat_lossFrame2=0;
	stat_allRedundantRecivedBits=0;
	stat_usefullRedundantRecivedBits=0;
	stat_allExistInBufferRedundantRecivedbits=0;
	FreeRideUpload=0;
	stat_allRedundantRecivedMsg=0;
	//stat_peresentTimeInSystem=simTime().dbl();
	// setting server and clients display string
	if(isVideoServer)
	{
		ready=true;
		getParentModule()->getParentModule()->setDisplayString("i=device/server;i2=block/circle_s");
	}
	else
	{
		 ready=false;
		getParentModule()->getParentModule()->setDisplayString("i=device/wifilaptop_vs;i2=block/circle_s");
	}
}
void VodApp::handleTimerEvent(cMessage* msg)
{
	try
	{
	if(msg == sendFrameTimer)
	{
		selectSenderSideScheduling();
	}
	else if(msg == requestChunkTimer)
	{
		if(playingState != STOP)
			schedulingSatisfaction = true;
	}
	else if(msg == bufferMapTimer)
	{
		if(!isVideoServer)
		{
			if(haveInteractive && playingState==BUFFERING)
				LV->hopCount=-1;
			else if(minHopCount>0)
				LV->hopCount=minHopCount;
			minHopCount=-1;
			resetFreeBandwidth();//AGH
			numberOfFrameRequested=0;// for rate control-AGH
		}
		bufferMapExchange();
	}
	else if(msg == structureImprovementTimer )
	{
		if(LV->StructureImproving==false&& LV->improveLock==false)
		{
			int maxScore1=-1000;
			int maxScore2=-1000;
			int maxScore3=-1000;
			int bestIndex1=-1;
			int bestIndex2=-1;
			int bestIndex3=-1;
			for(unsigned int i=0;i< neighborsBufferMaps.size();i++)
			{
			//	int hop1=LV->hopCount;
				//int casc1=LV->cascadeNumber;
			//	int hop2=neighborsBufferMaps[i].hopCount;
			//	int casc2=neighborsBufferMaps[i].cascadeNumber;

				if(neighborsBufferMaps[i].hopCount< LV->hopCount && neighborsBufferMaps[i].hopCount > 0&& neighborsBufferMaps[i].cascadeNumber==LV->cascadeNumber)
				{
					int upBandwidthDifference=(LV->getUpBandwidth()/1024)-neighborsBufferMaps[i].totalBandwidth;
					int playbackDifference=(neighborsBufferMaps[i].playbackPoint-playbackPoint)/Fps;
					if(upBandwidthDifference>improvementMinTreshold)
					{
						double treshold=upBandwidthDifference*improvmentLineGradientTreshold;
						if(playbackDifference < treshold ||neighborsBufferMaps[i].totalBandwidth < 100|| (neighborsBufferMaps[i].totalBandwidth < videoAverageRate[LV->currentFilmId] && (LV->getUpBandwidth()/1024)>videoAverageRate[LV->currentFilmId]) )
						// if neighbor is free rider improvement is allowed(<100)-AGH
						{
							int score=(improvmentTradeoffParam*treshold)-(1-improvmentTradeoffParam)*playbackDifference;
							if(neighborsBufferMaps[i].totalBandwidth < 100)
								score=(improvmentTradeoffParam*treshold);
							if(LV->previousimproveNeighbor.isUnspecified())
								LV->previousimproveNeighbor=*globalNodeList->getRandomAliveNode(1);// only for error prone-AGH
							if(LV->ppreviousimproveNeighbor.isUnspecified())
								LV->ppreviousimproveNeighbor=*globalNodeList->getRandomAliveNode(1);// only for error prone-AGH
							if(score>maxScore3 && neighborsBufferMaps[i].tAddress!=LV->previousimproveNeighbor && neighborsBufferMaps[i].tAddress!=LV->ppreviousimproveNeighbor)
							{
								if(score>maxScore2)
								{
									if(score>maxScore1)
									{
										maxScore3=maxScore2;
										maxScore2=maxScore1;
										maxScore1=score;
										bestIndex3=bestIndex2;
										bestIndex2=bestIndex1;
										bestIndex1=i;
									}
									else
									{
										maxScore3=maxScore2;
										maxScore2=score;
										bestIndex3=bestIndex2;
										bestIndex2=i;
									}
								}
								else
								{
									maxScore3=score;
									bestIndex3=i;
								}
							}
						}
					}
				}
			}
			if(LV->improveDeny==false)
			{
				improvementScoreindex=0;
				if(bestIndex1!=-1)
				{
					LV->improveNeighbor=neighborsBufferMaps[bestIndex1].tAddress;
					LV->StructureImproving=true;
					LV->improveScore=maxScore1;
				}
			}
			else
			{
				LV->improveDeny=false;
				improvementScoreindex++;
				switch(improvementScoreindex)
				{
					case 1:
					{
						if(bestIndex2!=-1)
						{
							LV->improveNeighbor=neighborsBufferMaps[bestIndex2].tAddress;
							LV->StructureImproving=true;
							LV->improveScore=maxScore2;
						}
						else
						{
							improvementScoreindex=0;
							if(bestIndex1!=-1)
							{
								LV->improveNeighbor=neighborsBufferMaps[bestIndex1].tAddress;
								LV->StructureImproving=true;
								LV->improveScore=maxScore1;
							}
						}
						 break;
					}
					case 2:
					{
						if(bestIndex3!=-1)
						{
							LV->improveDeny=false;
							LV->improveNeighbor=neighborsBufferMaps[bestIndex3].tAddress;
							LV->StructureImproving=true;
							LV->improveScore=maxScore3;
						}
						else
						{
							improvementScoreindex=0;
							if(bestIndex1!=-1)
							{
								LV->improveNeighbor=neighborsBufferMaps[bestIndex1].tAddress;
								LV->StructureImproving=true;
								LV->improveScore=maxScore1;
							}
						}
						 break;
					}
					case 3:
					{
						improvementScoreindex=0;
						if(bestIndex1!=-1)
						{
							LV->improveNeighbor=neighborsBufferMaps[bestIndex1].tAddress;
							LV->StructureImproving=true;
							LV->improveScore=maxScore1;
						}
					}

				}
			}

		}

		scheduleAt(simTime()+improvementPeriod,structureImprovementTimer);
	}
	else if(msg == playingTimer)
	{
		if(playbackPoint>=interactiveTimeStart*Fps&& interactiveTimeStart>0)// interactiveTimeStart=0 means that peer has not interactive-AGH
		{
			interactiveTimeStart=-1;
			haveInteractive=true;
			seekTime= intuniform(1,videolenght[LV->currentFilmId]-endTimeWatchVideo,0);
			if(limitSeekForwardForTransiantState==true && LV->playbackpoint+seekTime+limitTimeFromSimTime>simTime().dbl())
			{
				if((simTime().dbl()- LV->playbackpoint)-limitTimeFromSimTime>1)
				seekTime=intuniform(1,simTime().dbl()- LV->playbackpoint-limitTimeFromSimTime);
				else
				seekTime=0;
			}
			if (seekTime<0)
				seekTime=0;
			interactiveChunkStart=recivePoint/chunkSize;// peer not played some chunks but it must send them to others-AGH
			endTimeWatchVideo+=seekTime;
			if((seekTime*Fps)/chunkSize+playbackPoint<recivePoint)
				seekChuncknumbers=0;
			else
				seekChuncknumbers=((seekTime*Fps)/chunkSize)-recivePoint+playbackPoint;
			if(recivePoint>playbackPoint)
				stat_VideoFramesDownloadedButNotPlayedForInteractive+=recivePoint-playbackPoint;
			playbackPoint+=seekTime*Fps;
			if(playbackPoint<recivePoint)
				stat_VideoFramesDownloadedButNotPlayedForInteractive-=recivePoint-playbackPoint;
			if(limitedBuffer)
			{
				 if((playbackPoint/chunkSize)> LV->videoBuffer[LV->currentFilmId]->chunkBuffer[bufferSize[LV->currentFilmId]-2].getChunkNumber())
				 {
					 while((playbackPoint/chunkSize) > LV->videoBuffer[LV->currentFilmId]->chunkBuffer[bufferSize[LV->currentFilmId]-2].getChunkNumber())
						 LV->videoBuffer[LV->currentFilmId]->shiftChunkBuf();
				 }
				LV->updateLocalBufferMap(LV->currentFilmId);
			}
			else
			{
				LV->videoBuffer[LV->currentFilmId]->setinteractive(interactiveChunkStart,seekChuncknumbers);
				LV->hostBufferMap[LV->currentFilmId]->setinteractive(interactiveChunkStart,seekChuncknumbers);
			}
				stat_seekforwardDelay=-1;
			if(playbackPoint+ startUpBuffering*Fps > recivePoint)
			{
				discontinuityStart=false;
				VideoMessage* playermessage = new VideoMessage("PlayerMessage");
				playermessage->setCommand(PLAYER_MSG);
				playermessage->setType(8);
				send(playermessage,"to_upperTier");
				playingState = BUFFERING;
				LV->hopCount=-1;
				minHopCount=-1;
				stat_startSeekforward=simTime().dbl();
				stat_seekforwardDelay=0;
			//	if(playbackPoint>recivePoint)
				{
					recivePoint=playbackPoint;
					bool needNewNeightbor=true;
					/*for (unsigned int i=0 ; i < neighborsBufferMaps.size();i++)
					{
						if(neighborsBufferMaps[i].videoBufferlastSetChunk>playbackPoint &&neighborsBufferMaps[i].videoBufferlastSetChunk<videolenght[LV->currentFilmId]*Fps[LV->currentFilmId] )// its specially in my structure is true but in simple mesh its not always true-AGH
						{
							needNewNeightbor=false;
						}
					}*/
					if(needNewNeightbor==true)
					{
						LV->improveLock=true;
						LV->playbackpoint=playbackPoint/Fps;// its in second
						LV->recieveTime=recivePoint/Fps;// its in second
						LV->interactive=true;
						LV->seekTime=seekTime;
						LV->needNewneighbor=true;
						MeshMessage* seekFWD=new MeshMessage("seekFWD");
						seekFWD->setCommand(SEEK_FWD);
						send(seekFWD,"to_lowerTier");
					}
				}
			}
			std::cout <<"time: " << simTime()<< "seek forward "<<getParentModule()->getParentModule()->getFullName() <<" "<<seekTime<<std::endl;
		}
		LV->playbackpoint=playbackPoint/Fps;// its in second
		LV->recieveTime=recivePoint/Fps;// its in second
		if(playbackPoint>=endTimeWatchVideo*Fps)
		{
			LV->WatchExpired=true;
			playingState = STOP;
			 schedulingSatisfaction = false;
			 cancelEvent(bufferMapTimer);
			 cancelEvent(requestChunkTimer);
			 cancelEvent(playingTimer);
			 cancelEvent(sendFrameTimer);
		}
		else
		{
			if(playingState == PLAYING)
			{
				sendFrameToPlayer();
			}
			else if (playingState == BUFFERING)
			{	// check if we have video equal to startup buffering
				checkForPlaying();
			}
			scheduleAt(simTime()+1/(double)Fps,playingTimer);
		}
	}
	else
		delete msg;
	}
	catch (std::exception& e)
	{
		std::cout << "time: " << simTime()<< " handleTimerEvent error in vod app "<<getParentModule()->getParentModule()->getFullName() <<  std::endl;
	}
}
void VodApp::handleLowerMessage(cMessage* msg)
{
 try
 {
	if (dynamic_cast<VideoProperty*>(msg) != NULL)
	{
		VideoProperty* VideoPropertyMsg=dynamic_cast<VideoProperty*>(msg);
		//initialize global variables
		playbackPoint = -1;
		playingState = BUFFERING;
		bufferMapExchangeStart = false;
		schedulingSatisfaction = false;
		LV->currentFilmId=VideoPropertyMsg->getSelectedFilms();
		ready=true;
		endTimeWatchVideo=VideoPropertyMsg->getEndTimeWatching();
		interactiveTimeStart=VideoPropertyMsg->getInteractiveTime();
		videolenght[LV->currentFilmId]=VideoPropertyMsg->getLenthOfFilmsSec();
		LV->playbackpoint=-1;
		if(adaptiveBufferMapXchange==true)
		{
			if(limitedBuffer==false)
			{
				/*
				 * bufferSize[LV->currentFilmId] = videolenght[LV->currentFilmId]*Fps[LV->currentFilmId]/chunkSize[LV->currentFilmId];
								if(bufferSize[LV->currentFilmId]%chunkSize[LV->currentFilmId] > 0)
									bufferSize[LV->currentFilmId] += chunkSize[LV->currentFilmId] - (bufferSize[LV->currentFilmId]%chunkSize[LV->currentFilmId]);
						*/
				bufferSize[LV->currentFilmId] = (endTimeWatchVideo+maxDownloadFurtherPlayPoint+1)*Fps/chunkSize;
				if(interactiveTimeStart>0)// have interactive so it can download max chunks but not played (because interactive)-but we must use these chunks for other-AGH
				{
					bufferSize[LV->currentFilmId] += (maxDownloadFurtherPlayPoint*Fps)/chunkSize;
				}
				if(bufferSize[LV->currentFilmId]%chunkSize > 0)
					bufferSize[LV->currentFilmId] += chunkSize - (bufferSize[LV->currentFilmId]%chunkSize);
			}
			else if(limitedBuffer==true)
			{
				bufferSize[LV->currentFilmId] = windowOfIntrest*Fps/chunkSize;
				if(bufferSize[LV->currentFilmId]%chunkSize > 0)
					bufferSize[LV->currentFilmId] += chunkSize - (bufferSize[LV->currentFilmId]%chunkSize);
			}
		}
		else
		{
			bufferSize[LV->currentFilmId] = windowOfIntrest*Fps/chunkSize;
			if(bufferSize[LV->currentFilmId]%chunkSize > 0)
					bufferSize[LV->currentFilmId] += chunkSize - (bufferSize[LV->currentFilmId]%chunkSize);
			limitedBuffer=true;// when adaptive buffer not used we cannot use unlimited buffer
		}
		LV->videoBuffer[LV->currentFilmId] = new VideoBuffer(numOfBFrame,bufferSize[LV->currentFilmId],chunkSize,gopSize);
		LV->hostBufferMap[LV->currentFilmId] = new BufferMap();
		LV->hostBufferMap[LV->currentFilmId]->setValues(bufferSize[LV->currentFilmId]);
		LV->videoBuffer[LV->currentFilmId]->updateBufferMap(LV->hostBufferMap[LV->currentFilmId]);
		LV->watchFilm[LV->currentFilmId]=true;
		delete VideoPropertyMsg;
		stat_jointoMeshStart=simTime().dbl();
		//    start timeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee//eeeeeeeeeee//eeeeeeee//
	}
	else if (dynamic_cast<CreateStream*>(msg) != NULL)
	{
		CreateStream* CreateStreamMsg=dynamic_cast<CreateStream*>(msg);
		int filmid=CreateStreamMsg->getSelectedFilm();
		if(isVideoServer)
		{
			// this only on video Server -AGH
			bufferMapExchangeStart=true;
			//BufferMap BM;
			//BM.setValues(bufferSize[CreateStreamMsg->getSelectedFilm()]);
			//LV->videoBuffer[CreateStreamMsg->getSelectedFilm()]->updateBufferMap(&BM);
			BufferMapMessage *BufferMapmsg = new BufferMapMessage("BufferMap_exchange");
			BufferMapmsg->setFilmId(CreateStreamMsg->getSelectedFilm());
			//	BufferMapmsg->setBuffermap(BM);
			BufferMapmsg->setIsServer(true);
			BufferMapmsg->setWhatchInPast(false);
			BufferMapmsg->setUnicast(true);
			//BufferMapmsg->setBitLength(BUFFERMAPMESSAGE_L(msg)+LV->hostBufferMap[CreateStreamMsg->getSelectedFilm()]->getBitLength());
			BufferMapmsg->setBitLength(BUFFERMAPMESSAGE_L(msg));
			BufferMapmsg->setDstNode(CreateStreamMsg->getSrcNode());
			BufferMapmsg->setSrcNode(thisNode);
			send (BufferMapmsg,"to_lowerTier");
		}
		else if(LV->watchFilm[filmid]==true)
		{
			BufferMap BM;
			BufferMapMessage *BufferMapmsg = new BufferMapMessage("BufferMap_exchange");
			BufferMapmsg->setFirstBufferMap(true);
			BufferMapmsg->setTimestamp(simTime());
			BufferMapmsg->setFilmId(LV->currentFilmId);
			BufferMapmsg->setIsServer(false);
			BufferMapmsg->setDstNode(CreateStreamMsg->getSrcNode());
			BufferMapmsg->setSrcNode(thisNode);
			BufferMapmsg->setHopCount(LV->cascadeNumber);//we used hop count for cascade in first buffer map-AGH
			BufferMapmsg->setPlaybackPoint(playbackPoint);
			if(LV->currentFilmId!=filmid)
			{
				BufferMapmsg->setWhatchInPast(true);
			}
			else
			{
				BufferMapmsg->setWhatchInPast(false);
			}
			if(adaptiveBufferMapXchange==true)
			{

				FirstbufferExchangeWindowInterest=(2*bufferMapExchangePeriod+2*CreateStreamMsg->getDelay())*(CreateStreamMsg->getDownBandwith()/videoAverageRate[filmid])+firstAdaptiveEpsilon;
				BM.setValues(FirstbufferExchangeWindowInterest * Fps/chunkSize);
				LV->videoBuffer[filmid]->updateBufferMapXchange(&BM,CreateStreamMsg->getSelectedStartTimeSec()* Fps/chunkSize,FirstbufferExchangeWindowInterest* Fps/chunkSize,limitedBuffer,0);
				BufferMapmsg->setReceivePoint(recivePoint);
				BufferMapmsg->setMyVideoBufferLastSetChunk(LV->videoBuffer[filmid]->lastSetChunk);
				BufferMapmsg->setLastBufferPoint(0);
				BufferMapmsg->setUnicast(true);
				BufferMapmsg->setBitLength(BUFFERMAPMESSAGE_L(msg)+BM.getBitLength());
				globalStatistics->addStdDev("VodApp:Msg Size For First BufferExchange",BufferMapmsg->getBitLength());
				stat_BandwidthForFirstBufferExchange+=BufferMapmsg->getBitLength();
				stat_BandwidthForBufferExchange+=BufferMapmsg->getBitLength();
			}
			else
			{
				BM.setValues(bufferSize[LV->currentFilmId]);
				BufferMapmsg->setBitLength(BUFFERMAPMESSAGE_L(msg)-69+LV->hostBufferMap[LV->currentFilmId]->getBitLength());
				LV->videoBuffer[filmid]->updateBufferMap(&BM);// its not adaptive
				BufferMapmsg->setUnicast(true);
				stat_BandwidthForBufferExchange+=BufferMapmsg->getBitLength();
			}
			BufferMapmsg->setBuffermap(BM);
			send (BufferMapmsg,"to_lowerTier");
		}
		delete CreateStreamMsg;
	}
	else if (dynamic_cast<VideoMessage*>(msg) != NULL)
	{
		VideoMessage* VideoMsg=dynamic_cast<VideoMessage*>(msg);
		if(checkNeighborsTimeOut)
		{
			for (int i=0; i < LV->neighbors.size(); i++)
			{
				if(LV->neighbors[i].address==VideoMsg->getSrcNode())
				{
					LV->neighbors[i].lastnotifyed=simTime().dbl();
					break;
				}
			}
		}
		if(VideoMsg->getCommand() == CHUNK_REQ && !LV->peerExit)
		{
			requesterNode rN;
			rN.tAddress = VideoMsg->getSrcNode();
			rN.chunkNo = VideoMsg->getChunk().getChunkNumber();
			rN.filmId=VideoMsg->getChunk().getFilmNumber();
			//ev << "Here, I put the request into my sending queue." << endl;
			senderqueue.insert(std::make_pair<double,requesterNode>(VideoMsg->getDeadLine(),rN));
			//ev << "And I call sendersidescheduling here." << endl;
			selectSenderSideScheduling();
			delete msg;
		}
		else if(VideoMsg->getCommand() == CHUNK_RSP && !isVideoServer)
		{
			if(stat_recivefirstStreamPacket==0)
				stat_recivefirstStreamPacket=simTime().dbl();
			bool redundantState = false;
			if(VideoMsg->getChunk().isComplete() && !VideoMsg->hasBitError())
			{
				Chunk InputChunk = VideoMsg->getChunk();
				if(VideoMsg->getChunk().getChunkNumber() >= LV->videoBuffer[InputChunk.getFilmNumber()]->chunkBuffer[0].getChunkNumber())
				{
						if(LV->videoBuffer[InputChunk.getFilmNumber()]->getChunk(VideoMsg->getChunk().getChunkNumber(),limitedBuffer).isComplete())
					{
						stat_RedundentSize += VideoMsg->getChunk().getChunkByteLength();
						redundantState = true;
					}
					stat_TotalReceivedSize += VideoMsg->getChunk().getChunkByteLength();
					InputChunk.setHopCout(InputChunk.getHopCount()+1);
					if((minHopCount==-1||minHopCount>InputChunk.getHopCount()) && (VideoMsg->getSelectedFilms()==LV->currentFilmId||InputChunk.getHopCount()==1))
					{
						if(isInVector(VideoMsg->getSrcNode(),LV->neighbors))// it may this its previous neighbor-AGH
							minHopCount=InputChunk.getHopCount();
					}
					LV->videoBuffer[InputChunk.getFilmNumber()]->setChunk(InputChunk,limitedBuffer);
					LV->updateLocalBufferMap(InputChunk.getFilmNumber());
					// its for test system - AGH
					if(LV->currentFilmId!=InputChunk.getFilmNumber())
					{
						//must add one parameter
						//error("received error chunk on film number");
					}
					if(isMeasuring(VideoMsg->getChunk().getLastFrameNo()))
					{
						//globalStatistics->addStdDev("VodApp: end to end delay", simTime().dbl() - );
						globalStatistics->addStdDev("VodApp: Hop Count", VideoMsg->getChunk().getHopCount());
						//ev << "The last chunk is reached." << endl;
						//ev << "The end to end delay is " << LV->firstChunkCreation << endl;
						firstRequest = false;
						if(VideoMsg->getChunk().getLastFrameNo() < playbackPoint)
							LV->addToLateArivalLoss(VideoMsg->getChunk().getLateArrivalLossSize(playbackPoint));
					}
					deleteElement(VideoMsg->getChunk().getChunkNumber(),sendFrames);
				}
			}
			delete msg;
		}
		else if(VideoMsg->getCommand() == NEIGHBOR_LEAVE)
		{
			cancelEvent(bufferMapTimer);
			deleteNeighbor(VideoMsg->getSrcNode());
			// frame requested erase for requested again-AGH
			if(retryReqChunksIfNeighborLeave && VideoMsg->getKind()==7 )
			{
				for (unsigned int i=0; i!=sendFrames.size(); i++)
				{
					if (sendFrames[i].destNode == VideoMsg->getSrcNode()&& (sendFrames[i].frameNum-playbackPoint>deadLineFrame*Fps||playingState==BUFFERING))
					{
						sendFrames.erase(sendFrames.begin()+i,sendFrames.begin()+1+i);
						numberOfFrameRequested--;// when retry we must notice to rate control-AGH
						i--;
					}
				}
			}
			scheduleAt(simTime(),bufferMapTimer);
			delete msg;
		}
		else if(VideoMsg->getCommand() == LEAVING)
		{
			cancelEvent(bufferMapTimer);
			cancelEvent(requestChunkTimer);
			cancelEvent(playingTimer);
			cancelEvent(sendFrameTimer);
			cancelEvent(structureImprovementTimer);
			if(VideoMsg->getType()==0)
			{
				//stat_peresentTimeInSystem=simTime().dbl()-stat_peresentTimeInSystem;
				//statistics
				if(playingState==BUFFERING && haveInteractive==true && stat_seekforwardDelay==0 )
				{
					stat_seekforwardDelay=simTime().dbl()-stat_startSeekforward;
				}
				setStatistics();
				setStatisticsPosition=true;
			}
			// re new
			ready=false;
			///neighborsBufferMaps.erase(neighborsBufferMaps.begin(),neighborsBufferMaps.end());
			playingState = STOP;
			schedulingSatisfaction = false;
			bufferMapExchangeStart = false;
			sendFrames.clear();
			if(VideoMsg->getType()==1)
			{
				//its change film
				/// set property of pre film
				// average per video
				//stat_peresentTimeInSystem=simTime().dbl()-stat_startBufferMapExchange;
				setStatistics();							//statistics
				LV->WatchExpired=false;
				LV->playbackpoint=-1;
				LV->emergency=false;
				haveInteractive=false;
				recivePoint=0;
				playbackPoint = -1;
				endTimeWatchVideo=10;
				seekTime= 0;
				interactiveChunkStart=-1;
				seekChuncknumbers=0;
				stat_startupDelay = 0;
				stat_startSendToPlayer = 0;
				stat_startBuffering = 0;
				stat_startBufferMapExchange = 0;
				stat_TotalReceivedSize = 0;
				stat_RedundentSize = 0;
				stat_totalBufferExchange=0;
				stat_lossBufferExchange=0;
				stat_lossBufferExchange2=0;
				stat_BandwidthForBufferExchange=0;
				stat_videoDownloadBigerThanBufferXchange=0;
				stat_BandwidthForBufferExchangePerSending=0;
				stat_RedunduntReceiveBufferExchange=0;//AGH
				stat_BandwidthForFirstBufferExchange=0;//AGH
				stat_BandwidthSavedForNotSendingBuffermapWhenPeerNotHaveUsefullData=0;//AGH
				stat_BandwidthSavedForNotSendingBuffermapBiggerLastsendChunk=0;//AGH
				stat_BandwidthOverheadForsendingRedunduntInBuffermap=0;//AGH
				stat_VideoFramesDownloadedButNotPlayedForExit=0;
				stat_VideoFramesDownloadedButNotPlayedForInteractive=0;
				//	stat_peresentTimeInSystem=simTime().dbl();
				stat_continuityShow=1;//AGH 0 is not contiguous
				stat_discontinuityTime=0;//AGH
				stat_NumberOfDiscontinuity=0;//AGH
				peerAllWatchingTime=0;
				stat_startSeekforward=0;
				stat_seekforwardDelay=0;//AGH
				stat_seekforwardNewJoinDelay=0;
				stat_jointoMeshStart=0;
				stat_recivefirstStreamPacket=0;
				stat_allFramePlayed=0;
				stat_lossFrame=0;
				stat_lossFrame2=0;
				stat_allRedundantRecivedBits=0;
				stat_usefullRedundantRecivedBits=0;
				stat_allExistInBufferRedundantRecivedbits=0;
				stat_allRedundantRecivedMsg=0;
			}
			delete msg;
		}
		else
			delete VideoMsg;
	}
	else if(dynamic_cast <BufferMapMessage*> (msg) != NULL && ready==true)
	{
		BufferMapMessage* BufferMap_Recieved=dynamic_cast<BufferMapMessage*>(msg);
		if(checkNeighborsTimeOut)
		 {
			  for (int i=0; i < LV->neighbors.size(); i++)
			  {
				  if(LV->neighbors[i].address==BufferMap_Recieved->getSrcNode())
				  {
					  LV->neighbors[i].lastnotifyed=simTime().dbl();
					  break;
				  }
			  }
		 }
		if(BufferMap_Recieved->getFilmId()!=-2)
		{
			if(!bufferMapExchangeStart && !isVideoServer )
			{
				//scheduleAt(simTime(),requestChunkTimer);
				if(playingState != STOP)
				{
					schedulingSatisfaction = true;
				}
				cancelEvent(playingTimer);
				cancelEvent(bufferMapTimer);
				cancelEvent(structureImprovementTimer);
				scheduleAt(simTime(),playingTimer);
				scheduleAt(simTime()+bufferMapExchangePeriod,bufferMapTimer);
				if(improvmentStructure)
					scheduleAt(simTime()+1,structureImprovementTimer);
				stat_startBufferMapExchange = simTime().dbl() + bufferMapExchangePeriod;
				stat_startBuffering = simTime().dbl();
				bufferMapExchangeStart = true;
				if(limitedBuffer)
				{

				}
			}
				notInNeighbors.clear();
				updateNeighborBMList(BufferMap_Recieved);
		}
		if(schedulingSatisfaction && !isVideoServer)
			selectRecieverSideScheduling();
		delete msg;

	}
	else
		delete msg;

	}
	catch (std::exception& e)
	{
		std::cout << "time: " << simTime()<< " lower msg error in vod app "<<getParentModule()->getParentModule()->getFullName() <<  std::endl;
	}
}
void VodApp::handleUpperMessage(cMessage* msg)
{
	try
	{
	if (dynamic_cast<SelectFilm*>(msg) != NULL)
	{
		SelectFilm* SelectVideoMsg=dynamic_cast<SelectFilm*>(msg);
		if (SelectVideoMsg->getType()== 0)
		{
			for(int i=0;i<SelectVideoMsg->getNumberOfFilms();i++)
			{
				bufferSize[i]=SelectVideoMsg->getFrameOfFilms(i)/chunkSize;
				if(bufferSize[i]%chunkSize > 0)
					bufferSize[i] += chunkSize - (bufferSize[i]%chunkSize);
			}
			send (SelectVideoMsg,"to_lowerTier");
			scheduleAt(simTime()+bufferMapExchangePeriod,bufferMapTimer);
		}
		else if(SelectVideoMsg->getType()== 1)
		{
			delete SelectVideoMsg;
		}
	}
	else
		delete msg;
	}
	catch (std::exception& e)
	{
		std::cout << "time: " << simTime()<< " upper msg error in vod app "<<getParentModule()->getParentModule()->getFullName() <<  std::endl;
	}
}
void VodApp::updateNeighborBMList(BufferMapMessage* BufferMap_Recieved)
{

	try
	 {
		if(ready==true)
		{
			if(BufferMap_Recieved->getIsServer()==true)
			{
				 bool find = false;
				 for(unsigned int i=0 ; i<neighborsBufferMaps.size();i++)
					if(neighborsBufferMaps[i].tAddress == BufferMap_Recieved->getSrcNode())
					{
						//  this for only run 1 time because server set film id =-2
						find = true;
						break;
					}
				if(!find && isInVector( BufferMap_Recieved->getSrcNode(),LV->neighbors))
				{
					nodeBufferMap neighbourBM;
					neighbourBM.isServer=true;
					neighbourBM.buffermap.setValues(0);
					neighbourBM.tAddress = BufferMap_Recieved->getSrcNode();
					neighbourBM.totalBandwidth =  BufferMap_Recieved->getTotalBandwidth();
					neighbourBM.watchInPast=false;
					neighbourBM.requestCounter = 0;
					neighbourBM.videoId=LV->currentFilmId;
					neighborsBufferMaps.push_back(neighbourBM);
				}
			}
			else
			{
				if(adaptiveBufferMapXchange==true)
				{
					bool find = false;
					for(unsigned int i=0 ; i<neighborsBufferMaps.size();i++)
						if(neighborsBufferMaps[i].tAddress == BufferMap_Recieved->getSrcNode())
						{
							if(BufferMap_Recieved->getFilmId()==-7)//-7 is a AGREEMENT -AGH
							{
								deleteNeighbor(BufferMap_Recieved->getSrcNode());
								return;
							}

						neighborsBufferMaps[i].requestBigbufferMap=false;
						int recieve_redundantsize=BufferMap_Recieved->getRedundantsize();
						stat_allRedundantRecivedBits+=recieve_redundantsize;
						if(recieve_redundantsize>0)
							stat_allRedundantRecivedMsg++;
						if(BufferMap_Recieved->getMyVideoBufferLastSetChunk()>recivePoint)
						{
							stat_totalBufferExchange++;
							int pointer=0;
							pointer=recivePoint-neighborsBufferMaps[i].buffermap.getChunkNumber(0);
							if(pointer==-1)
								pointer=0;
							if(pointer>neighborsBufferMaps[i].buffermap.getBufferSize())
							{
								neighborsBufferMaps[i].requestBigbufferMap=true;
								stat_lossBufferExchange++;
								//	int pointer2=recivePoint-BufferMap_Recieved->getBuffermap().getChunkNumber(0);
								//int startfor=pointer-neighborsBufferMaps[i].buffermap.getBufferSize();
								if(recivePoint<BufferMap_Recieved->getBuffermap().getChunkNumber(0))//+recieve_redundantsize))
								{
								//	int test=BufferMap_Recieved->getBuffermap().getChunkNumber(0);
									neighborsBufferMaps[i].buffermap.setLastSetChunk(-8);//  if run is an error- but it fix error-AGH
								}
								else if(recivePoint<BufferMap_Recieved->getBuffermap().getLastSetChunk())
								{
									if(recieve_redundantsize>0 && recivePoint<BufferMap_Recieved->getBuffermap().getChunkNumber(recieve_redundantsize))
									{
										stat_usefullRedundantRecivedBits+=BufferMap_Recieved->getBuffermap().getChunkNumber(recieve_redundantsize)-recivePoint;
									}
									int maxindex=minimum(BufferMap_Recieved->getBuffermap().getBufferSize(),neighborsBufferMaps[i].buffermap.getBufferSize());
									for(int q=0;q<maxindex;q++)
									{
										neighborsBufferMaps[i].buffermap.setChunk(q,BufferMap_Recieved->getBuffermap().getChunkNumber(q),BufferMap_Recieved->getBuffermap().getChunk(q));
									}
									for(int q=BufferMap_Recieved->getBuffermap().getBufferSize();q<neighborsBufferMaps[i].buffermap.getBufferSize();q++)
									{
										neighborsBufferMaps[i].buffermap.setChunk(q,neighborsBufferMaps[i].buffermap.getChunkNumber(0)+q,false);
									}
									neighborsBufferMaps[i].buffermap.setLastSetChunk(BufferMap_Recieved->getBuffermap().getLastSetChunk());
								}
								else
								{
									for(int q=0;q<neighborsBufferMaps[i].buffermap.getBufferSize();q++)
									{
										neighborsBufferMaps[i].buffermap.setChunk(q,recivePoint+q,false);
									}
									neighborsBufferMaps[i].buffermap.setLastSetChunk(-8);
								}
							}
							else if(pointer>=0)
							{
								if(recivePoint>BufferMap_Recieved->getBuffermap().getLastSetChunk())
								{
									neighborsBufferMaps[i].requestBigbufferMap=true;
									stat_lossBufferExchange2++;
								}
								else if(pointer>BufferMap_Recieved->getBuffermap().getBufferSize()-recieve_redundantsize)
								{
									stat_videoDownloadBigerThanBufferXchange++;
									if(BufferMap_Recieved->getMyVideoBufferLastSetChunk()> BufferMap_Recieved->getBuffermap().getLastSetChunk())
									{
										if(recivePoint>BufferMap_Recieved->getBuffermap().getChunk(0)+recieve_redundantsize)
											neighborsBufferMaps[i].requestBigbufferMap=true;
									}
								}
								if(pointer!=0)
								{
									for(int j=0;j<neighborsBufferMaps[i].buffermap.getBufferSize()-pointer;j++)
									{
										neighborsBufferMaps[i].buffermap.setChunk(j,neighborsBufferMaps[i].buffermap.getChunkNumber(pointer+j),neighborsBufferMaps[i].buffermap.getChunk(pointer+j));
									}
								}
								int index=BufferMap_Recieved->getBuffermap().getChunkNumber(0)-neighborsBufferMaps[i].buffermap.getChunkNumber(0);
								int start=0;
								if(index <0 )
									start=-index;
								bool setLastSetChunk=false;
								for(int k=start;k<BufferMap_Recieved->getBuffermap().getBufferSize();k++)
								{
									if(index+k>=neighborsBufferMaps[i].buffermap.getBufferSize())
									{
										neighborsBufferMaps[i].buffermap.setLastSetChunk(BufferMap_Recieved->getBuffermap().getChunk(k-1));
										stat_RedunduntReceiveBufferExchange+=BufferMap_Recieved->getBuffermap().getLastSetChunk()-neighborsBufferMaps[i].buffermap.getLastSetChunk();
										setLastSetChunk=true;
										break;
									}
									//only for evaluating redundant recived buffer//AGH
									if(k<recieve_redundantsize)
									{
										stat_allExistInBufferRedundantRecivedbits++;
										if(BufferMap_Recieved->getBuffermap().getChunk(k)&& !neighborsBufferMaps[i].buffermap.getChunk(index+k))
											stat_usefullRedundantRecivedBits++;
									}
									// end evaluating redundant ... //AGH
									neighborsBufferMaps[i].buffermap.setChunk(index+k,BufferMap_Recieved->getBuffermap().getChunkNumber(k),BufferMap_Recieved->getBuffermap().getChunk(k));
								}
								if(!setLastSetChunk)
									neighborsBufferMaps[i].buffermap.setLastSetChunk(BufferMap_Recieved->getBuffermap().getLastSetChunk());
							}
						}
						else
						{
							neighborsBufferMaps[i].buffermap.setLastSetChunk(-8);
						}
						if(BufferMap_Recieved->getFirstBufferMap()==false)
							neighborsBufferMaps[i].hopCount=BufferMap_Recieved->getHopCount();
						if(BufferMap_Recieved->getLastBufferPoint()>=0)
							neighborsBufferMaps[i].bufferPoint=BufferMap_Recieved->getLastBufferPoint();
			//			else
				//			neighborsBufferMaps[i].bufferPoint=0;
						neighborsBufferMaps[i].recivePoint=BufferMap_Recieved->getReceivePoint();
						neighborsBufferMaps[i].playbackPoint=BufferMap_Recieved->getPlaybackPoint();
						neighborsBufferMaps[i].videoBufferlastSetChunk=BufferMap_Recieved->getMyVideoBufferLastSetChunk();
						neighborsBufferMaps[i].totalBandwidth = BufferMap_Recieved->getTotalBandwidth();
						neighborsBufferMaps[i].pleaseSendToMEBigbufferMap=BufferMap_Recieved->getPleaseSendBigbufferMap();
						find = true;
						break;
					}
					if(!find && isInVector( BufferMap_Recieved->getSrcNode(),LV->neighbors))
					{
						if(BufferMap_Recieved->getFirstBufferMap()==true)
						{
							nodeBufferMap neighbourBM;
							neighbourBM.videoId=BufferMap_Recieved->getFilmId();
							neighbourBM.recivePoint=BufferMap_Recieved->getReceivePoint();
							neighbourBM.playbackPoint=BufferMap_Recieved->getPlaybackPoint();
							neighbourBM.bufferPoint=0; //BufferMap_Recieved->getLastBufferPoint();
							neighbourBM.videoBufferlastSetChunk=BufferMap_Recieved->getMyVideoBufferLastSetChunk();
							neighbourBM.cascadeNumber=BufferMap_Recieved->getHopCount();//we used hop count for cascade in first buffer map-AGH
							neighbourBM.redundantSize=0;
							double temp=simTime().dbl();
							neighbourBM.delay=(temp-BufferMap_Recieved->getTimestamp().dbl());
							neighbourBM.isServer=false;
							neighbourBM.requestBigbufferMap=false;
							neighbourBM.buffermap.setValues(BufferMap_Recieved->getBuffermap().getBufferSize());
							if(BufferMap_Recieved->getBuffermap().getBufferSize()<2 ||(BufferMap_Recieved->getBuffermap().getBufferSize()>2 &&neighbourBM.videoId!=LV->currentFilmId))
							{
							// this node change film and receive previous buffer maps
								std::cout << "ERRoR!--"<<" pre buffer map received in "<<getParentModule()->getParentModule()->getFullName() <<"from"<<BufferMap_Recieved->getSrcNode() <<  std::endl;
								BufferMapMessage *BufferMapmsg = new BufferMapMessage("BufferMap_exchange");
								BufferMapmsg->setFilmId(-7);// for disconnect to this-AGH
								BufferMapmsg->setIsServer(false);
								BufferMapmsg->setUnicast(true);
								BufferMapmsg->setDstNode(BufferMap_Recieved->getSrcNode());
								BufferMapmsg->setBitLength(BUFFERMAPMESSAGE_L(msg));
								BufferMapmsg->setSrcNode(thisNode);
								send (BufferMapmsg,"to_lowerTier");
								return;
							}
							neighbourBM.tAddress = BufferMap_Recieved->getSrcNode();
							neighbourBM.totalBandwidth =  BufferMap_Recieved->getTotalBandwidth();
							neighbourBM.downBandwidth=BufferMap_Recieved->getDownBandwith();
							neighbourBM.watchInPast = BufferMap_Recieved->getWhatchInPast();
							neighbourBM.requestCounter = 0;
							neighbourBM.buffermap = BufferMap_Recieved->getBuffermap();
							neighbourBM.hopCount=-1;
							neighborsBufferMaps.push_back(neighbourBM);
						}
						else
						{
							std::cout << "!--"<<" pre buffer map received in "<<getParentModule()->getParentModule()->getFullName() <<"from"<<BufferMap_Recieved->getSrcNode() <<  std::endl;
						}
					}
				}
				else if(adaptiveBufferMapXchange!=true)
				{
					if(isVideoServer)
						return;// check for deleting message
					bool find = false;
					for(unsigned int i=0 ; i<neighborsBufferMaps.size();i++)
						if(neighborsBufferMaps[i].tAddress == BufferMap_Recieved->getSrcNode())
						{
							if(BufferMap_Recieved->getFilmId()==-7)//-7 is a AGREEMENT -AGH
							{
								deleteNeighbor(BufferMap_Recieved->getSrcNode());
								return;
							}
							neighborsBufferMaps[i].playbackPoint=BufferMap_Recieved->getPlaybackPoint();
							neighborsBufferMaps[i].buffermap = BufferMap_Recieved->getBuffermap();
							neighborsBufferMaps[i].totalBandwidth = BufferMap_Recieved->getTotalBandwidth();
							neighborsBufferMaps[i].hopCount=BufferMap_Recieved->getHopCount();
							find = true;
							break;
						}
					if(!find)
					{

							if(BufferMap_Recieved->getFilmId()!=LV->currentFilmId)
							{
							// this node change film and receive previous buffer maps
								std::cout << "ERRoR!--"<<" pre buffer map received in "<<getParentModule()->getParentModule()->getFullName() <<"from"<<BufferMap_Recieved->getSrcNode() <<  std::endl;
								BufferMapMessage *BufferMapmsg = new BufferMapMessage("BufferMap_exchange");
								BufferMapmsg->setFilmId(-7);// for disconnect to this-AGH
								BufferMapmsg->setIsServer(false);
								BufferMapmsg->setUnicast(true);
								BufferMapmsg->setDstNode(BufferMap_Recieved->getSrcNode());
								BufferMapmsg->setBitLength(BUFFERMAPMESSAGE_L(msg));
								BufferMapmsg->setSrcNode(thisNode);
								send (BufferMapmsg,"to_lowerTier");
								return;
							}
							nodeBufferMap neighbourBM;
							neighbourBM.videoId=BufferMap_Recieved->getFilmId();
							neighbourBM.buffermap.setValues(BufferMap_Recieved->getBuffermap().getBufferSize());
							neighbourBM.tAddress = BufferMap_Recieved->getSrcNode();
							neighbourBM.playbackPoint=BufferMap_Recieved->getPlaybackPoint();
							neighbourBM.totalBandwidth =  BufferMap_Recieved->getTotalBandwidth();
							neighbourBM.requestCounter = 0;
							neighbourBM.watchInPast = BufferMap_Recieved->getWhatchInPast();
							neighbourBM.buffermap = BufferMap_Recieved->getBuffermap();
							neighbourBM.cascadeNumber=BufferMap_Recieved->getHopCount();//we used hop count for cascade in first buffer map-AGH
							neighbourBM.isServer=false;
							neighbourBM.hopCount=-1;
							neighborsBufferMaps.push_back(neighbourBM);


					}

				 }
			}
		}
	 }
	 catch (std::exception& e)
	 {
		 std::cout << "time: " << simTime()<< " updateNeighborBMList error in vod app "<<getParentModule()->getParentModule()->getFullName() <<  std::endl;
	 }
}
void VodApp::deleteElement(int frameNum, std::vector <requestedFrames>& sendframes)
{
	try
	{
		for (unsigned int i=0; i!=sendframes.size(); i++)
		{
			if (sendframes[i].frameNum == frameNum)
			{
				double frameDelay=0;
				frameDelay=(simTime().dbl()-sendframes[i].requestTime);
				globalStatistics->addStdDev("VodApp:Average of frame Req to frame Res end to end delay ",frameDelay);
				//globalStatistics->recordOutVector("VodApp:Average of frame Req to frame Res end to end delay Vector",frameDelay);
				sendframes.erase(sendframes.begin()+i,sendframes.begin()+1+i);
				break;
			}
		}
	}
	catch(std::exception& e)
	{
		std::cout << "time: " << simTime()<< " deleteElement error in vod app "<<getParentModule()->getParentModule()->getFullName() <<  std::endl;
	}
}
void VodApp::checkForPlaying()
{
	try
	{
	int bitCounter = 0;
	int bufferFirstChunkNumber=LV->hostBufferMap[LV->currentFilmId]->chunkNumbers[0];
	if(haveInteractive==true && !limitedBuffer)
	{
		if(discontinuityStart==true)
		{
			for(int i=0;i<(Fps/chunkSize);i++)
				if(interactiveChunkStart>bufferFirstChunkNumber)
				{
					if(LV->hostBufferMap[LV->currentFilmId]->buffermap[i + (playbackPoint/chunkSize)-seekChuncknumbers-bufferFirstChunkNumber])
						bitCounter++;
				}
				else
				{
					if(LV->hostBufferMap[LV->currentFilmId]->buffermap[i + (playbackPoint/chunkSize)-bufferFirstChunkNumber])
						bitCounter++;
				}

		}
		else
		{
				if(interactiveChunkStart>bufferFirstChunkNumber)
				{
				int limitStart=LV->hostBufferMap[LV->currentFilmId]->chunkNumbers[interactiveChunkStart+1-bufferFirstChunkNumber];
					for(int i=limitStart; i< LV->hostBufferMap[LV->currentFilmId]->getLastSetChunk() + 1 ; i++)
						if(LV->hostBufferMap[LV->currentFilmId]->buffermap[interactiveChunkStart+1-bufferFirstChunkNumber+i-limitStart])
						{
							bitCounter++;
							if(stat_seekforwardNewJoinDelay==0)
							{
								stat_seekforwardNewJoinDelay=simTime().dbl()-stat_startSeekforward;// delay time for connected to new neighbors
								globalStatistics->recordOutVector(" VodApp: vector seek forward Delay for reconnect to new cascade and receive first chunk",stat_seekforwardNewJoinDelay);
							}
						}
				}
				else
				{
					for(int i=LV->hostBufferMap[LV->currentFilmId]->chunkNumbers[0]; i< LV->hostBufferMap[LV->currentFilmId]->getLastSetChunk() + 1 ; i++)
						if(LV->hostBufferMap[LV->currentFilmId]->buffermap[i + (playbackPoint/chunkSize)-bufferFirstChunkNumber])
						{
							bitCounter++;
							if(stat_seekforwardNewJoinDelay==0)
								stat_seekforwardNewJoinDelay=simTime().dbl()-stat_startSeekforward;// delay time for connected to new neighbors

						}
				}

		}
		//need add parameter like delay after seek forward;
	}
	else
	{
		if(discontinuityStart==true)
		{
			for(int i=0;i<(Fps/chunkSize);i++)//(playbackPoint/chunkSize[LV->currentFilmId]); i< (recivePoint/chunkSize[LV->currentFilmId]);i++)///LV->hostBufferMap[LV->currentFilmId]->getLastSetChunk() + 1 ; i++)
					if(LV->hostBufferMap[LV->currentFilmId]->buffermap[i + (playbackPoint/chunkSize)-bufferFirstChunkNumber])
						bitCounter++;
		}
		else
		{
			for(int i=0 ; i + LV->hostBufferMap[LV->currentFilmId]->chunkNumbers[0] < LV->hostBufferMap[LV->currentFilmId]->getLastSetChunk() + 1 ; i++)
				if(LV->hostBufferMap[LV->currentFilmId]->buffermap[i])
					bitCounter++;
		}
	}
	bitCounter *= chunkSize;
	if(discontinuityStart==true)
	{
		stat_discontinuityTime++;
		if(bitCounter>(0.8*Fps/chunkSize) || playbackPoint<recivePoint-discontinuityTimeParam*Fps )
		{
			playingState = PLAYING;
			VideoMessage* playermessage = new VideoMessage("PlayerMessage");
			playermessage->setCommand(PLAYER_MSG);
			playermessage->setType(7);
			send(playermessage,"to_upperTier");
			discontinuityStart=false;
		}
	}
	else if(bitCounter > Fps*startUpBuffering)
	{
		playingState = PLAYING;
		if(haveInteractive==false )
		{
			stat_startSendToPlayer = simTime().dbl();
			stat_startupDelay = simTime().dbl() - stat_startBuffering;
			playbackPoint = LV->hostBufferMap[LV->currentFilmId]->chunkNumbers[0]*chunkSize;
		}
		else if(haveInteractive==true)
		{
			VideoMessage* playermessage = new VideoMessage("PlayerMessage");
			playermessage->setCommand(PLAYER_MSG);
			playermessage->setType(9);
			send(playermessage,"to_upperTier");
			stat_seekforwardDelay=simTime().dbl()-stat_startSeekforward;
			std::cout <<"time: " << simTime()<< " start play after seek at time"<<stat_startSeekforward<<" with delay "<< stat_seekforwardDelay <<getParentModule()->getParentModule()->getFullName() <<std::endl;
			globalStatistics->recordOutVector("VodApp: vector of All seek forward Delay",stat_seekforwardDelay);
			if(playbackPoint<0)
				playbackPoint=0;
		}
	}
	}
	catch(std::exception& e)
	{
		std::cout << "time: " << simTime()<< " checkForPlaying error in vod app "<<getParentModule()->getParentModule()->getFullName() <<  std::endl;
	}
}
void VodApp::sendFrameToPlayer()
{
	try
	{
		peerAllWatchingTime++;
		int bufferFirstChunkNumber=LV->hostBufferMap[LV->currentFilmId]->chunkNumbers[0];
		if(LV->emergency==false && useEmergencyConnection && (recivePoint- playbackPoint)/Fps<emergencyMinVideoInBuffer)
		{
			LV->emergency=true;
		}
		if(LV->emergency==true && (recivePoint- playbackPoint)/Fps>emergencyVideolengthBuffer)
		{
			LV->emergency=false;
		}
		if(haveInteractive &&!limitedBuffer&& interactiveChunkStart>bufferFirstChunkNumber)
		{
			if(!LV->hostBufferMap[LV->currentFilmId]->buffermap[(playbackPoint/chunkSize)-bufferFirstChunkNumber-seekChuncknumbers])
			{
				int frameCount=0;
				for(int i=0 ; i<(Fps/chunkSize) ;i++)
				{
					if(LV->hostBufferMap[LV->currentFilmId]->buffermap[i + (playbackPoint/chunkSize)])
						frameCount++;
				}
				frameCount*=chunkSize;
				if(frameCount<(0.8*Fps/chunkSize) && playbackPoint>recivePoint-discontinuityTimeParam*Fps )
				{
					playingState=BUFFERING;
					VideoMessage* playermessage = new VideoMessage("PlayerMessage");
					playermessage->setCommand(PLAYER_MSG);
					playermessage->setType(6);
					send(playermessage,"to_upperTier");
					stat_continuityShow=0;//AGH 0 is not contiguous
					stat_discontinuityTime++;
					stat_NumberOfDiscontinuity++;
					discontinuityStart=true;
					return;
				}
				else
				{
					stat_lossFrame++;
				}
			}
		}
		else
		{
			if(!LV->hostBufferMap[LV->currentFilmId]->buffermap[(playbackPoint/chunkSize)-bufferFirstChunkNumber])
			{
				int frameCount=0;
				for(int i=0 ; i<(Fps/chunkSize) ;i++)
					if(LV->hostBufferMap[LV->currentFilmId]->buffermap[i + (playbackPoint/chunkSize)])
						frameCount++;
				frameCount*=chunkSize;
				if(frameCount<(0.8*Fps/chunkSize) && playbackPoint>recivePoint-discontinuityTimeParam*Fps )
				{
					playingState=BUFFERING;
					VideoMessage* playermessage = new VideoMessage("PlayerMessage");
					playermessage->setCommand(PLAYER_MSG);
					playermessage->setType(6);
					send(playermessage,"to_upperTier");
					stat_continuityShow=0;//AGH 0 is not contiguous
					stat_discontinuityTime++;
					stat_NumberOfDiscontinuity++;
					discontinuityStart=true;
					return;
				}
				else
				{
					stat_lossFrame++;
				}
			}
		}
		/*if(playbackPoint>recivePoint-30)//LV->hostBufferMap[LV->currentFilmId]->getLastSetChunk())//
		{
			playingState=BUFFERING;
			stat_continuityShow=0;//AGH 0 is not contiguous
			stat_discontinuityTime++;
			stat_NumberOfDiscontinuity++;
			discontinuityStart=true;
			return;
		}*/
		stat_allFramePlayed++;
		if((playbackPoint/chunkSize)> LV->videoBuffer[LV->currentFilmId]->chunkBuffer[bufferSize[LV->currentFilmId]-2].getChunkNumber())
		{
			while((playbackPoint/chunkSize) > LV->videoBuffer[LV->currentFilmId]->chunkBuffer[bufferSize[LV->currentFilmId]-2].getChunkNumber())
				LV->videoBuffer[LV->currentFilmId]->shiftChunkBuf();
			LV->updateLocalBufferMap(LV->currentFilmId);
		}

		VideoFrame vf = LV->videoBuffer[LV->currentFilmId]->getFrame(playbackPoint,limitedBuffer);
		if(!vf.isSet())
			stat_lossFrame2++;
		vf.setFrameNumber(playbackPoint);
		VideoMessage* playermessage = new VideoMessage("PlayerMessage");
		playermessage->setCommand(PLAYER_MSG);
		playermessage->setVFrame(vf);
		send(playermessage,"to_upperTier");
		if(vf.getFrameType() == 'N')
			checkAvailability_RateControlLoss();
		playbackPoint++;
		sendframeCleanUp();
	}
	catch(std::exception& e)
	{
		std::cout << "time: " << simTime()<< " sendFrameToPlayer error in vod app "<<getParentModule()->getParentModule()->getFullName() <<  std::endl;
	}
}
void VodApp::sendframeCleanUp()
{
	try
	{
		bool hasExpiredFrame = true;
		while (hasExpiredFrame)
		{
			hasExpiredFrame = false;
			for (unsigned int i=0; i!=sendFrames.size(); i++)
				if (sendFrames[i].frameNum < playbackPoint/chunkSize)// sendFrames store chunk numbers-AGH
					hasExpiredFrame = true;
			for (unsigned int i=0; i!=sendFrames.size(); i++)
			{
				if(sendFrames.size() < i)
					break;
				if(sendFrames[i].frameNum < playbackPoint/chunkSize)
					sendFrames.erase(sendFrames.begin()+i,sendFrames.begin()+1+i);
			}
		}
	}
	catch(std::exception& e)
	{
		std::cout << "time: " << simTime()<< " sendframeCleanUp error in vod app "<<getParentModule()->getParentModule()->getFullName() <<  std::endl;
	}
}
bool VodApp::isInVector(TransportAddress& Node, std::vector <neighbor> &neighbors)
{

	try
	{
		for (unsigned int i=0; i!=neighbors.size(); i++)
		{
			if (neighbors[i].address == Node)
			{
				return true;
			}
		}
		return false;
	}
	catch(std::exception& e)
	{
		return false;
		std::cout << "time: " << simTime()<< " isInVector error in vod app "<<getParentModule()->getParentModule()->getFullName() <<  std::endl;
	}
}
void VodApp::checkAvailability_RateControlLoss()
{
	try
	{
		if(playbackPoint/Fps > measuringTime)
			return;
		bool find = false;
		for(unsigned int i = 0; i < requestedChunks.size(); i++)
		{
			if(requestedChunks[i] == playbackPoint)
			{
				requestedChunks.erase(requestedChunks.begin()+i,requestedChunks.begin()+1+i);
				find = true;
				break;
			}
		}
		if(!find)
		{
			std::map <int,int>::iterator frameIt = bc->frameInfo.begin();
			frameIt = bc->frameInfo.find(playbackPoint);
			LV->addToAvailability_RateControlLoss(frameIt->second);
		}
	}
	catch(std::exception& e)
	{
		std::cout << "time: " << simTime()<< " checkAvailability_RateControlLoss error in vod app "<<getParentModule()->getParentModule()->getFullName() <<  std::endl;
	}
}
void VodApp::bufferMapExchange()
{

	try
	{
		if(ready==true)
		{
			if (isVideoServer)
			{
				BufferMapMessage *BufferMapmsg = new BufferMapMessage("BufferMap_exchange");
				BufferMapmsg->setFilmId(-2);
				BufferMapmsg->setIsServer(true);
				BufferMapmsg->setUnicast(false);
				BufferMapmsg->setBitLength(BUFFERMAPMESSAGE_L(msg));
				BufferMapmsg->setWhatchInPast(false);
				//BufferMapmsg->setReceivePoint(0);
				BufferMapmsg->setSrcNode(thisNode);
				send (BufferMapmsg,"to_lowerTier");
				//	std::cout << "time: " << simTime()<< " bufferMapExchange OK"<< filmId<<getParentModule()->getParentModule()->getFullName() <<  std::endl;
				scheduleAt(simTime()+bufferMapExchangePeriod,bufferMapTimer);
			}
			else if(playingState != STOP )
			{
				if(adaptiveBufferMapXchange==true)
				{
					double msgLenght=0;
					for(int i=0;i<neighborsBufferMaps.size();i++)
					{
						if(neighborsBufferMaps[i].isServer==false )
						{
							BufferMap BM;
							BufferMapMessage *BufferMapmsg = new BufferMapMessage("BufferMap_exchange");
							BufferMapmsg->setFirstBufferMap(false);
							FirstbufferExchangeWindowInterest=(2*bufferMapExchangePeriod+2*neighborsBufferMaps[i].delay)*(neighborsBufferMaps[i].downBandwidth/videoAverageRate[neighborsBufferMaps[i].videoId])+firstAdaptiveEpsilon;
							bufferExchangeWindowInterest=bufferMapExchangePeriod*(neighborsBufferMaps[i].downBandwidth/videoAverageRate[neighborsBufferMaps[i].videoId])+epsilon;
							int firstbufferExLentgh=FirstbufferExchangeWindowInterest* Fps/chunkSize;
							int bufferExLentgh=bufferExchangeWindowInterest* Fps/chunkSize;
							int bigbufferExLentgh=2*bufferMapExchangePeriod*(neighborsBufferMaps[i].downBandwidth/videoAverageRate[neighborsBufferMaps[i].videoId])+firstAdaptiveEpsilon;// in this case we don't use delay-AGH
							bigbufferExLentgh=bigbufferExLentgh* Fps/chunkSize;
							int redundantSize=0;
							if(LV->watchFilm[neighborsBufferMaps[i].videoId]==true)
							{
								if(LV->videoBuffer[neighborsBufferMaps[i].videoId]->lastSetChunk != 0)
								{
									if(neighborsBufferMaps[i].bufferPoint==0)
									{
										BufferMapmsg->setFirstBufferMap(true);
										BM.setValues(firstbufferExLentgh);
									}
									else
									{

										if(LV->videoBuffer[neighborsBufferMaps[i].videoId]->lastSetChunk>neighborsBufferMaps[i].bufferPoint && neighborsBufferMaps[i].bufferPoint+bufferExLentgh > LV->videoBuffer[neighborsBufferMaps[i].videoId]->chunkBuffer[0].getChunkNumber())
										{
											if(haveInteractive==true)
											{
												if(neighborsBufferMaps[i].bufferPoint>interactiveChunkStart&&neighborsBufferMaps[i].bufferPoint+bufferExLentgh<interactiveChunkStart+seekChuncknumbers )
													BM.setValues(1);// this node do not have useful data for neighbor
												else
												{

													if(neighborsBufferMaps[i].pleaseSendToMEBigbufferMap==true)
													{
														if(LV->videoBuffer[neighborsBufferMaps[i].videoId]->lastSetChunk < neighborsBufferMaps[i].bufferPoint+bigbufferExLentgh)
														{
															int temp=bigbufferExLentgh;
															bigbufferExLentgh=(LV->videoBuffer[neighborsBufferMaps[i].videoId]->lastSetChunk-neighborsBufferMaps[i].bufferPoint);
															stat_BandwidthSavedForNotSendingBuffermapBiggerLastsendChunk+= temp-bigbufferExLentgh;
														}
														//redundant sending
														redundantSize=neighborsBufferMaps[i].redundantSize;
														if(neighborsBufferMaps[i].bufferPoint-redundantSize<LV->videoBuffer[neighborsBufferMaps[i].videoId]->chunkBuffer[0].getChunkNumber())
															redundantSize=neighborsBufferMaps[i].bufferPoint-LV->videoBuffer[neighborsBufferMaps[i].videoId]->chunkBuffer[0].getChunkNumber();
														if(bigbufferExLentgh+redundantSize>firstbufferExLentgh)
															redundantSize=firstbufferExLentgh-bigbufferExLentgh;
														bigbufferExLentgh+=redundantSize;
														stat_BandwidthOverheadForsendingRedunduntInBuffermap+=redundantSize;

														BM.setValues(bigbufferExLentgh);
													}
													else
													{

														if(LV->videoBuffer[neighborsBufferMaps[i].videoId]->lastSetChunk < neighborsBufferMaps[i].bufferPoint+bufferExLentgh)
														{
															int temp=bufferExLentgh;
															bufferExLentgh=(LV->videoBuffer[neighborsBufferMaps[i].videoId]->lastSetChunk-neighborsBufferMaps[i].bufferPoint);
															stat_BandwidthSavedForNotSendingBuffermapBiggerLastsendChunk+= temp-bufferExLentgh;
														}
														//redundant sending
														redundantSize=neighborsBufferMaps[i].redundantSize;
														if(neighborsBufferMaps[i].bufferPoint-redundantSize<LV->videoBuffer[neighborsBufferMaps[i].videoId]->chunkBuffer[0].getChunkNumber())
															redundantSize=neighborsBufferMaps[i].bufferPoint-LV->videoBuffer[neighborsBufferMaps[i].videoId]->chunkBuffer[0].getChunkNumber();
														bufferExLentgh+=redundantSize;
														stat_BandwidthOverheadForsendingRedunduntInBuffermap+=redundantSize;

														BM.setValues(bufferExLentgh);
													}
												}
											}
											else
											{

												if(neighborsBufferMaps[i].pleaseSendToMEBigbufferMap==true)
												{
													if(LV->videoBuffer[neighborsBufferMaps[i].videoId]->lastSetChunk < neighborsBufferMaps[i].bufferPoint+bigbufferExLentgh)
													{
														int temp=bigbufferExLentgh;
														bigbufferExLentgh=(LV->videoBuffer[neighborsBufferMaps[i].videoId]->lastSetChunk-neighborsBufferMaps[i].bufferPoint);
														stat_BandwidthSavedForNotSendingBuffermapBiggerLastsendChunk+= temp-bigbufferExLentgh;
													}
													//redundant sending
													redundantSize=neighborsBufferMaps[i].redundantSize;
													if(neighborsBufferMaps[i].bufferPoint-redundantSize<LV->videoBuffer[neighborsBufferMaps[i].videoId]->chunkBuffer[0].getChunkNumber())
														redundantSize=neighborsBufferMaps[i].bufferPoint-LV->videoBuffer[neighborsBufferMaps[i].videoId]->chunkBuffer[0].getChunkNumber();
													if(bigbufferExLentgh+redundantSize>firstbufferExLentgh)
														redundantSize=firstbufferExLentgh-bigbufferExLentgh;
													bigbufferExLentgh+=redundantSize;
													stat_BandwidthOverheadForsendingRedunduntInBuffermap+=redundantSize;

													BM.setValues(bigbufferExLentgh);
												}
												else
												{
													if(LV->videoBuffer[neighborsBufferMaps[i].videoId]->lastSetChunk < neighborsBufferMaps[i].bufferPoint+bufferExLentgh)
													{
														int temp=bufferExLentgh;
														bufferExLentgh=(LV->videoBuffer[neighborsBufferMaps[i].videoId]->lastSetChunk-neighborsBufferMaps[i].bufferPoint);
														stat_BandwidthSavedForNotSendingBuffermapBiggerLastsendChunk+= temp-bufferExLentgh;
													}
													//redundant sending
													redundantSize=neighborsBufferMaps[i].redundantSize;
													if(neighborsBufferMaps[i].bufferPoint-redundantSize<LV->videoBuffer[neighborsBufferMaps[i].videoId]->chunkBuffer[0].getChunkNumber())
														redundantSize=neighborsBufferMaps[i].bufferPoint-LV->videoBuffer[neighborsBufferMaps[i].videoId]->chunkBuffer[0].getChunkNumber();
													bufferExLentgh+=redundantSize;
													stat_BandwidthOverheadForsendingRedunduntInBuffermap+=redundantSize;

													BM.setValues(bufferExLentgh);
												}
											}
										}
										else
										{
											BM.setValues(1);// this node do not have useful data for neighbor
										}
									}
								}
								else
								{
									// this node do not have useful data for neighbor
									if(neighborsBufferMaps[i].bufferPoint==0)
									{
										BufferMapmsg->setFirstBufferMap(true);
										BM.setValues(firstbufferExLentgh);
									}
									else
										BM.setValues(1);
								}

								if(neighborsBufferMaps[i].bufferPoint==0 && neighborsBufferMaps[i].recivePoint>0)
									LV->videoBuffer[neighborsBufferMaps[i].videoId]->updateBufferMapXchange(&BM,neighborsBufferMaps[i].recivePoint,BM.getBufferSize(),limitedBuffer, redundantSize);
								else
									LV->videoBuffer[neighborsBufferMaps[i].videoId]->updateBufferMapXchange(&BM,neighborsBufferMaps[i].bufferPoint,BM.getBufferSize(),limitedBuffer, redundantSize);
							}
							else
							{
								// this node do not watch this video and not have useful data for neighbor
								if(neighborsBufferMaps[i].bufferPoint==0)
								{
									BufferMapmsg->setFirstBufferMap(true);
									BM.setValues(firstbufferExLentgh);
								}
								else
									BM.setValues(1);
								BM.buffermap[0] = false;
								BM.chunkNumbers[0]=0;
								BM.setLastSetChunk(0);
							}
							BufferMapmsg->setRedundantsize(redundantSize);
							int next_redundant=(redundantEpsilon*Fps/chunkSize)-( LV->videoBuffer[neighborsBufferMaps[i].videoId]->lastSetChunk-BM.getLastSetChunk());
							if(next_redundant>0)
								neighborsBufferMaps[i].redundantSize=next_redundant;
							else
								neighborsBufferMaps[i].redundantSize=0;
							BufferMapmsg->setTimestamp(simTime());
							BufferMapmsg->setFilmId(LV->currentFilmId);
							BufferMapmsg->setBuffermap(BM);
							BufferMapmsg->setReceivePoint(recivePoint);
							BufferMapmsg->setPlaybackPoint(playbackPoint);
							if(useRequestBigBufferMap)
								BufferMapmsg->setPleaseSendBigbufferMap(neighborsBufferMaps[i].requestBigbufferMap);
							else
								BufferMapmsg->setPleaseSendBigbufferMap(false);
							BufferMapmsg->setHopCount(LV->hopCount);
							if(BufferMapmsg->getFirstBufferMap()==true)
								BufferMapmsg->setHopCount(LV->cascadeNumber);//we used hop count for cascade in first buffer map-AGH
							BufferMapmsg->setIsServer(false);
							BufferMapmsg->setUnicast(true);
							BufferMapmsg->setDstNode(neighborsBufferMaps[i].tAddress);
							BufferMapmsg->setBitLength(BUFFERMAPMESSAGE_L(msg)+BM.getBitLength());
							BufferMapmsg->setSrcNode(thisNode);
							if(neighborsBufferMaps[i].buffermap.getLastSetChunk()==-8)
							{
								if(recivePoint>0)
									BufferMapmsg->setLastBufferPoint(recivePoint);
								else
									BufferMapmsg->setLastBufferPoint(0);
							}
							else
							{
								BufferMapmsg->setLastBufferPoint(neighborsBufferMaps[i].buffermap.getLastSetChunk());
								//if(neighborsBufferMaps[i].buffermap.getLastSetChunk()<0)
								//	BufferMapmsg->setLastBufferPoint(0);
							}

							if(BM.getBufferSize()>1)
								BufferMapmsg->setMyVideoBufferLastSetChunk(LV->videoBuffer[neighborsBufferMaps[i].videoId]->lastSetChunk);
							else
							{
								BufferMapmsg->setMyVideoBufferLastSetChunk(0);
								BufferMapmsg->setFirstBufferMap(false);
								stat_BandwidthSavedForNotSendingBuffermapWhenPeerNotHaveUsefullData+=bufferExchangeWindowInterest* Fps/chunkSize;
							}
							if(neighborsBufferMaps[i].videoId!=LV->currentFilmId)
								BufferMapmsg->setWhatchInPast(true);
							else
								BufferMapmsg->setWhatchInPast(false);
							send (BufferMapmsg,"to_lowerTier");
							//	std::cout << "time: " << simTime()<< " bufferMapExchange OK"<< filmId<<getParentModule()->getParentModule()->getFullName() <<  std::endl;
							stat_BandwidthForBufferExchange+=BufferMapmsg->getBitLength();
							msgLenght+=BufferMapmsg->getBitLength();
							if(neighborsBufferMaps[i].bufferPoint==0)
							{
								globalStatistics->addStdDev("VodApp:Msg Size For First BufferExchange",BufferMapmsg->getBitLength());
								stat_BandwidthForFirstBufferExchange+=BufferMapmsg->getBitLength();
							}
						}
					}
				/*	char temp[100];
					sprintf(temp, "VodApp: Upload Bandwidth consume for Buffer Exchange At Time %d",(int) (simTime().dbl()-stat_startBufferMapExchange));
					globalStatistics->addStdDev(temp,msgLenght);
					sprintf(temp, "VodApp: neighbor size At Time %d",(int) (simTime().dbl()-stat_startBufferMapExchange));
					globalStatistics->addStdDev(temp,neighborsBufferMaps.size());
				 */	if(neighborsBufferMaps.size()!=0)
					{
						//sprintf(temp, "VodApp: Upload Bandwidth consume for Buffer Exchange Per Sending to each neighbor At Time %d",(int) (simTime().dbl()-stat_startBufferMapExchange));
						//globalStatistics->addStdDev(temp,msgLenght/neighborsBufferMaps.size());
						stat_BandwidthForBufferExchangePerSending+=msgLenght/neighborsBufferMaps.size();
					}
					scheduleAt(simTime()+bufferMapExchangePeriod,bufferMapTimer);
				}
				else if(adaptiveBufferMapXchange!= true)
				{
					if(playingState != STOP)
					{
						if(LV->videoBuffer[LV->currentFilmId]->lastSetChunk == 0)
							scheduleAt(simTime()+bufferMapExchangePeriod,bufferMapTimer);
						else
						{
							BufferMap BM;
							BufferMapMessage *BufferMapmsg = new BufferMapMessage("BufferMap_exchange");
							BM.setValues(bufferSize[LV->currentFilmId]);
							BufferMapmsg->setFilmId(LV->currentFilmId);
							BufferMapmsg->setHopCount(LV->hopCount);
							BufferMapmsg->setUnicast(false);
							BufferMapmsg->setPlaybackPoint(playbackPoint);
							LV->videoBuffer[LV->currentFilmId]->updateBufferMap(&BM);
							BufferMapmsg->setBuffermap(BM);
							BufferMapmsg->setSrcNode(thisNode);
							BufferMapmsg->setBitLength(BUFFERMAPMESSAGE_L(msg)-69+LV->hostBufferMap[LV->currentFilmId]->getBitLength());
							send (BufferMapmsg,"to_lowerTier");
							stat_BandwidthForBufferExchangePerSending+=BufferMapmsg->getBitLength();
							stat_BandwidthForBufferExchange+=BufferMapmsg->getBitLength()*neighborsBufferMaps.size();
							scheduleAt(simTime()+bufferMapExchangePeriod,bufferMapTimer);
						}
					}

				}
			}
		}
	}
	catch(std::exception& e)
	{
		std::cout << "time: " << simTime()<< " bufferMapExchange error in vod app "<<getParentModule()->getParentModule()->getFullName() <<  std::endl;
		scheduleAt(simTime()+ 0.1*bufferMapExchangePeriod,bufferMapTimer);
	}
}
bool VodApp::isMeasuring(int frameNo)
{
	try
	{
	if(frameNo/Fps > measuringTime)
		return true;
	else
		return false;
	}
	catch(std::exception& e)
	{
		std::cout << "time: " << simTime()<< " isMeasuring error in vod app  "<<getParentModule()->getParentModule()->getFullName() <<  std::endl;
	}
}
void VodApp::countRequest(TransportAddress& node)
{
	try
	{
		for(unsigned int i = 0; i < neighborsBufferMaps.size(); i++)
		{
			if(neighborsBufferMaps[i].tAddress == node)
			{
				neighborsBufferMaps[i].requestCounter -= 1;
			}
		}
	}
	catch(std::exception& e)
	{
		std::cout << "time: " << simTime()<< " countRequest error in vod app "<<getParentModule()->getParentModule()->getFullName() <<  std::endl;
	}
}
void VodApp::deleteNeighbor(TransportAddress& node)
{
	try
	{
	for(unsigned int i=0 ; i<neighborsBufferMaps.size() ; i++)
		if(neighborsBufferMaps[i].tAddress == node)
		{
			neighborsBufferMaps.erase(neighborsBufferMaps.begin()+i,neighborsBufferMaps.begin()+1+i);
			break;
		}
	}
	catch(std::exception& e)
	{
		std::cout << "time: " << simTime()<< " deleteNeighbor error in vod app "<<getParentModule()->getParentModule()->getFullName() <<  std::endl;
	}
}
int VodApp::minimum(int A,int B)
{
	if(A<=B)
		return A;
	else if(A>B)
		return B;

}
void VodApp::setStatistics()
{
	char temp[100];
	if(recivePoint-playbackPoint>0)
		stat_VideoFramesDownloadedButNotPlayedForExit=recivePoint-playbackPoint;
/*	if(stat_startupDelay != 0)
	{
		sprintf(temp, "VodApp: Startup Delay Video %d", LV->currentFilmId);
		globalStatistics->addStdDev(temp, stat_startupDelay);
		sprintf(temp, "VodApp: Startup Delay_vec Video %d", LV->currentFilmId);
		globalStatistics->recordOutVector(temp, stat_startupDelay);
	}
	if(stat_recivefirstStreamPacket != 0&& stat_jointoMeshStart!=0)
	{
		sprintf(temp, "VodApp: Startup join latency Video %d", LV->currentFilmId);
		globalStatistics->addStdDev(temp, stat_recivefirstStreamPacket-stat_jointoMeshStart);
		sprintf(temp, "VodApp: Startup join latency_vec Video %d", LV->currentFilmId);
		globalStatistics->recordOutVector(temp, stat_recivefirstStreamPacket-stat_jointoMeshStart);
	}
	if(stat_startSendToPlayer != 0)
	{
		sprintf(temp, "VodApp: start to send to player time Video %d", LV->currentFilmId);
		globalStatistics->addStdDev(temp, stat_startSendToPlayer);
	}
	if(stat_startBuffering != 0)
	{
		sprintf(temp, "VodApp: start to buffering time Video %d", LV->currentFilmId);
		globalStatistics->addStdDev(temp, stat_startBuffering);
	}
	if(stat_startBufferMapExchange != 0)
	{
		sprintf(temp, "VodApp: start exchanging bufferMap Video %d", LV->currentFilmId);
		globalStatistics->addStdDev(temp, stat_startBufferMapExchange);
	}
	if(stat_TotalReceivedSize != 0)
	{
		sprintf(temp, "VodApp: Frame Redundancy Video %d", LV->currentFilmId);
		globalStatistics->addStdDev(temp,stat_RedundentSize/stat_TotalReceivedSize *100);
	}
	if(stat_totalBufferExchange != 0)
	{
		sprintf(temp, "VodApp: loss Buffer Exchange %d", LV->currentFilmId);
		globalStatistics->addStdDev(temp,stat_lossBufferExchange/stat_totalBufferExchange *100);
	}
	if(stat_totalBufferExchange != 0)
	{
		sprintf(temp, "VodApp: loss Buffer Exchange2 %d", LV->currentFilmId);
		globalStatistics->addStdDev(temp,stat_lossBufferExchange2/stat_totalBufferExchange *100);
	}
	if(stat_totalBufferExchange != 0)
	{
		sprintf(temp, "VodApp: video Download Bigger Than Buffer EXchange %d", LV->currentFilmId);
		globalStatistics->addStdDev(temp,stat_videoDownloadBigerThanBufferXchange/stat_totalBufferExchange *100);
	}
	if(stat_lossBufferExchange != 0)
	{
		sprintf(temp, "VodApp: loss Buffer Exchange in losses peers %d", LV->currentFilmId);
		globalStatistics->addStdDev(temp,stat_lossBufferExchange/stat_totalBufferExchange *100);
	}
	if(stat_lossBufferExchange2 != 0)
	{
		sprintf(temp, "VodApp: loss Buffer Exchange2 in losses peers %d", LV->currentFilmId);
		globalStatistics->addStdDev(temp,stat_lossBufferExchange2/stat_totalBufferExchange *100);
	}
	if(stat_videoDownloadBigerThanBufferXchange != 0)
	{
		sprintf(temp, "VodApp: video Download Bigger Than Buffer EXchange in these peers %d", LV->currentFilmId);
		globalStatistics->addStdDev(temp,stat_videoDownloadBigerThanBufferXchange/stat_totalBufferExchange *100);
	}
	if(stat_BandwidthForBufferExchange!=0)
	{
		sprintf(temp, "VodApp: Upload Bandwidth consume for Buffer Exchange %d", LV->currentFilmId);
		globalStatistics->addStdDev(temp,stat_BandwidthForBufferExchange/(simTime().dbl()-stat_startBufferMapExchange));
	}

	if(stat_BandwidthForBufferExchange!=0)
	{
		sprintf(temp, "VodApp: Upload Bandwidth Saved For Not Sending Buffer map Bigger than LastsendChunk %d", LV->currentFilmId);
		globalStatistics->addStdDev(temp,stat_BandwidthSavedForNotSendingBuffermapBiggerLastsendChunk/(simTime().dbl()-stat_startBufferMapExchange));
	}
	if(stat_BandwidthForBufferExchange!=0)
	{
		sprintf(temp, "VodApp: Upload Bandwidth Saved For  Not Sending Buffer map When Peer Not Have Usefull Data %d", LV->currentFilmId);
		globalStatistics->addStdDev(temp,stat_BandwidthSavedForNotSendingBuffermapWhenPeerNotHaveUsefullData/(simTime().dbl()-stat_startBufferMapExchange));
	}
	if(stat_BandwidthForBufferExchange!=0)
	{
		sprintf(temp, "VodApp: Upload Bandwidth Overhead For sending Redundunt In Buffermap %d", LV->currentFilmId);
		globalStatistics->addStdDev(temp,stat_BandwidthOverheadForsendingRedunduntInBuffermap/(simTime().dbl()-stat_startBufferMapExchange));
	}
	if(stat_BandwidthForFirstBufferExchange!=0)
	{
		sprintf(temp, "VodApp: Upload Bandwidth consume for First Buffer Exchange %d", LV->currentFilmId);
		globalStatistics->addStdDev(temp,stat_BandwidthForFirstBufferExchange/(simTime().dbl()-stat_startBufferMapExchange));
	}
	if(stat_BandwidthForBufferExchangePerSending!=0)
	{
		sprintf(temp, "VodApp: Upload Bandwidth consume for Buffer Exchange Per Sending to each neighbor%d", LV->currentFilmId);
		globalStatistics->addStdDev(temp,stat_BandwidthForBufferExchangePerSending/(simTime().dbl()-stat_startBufferMapExchange));

	}
	if(stat_RedunduntReceiveBufferExchange!=0)
	{
		sprintf(temp, "VodApp: Receive useless Bandwidth consume for Buffer Exchange %d", LV->currentFilmId);
		globalStatistics->addStdDev(temp,stat_RedunduntReceiveBufferExchange/(simTime().dbl()-stat_startBufferMapExchange));
	}
	if(stat_RedunduntReceiveBufferExchange!=0)
	{
		sprintf(temp, "VodApp: useless Buffer Exchange Receive r%d", LV->currentFilmId);
		globalStatistics->addStdDev(temp,stat_RedunduntReceiveBufferExchange);
	}
	if(stat_VideoFramesDownloadedButNotPlayedForExit>=0)
	{
		sprintf(temp, "VodApp: Video Frames Downloaded But Not Played For Exit%d", LV->currentFilmId);
		globalStatistics->addStdDev(temp,stat_VideoFramesDownloadedButNotPlayedForExit);
	}
	if(stat_VideoFramesDownloadedButNotPlayedForInteractive>=0)
	{
		sprintf(temp, "VodApp: Video Frames Downloaded But Not Played For Interactive for all peers%d", LV->currentFilmId);
		globalStatistics->addStdDev(temp,stat_VideoFramesDownloadedButNotPlayedForInteractive);
	}
	if(stat_VideoFramesDownloadedButNotPlayedForInteractive>=0&& haveInteractive==true)
	{
		sprintf(temp, "VodApp: Video Frames Downloaded But Not Played For Interactive for only interactive peers %d", LV->currentFilmId);
		globalStatistics->addStdDev(temp,stat_VideoFramesDownloadedButNotPlayedForInteractive);
	}
	if(stat_startSendToPlayer != 0)
	{
		sprintf(temp, "VodApp:percent of peers that watch without discontinuity %d", LV->currentFilmId);
		globalStatistics->addStdDev(temp,100*stat_continuityShow);
	}
	if(stat_startSendToPlayer != 0)
	{
		sprintf(temp, "VodApp:Percent of discontinuity Show %d", LV->currentFilmId);
		globalStatistics->addStdDev(temp,(100*stat_discontinuityTime)/(peerAllWatchingTime+stat_discontinuityTime));
	}
	if(stat_startSendToPlayer != 0 && stat_continuityShow==0)
	{
		sprintf(temp, "VodApp:Percent of discontinuity Show in discontinuity peers %d", LV->currentFilmId);
		globalStatistics->addStdDev(temp,(100*stat_discontinuityTime)/(peerAllWatchingTime+stat_discontinuityTime));
	}
	if(stat_seekforwardDelay > 0)
	{
		sprintf(temp, "VodApp: seek forward Delay%d", LV->currentFilmId);
		globalStatistics->addStdDev(temp,stat_seekforwardDelay);
	}*/



	// all average
	if(stat_startupDelay != 0)
	{
		globalStatistics->addStdDev("VodApp:All Startup Delay", stat_startupDelay);
		globalStatistics->recordOutVector("VodApp:All Startup Delay_vec", stat_startupDelay);
	}
	if(stat_recivefirstStreamPacket != 0 && stat_jointoMeshStart!=0)
	{
		globalStatistics->addStdDev("VodApp:All Startup join latency ", stat_recivefirstStreamPacket-stat_jointoMeshStart);
		globalStatistics->recordOutVector("VodApp: Startup join latency_vec ", stat_recivefirstStreamPacket-stat_jointoMeshStart);
	}
	if(stat_allFramePlayed != 0)
	{
		globalStatistics->addStdDev("VodApp:Average loss frame Percent send to player", (100*stat_lossFrame)/stat_allFramePlayed);
		globalStatistics->addStdDev("VodApp:Average loss frame2 Percent send to player", (100*stat_lossFrame2)/stat_allFramePlayed);
	}
	if(stat_startSendToPlayer != 0)
		globalStatistics->addStdDev("VodApp:All start to send to player time", stat_startSendToPlayer);
	if(stat_startBuffering != 0)
		globalStatistics->addStdDev("VodApp:All start to buffering time", stat_startBuffering);
	if(stat_startBufferMapExchange != 0)
		globalStatistics->addStdDev("VodApp:All start exchanging bufferMap", stat_startBufferMapExchange);
	if(stat_TotalReceivedSize != 0)
		globalStatistics->addStdDev("VodApp:All Frame Redundancy",stat_RedundentSize/stat_TotalReceivedSize *100);	/////
	if(stat_totalBufferExchange != 0)
		globalStatistics->addStdDev("VodApp:All loss Buffer Exchange",stat_lossBufferExchange/stat_totalBufferExchange *100);
	if(stat_totalBufferExchange != 0)
		globalStatistics->addStdDev("VodApp:All loss Buffer Exchange2",stat_lossBufferExchange2/stat_totalBufferExchange *100);
	if(stat_totalBufferExchange != 0)
		globalStatistics->addStdDev("VodApp:All video Download Bigger Than Buffer EXchange",stat_videoDownloadBigerThanBufferXchange/stat_totalBufferExchange *100);
	if(stat_lossBufferExchange != 0)
		globalStatistics->addStdDev("VodApp:All loss Buffer Exchange in losses peers",stat_lossBufferExchange/stat_totalBufferExchange *100);
	if(stat_lossBufferExchange2 != 0)
		globalStatistics->addStdDev("VodApp:All loss Buffer Exchange2 in losses peers",stat_lossBufferExchange2/stat_totalBufferExchange *100);
	if(stat_videoDownloadBigerThanBufferXchange != 0)
		globalStatistics->addStdDev("VodApp:All video Download Bigger Than Buffer EXchange in these peers",stat_videoDownloadBigerThanBufferXchange/stat_totalBufferExchange *100);
	if(stat_BandwidthForBufferExchange!=0)
		globalStatistics->addStdDev("VodApp:All Upload Bandwidth consume for Buffer Exchange",stat_BandwidthForBufferExchange/(simTime().dbl()-stat_startBufferMapExchange));
	if(stat_BandwidthForBufferExchangePerSending!=0)
		globalStatistics->addStdDev("VodApp:All Upload Bandwidth consume for Buffer Exchange Per Sending to each neighbor",stat_BandwidthForBufferExchangePerSending/(simTime().dbl()-stat_startBufferMapExchange));
	if(stat_BandwidthForFirstBufferExchange!=0)
		globalStatistics->addStdDev("VodApp:All Upload Bandwidth consume for First Buffer Exchange",stat_BandwidthForFirstBufferExchange/(simTime().dbl()-stat_startBufferMapExchange));
	if(stat_BandwidthForBufferExchange!=0)
		globalStatistics->addStdDev("VodApp: Upload Bandwidth Saved For Not Sending Buffer map Bigger than LastsendChunk",stat_BandwidthSavedForNotSendingBuffermapBiggerLastsendChunk/(simTime().dbl()-stat_startBufferMapExchange));
	if(stat_BandwidthForBufferExchange!=0)
		globalStatistics->addStdDev("VodApp: Upload Bandwidth Saved For  Not Sending Buffer map When Peer Not Have Usefull Data",stat_BandwidthSavedForNotSendingBuffermapWhenPeerNotHaveUsefullData/(simTime().dbl()-stat_startBufferMapExchange));
	if(stat_BandwidthForBufferExchange!=0)
		globalStatistics->addStdDev("VodApp: Upload Bandwidth Overhead For sending Redundunt In Buffermap",stat_BandwidthOverheadForsendingRedunduntInBuffermap/(simTime().dbl()-stat_startBufferMapExchange));

	if(stat_allRedundantRecivedBits!=0)
		globalStatistics->addStdDev("VodApp:  Percent of useful bits from all received bits for Redundant In Buffer-map",(100*stat_usefullRedundantRecivedBits)/stat_allRedundantRecivedBits);
	if(stat_BandwidthForBufferExchange!=0)
		globalStatistics->addStdDev("VodApp:  all useful receive bandwidth  for Redundunt In Buffermap",stat_usefullRedundantRecivedBits/(simTime().dbl()-stat_startBufferMapExchange));
	if(stat_allRedundantRecivedBits!=0)
		globalStatistics->addStdDev("VodApp:  Percent of Exist In Buffer bits from all received bits for Redundant In Buffer-map",(100*stat_allExistInBufferRedundantRecivedbits)/stat_allRedundantRecivedBits);
	if(stat_allExistInBufferRedundantRecivedbits!=0)
		globalStatistics->addStdDev("VodApp:  Percent of useful bits from Exist In Buffer bits for Redundant In Buffer-map",(100*stat_usefullRedundantRecivedBits)/stat_allExistInBufferRedundantRecivedbits);
	if(stat_allRedundantRecivedMsg!=0)
		globalStatistics->addStdDev("VodApp:  useful bits received per Buffer-map Massage that have redundancy",stat_usefullRedundantRecivedBits/stat_allRedundantRecivedMsg);

	if(stat_RedunduntReceiveBufferExchange!=0)
		globalStatistics->addStdDev("VodApp:All  Useless Receive  Bandwidth consume for Buffer Exchange",stat_RedunduntReceiveBufferExchange/(simTime().dbl()-stat_startBufferMapExchange));
	if(stat_RedunduntReceiveBufferExchange!=0)
		globalStatistics->addStdDev("VodApp:All Useless Buffer Exchange Receive",stat_RedunduntReceiveBufferExchange);
	if(stat_VideoFramesDownloadedButNotPlayedForExit>=0)
		globalStatistics->addStdDev("Video Frames Downloaded But Not Played For Exit",stat_VideoFramesDownloadedButNotPlayedForExit);
	if(stat_VideoFramesDownloadedButNotPlayedForInteractive>=0)
		globalStatistics->addStdDev("VodApp:All Video Frames Downloaded But Not Played For Interactive for all peers",stat_VideoFramesDownloadedButNotPlayedForInteractive);
	if(stat_VideoFramesDownloadedButNotPlayedForInteractive>=0&& haveInteractive==true)
		globalStatistics->addStdDev("VodApp:All Video Frames Downloaded But Not Played For Interactive for only interactive peers ",stat_VideoFramesDownloadedButNotPlayedForInteractive);

	if(stat_startSendToPlayer != 0)
		globalStatistics->addStdDev("VodApp: Average Number of discontinuity in all peers  %d",stat_NumberOfDiscontinuity);
	if(stat_NumberOfDiscontinuity != 0)
		globalStatistics->addStdDev("VodApp: Average Number of discontinuity in  discontinuity peers %d",stat_NumberOfDiscontinuity);
	if(stat_startSendToPlayer != 0)
	/*{
		int up=round( LV->getUpBandwidth()/100000);
		sprintf(temp, "VodApp: Average Number of discontinuity in all peers per uploadBandwidth %d",up);
		globalStatistics->addStdDev(temp,stat_NumberOfDiscontinuity);
	}*/
	/*if(stat_NumberOfDiscontinuity != 0)
	{
		int up=round( LV->getUpBandwidth()/100000);
		sprintf(temp, "VodApp: Average Number of discontinuity in  discontinuity peers per uploadBandwidth %d",up);
		globalStatistics->addStdDev(temp,stat_NumberOfDiscontinuity);
	}*/
	if(stat_startSendToPlayer != 0)
	{
		globalStatistics->addStdDev("VodApp:All percent of peers that watch without discontinuity %d",100*stat_continuityShow);
		globalStatistics->recordOutVector("VodApp:Vector of All Percent of discontinuity Show",(100*stat_discontinuityTime)/(peerAllWatchingTime+stat_discontinuityTime));
	}
	if(stat_startSendToPlayer != 0)
		globalStatistics->addStdDev("VodApp:All Percent of discontinuity Show",(100*stat_discontinuityTime)/(peerAllWatchingTime+stat_discontinuityTime));
	if(stat_startSendToPlayer != 0&& stat_continuityShow==0)
			globalStatistics->addStdDev("VodApp:All Percent of discontinuity Show in discontinuity peers",(100*stat_discontinuityTime)/(peerAllWatchingTime+stat_discontinuityTime));
/*	if(stat_startSendToPlayer != 0)
	{
		int up=round( LV->getUpBandwidth()/100000);
		sprintf(temp, "VodApp:All percent of peers that watch without discontinuity per uploadBandwidth %d",up);
		globalStatistics->addStdDev(temp,100*stat_continuityShow);
	}*/
	if(stat_startSendToPlayer != 0)
	{
		int up=round( LV->getUpBandwidth()/100000);
		sprintf(temp, "VodApp:All Percent of discontinuity Show per uploadBandwidth %d",up);
		globalStatistics->addStdDev(temp,(100*stat_discontinuityTime)/(peerAllWatchingTime+stat_discontinuityTime));
	}
	/*if(stat_startSendToPlayer != 0&& stat_continuityShow==0)
	{
		int up=round( LV->getUpBandwidth()/100000);
		sprintf(temp, "VodApp:All Percent of discontinuity Show in discontinuity peers per uploadBandwidth %d",up);
		globalStatistics->addStdDev(temp,(100*stat_discontinuityTime)/(peerAllWatchingTime+stat_discontinuityTime));
	}*/
	if(stat_seekforwardDelay > 0)
	{
		globalStatistics->addStdDev("All VodApp:Average seek forward Delay",stat_seekforwardDelay);
	}
	if(stat_seekforwardNewJoinDelay > 0)
	{
		globalStatistics->addStdDev("All VodApp:Average seek forward Delay for reconnect to new cascade and receive first chunk",stat_seekforwardNewJoinDelay);
	}

	// categorize peers to 1-free riders 2-low bandwidth 3- medium 4-high
		if(stat_startSendToPlayer != 0)
	{
		int up= LV->getUpBandwidth()/1024;
		if(up <=(videoAverageRate[LV->currentFilmId]))// it is low bandwidth
		{
			globalStatistics->addStdDev("VodApp:All Percent of discontinuity Show in low-free up band width peers",(100*stat_discontinuityTime)/(peerAllWatchingTime+stat_discontinuityTime));
		}
		if(up <=100)// it is free rider
		{
			globalStatistics->addStdDev("VodApp:All Percent of discontinuity Show in free riders",(100*stat_discontinuityTime)/(peerAllWatchingTime+stat_discontinuityTime));
		}
		else if(up <=(videoAverageRate[LV->currentFilmId]))// it is low bandwidth
		{
			globalStatistics->addStdDev("VodApp:All Percent of discontinuity Show in low up band width peers",(100*stat_discontinuityTime)/(peerAllWatchingTime+stat_discontinuityTime));
		}
		else if(up >=(videoAverageRate[LV->currentFilmId])&&up <(2*videoAverageRate[LV->currentFilmId]) )// it is medium band width
		{
			globalStatistics->addStdDev("VodApp:All Percent of discontinuity Show in medium up band width peers",(100*stat_discontinuityTime)/(peerAllWatchingTime+stat_discontinuityTime));
		}
		else if(up >=(2*videoAverageRate[LV->currentFilmId]))// it is high bandwidth
		{
			globalStatistics->addStdDev("VodApp:All Percent of discontinuity Show in high up band width peers",(100*stat_discontinuityTime)/(peerAllWatchingTime+stat_discontinuityTime));
		}
	}
}
void VodApp::finishApp()
{

	//stat_peresentTimeInSystem=simTime().dbl()-stat_peresentTimeInSystem;
	cancelAndDelete(bufferMapTimer);
	cancelAndDelete(requestChunkTimer);
	cancelAndDelete(playingTimer);
	cancelAndDelete(sendFrameTimer);
	cancelAndDelete(structureImprovementTimer);
	if(setStatisticsPosition==false)
	{	//statistics
		if(playingState==BUFFERING && haveInteractive==true && stat_seekforwardDelay==0 )
		{
			stat_seekforwardDelay=simTime().dbl()-stat_startSeekforward;
		}
		setStatistics();
	}
}
