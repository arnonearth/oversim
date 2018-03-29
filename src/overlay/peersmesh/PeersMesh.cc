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
 * @file PeersMesh.cc
 * @author Abdollah Ghaffari Sheshjavani(AGH)
 */

#include <SimpleInfo.h>
#include "PeersMesh.h"
#include <GlobalStatistics.h>
#include "TrackerMessage_m.h"
#include "VideoMessage_m.h"
#include "MeshMessage_m.h"

Define_Module(PeersMesh);

void PeersMesh::initializeOverlay(int stage)
{
	if (stage != MIN_STAGE_OVERLAY)
		return;
	isRegistered = false;
   	isSource = false;
    hopCount=-1;
    emergency=false;
    specialInteractiveNeghborReq=false;
    usingSpecialInteractiveNeghborReq=par("usingSpecialInteractiveNeghborReq");
    passiveNeighbors = par("passiveNeighbors");
    activeNeighbors = par("activeNeighbors");
    changeProbability=par("changeProbability");
    meshStructure=par("meshStructure");
    isStable=par("isStable");// if is true peer dont exit from system-AGH
    percentofExit=par("percentofExit");
    percentOfNotGracefullExit=par("percentOfNotGracefullExit");// its between 0 and 1-AGH;
    checkNeighborsTimeOut=par("checkNeighborsTimeOut");
    neighborsTimeOutLimit=par("neighborsTimeOutLimit");
    startExitSimTime=par("startExitSimTime");
    simpleMeshType=par("simpleMeshType");
    neighborNotificationPeriod = par("neighborNotificationPeriod");
    improvementProcessTimeLenghtTimer=new cMessage("improvementProcessTimeLenghtTimer");
    improvementProcessTimeLenght=par("improvementProcessTimeLenght");
	neighborNum = passiveNeighbors + activeNeighbors;
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
	//adaptiveNeighboring = par("adaptiveNeighboring");
 	percentofInteractive=par("percentofInteractive");//if is -1 then interactive is base on result of paper"can vod profitable" AGH
 	startIntractiveSimTime=par("startIntractiveSimTime");
 	usePreVideo=par("usePreVideo");
 	numberOfVideos=0;
 	connectDirectToServer=false;
	isheadnode=false;
	connectToHeadNode=false;
	neighborSelelctionStart=false;
	expireTime=0;
	haveInteractive=false;
	improvmentStructureStart=false;//AGH
	interactiveTime=0;
	haveChangecascadeForInteractive=false;
	Timer=0;
	//if(adaptiveNeighboring)
	//	neighborNum = (int)(upBandwidth/(videoAverageRate*1024) + 1);
	MeshOverlay::initializeOverlay(stage);
	WATCH(neighborNum);
	WATCH(downBandwidth);
	WATCH(upBandwidth);
	LV->peerExit=false;
	LV->cascadeNumber=-1;
	for(int i=0;i<10;i++)
	{
		LV->watchFilm[i]=false;
		watchFilmEndTime[i]=-1;
	}
	stat_TotalDownBandwidthUsage = 0;
	stat_TotalUpBandwidthUsage = 0;
    stat_joinREQ = 0;
	stat_joinREQBytesSent = 0;
	stat_joinRSP = 0;
	stat_joinRSPBytesSent = 0;
	stat_joinACK = 0;
	stat_joinACKBytesSent = 0;
	stat_joinDNY = 0;
	stat_joinDNYBytesSent = 0;
	stat_disconnectMessages = 0;
	stat_disconnectMessagesBytesSent = 0;
	stat_addedNeighbors = 0;
	stat_nummeshJoinRequestTimer = 0;
	//improvement
	stat_improvementREQ=0;
	stat_improvementREQBytesSent=0;
	stat_improvementRSP=0;
	stat_improvementRSPBytesSent=0;
	stat_improvementACK=0;
	stat_improvementACKBytesSent=0;
	stat_improvementDNY=0;
	stat_improvementDNYBytesSent=0;
	stat_numberOfImprovement=0;
	//change place for improvement parameters-AGH
	stat_ChangePlaceREQ=0;
	stat_ChangePlaceREQBytesSent=0;
	stat_ChangePlaceRSP=0;
	stat_ChangePlaceRSPBytesSent=0;
	stat_ChangePlaceACK=0;
	stat_ChangePlaceACKBytesSent=0;
	stat_ChangePlaceDNY=0;
	stat_ChangePlaceDNYBytesSent=0;

	introduceItself();
}

void PeersMesh::introduceItself()
{
	//ev <<"Hi. I am "<< this->getFullName() << ". My parent class is " << this->getParentModule() << ". And my grandparent class is " << this->getParentModule()->getParentModule() << "." << endl;

	//ev <<"Now, I will list out my gates " << endl;

	//for (cModule::GateIterator i(this->getParentModule()->getParentModule()); !i.end(); i++) 
	//{
    //cGate *gate = i();
    //ev << gate->getFullName() << ": ";
    //ev << "id=" << gate->getId() << ", ";
    //if (!gate->isVector())
    //    ev << "scalar gate, ";
    //else
    //    ev << "gate " << gate->getIndex()
    //       << " in vector " << gate->getName()
    //       << " of size " << gate->getVectorSize() << ", ";
    //ev << "type:" << cGate::getTypeName(gate->getType());
    //ev << "\n";
	//}
	
	//ev << "I connect to " << this->getParentModule()->getParentModule()->gate("pppg$o",0)->getNextGate()->getOwnerModule()->getFullName() << endl;

	// ev << "which its IP address is " << IPAddressResolver().resolve(this->getParentModule()->getParentModule()->gate("pppg$o",0)->getNextGate()->getOwnerModule()->getFullName()) << endl;
	accessAddress = IPAddressResolver().resolve(this->getParentModule()->getParentModule()->gate("pppg$o",0)->getNextGate()->getOwnerModule()->getFullName());

    //ev << "which its ip address is" << accessAddress << endl;
}

