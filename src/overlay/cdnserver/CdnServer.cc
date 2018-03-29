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
 * @file CdnServer.cc
 * @author Abdollah Ghaffari Sheshjavani(AGH)
 */

#include <SimpleInfo.h>
#include "CdnServer.h"
#include <GlobalStatistics.h>
#include "TrackerMessage_m.h"
#include "VideoMessage_m.h"
#include "MeshMessage_m.h"

Define_Module(CdnServer);

void CdnServer::initializeOverlay(int stage)
{
	if (stage != MIN_STAGE_OVERLAY)
		return;
	isRegistered = false;
   	isSource = true;
    passiveNeighbors = par("passiveNeighbors");
    activeNeighbors = par("activeNeighbors");
    cdnClients=par("cdnClients");
    meshStructure=par("meshStructure");
    directConnectionToServers=par("directConnectionToServers");
	meshType=par("meshType"); // 0 is randomly and 1 is ORDINAL -AGH
    neighborNotificationPeriod = par("neighborNotificationPeriod");
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
	serverGradualNeighboring = par("serverGradualNeighboring");
	serverGradualNeighboringAverageTime=par("serverGradualNeighboringAverageTime");
	neighborsTimeOutLimit=par("neighborsTimeOutLimit");
	checkNeighborsTimeOut=par("checkNeighborsTimeOut");
 	if((serverGradualNeighboring||meshType==1) && meshStructure==0)
		neighborNum=passiveNeighbors;
	else
		neighborNum = cdnClients;
	//currentFilmNum=-1;

	getParentModule()->getParentModule()->setName("CDN-Server");
	MeshOverlay::initializeOverlay(stage);
	WATCH(neighborNum);
	WATCH(downBandwidth);
	WATCH(upBandwidth);
	for(int i=0;i<10;i++)
	{
		serverStress[i]=0;
		for(int j=0;j<100;j++)
			filmStreamed[i][j]=0;
	}
	stat_TotalDownBandwidthUsage = 0;
	stat_TotalUpBandwidthUsage = 0;
   	stat_joinRSP = 0;
	stat_joinRSPBytesSent = 0;
	stat_joinDNY = 0;
	stat_joinDNYBytesSent = 0;
	stat_disconnectMessages = 0;
	stat_disconnectMessagesBytesSent = 0;
	stat_addedNeighbors = 0;
	//change place for improvement parameters-AGH
	stat_ChangePlaceRSP=0;
	stat_ChangePlaceRSPBytesSent=0;
	stat_ChangePlaceDNY=0;
	stat_ChangePlaceDNYBytesSent=0;

	introduceItself();
}

void CdnServer::introduceItself()
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

	//ev << "I connect to " << this->getParentModule()->getParentModule()->gate("pppg$o",0)->getNextGate()->getOwnerModule()->getFullName();
	//ev << "which its IP address is " << IPAddressResolver().resolve(this->getParentModule()->getParentModule()->gate("pppg$o",0)->getNextGate()->getOwnerModule()->getFullName()) << endl;
    accessAddress = IPAddressResolver().resolve(this->getParentModule()->getParentModule()->gate("pppg$o",0)->getNextGate()->getOwnerModule()->getFullName());
	//ev << "which its IP address is " << accessAddress << endl;
}