void PeersMesh::joinOverlay()
{
	try
	{
		LV->WatchExpired=false;
		LV->playbackpoint=-1;
		LV->recieveTime=0;
		recivelistfilm=false;
		trackerAddress  = *globalNodeList->getRandomAliveNode(1);
		remainNotificationTimer = new cMessage ("remainNotificationTimer");
		scheduleAt(simTime()+neighborNotificationPeriod,remainNotificationTimer);
		std::stringstream ttString;
		ttString << thisNode;
		getParentModule()->getParentModule()->getDisplayString().setTagArg("tt",0,ttString.str().c_str());
		meshJoinRequestTimer = new cMessage("meshJoinRequestTimer");
		scheduleAt(simTime(),meshJoinRequestTimer);
		setOverlayReady(true);
	}
	catch(std::exception& e)
	{
		std::cout << "time: " << simTime()<< "joinOverlay error in simple mesh "<<getParentModule()->getParentModule()->getFullName() <<  std::endl;
	}
}
void PeersMesh::handleTimerEvent(cMessage* msg)
{
	try
	{
		if(msg == meshJoinRequestTimer )
		{
			if(recivelistfilm==false)
			{
				TrackerMessage* RequestListOfFilm = new TrackerMessage("Request_ListOfFilm");
				RequestListOfFilm->setCommand(Request_ListOfFilm);
				RequestListOfFilm->setSrcNode(thisNode);
				RequestListOfFilm->setIsServer(false);
				sendMessageToUDP(trackerAddress,RequestListOfFilm);
				scheduleAt(simTime()+1,meshJoinRequestTimer);
			}
			else if(LV->needNewneighbor==true)// seek forward
			{
				if(meshStructure==0)
				{
					LV->needNewneighbor=false;
				}
				if(meshStructure==1||meshStructure==2)
				{
					if(LV->interactive==true)
					{
						TrackerMessage* RequestCascadeNumber = new TrackerMessage("CascadeNumRequest");
						RequestCascadeNumber->setCommand(CascadeNumRequest);
						RequestCascadeNumber->setSrcNode(thisNode);
						RequestCascadeNumber->setSelectedFilmStartTime(LV->playbackpoint);
						RequestCascadeNumber->setConnectedAccess(accessAddress);
						startTime=LV->recieveTime;
						//RequestCascadeNumber->setPreviousFilmStartTime(LV->recieveTime-LV->seekTime);
						RequestCascadeNumber->setCascadenumber(LV->cascadeNumber);
						RequestCascadeNumber->setSelectedFilmNum(LV->currentFilmId);
						neighborSelelctionStart=false;
						haveChangecascadeForInteractive=true;
						RequestCascadeNumber->setIsServer(false);
						RequestCascadeNumber->setNeighborSize(activeNeighbors);
						if(usingSpecialInteractiveNeghborReq)
							specialInteractiveNeghborReq=true;
						RequestCascadeNumber->setSpecialInteractiveNeghborReq(specialInteractiveNeghborReq);
						if(isRegistered)
						{
							RequestCascadeNumber->setConnectDirectToServer(connectDirectToServer);
							RequestCascadeNumber->setIsHeadnode(isheadnode);
							isRegistered = false;
						}
						sendMessageToUDP(trackerAddress,RequestCascadeNumber);
					}
					else
					{
						LV->needNewneighbor=false;
					}
				}
				scheduleAt(simTime()+2,meshJoinRequestTimer);
			}
			else
			{
				if(checkNeighborsTimeOut==true)
					checkNeighborsTimOuts();
				if(meshStructure==0)
				{
					int existActiveNeighbors=0;
					for(unsigned int i=0; i < LV->neighbors.size();i++)
					{
						if(LV->neighbors[i].type==0)
						existActiveNeighbors++;
					}
					if(simpleMeshType==1 && Timer<=0)
					{
						if(existActiveNeighbors < activeNeighbors)
							neighborRequest(activeNeighbors - existActiveNeighbors);
						Timer=2;
					}
					else if( simpleMeshType==0  && Timer<=0)
					{
						if(LV->neighbors.size() < neighborNum)
							neighborRequest(activeNeighbors - existActiveNeighbors);
						Timer=2;
					}
					else if(Timer<=0)
					{
						Timer=5;
						if(existActiveNeighbors<=1)
							Timer=1;
					}
				}
				else if(meshStructure==1||meshStructure==2)
				{
				// in my structure connecting to server is enough for peers-in VOD -AGH
					int existActiveNeighbors=0;
					for(unsigned int i=0; i < LV->neighbors.size();i++)
					{
						if(LV->neighbors[i].type==0)
							existActiveNeighbors++;
					}
					if(specialInteractiveNeghborReq&& existActiveNeighbors>=activeNeighbors)
						specialInteractiveNeghborReq=false; // we use this only for one time after -AGH
					if(specialInteractiveNeghborReq)
						Timer=0;
					if(connectDirectToServer==false && Timer<=0)
					{
						if(simpleMeshType==0&& LV->neighbors.size() < neighborNum)
						{
							neighborRequest(activeNeighbors-existActiveNeighbors);
							Timer=2;
						}
						else if(simpleMeshType==1&&existActiveNeighbors< activeNeighbors)
						{
							neighborRequest(activeNeighbors-existActiveNeighbors);
							Timer=2;
						}
						else
							Timer=5;
					}
					else if(Timer<=0)
					{
						Timer=5;
						if(existActiveNeighbors<=1)
							Timer=1;
					}
				}
				Timer--;
				 if(LV->StructureImproving==true && improvmentStructureStart==false)
				 {
					 int index =findNeighborIndex(LV->improveNeighbor,LV->neighbors);// may peer exit before this
					 if(index>-1)
					 {
						 improvementMessage* improveRequest=new improvementMessage("IMPROVE_REQUEST");
						 improveRequest->setCommand(IMPROVE_REQUEST);
						 improveRequest->setFilmId(LV->currentFilmId);
						 improveRequest->setCascadeNumber(LV->cascadeNumber);
						 improveRequest->setScore(LV->improveScore);
						 improveRequest->setSrcNode(thisNode);
						 improveRequest->setBitLength(IMPROVEMENT_L(msg));
						 sendMessageToUDP(LV->improveNeighbor,improveRequest);
						 stat_improvementREQ++;
						 stat_improvementREQBytesSent+=improveRequest->getByteLength();
						 improvmentStructureStart=true;
						// scheduleAt(simTime()+improvementProcessTimeLenght,improvementProcessTimeLenghtTimer);
					 }
					 else
					 {
						 LV->StructureImproving=false;
					 }
				 }
				 if(LV->emergency==true && emergency==false)
				 {
					if(!isInVector(serverAddress,LV->neighbors))
					{
						MeshMessage* emergencyJoinRequest = new MeshMessage("emergencyJoinRequest");
						emergencyJoinRequest->setCommand(EMERGENCY_jOIN_REQUEST);
						emergencyJoinRequest->setSrcNode(thisNode);
						emergencyJoinRequest->setCascadeNumber(LV->cascadeNumber);
						emergencyJoinRequest->setBitLength(MESHMESSAGE_L(msg));
						emergencyJoinRequest->setFilmId(LV->currentFilmId);
						emergencyJoinRequest->setFilmStartTime(startTime);
						sendMessageToUDP(serverAddress,emergencyJoinRequest);
						neighbor ne;
						ne.address=serverAddress;
						ne.type=0;
						ne.endTime=-1;
						requestedNeighbors.push_back(ne);
						emergency=true;
					}
				 }
				 if(LV->emergency==false && emergency==true)
				 {
					 MeshMessage* disconnectMsg = new MeshMessage("disconnect");
					 disconnectMsg->setCascadeNumber(LV->cascadeNumber);
					 disconnectMsg->setCommand(EMERGENCY_DISCONNECT);
					 disconnectMsg->setSrcNode(thisNode);
					 disconnectMsg->setFilmId(LV->currentFilmId);
					 disconnectMsg->setBitLength(MESHMESSAGE_L(msg));
					 sendMessageToUDP(serverAddress,disconnectMsg);
					 deleteOverlayNeighborArrow(serverAddress);
					 deleteVector(serverAddress,LV->neighbors);
					 VideoMessage* videoMsg = new VideoMessage();
					 videoMsg->setCommand(NEIGHBOR_LEAVE);
					 videoMsg->setSrcNode(serverAddress);
					 send(videoMsg,"appOut");
					 emergency=false;
				 }
				if( LV->hopCount!=-1)
				{
					if(connectDirectToServer)
						hopCount=1;
					else if (isheadnode)
						hopCount=2;
					else if(connectToHeadNode)
						hopCount=3;

					else if(LV->hopCount>3)
						hopCount=LV->hopCount;
				}
				if(LV->WatchExpired==true )
				{
					 cancelAllRpcs();
					 cancelEvent(meshJoinRequestTimer);
					 cancelEvent(remainNotificationTimer);
					 neighborSelelctionStart=false;
					 int prob=bernoulli(changeProbability,0);
					 int unwatchfilmCount=0;
					 for(int i=0;i<numberOfVideos;i++)
					 {
					 	if(LV->watchFilm[i]==false)
					 		unwatchfilmCount++;
					 }
					 if(unwatchfilmCount==0)
						 prob=0;
					 if (prob==0)
					 {
						 cancelEvent(improvementProcessTimeLenghtTimer);
						 int exitType=bernoulli(percentOfNotGracefullExit,0);
						if(exitType==0)
							handleNodeGracefulLeaveNotification();
						else
							 nodeLeave();
					 }
					 else if (prob==1)
					 {
						 LV->WatchExpired=false;
						 changeFilm();
					 }
				 }
				 else
				 {
					scheduleAt(simTime()+1,meshJoinRequestTimer);
				 }
			}
		}
		else if(msg == remainNotificationTimer)
		{
			if(recivelistfilm==false)
			{
				TrackerMessage* RequestListOfFilm = new TrackerMessage("Request_ListOfFilm");
				RequestListOfFilm->setCommand(Request_ListOfFilm);
				RequestListOfFilm->setSrcNode(thisNode);
				RequestListOfFilm->setIsServer(false);
				sendMessageToUDP(trackerAddress,RequestListOfFilm);
			}
			else if(meshStructure==0|| LV->cascadeNumber!=-1)
			{
				TrackerMessage* remainNotification = new TrackerMessage("remainNotification");
				remainNotification->setCommand(REMAIN_NEIGHBOR);
				remainNotification->setSrcNode(thisNode);
				remainNotification->setSelectedFilmNum(LV->currentFilmId);
				remainNotification->setConnectDirectToServer(connectDirectToServer);
				remainNotification->setIsHeadnode(isheadnode);
				remainNotification->setCascadenumber(LV->cascadeNumber);
				remainNotification->setHopCount(hopCount);
				remainNotification->setIsServer(false);
				if(simpleMeshType==0)
				{
					remainNotification->setRemainNeighbor(activeNeighbors - LV->neighbors.size() + passiveNeighbors);
				}
				else
				{
					int existPasiveNeighbors=0;
					for(unsigned int i=0; i < LV->neighbors.size();i++)
					{
						if(LV->neighbors[i].type==1)
							existPasiveNeighbors++;
					}
					remainNotification->setRemainNeighbor( passiveNeighbors-existPasiveNeighbors);
				}

				sendMessageToUDP(trackerAddress,remainNotification);
			}
			scheduleAt(simTime()+2,remainNotificationTimer);
		}
		else if(msg==improvementProcessTimeLenghtTimer)
		{
			improvmentStructureStart=false;
			LV->StructureImproving=false;
		}
		else
			MeshOverlay::handleAppMessage(msg);
	}
	catch(std::exception& e)
	{
		std::cout << "time: " << simTime()<< "handleTimerEvent error in Peers mesh "<<getParentModule()->getParentModule()->getFullName() <<  std::endl;
	}
}
void PeersMesh::handleUDPMessage(BaseOverlayMessage* msg)
{
try
{
	if (dynamic_cast<EncapSelectFilm*>(msg) != NULL)
	{
		SelectFilm* listOfFilmsMsg = check_and_cast<SelectFilm*>(msg->decapsulate());
		numberOfVideos=listOfFilmsMsg->getNumberOfFilms();
		int SelectedVideoNumber=selectingOneFilm(0,numberOfVideos);
		if(SelectedVideoNumber==-6)
		{
			// peer watch all films so it exit
			delete listOfFilmsMsg;
			return;
		}
		LV->currentFilmId = SelectedVideoNumber;
		recivelistfilm=true;
		int filmlength=listOfFilmsMsg->getLenthOfFilmsSec(LV->currentFilmId);
		startTime=0;
		timeWatchingFilm(filmlength);
		if(expireTime<1)
		{
			LV->WatchExpired=true;
		}
		else
		{
			VideoProperty* vmsg=new VideoProperty("Video Property");
			vmsg->setSelectedFilms(LV->currentFilmId);
			vmsg->setSelectedStartTimeSec(startTime);
			vmsg->setLenthOfFilmsSec(filmlength);
			vmsg->setEndTimeWatching(expireTime);
			vmsg->setInteractiveTime(interactiveTime);
			MeshOverlay::handleAppMessage(vmsg);
		}
		cancelEvent(meshJoinRequestTimer);
		scheduleAt(simTime(),meshJoinRequestTimer);
		delete msg;
		delete listOfFilmsMsg;
	}
	/*if (dynamic_cast<EncapVideoMessage*>(msg) != NULL)
	{
		VideoMessage* listOfFilmsMsg=  check_and_cast<VideoMessage*> (msg->decapsulate());
		EncapVideoMessage* LOF = new EncapVideoMessage ("chank-req");
		LOF->encapsulate(listOfFilmsMsg->dup());
		MeshOverlay::handleUDPMessage(LOF);
		delete listOfFilmsMsg;
	}*/
	else if (dynamic_cast<TrackerMessage*>(msg) != NULL)
	{
		TrackerMessage* trackerMsg = check_and_cast<TrackerMessage*>(msg);
		if(trackerMsg->getCommand() == NEIGHBOR_RESPONSE && neighborSelelctionStart==true &&!connectDirectToServer)
		{
			LV->cascadeNumber=trackerMsg->getCascadenumber();
			MeshMessage* joinRequest = new MeshMessage("joinRequest");
			joinRequest->setCommand(JOIN_REQUEST);
			joinRequest->setSrcNode(thisNode);
			joinRequest->setIsHeadNode(isheadnode);
			joinRequest->setCascadeNumber(LV->cascadeNumber);
			joinRequest->setBitLength(MESHMESSAGE_L(msg));
			joinRequest->setFilmId(LV->currentFilmId);
			joinRequest->setFilmStartTime(startTime);
			joinRequest->setSpecialInteractiveNeghborReq(trackerMsg->getSpecialInteractiveNeghborReq());// in structiure 1 &2 use for intractive peers
			serverAddress=trackerMsg->getNeighbors(0);// we used server address to connect to server in emergency times-AGH
			for(unsigned int i=1 ; i <trackerMsg->getNeighborsArraySize() ; i++)
			{
				if(!isInVector(trackerMsg->getNeighbors(i),LV->neighbors))
				{
					sendMessageToUDP(trackerMsg->getNeighbors(i),joinRequest->dup());
					stat_joinREQ++;
					stat_joinREQBytesSent+=joinRequest->getByteLength();
					neighbor ne;
					ne.address=trackerMsg->getNeighbors(i);
					ne.type=0;
					ne.endTime=-1;
					requestedNeighbors.push_back(ne);
				}
			}
			delete joinRequest;
		}
		else if (trackerMsg->getCommand() == CascadeNumRequest)
		{
		// in this system we only have seek forward so... // this is work only in my structure and do not in simple mesh-AGH
			cancelEvent(meshJoinRequestTimer);
			cancelEvent(remainNotificationTimer);
			if(LV->cascadeNumber!=trackerMsg->getCascadenumber())
			{
				disconnectFromPreviouseNeighbors();
				std::cout <<"time: " << simTime()<< " seek forward from cascade "<<LV->cascadeNumber<<" to "<< trackerMsg->getCascadenumber() <<getParentModule()->getParentModule()->getFullName() <<std::endl;
				LV->cascadeNumber=trackerMsg->getCascadenumber();
				hopCount=-1;
				connectDirectToServer=false;
				isheadnode=false;
				connectToHeadNode=false;
				haveChangecascadeForInteractive=true;
				MeshMessage* joinRequest = new MeshMessage("joinRequest");
				joinRequest->setCommand(JOIN_REQUEST);
				joinRequest->setSrcNode(thisNode);
				joinRequest->setCascadeNumber(LV->cascadeNumber);
				joinRequest->setBitLength(MESHMESSAGE_L(msg));
				joinRequest->setFilmId(LV->currentFilmId);
				joinRequest->setFilmStartTime(startTime);
				joinRequest->setSpecialInteractiveNeghborReq(trackerMsg->getSpecialInteractiveNeghborReq());// in structiure 1 & 2 use for intractive peers
				// we set on tracker a non interactive neighbor at end of list- this neighbor have more important-AGH
				if(trackerMsg->getNeighborsArraySize()>0)
				{
					sendMessageToUDP(trackerMsg->getNeighbors(trackerMsg->getNeighborsArraySize()-1),joinRequest->dup());
					stat_joinREQ++;
					stat_joinREQBytesSent+=joinRequest->getByteLength();
					for(unsigned int i=0 ; i <trackerMsg->getNeighborsArraySize()-1 ; i++)
					{
						if(!isInVector(trackerMsg->getNeighbors(i),LV->neighbors))
						{
							sendMessageToUDP(trackerMsg->getNeighbors(i),joinRequest->dup());
							stat_joinREQ++;
							stat_joinREQBytesSent+=joinRequest->getByteLength();
						}
					}
				}
				delete joinRequest;
				//if(trackerMsg->getNeighborsArraySize()>=activeNeighbors)
				//	specialInteractiveNeghborReq=false; // we use this only when neighbor is not enough -AGH
			}
			neighborSelelctionStart=true;
			LV->improveLock=false;//AGH - cascade change(if)- all old neighbor disconnect-so it can improve
			LV->interactive=false;
			LV->needNewneighbor=false;
			Timer=0;
			scheduleAt(simTime(),meshJoinRequestTimer);
			scheduleAt(simTime()+neighborNotificationPeriod,remainNotificationTimer);
		}
		else if (trackerMsg->getCommand() == You_UnRegistered)
		{
			if(!LV->peerExit)
			{
				isRegistered=false;
				isheadnode=false;
				connectDirectToServer=false;
				if(connectDirectToServer)
				{
					disconnectFromPreviouseNeighbors();
				}
			}
		}
		delete trackerMsg;
	}
	else if (dynamic_cast<improvementMessage*>(msg) != NULL)
	{
		improvementMessage* improvemsg=check_and_cast<improvementMessage*>(msg);
		if(improvemsg->getCommand()== IMPROVE_REQUEST)
		{
			if(improvmentStructureStart==false &&improvemsg->getFilmId()==LV->currentFilmId && improvemsg->getCascadeNumber()==LV->cascadeNumber && !LV->peerExit&& LV->improveLock==false)
			{
				improvmentStructureStart=true;
				LV->StructureImproving=true;
				improvementMessage* improveResponse=new improvementMessage("IMPROVE_RESPONSE");
				improveResponse->setCommand(IMPROVE_RESPONSE);
				improveResponse->setFilmId(LV->currentFilmId);
				improveResponse->setCascadeNumber(LV->cascadeNumber);
				improveResponse->setSrcNode(thisNode);
				improveResponse->setNeighborsArraySize(LV->neighbors.size());
				for(unsigned int i=0 ; i< LV->neighbors.size() ; i++ )
					improveResponse->setNeighbors(i,LV->neighbors[i].address);
				improveResponse->setBitLength(IMPROVEMENT_L(msg)+TRANSPORTADDRESS_L*LV->neighbors.size());
				sendMessageToUDP(improvemsg->getSrcNode(),improveResponse);
				stat_improvementRSP++;
				stat_improvementRSPBytesSent+=improveResponse->getByteLength();
				//scheduleAt(simTime()+improvementProcessTimeLenght,improvementProcessTimeLenghtTimer);
			}
			else
			{
				improvementMessage* improveDeny=new improvementMessage("IMPROVE_DENY");
				improveDeny->setCommand(IMPROVE_DENY);
				improveDeny->setBitLength(IMPROVEMENT_L(msg));
				sendMessageToUDP(improvemsg->getSrcNode(),improveDeny);
				stat_improvementDNY++;
				stat_improvementDNYBytesSent+=improveDeny->getByteLength();
			}
		}
		else if(improvemsg->getCommand()== IMPROVE_RESPONSE)
		{
			//cancelEvent(improvementProcessTimeLenghtTimer);
			if(improvmentStructureStart==false)
			{
				std::cout << " Error improvement Process Time Length is low "<<getParentModule()->getParentModule()->getFullName() <<  std::endl;
			}
			if(improvemsg->getFilmId()==LV->currentFilmId && improvemsg->getCascadeNumber()==LV->cascadeNumber && !LV->peerExit&& LV->improveLock==false)
			{
				std::cout << " improving "<<getParentModule()->getParentModule()->getFullName() <<" to"<<improvemsg->getSrcNode()<<  std::endl;
				// send list of neighbors
				improvementMessage* improveAck=new improvementMessage("IMPROVE_ACK");
				improveAck->setCommand(IMPROVE_ACK);
				improveAck->setNeighborsArraySize(LV->neighbors.size());
				for(unsigned int i=0 ; i< LV->neighbors.size() ; i++ )
					improveAck->setNeighbors(i,LV->neighbors[i].address);
				improveAck->setSrcNode(thisNode);
				improveAck->setBitLength(IMPROVEMENT_L(msg)+TRANSPORTADDRESS_L*LV->neighbors.size());
				sendMessageToUDP(improvemsg->getSrcNode(),improveAck);
				LV->ppreviousimproveNeighbor=LV->previousimproveNeighbor;
				LV->previousimproveNeighbor=improvemsg->getSrcNode();
				stat_improvementACK++;
				stat_improvementACKBytesSent+=improveAck->getByteLength();
				// unregister
				selfUnRegister();
				connectDirectToServer=false;
				isheadnode=false;
				connectToHeadNode=false;
				// connect to new neighbor
				improvementchangePlaceMessage* changeNeighbor=new improvementchangePlaceMessage("CHANGE_PLACE_REQUEST");
				changeNeighbor->setCommand(CHANGE_PLACE_REQUEST);
				changeNeighbor->setFilmId(LV->currentFilmId);
				changeNeighbor->setCascadeNumber(LV->cascadeNumber);
				changeNeighbor->setSrcNode(thisNode);
				changeNeighbor->setPreviusNode(improvemsg->getSrcNode());
				changeNeighbor->setBitLength(IMPROVEMENTCHANGEPLACE_L(msg));
				for(unsigned int i=0 ; i< improvemsg->getNeighborsArraySize(); i++ )
					if(improvemsg->getNeighbors(i)!=thisNode)
					{
						sendMessageToUDP(improvemsg->getNeighbors(i),changeNeighbor->dup());
						neighbor ne;
						ne.address=improvemsg->getNeighbors(i);
						ne.type=0;
						ne.endTime=-1;
						ne.lastnotifyed=simTime().dbl()+10;
						requestedNeighbors.push_back(ne);
						stat_ChangePlaceREQ++;
						stat_ChangePlaceREQBytesSent=changeNeighbor->getByteLength();
					}
				delete changeNeighbor;
				// change type of improve partner neighbor-AGH
				int	index=findNeighborIndex(improvemsg->getSrcNode(),LV->neighbors);
				if(LV->neighbors[index].type==0)
					LV->neighbors[index].type=1;
				else
					LV->neighbors[index].type=0;
				scheduleAt(simTime()+improvementProcessTimeLenght,improvementProcessTimeLenghtTimer);
			}
			else
			{
				improvementMessage* improveDeny=new improvementMessage("IMPROVE_DENY");
				improveDeny->setCommand(IMPROVE_DENY);
				improveDeny->setBitLength(IMPROVEMENT_L(msg));
				sendMessageToUDP(improvemsg->getSrcNode(),improveDeny);
				stat_improvementDNY++;
				stat_improvementDNYBytesSent+=improveDeny->getByteLength();
				cancelEvent(improvementProcessTimeLenghtTimer);
				improvmentStructureStart=false;
				LV->StructureImproving=false;
			}
		}
		else if(improvemsg->getCommand()== IMPROVE_ACK)
		{
			//cancelEvent(improvementProcessTimeLenghtTimer);
			//for test
			if(improvmentStructureStart==false)
			{
				std::cout << " Error improvement Process Time Length is low "<<getParentModule()->getParentModule()->getFullName() <<  std::endl;
			}
			stat_numberOfImprovement++;
			// unregister
			if(connectDirectToServer)
			{
				selfUnRegister();
				connectDirectToServer=false;
				isheadnode=true;
				connectToHeadNode=false;
			}
			else if(isheadnode)
			{
				connectDirectToServer=false;
				isheadnode=false;
				connectToHeadNode=true;
			}
			else if(connectToHeadNode)
			{
				connectDirectToServer=false;
				isheadnode=false;
				connectToHeadNode=false;
			}
			if(!isRegistered)
				selfRegister(connectDirectToServer);
			// connect to new neighbor
			improvementchangePlaceMessage* changeNeighbor=new improvementchangePlaceMessage("CHANGE_PLACE_REQUEST");
			changeNeighbor->setCommand(CHANGE_PLACE_REQUEST);
			changeNeighbor->setFilmId(LV->currentFilmId);
			changeNeighbor->setCascadeNumber(LV->cascadeNumber);
			changeNeighbor->setSrcNode(thisNode);
			changeNeighbor->setPreviusNode(improvemsg->getSrcNode());
			changeNeighbor->setBitLength(IMPROVEMENTCHANGEPLACE_L(msg));
			for(unsigned int i=0 ; i< improvemsg->getNeighborsArraySize(); i++ )
				if(improvemsg->getNeighbors(i)!=thisNode)
				{
					sendMessageToUDP(improvemsg->getNeighbors(i),changeNeighbor->dup());
					neighbor ne;
					ne.address=improvemsg->getNeighbors(i);
					ne.type=0;
					ne.endTime=-1;
					ne.lastnotifyed=simTime().dbl()+10;
					requestedNeighbors.push_back(ne);
					stat_ChangePlaceREQ++;
					stat_ChangePlaceREQBytesSent=changeNeighbor->getByteLength();
				}
			delete changeNeighbor;
			// change type of improve partner neighbor-AGH
			int	index=findNeighborIndex(improvemsg->getSrcNode(),LV->neighbors);
			if(LV->neighbors[index].type==0)
				LV->neighbors[index].type=1;
			else
				LV->neighbors[index].type=0;
			scheduleAt(simTime()+improvementProcessTimeLenght,improvementProcessTimeLenghtTimer);
		}
		else if(improvemsg->getCommand()== IMPROVE_DENY)
		{
			cancelEvent(improvementProcessTimeLenghtTimer);
			improvmentStructureStart=false;
			LV->StructureImproving=false;
			LV->improveDeny=true;
		}
		delete improvemsg;
	}
	else if (dynamic_cast<improvementchangePlaceMessage*>(msg) != NULL)
	{
		improvementchangePlaceMessage* ChangePlacemsg = check_and_cast<improvementchangePlaceMessage*>(msg);
		if(ChangePlacemsg->getCommand()==CHANGE_PLACE_REQUEST)
		{
			//int index=-1;
			//if(LV->neighbors.size()>0 )//&& !ChangePlacemsg->getPreviusNode().isUnspecified())
			int	index=findNeighborIndex(ChangePlacemsg->getPreviusNode(),LV->neighbors);
			if(index>-1 && LV->watchFilm[ChangePlacemsg->getFilmId()]==true && !LV->peerExit && LV->improveLock==false && (ChangePlacemsg->getCascadeNumber()==LV->cascadeNumber||ChangePlacemsg->getCascadeNumber()==cascadeNumber-1 ||  ChangePlacemsg->getCascadeNumber() ==cascadeNumber+1))
			{
				if(isInVector(ChangePlacemsg->getSrcNode(),LV->neighbors))
				{
					int index2=findNeighborIndex(ChangePlacemsg->getSrcNode(),LV->neighbors);
					improvementchangePlaceMessage* changeNeighborType = new improvementchangePlaceMessage("changeNeighborType");
					changeNeighborType->setCommand(CHANGE_NEIGHBOR_TYPE);
					changeNeighborType->setNeighborType(LV->neighbors[index].type);
					changeNeighborType->setSrcNode(thisNode);
					changeNeighborType->setBitLength(IMPROVEMENTCHANGEPLACE_L(msg));
					sendMessageToUDP(ChangePlacemsg->getSrcNode(),changeNeighborType->dup());
					changeNeighborType->setNeighborType(LV->neighbors[index2].type);
					sendMessageToUDP(ChangePlacemsg->getPreviusNode(),changeNeighborType);
					stat_ChangePlaceRSP+=2;
					stat_ChangePlaceRSPBytesSent+=2*changeNeighborType->getByteLength();
					int temp=LV->neighbors[index2].type;
					LV->neighbors[index2].type=LV->neighbors[index].type;
					LV->neighbors[index].type=temp;
				}
				else
				{
					// add this node to its neighbor
					neighbor ne;
					ne.address=ChangePlacemsg->getSrcNode();
					ne.type=LV->neighbors[index].type;
					ne.endTime=-1;
					ne.cascadeNumber=ChangePlacemsg->getCascadeNumber();
					ne.lastnotifyed=simTime().dbl()+10;
					LV->neighbors.push_back(ne);
					// disconnect from previous peer
					disconnectFromNode(LV->neighbors[index].address,LV->cascadeNumber);
					// send response
					improvementchangePlaceMessage* changePlaceResponse = new improvementchangePlaceMessage("changePlaceResponse");
					changePlaceResponse->setCommand(CHANGE_PLACE_RESPONSE);
					changePlaceResponse->setFilmId(LV->currentFilmId);
					changePlaceResponse->setRecieveTime(LV->recieveTime);
					changePlaceResponse->setTimestamp(simTime());
					changePlaceResponse->setDownBandwidth(downBandwidth/1024);
					changePlaceResponse->setIsServer(false);
					changePlaceResponse->setConnectDirectToServer(connectDirectToServer);
					changePlaceResponse->setCascadeNumber(LV->cascadeNumber);
					changePlaceResponse->setIsHeadNode(isheadnode);
					if(ne.type==0)
						changePlaceResponse->setNeighborType(1);
					else
						changePlaceResponse->setNeighborType(0);
					changePlaceResponse->setSrcNode(thisNode);
					changePlaceResponse->setBitLength(IMPROVEMENTCHANGEPLACE_L(msg));
					sendMessageToUDP(ChangePlacemsg->getSrcNode(),changePlaceResponse);
					stat_ChangePlaceRSP+=1;
					stat_ChangePlaceRSPBytesSent+=changePlaceResponse->getByteLength();
				}
			}
			else if(index<=-1 && LV->neighbors.size() < neighborNum && !LV->peerExit && LV->improveLock==false && (ChangePlacemsg->getCascadeNumber()==LV->cascadeNumber||ChangePlacemsg->getCascadeNumber()==cascadeNumber-1 ||  ChangePlacemsg->getCascadeNumber() ==cascadeNumber+1))
			{
				if(!isInVector(ChangePlacemsg->getSrcNode(),LV->neighbors))
				{
					// add this node to its neighbor
					neighbor ne;
					ne.address=ChangePlacemsg->getSrcNode();
					ne.type=1;
					ne.endTime=-1;
					ne.cascadeNumber=ChangePlacemsg->getCascadeNumber();
					ne.lastnotifyed=simTime().dbl()+10;
					LV->neighbors.push_back(ne);
					// send response
					improvementchangePlaceMessage* changePlaceResponse = new improvementchangePlaceMessage("changePlaceResponse");
					changePlaceResponse->setCommand(CHANGE_PLACE_RESPONSE);
					changePlaceResponse->setFilmId(LV->currentFilmId);
					changePlaceResponse->setRecieveTime(LV->recieveTime);
					changePlaceResponse->setTimestamp(simTime());
					changePlaceResponse->setDownBandwidth(downBandwidth/1024);
					changePlaceResponse->setIsServer(false);
					changePlaceResponse->setConnectDirectToServer(connectDirectToServer);
					changePlaceResponse->setCascadeNumber(LV->cascadeNumber);
					changePlaceResponse->setIsHeadNode(isheadnode);
					changePlaceResponse->setNeighborType(0);
					changePlaceResponse->setSrcNode(thisNode);
					changePlaceResponse->setBitLength(IMPROVEMENTCHANGEPLACE_L(msg));
					sendMessageToUDP(ChangePlacemsg->getSrcNode(),changePlaceResponse);
					stat_ChangePlaceRSP+=1;
					stat_ChangePlaceRSPBytesSent+=changePlaceResponse->getByteLength();
				}
			}
			else
			{
				improvementchangePlaceMessage* changePlaceDeny = new improvementchangePlaceMessage("changePlaceDeny");
				changePlaceDeny->setCommand(CHANGE_PLACE_DENY);
				changePlaceDeny->setSrcNode(thisNode);
				changePlaceDeny->setBitLength(IMPROVEMENTCHANGEPLACE_L(msg));
				sendMessageToUDP(ChangePlacemsg->getSrcNode(),changePlaceDeny);
				stat_ChangePlaceDNY++;
				stat_ChangePlaceDNYBytesSent+=changePlaceDeny->getByteLength();
			}
		}
		if(ChangePlacemsg->getCommand()==CHANGE_NEIGHBOR_TYPE)
		{
			if(isInVector(ChangePlacemsg->getSrcNode(),requestedNeighbors)&& !LV->peerExit)
			{
				deleteVector(ChangePlacemsg->getSrcNode(),requestedNeighbors);
				int index=findNeighborIndex(ChangePlacemsg->getSrcNode(),LV->neighbors);
				if(index>-1)
				{
					LV->neighbors[index].type=ChangePlacemsg->getNeighborType();
				}
			}
		}
		if(ChangePlacemsg->getCommand()==CHANGE_PLACE_DENY)
		{
			if(isInVector(ChangePlacemsg->getSrcNode(),requestedNeighbors)&& !LV->peerExit)
			{
				deleteVector(ChangePlacemsg->getSrcNode(),requestedNeighbors);
			}
		}
		else if(ChangePlacemsg->getCommand()==CHANGE_PLACE_RESPONSE)
		{
			if(isInVector(ChangePlacemsg->getSrcNode(),requestedNeighbors)&& !LV->peerExit)
			{
				deleteVector(ChangePlacemsg->getSrcNode(),requestedNeighbors);
				if(ChangePlacemsg->getIsServer()==true)
				{
					if(meshStructure==1||meshStructure==2)
					{
							selfUnRegister();
							disconnectFromPreviouseActiveNeighbors();
					}
					connectDirectToServer=true;
					isheadnode=false;
					connectToHeadNode=false;
					hopCount=1;
				}
				else if(ChangePlacemsg->getConnectDirectToServer()==true )
				{
					isheadnode=true;
					connectDirectToServer=false;
					connectToHeadNode=false;
				}
				else if(ChangePlacemsg->getIsHeadNode()==true&& ChangePlacemsg->getCascadeNumber()!=LV->cascadeNumber)
				{
					isheadnode=true;
					connectDirectToServer=false;
					connectToHeadNode=false;
				}
				else if(ChangePlacemsg->getIsHeadNode()==true )
				{
					connectToHeadNode=true;
					connectDirectToServer=false;
					isheadnode=false;
				}
				/*if(hopCount==-1 || hopCount>ChangePlacemsg->getHopCount())
				{
					hopCount=ChangePlacemsg->getHopCount()+1;
				}*/
				neighbor ne;
				ne.address=ChangePlacemsg->getSrcNode();
				ne.type=ChangePlacemsg->getNeighborType();
				ne.endTime=-1;// in change place we dont change use pre video peers-AGH
				ne.cascadeNumber=ChangePlacemsg->getCascadeNumber();
				ne.lastnotifyed=simTime().dbl()+10;
				LV->neighbors.push_back(ne);
				if(!isRegistered)
				{
					selfRegister(connectDirectToServer);

				}
				showOverlayNeighborArrow(ChangePlacemsg->getSrcNode(), false, "m=m,50,0,50,0;ls=red,1");
				if(ne.type==1)
				{
					// in below we send message to upper layer that send video for node (AGH)
					CreateStream* givefilm=new  CreateStream("Create Stream");
					givefilm->setDownBandwith(ChangePlacemsg->getDownBandwidth());// in this case we use end time for downBandwidth
					givefilm->setDelay(simTime().dbl()-ChangePlacemsg->getTimestamp().dbl());
					givefilm->setSelectedFilm(ChangePlacemsg->getFilmId());
					givefilm->setSrcNode(ChangePlacemsg->getSrcNode());
					givefilm->setSelectedStartTimeSec(ChangePlacemsg->getRecieveTime());
					MeshOverlay::handleAppMessage(givefilm);
				}
				improvementchangePlaceMessage* changePlaceAck = new improvementchangePlaceMessage("changePlaceAck");
				changePlaceAck->setCommand(CHANGE_PLACE_ACK);
				changePlaceAck->setFilmId(LV->currentFilmId);
				changePlaceAck->setNeighborType(ne.type);
				changePlaceAck->setRecieveTime(LV->recieveTime);
				changePlaceAck->setTimestamp(simTime());
				changePlaceAck->setDownBandwidth(downBandwidth/1024);
				changePlaceAck->setSrcNode(thisNode);
				changePlaceAck->setIsServer(false);
				changePlaceAck->setBitLength(IMPROVEMENTCHANGEPLACE_L(msg));
				sendMessageToUDP(ChangePlacemsg->getSrcNode(),changePlaceAck);
				stat_ChangePlaceACK++;
				stat_ChangePlaceACKBytesSent+=changePlaceAck->getByteLength();
			}
		}
		else if(ChangePlacemsg->getCommand()==CHANGE_PLACE_ACK)
		{
			showOverlayNeighborArrow(ChangePlacemsg->getSrcNode(), false, "m=m,50,0,50,0;ls=red,1");
			// in below we send message to upper layer that send video for node (AGH)
			if(ChangePlacemsg->getNeighborType()==0)
			{
				CreateStream* givefilm=new  CreateStream("Create Stream");
				givefilm->setDownBandwith(ChangePlacemsg->getDownBandwidth());// in this case we use end time for downBandwidth
				givefilm->setDelay(simTime().dbl()-ChangePlacemsg->getTimestamp().dbl());
				givefilm->setSelectedFilm(ChangePlacemsg->getFilmId());
				givefilm->setSrcNode(ChangePlacemsg->getSrcNode());
				givefilm->setSelectedStartTimeSec(ChangePlacemsg->getRecieveTime());
				MeshOverlay::handleAppMessage(givefilm);
			}
		}
		delete ChangePlacemsg;
	}
	else if (dynamic_cast<MeshMessage*>(msg) != NULL)
	{
		MeshMessage* Meshmsg = check_and_cast<MeshMessage*>(msg);
		if (Meshmsg->getCommand() == EMERGENCY_RESPONSE)
		{
			if(isInVector(Meshmsg->getSrcNode(),requestedNeighbors)&& !LV->peerExit)
			{
				deleteVector(Meshmsg->getSrcNode(),requestedNeighbors);
				if(neighborSelelctionStart==true )
				{
					neighbor ne;
					ne.address=Meshmsg->getSrcNode();
					ne.type=0;
					ne.endTime=-1;
					ne.cascadeNumber=-2; //its a server
					ne.lastnotifyed=simTime().dbl()+10;
					LV->neighbors.push_back(ne);
					showOverlayNeighborArrow(Meshmsg->getSrcNode(), false, "m=m,50,0,50,0;ls=red,1");
					MeshMessage* emerjencyAck = new MeshMessage("emerjencyAck");
					emerjencyAck->setCommand(EMERGENCY_ACK);
					emerjencyAck->setSrcNode(thisNode);
					emerjencyAck->setFilmId(LV->currentFilmId);
					emerjencyAck->setBitLength(MESHMESSAGE_L(msg));
					sendMessageToUDP(Meshmsg->getSrcNode(),emerjencyAck);
				}
				else
				{
					MeshMessage* emergencyDeny = new MeshMessage("emergencyDeny");
					emergencyDeny->setCommand(EMERGENCY_DENY);
					emergencyDeny->setSrcNode(thisNode);
					emergencyDeny->setIsServer(false);
					emergencyDeny->setCascadeNumber(LV->cascadeNumber);
					emergencyDeny->setBitLength(MESHMESSAGE_L(msg));
					sendMessageToUDP(Meshmsg->getSrcNode(),emergencyDeny);
				}
			}
		}
		else if (Meshmsg->getCommand() == JOIN_REQUEST)
		{
			if(!LV->peerExit)
			{
				if(neighborSelelctionStart==true && LV->watchFilm[Meshmsg->getFilmId()]==true)
				{
					if((Meshmsg->getFilmId()!=LV->currentFilmId && usePreVideo==false))
					{
							joinDeny(false,Meshmsg->getSrcNode());
					}
					else
					{
						int existPassiveNeighbors=0;
						int existInCascadePasiveN=0;
						int existOutCascadePasive=0;
						for(unsigned int i=0; i < LV->neighbors.size();i++)
						{
							if(LV->neighbors[i].type==1)
							{
								existPassiveNeighbors++;
								if(LV->neighbors[i].cascadeNumber==LV->cascadeNumber)
									existInCascadePasiveN++;
								else
									existOutCascadePasive++;
							}
						}
						if(meshStructure==0)
						{
							if(simpleMeshType==1 && existPassiveNeighbors>=passiveNeighbors)
							{
								joinDeny(false,Meshmsg->getSrcNode());
							}
							else if(LV->neighbors.size() < neighborNum||Meshmsg->getSpecialInteractiveNeghborReq()==true)
							{
								neighbor ne;
								ne.address=Meshmsg->getSrcNode();
								ne.type=1;
								ne.endTime=-1;
								ne.cascadeNumber=0;
								ne.lastnotifyed=simTime().dbl()+10;
								LV->neighbors.push_back(ne);
								joinResponse(false,watchFilmEndTime[Meshmsg->getFilmId()],Meshmsg->getSrcNode(),Meshmsg->getSpecialInteractiveNeghborReq());
							}
							else
							{
								joinDeny(false,Meshmsg->getSrcNode());
							}
						}
						else if(meshStructure==1)
						{
							if(Meshmsg->getSpecialInteractiveNeghborReq()==true)
									existPassiveNeighbors-=2;
							if((simpleMeshType==1 && existPassiveNeighbors>=passiveNeighbors)||(Meshmsg->getCascadeNumber()<LV->cascadeNumber-1||Meshmsg->getCascadeNumber()>LV->cascadeNumber+1))
							{
								joinDeny(false,Meshmsg->getSrcNode());
							}
							else if(LV->neighbors.size() < neighborNum||Meshmsg->getSpecialInteractiveNeghborReq()==true)
							{
								neighbor ne;
								ne.address=Meshmsg->getSrcNode();
								ne.type=1;
								ne.endTime=-1;
								ne.cascadeNumber=Meshmsg->getCascadeNumber();
								ne.lastnotifyed=simTime().dbl()+10;
								LV->neighbors.push_back(ne);
								joinResponse(false,watchFilmEndTime[Meshmsg->getFilmId()],Meshmsg->getSrcNode(),Meshmsg->getSpecialInteractiveNeghborReq());
							}
						}
						else if(meshStructure==2)
						{
							if(Meshmsg->getSpecialInteractiveNeghborReq()==true)
								existPassiveNeighbors-=2;
							if((simpleMeshType==1 && existPassiveNeighbors>=passiveNeighbors)||(Meshmsg->getCascadeNumber()!=LV->cascadeNumber))
							{
								joinDeny(false,Meshmsg->getSrcNode());
							}
							else if(LV->neighbors.size() < neighborNum||Meshmsg->getSpecialInteractiveNeghborReq()==true)
							{
								neighbor ne;
								ne.address=Meshmsg->getSrcNode();
								ne.type=1;
								ne.endTime=-1;
								ne.cascadeNumber=Meshmsg->getCascadeNumber();
								ne.lastnotifyed=simTime().dbl()+10;
								LV->neighbors.push_back(ne);
								joinResponse(false,watchFilmEndTime[Meshmsg->getFilmId()],Meshmsg->getSrcNode(),Meshmsg->getSpecialInteractiveNeghborReq());
							}
						}
						else
						{
							joinDeny(false,Meshmsg->getSrcNode());
						}
					}
				}
				else
				{
					joinDeny(false,Meshmsg->getSrcNode());
				}
			}
		}
		else if (Meshmsg->getCommand() == JOIN_RESPONSE)
		{
			if(isInVector(Meshmsg->getSrcNode(),requestedNeighbors)&& !LV->peerExit)
			{
					deleteVector(Meshmsg->getSrcNode(),requestedNeighbors);
				if(neighborSelelctionStart==true )
				{
					int existActiveNeighbors=0;
					int existInCascadeActive=0;
					int existOutCascadeActive=0;
					for(unsigned int i=0; i < LV->neighbors.size();i++)
					{
						if(LV->neighbors[i].type==0)
						{
							existActiveNeighbors++;
							if(LV->neighbors[i].cascadeNumber==LV->cascadeNumber)
								existInCascadeActive++;
							else
								existOutCascadeActive++;
						}
					}
					if(meshStructure==0 )
					{
						if(simpleMeshType==0 && LV->neighbors.size() < neighborNum)
						{
							requesterJoinAck(Meshmsg);
						}
						else if(simpleMeshType==1 && existActiveNeighbors<activeNeighbors)
						{
							requesterJoinAck(Meshmsg);
						}
						else
						{
							joinDeny(false,Meshmsg->getSrcNode());
						}
					}
					else if (meshStructure==1||meshStructure==2)
					{
						if(connectDirectToServer)
						{
							if(existActiveNeighbors<activeNeighbors)
							{
								requesterJoinAck(Meshmsg);
							}
							else
							{
								joinDeny(false,Meshmsg->getSrcNode());
							}
						}
						else if(simpleMeshType==0 && ( LV->neighbors.size() < neighborNum ||Meshmsg->getSpecialInteractiveNeghborReq()==true))
						{
							requesterJoinAck(Meshmsg);
						}
						else if(simpleMeshType==1 && (existActiveNeighbors<activeNeighbors || Meshmsg->getSpecialInteractiveNeghborReq()==true))
						{
							requesterJoinAck(Meshmsg);
						}
						else
						{
							joinDeny(false,Meshmsg->getSrcNode());
						}
					}
				}
				else
				{
					joinDeny(false,Meshmsg->getSrcNode());
				}
			}
		/*	else
			{
				joinDeny(false,Meshmsg->getSrcNode());
			}*/
		}
		else if(Meshmsg->getCommand() == JOIN_ACK)
		{
			if( neighborSelelctionStart==true && !LV->peerExit)
			{
				if(LV->neighbors.size() <= neighborNum ||Meshmsg->getSpecialInteractiveNeghborReq()==true)
				{
					if(!isRegistered)
						selfRegister(false);
					stat_addedNeighbors += 1;
					showOverlayNeighborArrow(Meshmsg->getSrcNode(), false,"m=m,50,0,50,0;ls=red,1");
					// in below we send message to upper layer that send video for node (AGH)
					CreateStream* givefilm=new  CreateStream("Create Stream");
					givefilm->setDownBandwith(Meshmsg->getFilmEndTime());// in this case we use end time for downBandwidth
					givefilm->setDelay(simTime().dbl()-Meshmsg->getTimestamp().dbl());
					givefilm->setSelectedFilm(Meshmsg->getFilmId());
					givefilm->setSrcNode(Meshmsg->getSrcNode());
					givefilm->setSelectedStartTimeSec(Meshmsg->getFilmStartTime());
					MeshOverlay::handleAppMessage(givefilm);
				}
				else
				{
					joinDeny(false,Meshmsg->getSrcNode());
					if(isInVector(Meshmsg->getSrcNode(),LV->neighbors))
						deleteVector(Meshmsg->getSrcNode(),LV->neighbors);
					deleteOverlayNeighborArrow(Meshmsg->getSrcNode());
				}
			}
			else
			{
				joinDeny(false,Meshmsg->getSrcNode());
				if(isInVector(Meshmsg->getSrcNode(),LV->neighbors))
					deleteVector(Meshmsg->getSrcNode(),LV->neighbors);
				deleteOverlayNeighborArrow(Meshmsg->getSrcNode());
			}
		}
		else if(Meshmsg->getCommand() == JOIN_DENY)
		{
			deleteVector(Meshmsg->getSrcNode(),requestedNeighbors);
			if(isInVector(Meshmsg->getSrcNode(),LV->neighbors))
			{
				if(Meshmsg->getIsServer()==true && connectDirectToServer)
				{
					selfUnRegister();
					connectDirectToServer=false;
				}
				deleteVector(Meshmsg->getSrcNode(),LV->neighbors);
			}
			deleteOverlayNeighborArrow(Meshmsg->getSrcNode());
		}
		else if(Meshmsg->getCommand() == DISCONNECT)
		{
			if(Meshmsg->getDisconnecthopCount()==1)
			{
				MeshMessage* disconnect=new MeshMessage("DISCONNECT");
				disconnect->setCommand(DISCONNECT);
				disconnect->setIsServer(false);
				disconnect->setConnectDirectToServer(false);
				disconnect->setCascadeNumber(LV->cascadeNumber);
				disconnect->setSrcNode(thisNode);
				disconnect->setFilmId(LV->currentFilmId);
				disconnect->setDisconnecthopCount(2);
				sendMessageToUDP(Meshmsg->getSrcNode(),disconnect);
				std::cout << "time: " << simTime()<< " End time previous film out "<<getParentModule()->getParentModule()->getFullName() <<  std::endl;
				stat_disconnectMessages += 1;
				stat_disconnectMessagesBytesSent += disconnect->getByteLength();
			}
			if(Meshmsg->getIsServer()&& connectDirectToServer)
			{
				selfUnRegister();
				connectDirectToServer=false;
			}
			disconnectProcess(Meshmsg->getSrcNode(),Meshmsg->getFilmId(),Meshmsg->getCascadeNumber(),Meshmsg->getDisconnecthopCount());
		}
		delete Meshmsg;
	}
	/*else if(dynamic_cast<VideoMessage*>(msg) != NULL)
	{
		VideoMessage* videoplayermsg = check_and_cast<VideoMessage*>(msg);
		if(videoplayermsg->getCommand()==select_film)
		{
			nof=15;
			std::cout << "time: " << simTime()<< " nof=15; error  "<<getParentModule()->getParentModule()->getFullName() <<  std::endl;
		}
		MeshOverlay::handleUDPMessage(msg);
	}*/
	else
	{
		MeshOverlay::handleUDPMessage(msg);
	}
}
catch(std::exception& e)
	{
		std::cout << "time: " << simTime()<< " handleUDPMessage error in Peers Mesh  "<<getParentModule()->getParentModule()->getFullName() <<  std::endl;
	}
}
void PeersMesh::handleAppMessage(cMessage* msg)
{
	 if (dynamic_cast<MeshMessage*>(msg) != NULL)
	 {
		 MeshMessage* meshMsg=dynamic_cast<MeshMessage*>(msg);
		 if(meshMsg->getCommand()==SEEK_FWD)
		 {
			 cancelEvent(meshJoinRequestTimer);
			 scheduleAt(simTime(),meshJoinRequestTimer);
			 delete msg;
		 }
		 else
		 {
		 	MeshOverlay::handleAppMessage(msg);
		 }
	}
	else
	{
			MeshOverlay::handleAppMessage(msg);
	}
}
void PeersMesh:: neighborRequest(int neighborSize)
{
	TrackerMessage* NeighborReq = new TrackerMessage("NeighborReq");
	NeighborReq->setCommand(NEIGHBOR_REQUEST);
	NeighborReq->setNeighborSize(neighborSize);  // video on demand
	NeighborReq->setSrcNode(thisNode);
	NeighborReq->setConnectedAccess(accessAddress); // for topo-aware
	NeighborReq->setIsServer(false);
	NeighborReq->setSpecialInteractiveNeghborReq(specialInteractiveNeghborReq);
	NeighborReq->setCascadenumber(LV->cascadeNumber);
	NeighborReq->setHopCount(hopCount);
	NeighborReq->setSelectedFilmNum(LV->currentFilmId);
	NeighborReq->setSelectedFilmStartTime(startTime);
	sendMessageToUDP(trackerAddress,NeighborReq);
	neighborSelelctionStart=true;// in changing film pre msg must  RECOGNITION . (AGH)
	if(LV->neighbors.size() >= activeNeighbors)
		specialInteractiveNeghborReq=false; // we let second chance for seek peers to earn special neighbor time-AGH
}
void PeersMesh::joinResponse(bool isServer,int FilmEndTime,TransportAddress& destinationNode,bool specialInteractiveNeghborReq)
{
	MeshMessage* joinResponse = new MeshMessage("joinResponse");
	joinResponse->setCommand(JOIN_RESPONSE);
	joinResponse->setSrcNode(thisNode);
	joinResponse->setIsServer(isServer);
	joinResponse->setFilmEndTime(FilmEndTime);
	joinResponse->setConnectDirectToServer(connectDirectToServer);
	joinResponse->setSpecialInteractiveNeghborReq(specialInteractiveNeghborReq);
	joinResponse->setCascadeNumber(LV->cascadeNumber);
	joinResponse->setHopCount(hopCount);
	joinResponse->setIsHeadNode(isheadnode);
	joinResponse->setBitLength(MESHMESSAGE_L(msg));
	sendMessageToUDP(destinationNode,joinResponse);
	stat_joinRSP += 1;
	stat_joinRSPBytesSent += joinResponse->getByteLength();
}
void PeersMesh::joinDeny(bool isserver,TransportAddress& destinationNode)
{
	MeshMessage* joinDeny = new MeshMessage("joinDeny");
	joinDeny->setCommand(JOIN_DENY);
	joinDeny->setSrcNode(thisNode);
	joinDeny->setIsServer(isserver);
	joinDeny->setCascadeNumber(LV->cascadeNumber);
	joinDeny->setBitLength(MESHMESSAGE_L(msg));
	sendMessageToUDP(destinationNode,joinDeny);
	stat_joinDNY += 1;
	stat_joinDNYBytesSent += joinDeny->getByteLength();
}
// for select film (AGH)
int PeersMesh::selectingOneFilm(int min, int max)
{
	try
	{
	// in this section peer must select one film and send to tracker;
		int unwatchfilmCount=0;
		for(int i=min;i<max;i++)
		{
			if(LV->watchFilm[i]==false)
				unwatchfilmCount++;
		}
		int selected= intuniform(min,unwatchfilmCount-1,0);
		if(min>selected || selected>=max)
		{
			// if peer watch all films
			cancelAllRpcs();
			cancelEvent(meshJoinRequestTimer);
			cancelEvent(remainNotificationTimer);
			neighborSelelctionStart=false;
			int exitType=bernoulli(percentOfNotGracefullExit,0);
			if(exitType==0)
				handleNodeGracefulLeaveNotification();
			else
				nodeLeave();
			selected=-6;
		}
		else
		{
			for(int i=min;i<=selected;i++)
			{
				if(LV->watchFilm[i]==true)
					selected++;
			}
		}
		return selected;
	}
	catch(std::exception& e)
	{
		std::cout << "time: " << simTime()<< "selectingOneFilm error in Peers mesh "<<getParentModule()->getParentModule()->getFullName() <<  std::endl;
		return 0;
	}
}
// end randomize (AGH)
// its for find time that peer watching one film(in gamma distribution)-AGH
void PeersMesh::timeWatchingFilm(int FilmLenghtSecond)
{
	try
	{
		double interactiveProb=0;//interactive probability of peers that don't watch video complete-AGH
		// its real world
				// we have several gamma distribution for several film length
			double alpha=0;
			double beta=0;
			// if a peer dont watch video complete, it may have interactive to video.
			if(FilmLenghtSecond<360)
			{
				alpha=0.84;
				beta=3;
				interactiveProb=0.29;
			}
			else if(FilmLenghtSecond<720)
			{
				alpha=0.39;
				beta=5;
				interactiveProb=0.26;
			}
			else if(FilmLenghtSecond<1080)
			{
				alpha=0.33;
				beta=5;
				interactiveProb=0.29;
			}
			else if(FilmLenghtSecond<1440)
			{
				alpha=0.33;
				beta=3;
				interactiveProb=0.47;
			}
			else if(FilmLenghtSecond<1800)
			{
				alpha=0.22;
				beta=3;
				interactiveProb=0.27;
			}
			else if(FilmLenghtSecond<3240)
			{
				alpha=0.4;
				beta=1;
				interactiveProb=0.45;
			}
			else
			{
				alpha=0.16;
				beta=3;
				interactiveProb=0.43;
			}
	// end of real world
	if (isStable==0)
	{
		double percentWatchingFilm= gamma_d(alpha,beta,0);
		if(percentWatchingFilm>1)
			percentWatchingFilm=1;
		expireTime=percentWatchingFilm*FilmLenghtSecond;
	}
	// Don't exit before end of film and Don't interactive
	else if(isStable==1)
	{
		int simTlimit =minimum(atof(ev.getConfig()->getConfigValue("sim-time-limit"))-simTime().dbl(),FilmLenghtSecond);
			expireTime=simTlimit;//we use this only for reduce memory consumed in simulation-AGH
		interactiveProb=0;
	}
	//Can interactive but Don't exit before end of film
	else if(isStable==2)
	{
		// for used this configure you must set film length 2*simtime limit
		int simTlimit =minimum(atof(ev.getConfig()->getConfigValue("sim-time-limit"))-simTime().dbl(),FilmLenghtSecond);
			expireTime=(simTlimit);//we use this only for reduce memory consumed in simulation-AGH
		if(percentofInteractive!=-1)
		 {
			interactiveProb=percentofInteractive;
		 }
	}
	//Can exit but Don't interactive
	else if(isStable==3)
	{
		if(percentofExit==-1)
		{
			double percentWatchingFilm= gamma_d(alpha,beta,0);
			if(percentWatchingFilm>1)
				percentWatchingFilm=1;
			expireTime=percentWatchingFilm*FilmLenghtSecond;
		}
		else
		{
			int exitprob=bernoulli(percentofExit,0);
			int simTlimit =minimum(atof(ev.getConfig()->getConfigValue("sim-time-limit"))-simTime().dbl(),FilmLenghtSecond);
			if(exitprob==0)
				expireTime=simTlimit;//we use this only for reduce memory consumed in simulation-AGH
			else if(exitprob==1)
			  expireTime=intuniform(startExitSimTime-simTime().dbl(),simTlimit,0);
			if(expireTime<0)
				expireTime=0;
		}
		interactiveProb=0;
	}
	//Can exit and can interactive
	else if(isStable==4)
	{
		// for used this configure you must set film length 2*simtime limit
		if(percentofExit==-1)
		{
			double percentWatchingFilm= gamma_d(alpha,beta,0);
			if(percentWatchingFilm>1)
				percentWatchingFilm=1;
			expireTime=percentWatchingFilm*FilmLenghtSecond;
		}
		else
		{
			int exitprob=bernoulli(percentofExit,0);
			int simTlimit =minimum(atof(ev.getConfig()->getConfigValue("sim-time-limit"))-simTime().dbl(),FilmLenghtSecond);
			if(exitprob==0)
				expireTime=simTlimit;//we use this only for reduce memory consumed in simulation-AGH
			else if(exitprob==1)
				expireTime=intuniform(startExitSimTime-simTime().dbl(),simTlimit,0);
			if(expireTime<0)
				expireTime=0;
		}
		if(percentofInteractive!=-1)
		{
			interactiveProb=percentofInteractive;
		}
	}
	if(expireTime<FilmLenghtSecond-10&& expireTime>1)// its denote that peers who watch all film have not interactive-AGH
	 {
		 int prob=bernoulli(interactiveProb,0);
		 if(prob==0)
		 {
			 interactiveTime=0;
		 }
		 else if(prob==1)
		 {
			 if(expireTime>=FilmLenghtSecond-10)
				expireTime=0.5*expireTime;
			 haveInteractive=true;
			 int seekmaxlimit =minimum(atof(ev.getConfig()->getConfigValue("sim-time-limit"))-simTime().dbl(),expireTime);
			 int minstart=startIntractiveSimTime-simTime().dbl();
			 if(minstart<0)
				 minstart=0;
			 interactiveTime= intuniform(minstart,seekmaxlimit,0);
		 }
	 }
	}
	catch(std::exception& e)
	{
		std::cout << "time: " << simTime()<< "timeWatchingFilm error in simple mesh "<<getParentModule()->getParentModule()->getFullName() <<  std::endl;
	}
}
void PeersMesh::changeFilm()
{
	try
	{
		if(isRegistered)
		{
			setStatistics();
			// re new
			stat_TotalDownBandwidthUsage = 0;
			stat_TotalUpBandwidthUsage = 0;
			stat_joinREQ = 0;
			stat_joinREQBytesSent = 0;
			stat_joinRSP = 0;
			stat_joinRSPBytesSent = 0;
			stat_joinACK = 0;
			stat_joinACKBytesSent = 0;
			stat_joinDNY = 0;
			stat_joinDNYBytesSent = 0;
			stat_disconnectMessages = 0;
			stat_disconnectMessagesBytesSent = 0;
			stat_addedNeighbors = 0;
			stat_nummeshJoinRequestTimer = 0;
			//improvement
			stat_improvementREQ=0;
			stat_improvementREQBytesSent=0;
			stat_improvementRSP=0;
			stat_improvementRSPBytesSent=0;
			stat_improvementACK=0;
			stat_improvementACKBytesSent=0;
			stat_improvementDNY=0;
			stat_improvementDNYBytesSent=0;
			stat_numberOfImprovement=0;
			//change place for improvement parameters-AGH
			stat_ChangePlaceREQ=0;
			stat_ChangePlaceREQBytesSent=0;
			stat_ChangePlaceRSP=0;
			stat_ChangePlaceRSPBytesSent=0;
			stat_ChangePlaceACK=0;
			stat_ChangePlaceACKBytesSent=0;
			stat_ChangePlaceDNY=0;
			stat_ChangePlaceDNYBytesSent=0;
		// change
			handleNodeGracefulChangeFilmNotification();
			watchFilmEndTime[LV->currentFilmId]=LV->playbackpoint;
			TrackerMessage* ChangeFilmMsg = new TrackerMessage("Change Film Msg");
			ChangeFilmMsg->setCommand(ChangeFilm);
			ChangeFilmMsg->setSrcNode(thisNode);
			ChangeFilmMsg->setSelectedFilmNum(-1);
			ChangeFilmMsg->setRemainNeighbor(activeNeighbors - LV->neighbors.size() + passiveNeighbors);
			ChangeFilmMsg->setPreviousFilm(LV->currentFilmId);
			ChangeFilmMsg->setPreviousFilmStartTime(startTime);
			ChangeFilmMsg->setPreviousFilmEndTime(LV->playbackpoint);
			ChangeFilmMsg->setIsServer(false);
			ChangeFilmMsg->setConnectDirectToServer(connectDirectToServer);
			ChangeFilmMsg->setIsHeadnode(isheadnode);
			ChangeFilmMsg->setCascadenumber(LV->cascadeNumber);
			sendMessageToUDP(trackerAddress,ChangeFilmMsg);
			isRegistered=false;
			//cancelEvent(meshJoinRequestTimer);
			}
			else
			{
				LV->watchFilm[LV->currentFilmId]=false;
			}
			specialInteractiveNeghborReq=false;
			haveChangecascadeForInteractive=false;
			emergency=false;
			recivelistfilm=false;// send request to receive list of film
			connectDirectToServer=false;
			isheadnode=false;
			connectToHeadNode=false;
			LV->cascadeNumber=-1;
			hopCount=-1;
			scheduleAt(simTime(),meshJoinRequestTimer);
			scheduleAt(simTime()+neighborNotificationPeriod,remainNotificationTimer);
	}
	catch(std::exception& e)
	{
		std::cout << "time: " << simTime()<< "changeFilm error in Peers mesh "<<getParentModule()->getParentModule()->getFullName() <<  std::endl;
	}
}
void PeersMesh::handleNodeGracefulChangeFilmNotification()
{
	try
	{
		std::cout << "time: " << simTime()<< "  ChangeFilm    "<<getParentModule()->getParentModule()->getFullName() <<  std::endl;
		VideoMessage* videoMsg = new VideoMessage();
		videoMsg->setCommand(LEAVING);
		videoMsg->setType(1);//change video
		send(videoMsg,"appOut");
		MeshMessage* disconnectMsg = new MeshMessage("disconnect");
		disconnectMsg->setCommand(DISCONNECT);
		disconnectMsg->setIsServer(false);
		disconnectMsg->setConnectDirectToServer(connectDirectToServer);
		disconnectMsg->setCascadeNumber(LV->cascadeNumber);
		disconnectMsg->setSrcNode(thisNode);
		disconnectMsg->setFilmId(LV->currentFilmId);
		disconnectMsg->setBitLength(MESHMESSAGE_L(msg));
		for (unsigned int i=0; i != LV->neighbors.size(); i++)
		{
			sendMessageToUDP(LV->neighbors[i].address,disconnectMsg->dup());
			deleteOverlayNeighborArrow(LV->neighbors[i].address);
			stat_disconnectMessages += 1;     //change must add
			stat_disconnectMessagesBytesSent += disconnectMsg->getByteLength();
			VideoMessage* videoMsg = new VideoMessage();
			videoMsg->setCommand(NEIGHBOR_LEAVE);
			videoMsg->setSrcNode(LV->neighbors[i].address);
			send(videoMsg,"appOut");
		}
		for (unsigned int i=0; i != requestedNeighbors.size(); i++)
		{
			sendMessageToUDP(requestedNeighbors[i].address,disconnectMsg->dup());
			deleteOverlayNeighborArrow(requestedNeighbors[i].address);
			stat_disconnectMessages += 1;     //change must add
			stat_disconnectMessagesBytesSent += disconnectMsg->getByteLength();
			VideoMessage* videoMsg = new VideoMessage();
			videoMsg->setCommand(NEIGHBOR_LEAVE);
			videoMsg->setSrcNode(requestedNeighbors[i].address);
			send(videoMsg,"appOut");
		}
		int neighborSize=LV->neighbors.size();
		for (int i=0; i <neighborSize; i++)
		{
			deleteVector(LV->neighbors[0].address,LV->neighbors);   //(AGH)
		}
		neighborSize=requestedNeighbors.size();
		for (int i=0; i<neighborSize; i++)
		{
			deleteVector(requestedNeighbors[0].address,requestedNeighbors);   //(AGH)
		}
		delete disconnectMsg;
	}
	catch(std::exception& e)
	{
		std::cout << "time: " << simTime()<< "handleNodeGracefulChangeFilmNotification error in Peers mesh "<<getParentModule()->getParentModule()->getFullName() <<  std::endl;
	}
}
void PeersMesh::handleNodeGracefulLeaveNotification()
{
	try
	{
		neighborNum = 0;
		selfUnRegister();
		std::cout << "time:graceful exit " << simTime()<< "  "<<getParentModule()->getParentModule()->getFullName() <<  std::endl;
		VideoMessage* videoMsg = new VideoMessage();
		videoMsg->setCommand(LEAVING);
		videoMsg->setType(0);//leave
		send(videoMsg,"appOut");
		MeshMessage* disconnectMsg = new MeshMessage("disconnect");
		disconnectMsg->setCommand(DISCONNECT);
		disconnectMsg->setDisconnecthopCount(7);// it means that peer exit from system-AGH
		disconnectMsg->setIsServer(false);
		disconnectMsg->setConnectDirectToServer(connectDirectToServer);
		disconnectMsg->setCascadeNumber(LV->cascadeNumber);
		disconnectMsg->setSrcNode(thisNode);
		disconnectMsg->setFilmId(LV->currentFilmId);
		disconnectMsg->setBitLength(MESHMESSAGE_L(msg));
		for (unsigned int i=0; i != LV->neighbors.size(); i++)
		{
			sendMessageToUDP(LV->neighbors[i].address,disconnectMsg->dup());
			deleteOverlayNeighborArrow(LV->neighbors[i].address);
			stat_disconnectMessages += 1;
			stat_disconnectMessagesBytesSent += disconnectMsg->getByteLength();
			VideoMessage* videoMsg = new VideoMessage();
			videoMsg->setCommand(NEIGHBOR_LEAVE);
			videoMsg->setSrcNode(LV->neighbors[i].address);
			send(videoMsg,"appOut");
		}
		for (unsigned int i=0; i != requestedNeighbors.size(); i++)
		{
			sendMessageToUDP(requestedNeighbors[i].address,disconnectMsg->dup());
			deleteOverlayNeighborArrow(requestedNeighbors[i].address);
			stat_disconnectMessages += 1;     //change must add
			stat_disconnectMessagesBytesSent += disconnectMsg->getByteLength();
			VideoMessage* videoMsg = new VideoMessage();
			videoMsg->setCommand(NEIGHBOR_LEAVE);
			videoMsg->setSrcNode(requestedNeighbors[i].address);
			send(videoMsg,"appOut");
		}
		int neighborSize=LV->neighbors.size();
		for (int i=0; i <neighborSize; i++)
		{
			deleteVector(LV->neighbors[0].address,LV->neighbors);   //(AGH)
		}
		neighborSize=requestedNeighbors.size();
		for (int i=0; i<neighborSize; i++)
		{
			deleteVector(requestedNeighbors[0].address,requestedNeighbors);   //(AGH)
		}
		delete disconnectMsg;
		LV->peerExit=true;
		setStatistics();
		setOverlayReady(false);

	}
	catch(std::exception& e)
	{
		std::cout << "time: " << simTime()<< "handleNodeGracefulLeaveNotification error in Peers mesh "<<getParentModule()->getParentModule()->getFullName() <<  std::endl;
	}
}

void PeersMesh::nodeLeave()
{
	// this node exit without notify to neighbors and tracker- exit non graceful
	try
	{
		neighborNum = 0;
		std::cout << "time: not graceful exit" << simTime()<< "  "<<getParentModule()->getParentModule()->getFullName() <<  std::endl;
		VideoMessage* videoMsg = new VideoMessage();
		videoMsg->setCommand(LEAVING);
		videoMsg->setType(0);//leave
		send(videoMsg,"appOut");
		for (unsigned int i=0; i != LV->neighbors.size(); i++)
		{
			deleteOverlayNeighborArrow(LV->neighbors[i].address);
			VideoMessage* videoMsg = new VideoMessage();
			videoMsg->setCommand(NEIGHBOR_LEAVE);
			videoMsg->setSrcNode(LV->neighbors[i].address);
			send(videoMsg,"appOut");
		}
		for (unsigned int i=0; i != requestedNeighbors.size(); i++)
		{
			deleteOverlayNeighborArrow(requestedNeighbors[i].address);
			VideoMessage* videoMsg = new VideoMessage();
			videoMsg->setCommand(NEIGHBOR_LEAVE);
			videoMsg->setSrcNode(requestedNeighbors[i].address);
			send(videoMsg,"appOut");
		}
		int neighborSize=LV->neighbors.size();
		for (int i=0; i <neighborSize; i++)
		{
			deleteVector(LV->neighbors[0].address,LV->neighbors);   //(AGH)
		}
		neighborSize=requestedNeighbors.size();
		for (int i=0; i<neighborSize; i++)
		{
			deleteVector(requestedNeighbors[0].address,requestedNeighbors);   //(AGH)
		}
		LV->peerExit=true;
		setStatistics();
		setOverlayReady(false);
	}
	catch(std::exception& e)
	{
		std::cout << "time: " << simTime()<< "handle Not Graceful Exit error in Peers mesh "<<getParentModule()->getParentModule()->getFullName() <<  std::endl;
	}
}