void CdnServer::joinOverlay()
{
	try
	{
		LV->WatchExpired=false;
		LV->playbackpoint=-1;
		LV->recieveTime=0;
		recivelistfilm=false;
		hopCount=0;
		trackerAddress  = *globalNodeList->getRandomAliveNode(1);
		remainNotificationTimer = new cMessage ("remainNotificationTimer");
		scheduleAt(simTime()+neighborNotificationPeriod,remainNotificationTimer);
		std::stringstream ttString;
		ttString << thisNode;
		getParentModule()->getParentModule()->getDisplayString().setTagArg("tt",0,ttString.str().c_str());
		serverNeighborTimer = new cMessage("serverNeighborTimer");
		serverStressTimer= new cMessage("serverStressTimer");
		scheduleAt(simTime()+1,serverStressTimer);
		if(serverGradualNeighboring)
			scheduleAt(simTime()+serverGradualNeighboringAverageTime+8,serverNeighborTimer);
		selfRegister(false);
		setOverlayReady(true);
	}
	catch(std::exception& e)
	{
		std::cout << "time: " << simTime()<< "joinOverlay error in Cdn Server "<<getParentModule()->getParentModule()->getFullName() <<  std::endl;
	}
}
void CdnServer::handleTimerEvent(cMessage* msg)
{
	try
	{
		 if(msg == serverStressTimer)
		 {
			 if(checkNeighborsTimeOut==true)
				 checkNeighborsTimOuts();
			 int stress=0;
			 char temp[100];
			 for(int i=0;i<10;i++)
			 {
				if(serverStress[i]!=0)
				{
				 sprintf(temp, "CdnServer: Average Server Stream Stress %d", i);
				 globalStatistics->addStdDev(temp,serverStress[i]);
				 sprintf(temp, "CdnServer: Average Server Stream Stress Vector %d", i);
				 globalStatistics->recordOutVector(temp,serverStress[i]);
				 stress+=serverStress[i];
				}
			 }
			 globalStatistics->addStdDev("CdnServer: All Average Server Stream Stress",stress);
			 globalStatistics->recordOutVector("CdnServer: All Server Stream Stress vector",stress);
			 scheduleAt(simTime()+1,serverStressTimer);
		 }
		 else if(msg == remainNotificationTimer)
		{
			TrackerMessage* remainNotification = new TrackerMessage("remainNotification");
			remainNotification->setCommand(REMAIN_NEIGHBOR);
			remainNotification->setSrcNode(thisNode);
			//remainNotification->setSelectedFilmNum(currentFilmNum);
			remainNotification->setRemainNeighbor(neighborNum);// we manage it on tracker-AGH
			remainNotification->setIsServer(true);
			sendMessageToUDP(trackerAddress,remainNotification);
			scheduleAt(simTime()+2,remainNotificationTimer);
		}
		else if (msg == serverNeighborTimer)
		{
			if(meshStructure==0)
			{
				if(neighborNum < cdnClients)
				{
					neighborNum +=1;
					cancelEvent(remainNotificationTimer);
					scheduleAt(simTime(),remainNotificationTimer);
					//scheduleAt(simTime()+uniform(15,25),serverNeighborTimer);
					scheduleAt(simTime()+serverGradualNeighboringAverageTime,serverNeighborTimer);
				}
			}
		}
		else
			MeshOverlay::handleAppMessage(msg);
	}
	catch(std::exception& e)
	{
		std::cout << "time: " << simTime()<< "handleTimerEvent error in Cdn Server "<<getParentModule()->getParentModule()->getFullName() <<  std::endl;
	}
}
void CdnServer::handleUDPMessage(BaseOverlayMessage* msg)
{
	try
	{
		if (dynamic_cast<TrackerMessage*>(msg) != NULL)
		{
			TrackerMessage* trackerMsg = check_and_cast<TrackerMessage*>(msg);
			delete trackerMsg;
		}
		else if (dynamic_cast<MeshMessage*>(msg) != NULL)
		{
			MeshMessage* Meshmsg = check_and_cast<MeshMessage*>(msg);
			if (Meshmsg->getCommand() == EMERGENCY_jOIN_REQUEST)
			{
				if(!isInVector(Meshmsg->getSrcNode(),LV->neighbors))
				{
					neighbor ne;
					ne.address=Meshmsg->getSrcNode();
					ne.type=1;
					ne.endTime=-1;
					ne.lastnotifyed=simTime().dbl()+7;
					ne.videoId=Meshmsg->getFilmId();
					ne.cascadeNumber=-2;// its emergency
					LV->neighbors.push_back(ne);
					serverStress[Meshmsg->getFilmId()]++;
					MeshMessage* joinResponse = new MeshMessage("emergencyResponse");
					joinResponse->setCommand(EMERGENCY_RESPONSE);
					joinResponse->setSrcNode(thisNode);
					joinResponse->setIsServer(true);
					joinResponse->setFilmEndTime(-1);
					joinResponse->setBitLength(MESHMESSAGE_L(msg));
					sendMessageToUDP(Meshmsg->getSrcNode(),joinResponse);
				}
			}
			else if (Meshmsg->getCommand() == EMERGENCY_ACK)
			{
				//stat_addedNeighbors += 1;
				showOverlayNeighborArrow(Meshmsg->getSrcNode(), false, "m=m,50,0,50,0;ls=red,1");
				// in below we send message to upper layer that send video for node (AGH)
				CreateStream* givefilm=new  CreateStream("Create Stream");
				givefilm->setSelectedFilm(Meshmsg->getFilmId());
				givefilm->setSrcNode(Meshmsg->getSrcNode());
				MeshOverlay::handleAppMessage(givefilm);
			}
			else if(Meshmsg->getCommand() == EMERGENCY_DENY)
			{
				if(isInVector(Meshmsg->getSrcNode(),LV->neighbors))
				{
					deleteVector(Meshmsg->getSrcNode(),LV->neighbors);
					serverStress[Meshmsg->getFilmId()]--;
				}
				deleteOverlayNeighborArrow(Meshmsg->getSrcNode());
			}
			else if (Meshmsg->getCommand() == EMERGENCY_DISCONNECT)
			{
				if(isInVector(Meshmsg->getSrcNode(),LV->neighbors))
				{
					VideoMessage* videoMsg = new VideoMessage();
					videoMsg->setCommand(NEIGHBOR_LEAVE);
					videoMsg->setSrcNode(Meshmsg->getSrcNode());
					send(videoMsg,"appOut");
					deleteVector(Meshmsg->getSrcNode(),LV->neighbors);
					deleteOverlayNeighborArrow(Meshmsg->getSrcNode());
					serverStress[Meshmsg->getFilmId()]--;
				}
			}
			else if (Meshmsg->getCommand() == JOIN_REQUEST)
			{
				if(!isInVector(Meshmsg->getSrcNode(),LV->neighbors))
				{
					if(meshStructure==0)
					{
						if(filmStreamed[Meshmsg->getFilmId()][0] < neighborNum)
						{
							neighbor ne;
							ne.address=Meshmsg->getSrcNode();
							ne.type=1;
							ne.endTime=-1;
							ne.lastnotifyed=simTime().dbl()+5;
							ne.videoId=Meshmsg->getFilmId();
							ne.cascadeNumber=Meshmsg->getCascadeNumber();// its emergency
							LV->neighbors.push_back(ne);
							filmStreamed[Meshmsg->getFilmId()][0]++;
							serverStress[Meshmsg->getFilmId()]++;
							joinResponse(true,-1,Meshmsg->getSrcNode());
						}
						else
						{
							joinDeny(true,Meshmsg->getSrcNode());
						}
					}
					else if(meshStructure==1||meshStructure==2)
					{
						// it must modified
						if(filmStreamed[Meshmsg->getFilmId()][Meshmsg->getCascadeNumber()]>=directConnectionToServers)
						{
							joinDeny(true,Meshmsg->getSrcNode());
						}
						else
						{
							neighbor ne;
							ne.address=Meshmsg->getSrcNode();
							ne.type=1;
							ne.endTime=-1;
							ne.lastnotifyed=simTime().dbl()+5;
							ne.videoId=Meshmsg->getFilmId();
							ne.cascadeNumber=Meshmsg->getCascadeNumber();// its emergency
							LV->neighbors.push_back(ne);
							filmStreamed[Meshmsg->getFilmId()][Meshmsg->getCascadeNumber()]++;
							serverStress[Meshmsg->getFilmId()]++;
							joinResponse(true,-1,Meshmsg->getSrcNode());
						}
					}
				}
				else
				{
					joinDeny(true,Meshmsg->getSrcNode());
				}
			}
			else if(Meshmsg->getCommand() == JOIN_ACK)
			{
				if((meshStructure==0 && filmStreamed[Meshmsg->getFilmId()][0] < neighborNum+1)||((meshStructure==1||meshStructure==2)&&filmStreamed[Meshmsg->getFilmId()][Meshmsg->getCascadeNumber()] <directConnectionToServers+1))
				{
					//	if(!isRegistered)
					//	selfRegister(false);
					stat_addedNeighbors += 1;
					showOverlayNeighborArrow(Meshmsg->getSrcNode(), false, "m=m,50,0,50,0;ls=red,1");
					// in below we send message to upper layer that send video for node (AGH)
					CreateStream* givefilm=new  CreateStream("Create Stream");
					givefilm->setSelectedFilm(Meshmsg->getFilmId());
					givefilm->setSrcNode(Meshmsg->getSrcNode());
					MeshOverlay::handleAppMessage(givefilm);
				}
				else
				{
					joinDeny(true,Meshmsg->getSrcNode());
					if(isInVector(Meshmsg->getSrcNode(),LV->neighbors))
					{
						deleteVector(Meshmsg->getSrcNode(),LV->neighbors);
						if(meshStructure==0)
							filmStreamed[Meshmsg->getFilmId()][0]--;
						else
							filmStreamed[Meshmsg->getFilmId()][Meshmsg->getCascadeNumber()]--;
						serverStress[Meshmsg->getFilmId()]--;
					}
						deleteOverlayNeighborArrow(Meshmsg->getSrcNode());
				}
			}
			else if(Meshmsg->getCommand() == JOIN_DENY)
			{
				if(isInVector(Meshmsg->getSrcNode(),LV->neighbors))
				{
					deleteVector(Meshmsg->getSrcNode(),LV->neighbors);
					if(meshStructure==0)
						filmStreamed[Meshmsg->getFilmId()][0]--;
					else
						filmStreamed[Meshmsg->getFilmId()][Meshmsg->getCascadeNumber()]--;
					serverStress[Meshmsg->getFilmId()]--;
				}
				deleteOverlayNeighborArrow(Meshmsg->getSrcNode());
			}
			else if(Meshmsg->getCommand() == DISCONNECT)
			{
				disconnectProcess(Meshmsg->getSrcNode(),Meshmsg->getFilmId(),Meshmsg->getCascadeNumber());
			}
			delete Meshmsg;
		}
		else if (dynamic_cast<improvementchangePlaceMessage*>(msg) != NULL)
		{
			improvementchangePlaceMessage* ChangePlacemsg = check_and_cast<improvementchangePlaceMessage*>(msg);
			if(ChangePlacemsg->getCommand()==CHANGE_PLACE_REQUEST)
			{
				if(isInVector(ChangePlacemsg->getPreviusNode(),LV->neighbors) && !isInVector(ChangePlacemsg->getSrcNode(),LV->neighbors))
				{
					// add this node to its neighbor
					neighbor ne;
					ne.address=ChangePlacemsg->getSrcNode();
					ne.type=1;
					ne.endTime=-1;
					ne.lastnotifyed=simTime().dbl()+5;
					ne.videoId=ChangePlacemsg->getFilmId();
					ne.cascadeNumber=ChangePlacemsg->getCascadeNumber();// its emergency
					LV->neighbors.push_back(ne);
					// send response
					improvementchangePlaceMessage* changePlaceResponse = new improvementchangePlaceMessage("changePlaceResponse");
					changePlaceResponse->setCommand(CHANGE_PLACE_RESPONSE);
					changePlaceResponse->setSrcNode(thisNode);
					changePlaceResponse->setIsServer(true);
					changePlaceResponse->setNeighborType(0);
					changePlaceResponse->setBitLength(IMPROVEMENTCHANGEPLACE_L(msg));
					sendMessageToUDP(ChangePlacemsg->getSrcNode(),changePlaceResponse);
					stat_ChangePlaceRSP++;
					stat_ChangePlaceRSPBytesSent+=changePlaceResponse->getByteLength();
					// disconnect from previous peer
					MeshMessage* disconnectMsg = new MeshMessage("disconnect");
					disconnectMsg->setCommand(DISCONNECT);
					disconnectMsg->setIsServer(true);
					disconnectMsg->setConnectDirectToServer(false);
					disconnectMsg->setCascadeNumber(ChangePlacemsg->getCascadeNumber());
					disconnectMsg->setFilmId(ChangePlacemsg->getFilmId());
					disconnectMsg->setSrcNode(thisNode);
					disconnectMsg->setBitLength(MESHMESSAGE_L(msg));
					sendMessageToUDP(ChangePlacemsg->getPreviusNode(),disconnectMsg);
					deleteVector(ChangePlacemsg->getPreviusNode(),LV->neighbors);
					deleteOverlayNeighborArrow(ChangePlacemsg->getPreviusNode());
					VideoMessage* videoMsg = new VideoMessage();
					videoMsg->setCommand(NEIGHBOR_LEAVE);
					videoMsg->setSrcNode(ChangePlacemsg->getPreviusNode());
					send(videoMsg,"appOut");
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
			if(ChangePlacemsg->getCommand()==CHANGE_PLACE_ACK)
			{
				showOverlayNeighborArrow(ChangePlacemsg->getSrcNode(), false, "m=m,50,0,50,0;ls=red,1");
				// in below we send message to upper layer that send video for node (AGH)
				CreateStream* givefilm=new  CreateStream("Create Stream");
				givefilm->setSelectedFilm(ChangePlacemsg->getFilmId());
				givefilm->setSrcNode(ChangePlacemsg->getSrcNode());
				MeshOverlay::handleAppMessage(givefilm);
			}
			delete ChangePlacemsg;
		}
		else
		{
			MeshOverlay::handleUDPMessage(msg);
		}
	}
	catch(std::exception& e)
		{
			std::cout << "time: " << simTime()<< " handleUDPMessage error in Cdn Server  "<<getParentModule()->getParentModule()->getFullName() <<  std::endl;
		}
}
void CdnServer::joinResponse(bool isServer,int FilmEndTime,TransportAddress& destinationNode)
{
	MeshMessage* joinResponse = new MeshMessage("joinResponse");
	joinResponse->setCommand(JOIN_RESPONSE);
	joinResponse->setSrcNode(thisNode);
	joinResponse->setIsServer(isServer);
	joinResponse->setFilmEndTime(FilmEndTime);
	joinResponse->setBitLength(MESHMESSAGE_L(msg));
	sendMessageToUDP(destinationNode,joinResponse);
	stat_joinRSP += 1;
	stat_joinRSPBytesSent += joinResponse->getByteLength();
}
void CdnServer::joinDeny(bool isserver,TransportAddress& destinationNode)
{
	MeshMessage* joinDeny = new MeshMessage("joinDeny");
	joinDeny->setCommand(JOIN_DENY);
	joinDeny->setSrcNode(thisNode);
	joinDeny->setIsServer(isserver);
	joinDeny->setBitLength(MESHMESSAGE_L(msg));
	sendMessageToUDP(destinationNode,joinDeny);
	stat_joinDNY += 1;
	stat_joinDNYBytesSent += joinDeny->getByteLength();
}
// its for find time that peer watching one film(in gamma distribution)-AGH
bool CdnServer::isInVector(TransportAddress& Node, std::vector <neighbor> &neighbors)
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
		std::cout << "time: " << simTime()<< "isInVector error in Cdn Server "<<getParentModule()->getParentModule()->getFullName() <<  std::endl;
		return false;
	}
}
void CdnServer::deleteVector(TransportAddress Node,std::vector <neighbor> &neighbors)
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
		if(Node.isUnspecified())
			return;
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
		std::cout << "time: " << simTime()<< "deleteVector error in Cdn Server "<<getParentModule()->getParentModule()->getFullName() <<  std::endl;
	}
}
void CdnServer::selfRegister(bool connectDirecttoServer)
{
	try
	{
		TrackerMessage* selfReg = new TrackerMessage("selfRegister");
		selfReg->setCommand(SELF_REGISTER);
		selfReg->setSrcNode(thisNode);
		selfReg->setIsServer(true);
		selfReg->setHopCount(hopCount);
		selfReg->setRemainNeighbor(neighborNum);
		selfReg->setConnectedAccess(accessAddress);

        //ev << "I send a self register message to the tracker where the accessrouterAddress is " << accessAddress << endl;

		//selfReg->setSelectedFilmNum(currentFilmNum);
		sendMessageToUDP(trackerAddress,selfReg);
		isRegistered = true;
	}
	catch(std::exception& e)
	{
		std::cout << "time: " << simTime()<< "selfRegister error in Cdn Server "<<getParentModule()->getParentModule()->getFullName() <<  std::endl;
	}
}
void CdnServer::selfUnRegister()
{
	try
	{
		if(isRegistered)
		{
			TrackerMessage* selfUnReg = new TrackerMessage("selfUnRegister");
			selfUnReg->setCommand(SELF_UNREGISTER);
			selfUnReg->setSrcNode(thisNode);
			selfUnReg->setIsServer(true);
			selfUnReg->setConnectedAccess(accessAddress);
			//selfUnReg->setSelectedFilmNum(currentFilmNum);
			sendMessageToUDP(trackerAddress,selfUnReg);
			isRegistered = false;
		}
	}
	catch(std::exception& e)
	{
		std::cout << "time: " << simTime()<< "selfUnRegister error in Cdn Server "<<getParentModule()->getParentModule()->getFullName() <<  std::endl;
	}
}
void CdnServer::disconnectProcess(TransportAddress Node,int filmId, int cascadeNumber)
{
	try
	{
		if(isInVector(Node,LV->neighbors))
		{
			deleteVector(Node,LV->neighbors);
			VideoMessage* videoMsg = new VideoMessage();
			videoMsg->setCommand(NEIGHBOR_LEAVE);
			videoMsg->setSrcNode(Node);
			send(videoMsg,"appOut");
			deleteOverlayNeighborArrow(Node);
			if(meshStructure==0)
				filmStreamed[filmId][0]--;
			else
				filmStreamed[filmId][cascadeNumber]--;
			serverStress[filmId]--;
		}
	}
	catch(std::exception& e)
	{
		std::cout << "time: " << simTime()<< "disconnectProcess error in CDN Server "<<getParentModule()->getParentModule()->getFullName() <<  std::endl;
	}
}
void CdnServer::checkNeighborsTimOuts()
{
	int neighborSize=LV->neighbors.size();
	int n=0;

	double spendTimeFromLastnotification=0;
	for (int i=0; i < neighborSize; i++)
	{
		spendTimeFromLastnotification=simTime().dbl()-LV->neighbors[i+n].lastnotifyed;
		if(spendTimeFromLastnotification>neighborsTimeOutLimit )//notice to not emergency user-AGH
		{
			VideoMessage* videoMsg = new VideoMessage();
			videoMsg->setCommand(NEIGHBOR_LEAVE);
			videoMsg->setKind(7);
			videoMsg->setSrcNode(LV->neighbors[i+n].address);
			send(videoMsg,"appOut");
			if(LV->neighbors[i+n].cascadeNumber>=0)
			{
				if(meshStructure==0)
					filmStreamed[LV->neighbors[i+n].videoId][0]--;
				else
					filmStreamed[LV->neighbors[i+n].videoId][LV->neighbors[i+n].cascadeNumber]--;
				//we send to tracker that this neighbor (maybe) Exit
				TrackerMessage* neighborExitMsg =new TrackerMessage(" neighborExit");
				neighborExitMsg->setCommand(NEIGHBOR_Exit);
				neighborExitMsg->setIsServer(true);// I am a server
				neighborExitMsg->setConnectDirectToServer(false);
				neighborExitMsg->setSrcNode(LV->neighbors[i+n].address);// neighbor address
				neighborExitMsg->setCascadenumber(LV->neighbors[i+n].cascadeNumber);
				neighborExitMsg->setSelectedFilmNum(LV->neighbors[i+n].videoId);
				sendMessageToUDP(trackerAddress,neighborExitMsg);
				// end notification to tracker
			}
			serverStress[LV->neighbors[i+n].videoId]--;
			//maybe this neighbor dont exit and only congested so we send disconnect message-AGH
			MeshMessage* disconnectMsg = new MeshMessage("disconnect");
			disconnectMsg->setCascadeNumber(LV->neighbors[i+n].cascadeNumber);
			disconnectMsg->setCommand(DISCONNECT);
			disconnectMsg->setIsServer(true);
			disconnectMsg->setConnectDirectToServer(false);
			disconnectMsg->setDisconnecthopCount(5);// it means peer don't exit from system-AGH
			disconnectMsg->setSrcNode(thisNode);
			//disconnectMsg->setFilmId(currentFilmNum);
			disconnectMsg->setBitLength(MESHMESSAGE_L(msg));
			sendMessageToUDP(LV->neighbors[i+n].address,disconnectMsg);
			deleteOverlayNeighborArrow(LV->neighbors[i+n].address);
			deleteVector(LV->neighbors[i+n].address,LV->neighbors);   //(AGH)
			n--;
		}
	}
}
void CdnServer::setStatistics()
{
		// average all video
		globalStatistics->addStdDev("CdnServer: JOIN::RSP Messages", stat_joinRSP);
		globalStatistics->addStdDev("CdnServer: JOIN::RSP Bytes sent", stat_joinRSPBytesSent);
		globalStatistics->addStdDev("CdnServer: JOIN::DNY Messages", stat_joinDNY);
		globalStatistics->addStdDev("CdnServer: JOIN::DNY Bytes sent", stat_joinDNYBytesSent);
		//change place statics
		globalStatistics->addStdDev("CdnServer: Change Place::RSP Messages", stat_ChangePlaceRSP);
		globalStatistics->addStdDev("CdnServer: Change Place::RSP Bytes sent", stat_ChangePlaceRSPBytesSent);
		globalStatistics->addStdDev("CdnServer: Change Place::DNY Messages", stat_ChangePlaceDNY);
		globalStatistics->addStdDev("CdnServer: Change Place::DNY Bytes sent", stat_ChangePlaceDNYBytesSent);
		//
		globalStatistics->addStdDev("CdnServer: Neighbors added", stat_addedNeighbors);
		globalStatistics->addStdDev("CdnServer: Download bandwidth", downBandwidth);
		globalStatistics->addStdDev("CdnServer: Upload bandwidth", upBandwidth);

		if(stat_TotalSendByte != 0)
		{
			globalStatistics->addStdDev("TMVOD: average server all sent byte", stat_TotalSendByte);
			globalStatistics->addStdDev("TMVOD: average server all sent Data byte", stat_TotalDataSendByte);
			globalStatistics->addStdDev("TMVOD: average server all sent control byte", stat_TotalControlSendByte);
			globalStatistics->addStdDev("TMVOD: average server consumed upload bandwidth", (8*stat_TotalSendByte/1024)/(simTime().dbl()-stat_PeerEntranceTime));
			globalStatistics->addStdDev("TMVOD: average server consumed Data upload bandwidth", (8*stat_TotalDataSendByte/1024)/(simTime().dbl()-stat_PeerEntranceTime));
			globalStatistics->addStdDev("TMVOD: average server consumed control upload bandwidth", (8*stat_TotalControlSendByte/1024)/(simTime().dbl()-stat_PeerEntranceTime));
			globalStatistics->addStdDev("TMVOD: percent Of server upload bandwidth utilization ", (100*8*stat_TotalSendByte)/(LV->getUpBandwidth()*(simTime().dbl()-stat_PeerEntranceTime)));
			globalStatistics->addStdDev("TMVOD: percent Of server Data upload bandwidth utilization ", (100*8*stat_TotalDataSendByte)/(LV->getUpBandwidth()*(simTime().dbl()-stat_PeerEntranceTime)));
			globalStatistics->addStdDev("TMVOD: percent Of server control upload bandwidth utilization ", (100*8*stat_TotalControlSendByte)/(LV->getUpBandwidth()*(simTime().dbl()-stat_PeerEntranceTime)));
		}

}
void CdnServer::finishOverlay()
{
	cancelAndDelete(serverNeighborTimer);
	setStatistics();
	setOverlayReady(false);
}