int PeersMesh::findNeighborIndex(TransportAddress& Node, std::vector <neighbor> &neighbors)
{
	try
	{
		//if(Node.isUnspecified())
		//{
		//	Node=Node.getAddress()
		//}
		for (unsigned int i=0; i!=neighbors.size(); i++)
		{
			if (neighbors[i].address == Node)
			{
				return i;
			}
		}
		return -1;
	}
	catch(std::exception& e)
	{
			std::cout << "time: " << simTime()<< "findNeighborIndex error in Peers mesh "<<getParentModule()->getParentModule()->getFullName() <<  std::endl;
			return -1;
	}
}
bool PeersMesh::isInVector(TransportAddress& Node, std::vector <neighbor> &neighbors)
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
		std::cout << "time: " << simTime()<< "isInVector error in Peers mesh "<<getParentModule()->getParentModule()->getFullName() <<  std::endl;
		return false;
	}
}
void PeersMesh::deleteVector(TransportAddress Node,std::vector <neighbor> &neighbors)
{
	try
	{
		for (unsigned int i=0; i!=neighbors.size(); i++)
		{
			if(neighbors[i].address.isUnspecified())
			{
				neighbors.erase(neighbors.begin()+i,neighbors.begin()+1+i);
				break;
			}
		}
		//if(Node.isUnspecified())
			//return;
		for (unsigned int i=0; i!=neighbors.size(); i++)
		{
			if (Node == neighbors[i].address)
			{
				neighbors.erase(neighbors.begin()+i,neighbors.begin()+1+i);
				break;
			}
		}
	}
	catch(std::exception& e)
	{
		std::cout << "time: " << simTime()<< "deleteVector error in Peers mesh "<<getParentModule()->getParentModule()->getFullName() <<  std::endl;
	}
}
void PeersMesh::requesterJoinAck(MeshMessage* Meshmsg)
{
	if(Meshmsg->getIsServer()==true)
	{
		if(meshStructure==1||meshStructure==2)
		{
			// when a peer connect to server it must disconnect from all neighbors-AGH
			selfUnRegister();
			if(meshStructure==1||meshStructure==2)
				disconnectFromPreviouseActiveNeighbors();
			else
				disconnectFromPreviouseNeighbors();
			// end of disconnect from all neighbors
		}
		connectDirectToServer=true;
		isheadnode=false;
		hopCount=1;
	}
	else if(Meshmsg->getConnectDirectToServer()==true )
	{
		connectDirectToServer=false;
		connectToHeadNode=false;
	}
	else if(Meshmsg->getIsHeadNode()==true&& Meshmsg->getCascadeNumber()!=LV->cascadeNumber)
	{
		isheadnode=true;
		connectDirectToServer=false;
		connectToHeadNode=false;
	}
	else if(Meshmsg->getIsHeadNode()==true )
	{
		connectToHeadNode=true;
		isheadnode=false;
		connectDirectToServer=false;

	}
	if(hopCount==-1 || hopCount>Meshmsg->getHopCount())
	{
		hopCount=Meshmsg->getHopCount()+1;
	}
	neighbor ne;
	ne.address=Meshmsg->getSrcNode();
	ne.type=0;
	ne.lastnotifyed=simTime().dbl()+10;
	ne.endTime=Meshmsg->getFilmEndTime();
	ne.cascadeNumber=Meshmsg->getCascadeNumber();
	if(Meshmsg->getIsServer()==true)
		ne.cascadeNumber=-2;
	LV->neighbors.push_back(ne);
	if(!isRegistered)
	{
		selfRegister(connectDirectToServer);
	}
	showOverlayNeighborArrow(Meshmsg->getSrcNode(), false, "m=m,50,0,50,0;ls=red,1");
	MeshMessage* joinAck = new MeshMessage("joinAck");
	joinAck->setCommand(JOIN_ACK);
	joinAck->setSrcNode(thisNode);
	joinAck->setFilmId(LV->currentFilmId);
	joinAck->setFilmStartTime(LV->recieveTime);
	joinAck->setTimestamp(simTime());
	joinAck->setFilmEndTime(downBandwidth/1024);// in this case we use end time for downBandwidth
	joinAck->setBitLength(MESHMESSAGE_L(msg));
	joinAck->setSpecialInteractiveNeghborReq(Meshmsg->getSpecialInteractiveNeghborReq());
	sendMessageToUDP(Meshmsg->getSrcNode(),joinAck);
	stat_joinACK += 1;
	stat_joinACKBytesSent += joinAck->getByteLength();
}
void PeersMesh::selfRegister(bool connectDirecttoServer)
{
	try
	{
		TrackerMessage* selfReg = new TrackerMessage("selfRegister");
		selfReg->setCommand(SELF_REGISTER);
		selfReg->setSrcNode(thisNode);
		selfReg->setIsServer(isSource);
		selfReg->setConnectDirectToServer(connectDirectToServer);
		selfReg->setIsHeadnode(isheadnode);
		selfReg->setCascadenumber(LV->cascadeNumber);
		selfReg->setHopCount(hopCount);
		selfReg->setRemainNeighbor(passiveNeighbors);
		selfReg->setSelectedFilmNum(LV->currentFilmId);
		selfReg->setSpecialInteractiveNeghborReq(haveChangecascadeForInteractive);
		selfReg->setConnectedAccess(accessAddress); //for topo-aware
		sendMessageToUDP(trackerAddress,selfReg);
		isRegistered = true;
	}
	catch(std::exception& e)
	{
		std::cout << "time: " << simTime()<< "selfRegister error in Peers mesh "<<getParentModule()->getParentModule()->getFullName() <<  std::endl;
	}
}
void PeersMesh::selfUnRegister()
{
	try
	{
		if(isRegistered)
		{
			TrackerMessage* selfUnReg = new TrackerMessage("selfUnRegister");
			selfUnReg->setCommand(SELF_UNREGISTER);
			selfUnReg->setSrcNode(thisNode);
			selfUnReg->setIsServer(false);
			selfUnReg->setConnectDirectToServer(connectDirectToServer);
			selfUnReg->setIsHeadnode(isheadnode);
			selfUnReg->setCascadenumber(LV->cascadeNumber);
			selfUnReg->setSelectedFilmNum(LV->currentFilmId);
			sendMessageToUDP(trackerAddress,selfUnReg);
			isRegistered = false;
		}
	}
	catch(std::exception& e)
	{
		std::cout << "time: " << simTime()<< "selfUnRegister error in Peers mesh "<<getParentModule()->getParentModule()->getFullName() <<  std::endl;
	}
}
void PeersMesh::disconnectFromPreviouseNeighbors()
{
	MeshMessage* disconnectMsg = new MeshMessage("disconnect");
	disconnectMsg->setCascadeNumber(LV->cascadeNumber);
	disconnectMsg->setCommand(DISCONNECT);
	disconnectMsg->setDisconnecthopCount(5);// it means peer don't exit from system-AGH
	disconnectMsg->setSrcNode(thisNode);
	disconnectMsg->setFilmId(LV->currentFilmId);
	disconnectMsg->setBitLength(MESHMESSAGE_L(msg));
	for (unsigned int i=0; i != LV->neighbors.size(); i++)
	{
		sendMessageToUDP(LV->neighbors[i].address,disconnectMsg->dup());
		deleteOverlayNeighborArrow(LV->neighbors[i].address);
		stat_disconnectMessages += 1;
		stat_disconnectMessagesBytesSent += disconnectMsg->getByteLength();
		disconnectMsg->setIsServer(false);
		disconnectMsg->setConnectDirectToServer(connectDirectToServer);
		VideoMessage* videoMsg = new VideoMessage();
		videoMsg->setCommand(NEIGHBOR_LEAVE);
		videoMsg->setSrcNode(LV->neighbors[i].address);
		send(videoMsg,"appOut");
	}
	for (unsigned int i=0; i != requestedNeighbors.size(); i++)
	{
		sendMessageToUDP(requestedNeighbors[i].address,disconnectMsg->dup());
		deleteOverlayNeighborArrow(requestedNeighbors[i].address);
		stat_disconnectMessages += 1;     //change must add
		stat_disconnectMessagesBytesSent += disconnectMsg->getByteLength();
		VideoMessage* videoMsg = new VideoMessage();
		videoMsg->setCommand(NEIGHBOR_LEAVE);
		videoMsg->setSrcNode(requestedNeighbors[i].address);
		send(videoMsg,"appOut");
	}
	int neighborSize=LV->neighbors.size();
	for (int i=0; i <neighborSize; i++)
	{
		deleteVector(LV->neighbors[0].address,LV->neighbors);   //(AGH)
	}
	neighborSize=requestedNeighbors.size();
	for (int i=0; i<neighborSize; i++)
	{
		deleteVector(requestedNeighbors[0].address,requestedNeighbors);   //(AGH)
	}
	delete disconnectMsg;
}
void PeersMesh::disconnectFromPreviouseActiveNeighbors()
{
	MeshMessage* disconnectMsg = new MeshMessage("disconnect");
	disconnectMsg->setCascadeNumber(LV->cascadeNumber);
	disconnectMsg->setCommand(DISCONNECT);
	disconnectMsg->setDisconnecthopCount(5);// it means peer don't exit from system-AGH
	disconnectMsg->setIsServer(false);
	disconnectMsg->setConnectDirectToServer(connectDirectToServer);
	disconnectMsg->setSrcNode(thisNode);
	disconnectMsg->setFilmId(LV->currentFilmId);
	disconnectMsg->setBitLength(MESHMESSAGE_L(msg));
	int neighborSize=LV->neighbors.size();
	for (int i=0; i != neighborSize; i++)
	{
		if(LV->neighbors[i].type==0)
		{
			sendMessageToUDP(LV->neighbors[i].address,disconnectMsg->dup());
			deleteOverlayNeighborArrow(LV->neighbors[i].address);
			stat_disconnectMessages += 1;
			stat_disconnectMessagesBytesSent += disconnectMsg->getByteLength();
			VideoMessage* videoMsg = new VideoMessage();
			videoMsg->setCommand(NEIGHBOR_LEAVE);
			videoMsg->setSrcNode(LV->neighbors[i].address);
			send(videoMsg,"appOut");
		}
	}
	for (unsigned int i=0; i != requestedNeighbors.size(); i++)
	{
		sendMessageToUDP(requestedNeighbors[i].address,disconnectMsg->dup());
		deleteOverlayNeighborArrow(requestedNeighbors[i].address);
		stat_disconnectMessages += 1;     //change must add
		stat_disconnectMessagesBytesSent += disconnectMsg->getByteLength();
		VideoMessage* videoMsg = new VideoMessage();
		videoMsg->setCommand(NEIGHBOR_LEAVE);
		videoMsg->setSrcNode(requestedNeighbors[i].address);
		send(videoMsg,"appOut");
	}
	int n=0;
	for (int i=0; i <neighborSize; i++)
	{
		if(LV->neighbors[i+n].type==0)
		{
			deleteVector(LV->neighbors[i+n].address,LV->neighbors);   //(AGH)
			n--;
		}
	}
	neighborSize=requestedNeighbors.size();
	for (int i=0; i<neighborSize; i++)
	{
		deleteVector(requestedNeighbors[0].address,requestedNeighbors);   //(AGH)
	}
	delete disconnectMsg;
}
void PeersMesh::disconnectProcess(TransportAddress Node,int filmId, int cascadeNumber, int disconnectType)
{
	try
	{
		for (unsigned int i=0; i!=LV->neighbors.size(); i++)
		{
			if (Node == LV->neighbors[i].address)
			{
				if(LV->neighbors[i].type==0)
					Timer=0;
				else if(Timer<3)
					Timer=3;
				LV->neighbors.erase(LV->neighbors.begin()+i,LV->neighbors.begin()+1+i);
				break;
			}
		}
		VideoMessage* videoMsg = new VideoMessage();
		videoMsg->setCommand(NEIGHBOR_LEAVE);
		videoMsg->setKind(disconnectType);
		videoMsg->setSrcNode(Node);
		send(videoMsg,"appOut");
		deleteOverlayNeighborArrow(Node);
		cancelEvent(meshJoinRequestTimer);
		scheduleAt(simTime(),meshJoinRequestTimer);
	}
	catch(std::exception& e)
	{
		std::cout << "time: " << simTime()<< "disconnectProcess error in Peers mesh "<<getParentModule()->getParentModule()->getFullName() <<  std::endl;
	}
}
void PeersMesh::disconnectFromNode(TransportAddress Node ,int cascadeNumber)
{
	try
	{
		//std::cout << "time: " << simTime()<< " End time previous film out "<<getParentModule()->getParentModule()->getFullName() <<  std::endl;
		deleteVector(Node,LV->neighbors);
		MeshMessage* disconnectMsg = new MeshMessage("disconnect");
		disconnectMsg->setCascadeNumber(cascadeNumber);
		disconnectMsg->setCommand(DISCONNECT);
		disconnectMsg->setDisconnecthopCount(5);// it means peer don't exit from system-AGH
		disconnectMsg->setSrcNode(thisNode);
		disconnectMsg->setIsServer(false);
		disconnectMsg->setConnectDirectToServer(connectDirectToServer);
		disconnectMsg->setFilmId(LV->currentFilmId);
		disconnectMsg->setBitLength(MESHMESSAGE_L(msg));
		sendMessageToUDP(Node,disconnectMsg);
		deleteOverlayNeighborArrow(Node);
		VideoMessage* videoMsg = new VideoMessage();
		videoMsg->setCommand(NEIGHBOR_LEAVE);
		videoMsg->setSrcNode(Node);
		send(videoMsg,"appOut");
		cancelEvent(meshJoinRequestTimer);
		scheduleAt(simTime(),meshJoinRequestTimer);
	}
	catch(std::exception& e)
	{
		std::cout << "time: " << simTime()<< "disconnectFromNode error in Peers mesh "<<getParentModule()->getParentModule()->getFullName() <<  std::endl;
	}
}
void PeersMesh::checkNeighborsTimOuts()
{
	int neighborSize=LV->neighbors.size();
	int n=0;
	double spendTimeFromLastnotification=0;
		for (int i=0; i < neighborSize; i++)
	{
		spendTimeFromLastnotification=simTime().dbl()-LV->neighbors[i+n].lastnotifyed;
		if(spendTimeFromLastnotification>neighborsTimeOutLimit && LV->neighbors[i+n].cascadeNumber>=0)
		{
			VideoMessage* videoMsg = new VideoMessage();
			videoMsg->setCommand(NEIGHBOR_LEAVE);
			videoMsg->setKind(7);
			videoMsg->setSrcNode(LV->neighbors[i+n].address);
			send(videoMsg,"appOut");
			//maybe this neighbor dont exit and only congested so we send disconnect message-AGH
			MeshMessage* disconnectMsg = new MeshMessage("disconnect");
			disconnectMsg->setCascadeNumber(LV->cascadeNumber);
			disconnectMsg->setCommand(DISCONNECT);
			disconnectMsg->setDisconnecthopCount(5);// it means peer don't exit from system-AGH
			disconnectMsg->setIsServer(false);
			disconnectMsg->setConnectDirectToServer(connectDirectToServer);
			disconnectMsg->setSrcNode(thisNode);
			disconnectMsg->setFilmId(LV->currentFilmId);
			disconnectMsg->setBitLength(MESHMESSAGE_L(msg));
			sendMessageToUDP(LV->neighbors[i+n].address,disconnectMsg);
			//we send to tracker that this neighbor (maybe) Exit
			TrackerMessage* neighborExitMsg =new TrackerMessage(" neighborExit");
			neighborExitMsg->setCommand(NEIGHBOR_Exit);
			neighborExitMsg->setIsServer(false);// I'm not a server
			neighborExitMsg->setConnectDirectToServer(connectDirectToServer);
			neighborExitMsg->setSrcNode(LV->neighbors[i+n].address);// neighbor address
			neighborExitMsg->setCascadenumber(LV->neighbors[i+n].cascadeNumber);
			neighborExitMsg->setSelectedFilmNum(LV->currentFilmId);
			sendMessageToUDP(trackerAddress,neighborExitMsg);
			// end notification to tracker
			if(LV->neighbors[i+n].type==0)
			{
				//
				Timer=0;
			}
			else if(Timer<3)
				Timer=3;
			deleteOverlayNeighborArrow(LV->neighbors[i+n].address);
			deleteVector(LV->neighbors[i+n].address,LV->neighbors);   //(AGH)
			n--;

		}
	}
}
int PeersMesh::minimum(int A,int B)
{
	if(A<=B)
		return A;
	else if(A>B)
		return B;

}
void PeersMesh::setStatistics()
{
	//average per video
		char temp[100];
		sprintf(temp, "PeersMesh: JOIN::REQ Messages video %d", LV->currentFilmId);
		//average per video
		globalStatistics->addStdDev(temp, stat_joinREQ);
		sprintf(temp, "PeersMesh: JOIN::REQ Bytes sent video %d", LV->currentFilmId);
		globalStatistics->addStdDev(temp, stat_joinREQBytesSent);
		sprintf(temp, "PeersMesh: JOIN::RSP Messages video %d", LV->currentFilmId);
		globalStatistics->addStdDev(temp, stat_joinRSP);
		sprintf(temp, "PeersMesh: JOIN::RSP Bytes sent video %d", LV->currentFilmId);
		globalStatistics->addStdDev(temp, stat_joinRSPBytesSent);
		sprintf(temp, "PeersMesh: JOIN::ACK Messages video %d", LV->currentFilmId);
		globalStatistics->addStdDev(temp, stat_joinACK);
		sprintf(temp, "PeersMesh: JOIN::ACK Bytes sent video %d", LV->currentFilmId);
		globalStatistics->addStdDev(temp, stat_joinACKBytesSent);
		sprintf(temp, "PeersMesh: JOIN::DNY Messages video %d", LV->currentFilmId);
		globalStatistics->addStdDev(temp, stat_joinDNY);
		sprintf(temp, "PeersMesh: JOIN::DNY Bytes sent video %d", LV->currentFilmId);
		globalStatistics->addStdDev(temp, stat_joinDNYBytesSent);
		sprintf(temp, "PeersMesh: DISCONNECT:IND Messages video %d", LV->currentFilmId);
		globalStatistics->addStdDev(temp, stat_disconnectMessages);
		sprintf(temp, "PeersMesh: DISCONNECT:IND Bytes sent video %d", LV->currentFilmId);
		globalStatistics->addStdDev(temp, stat_disconnectMessagesBytesSent);
		sprintf(temp, "PeersMesh: Neighbors added video %d", LV->currentFilmId);
		globalStatistics->addStdDev(temp, stat_addedNeighbors);
		sprintf(temp, "PeersMesh: Number of JOINRQ selfMessages video %d", LV->currentFilmId);
		globalStatistics->addStdDev(temp, stat_nummeshJoinRequestTimer);
		sprintf(temp, "PeersMesh: Download bandwidth video %d", LV->currentFilmId);
		globalStatistics->addStdDev(temp, downBandwidth);
		sprintf(temp, "PeersMesh: Upload bandwidth video %d", LV->currentFilmId);
		globalStatistics->addStdDev(temp, upBandwidth);
		// average all video
		globalStatistics->addStdDev("PeersMesh: JOIN::REQ Messages", stat_joinREQ);
		globalStatistics->addStdDev("PeersMesh: JOIN::REQ Bytes sent", stat_joinREQBytesSent);
		globalStatistics->addStdDev("PeersMesh: JOIN::RSP Messages", stat_joinRSP);
		globalStatistics->addStdDev("PeersMesh: JOIN::RSP Bytes sent", stat_joinRSPBytesSent);
		globalStatistics->addStdDev("PeersMesh: JOIN::ACK Messages", stat_joinACK);
		globalStatistics->addStdDev("PeersMesh: JOIN::ACK Bytes sent", stat_joinACKBytesSent);
		globalStatistics->addStdDev("PeersMesh: JOIN::DNY Messages", stat_joinDNY);
		globalStatistics->addStdDev("PeersMesh: JOIN::DNY Bytes sent", stat_joinDNYBytesSent);
		//improvement
		globalStatistics->addStdDev("PeersMesh: improvement::REQ Messages", stat_improvementREQ);
		globalStatistics->addStdDev("PeersMesh: improvement::REQ Bytes sent", stat_improvementREQBytesSent);
		globalStatistics->addStdDev("PeersMesh: improvement::RSP Messages", stat_improvementRSP);
		globalStatistics->addStdDev("PeersMesh: improvement::RSP Bytes sent", stat_improvementRSPBytesSent);
		globalStatistics->addStdDev("PeersMesh: improvement::ACK Messages", stat_improvementACK);
		globalStatistics->addStdDev("PeersMesh: improvement::ACK Bytes sent", stat_improvementACKBytesSent);
		globalStatistics->addStdDev("PeersMesh: improvement::DNY Messages", stat_improvementDNY);
		globalStatistics->addStdDev("PeersMesh: improvement::DNY Bytes sent", stat_improvementDNYBytesSent);
		globalStatistics->addStdDev("PeersMesh: NumberOfImprovement", stat_numberOfImprovement);
		//Change Place For improvement
		globalStatistics->addStdDev("PeersMesh: changePlace::REQ Messages", stat_ChangePlaceREQ);
		globalStatistics->addStdDev("PeersMesh: changePlace::REQ Bytes sent", stat_ChangePlaceREQBytesSent);
		globalStatistics->addStdDev("PeersMesh: changePlace::RSP Messages", stat_ChangePlaceRSP);
		globalStatistics->addStdDev("PeersMesh: changePlace::RSP Bytes sent", stat_ChangePlaceRSPBytesSent);
		globalStatistics->addStdDev("PeersMesh: changePlace::ACK Messages", stat_ChangePlaceACK);
		globalStatistics->addStdDev("PeersMesh: changePlace::ACK Bytes sent", stat_ChangePlaceACKBytesSent);
		globalStatistics->addStdDev("PeersMesh: changePlace::DNY Messages", stat_ChangePlaceDNY);
		globalStatistics->addStdDev("PeersMesh: changePlace::DNY Bytes sent", stat_ChangePlaceDNYBytesSent);
		//
		globalStatistics->addStdDev("PeersMesh: DISCONNECT:IND Messages", stat_disconnectMessages);
		globalStatistics->addStdDev("PeersMesh: DISCONNECT:IND Bytes sent", stat_disconnectMessagesBytesSent);
		globalStatistics->addStdDev("PeersMesh: Neighbors added", stat_addedNeighbors);
		globalStatistics->addStdDev("PeersMesh: Number of JOINRQ selfMessages", stat_nummeshJoinRequestTimer);
		globalStatistics->addStdDev("PeersMesh: Download bandwidth", downBandwidth);
		globalStatistics->addStdDev("PeersMesh: Upload bandwidth", upBandwidth);
		if(stat_TotalSendByte != 0)
		{
			globalStatistics->addStdDev("TMVOD: average all sent byte", stat_TotalSendByte);
			globalStatistics->addStdDev("TMVOD: average all sent Data byte", stat_TotalDataSendByte);
			globalStatistics->addStdDev("TMVOD: average all sent control byte", stat_TotalControlSendByte);
			globalStatistics->addStdDev("TMVOD: average consumed upload bandwidth", (8*stat_TotalSendByte/1024)/(simTime().dbl()-stat_PeerEntranceTime));
			globalStatistics->addStdDev("TMVOD: average consumed Data upload bandwidth", (8*stat_TotalDataSendByte/1024)/(simTime().dbl()-stat_PeerEntranceTime));
			globalStatistics->addStdDev("TMVOD: average consumed control upload bandwidth", (8*stat_TotalControlSendByte/1024)/(simTime().dbl()-stat_PeerEntranceTime));
			globalStatistics->addStdDev("TMVOD: percent Of upload bandwidth utilization ", (100*8*stat_TotalSendByte/LV->getUpBandwidth())/(simTime().dbl()-stat_PeerEntranceTime));
			globalStatistics->addStdDev("TMVOD: percent Of Data upload bandwidth utilization ", (100*8*stat_TotalDataSendByte/LV->getUpBandwidth())/(simTime().dbl()-stat_PeerEntranceTime));
			globalStatistics->addStdDev("TMVOD: percent Of control upload bandwidth utilization ", (100*8*stat_TotalControlSendByte/LV->getUpBandwidth())/(simTime().dbl()-stat_PeerEntranceTime));

			char temp[100];
			int up=round( LV->getUpBandwidth()/100000);
			sprintf(temp, "TMVOD: average consumed upload bandwidth per uploadBandwidth %d",up);
			globalStatistics->addStdDev(temp, (8*stat_TotalSendByte/1024)/(simTime().dbl()-stat_PeerEntranceTime));

			sprintf(temp, "TMVOD: percent Of upload bandwidth utilization per uploadBandwidth %d",up);
			int utilizationUpPercent=0;
			utilizationUpPercent=100*8*(stat_TotalSendByte)/(LV->getUpBandwidth()*(simTime().dbl()-stat_PeerEntranceTime));
			globalStatistics->addStdDev(temp,utilizationUpPercent);

			up= LV->getUpBandwidth()/1024;
			if(up <=(videoAverageRate[LV->currentFilmId]))// it is low bandwidth
			{
				globalStatistics->addStdDev("TMVOD: percent Of upload bandwidth utilization in low-free up band width peers",(100*8*stat_TotalSendByte)/(LV->getUpBandwidth()*(simTime().dbl()-stat_PeerEntranceTime)));
				globalStatistics->addStdDev("TMVOD: average consumed upload bandwidth in low-free up band width peers", (8*stat_TotalSendByte/1024)/(simTime().dbl()-stat_PeerEntranceTime));
			}
			if(up <=100)// it is free rider
			{
				globalStatistics->addStdDev("TMVOD: percent Of upload bandwidth utilization in free riders",(100*8*stat_TotalSendByte)/(LV->getUpBandwidth()*(simTime().dbl()-stat_PeerEntranceTime)));
				globalStatistics->addStdDev("TMVOD: average consumed upload bandwidth in free riders", (8*stat_TotalSendByte/1024)/(simTime().dbl()-stat_PeerEntranceTime));
			}
			else if(up <=(videoAverageRate[LV->currentFilmId]))// it is low bandwidth
			{
				globalStatistics->addStdDev("TMVOD: percent Of upload bandwidth utilization in low up band width peers",(100*8*stat_TotalSendByte)/(LV->getUpBandwidth()*(simTime().dbl()-stat_PeerEntranceTime)));
				globalStatistics->addStdDev("TMVOD: average consumed upload bandwidth in low up band width peers", (8*stat_TotalSendByte/1024)/(simTime().dbl()-stat_PeerEntranceTime));
			}
			else if(up >=(videoAverageRate[LV->currentFilmId])&&up <(2*videoAverageRate[LV->currentFilmId]) )// it is medium band width
			{
				globalStatistics->addStdDev("TMVOD: percent Of upload bandwidth utilization in medium up band width peers",(100*8*stat_TotalSendByte)/(LV->getUpBandwidth()*(simTime().dbl()-stat_PeerEntranceTime)));
				globalStatistics->addStdDev("TMVOD: average consumed upload bandwidth in medium up band width peers", (8*stat_TotalSendByte/1024)/(simTime().dbl()-stat_PeerEntranceTime));
			}
			else if(up >=(2*videoAverageRate[LV->currentFilmId]))// it is high bandwidth
			{
				globalStatistics->addStdDev("TMVOD: percent Of upload bandwidth utilization in high up band width peers",(100*8*stat_TotalSendByte)/(LV->getUpBandwidth()*(simTime().dbl()-stat_PeerEntranceTime)));
				globalStatistics->addStdDev("TMVOD: average consumed upload bandwidth in high up band width peers", (8*stat_TotalSendByte/1024)/(simTime().dbl()-stat_PeerEntranceTime));
			}
		}
}
void PeersMesh::finishOverlay()
{
	cancelAndDelete(meshJoinRequestTimer);
	cancelAndDelete(remainNotificationTimer);
	cancelAndDelete(improvementProcessTimeLenghtTimer);
	if(!LV->peerExit)
	{
		setStatistics();
		setOverlayReady(false);
	}
}
