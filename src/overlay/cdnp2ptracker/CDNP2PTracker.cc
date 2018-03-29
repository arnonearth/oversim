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
 * @file CDNP2PTracker.cc
 * @author Abdollah Ghaffari Sheshjavani(AGH)
 */

#include "CDNP2PTracker.h"

Define_Module(CDNP2PTracker);
void CDNP2PTracker::initializeOverlay(int stage)
{
	if (stage != MIN_STAGE_OVERLAY)
		return;
	selectPolicy=par("selectPolicy");
	meshStructure=par("meshStructure");
	directConnectionToServers=par("directConnectionToServers");
	meshType=par("meshType");//0 is randomly and 1 is ORDINAL -AGH
	connectedMesh = par ("connectedMesh");
    usePreVideo=par("usePreVideo");
    CdnClients=par("cdnClients");
    varianceTime =par("varianceTime");
    reserveConnectionTime=par("reserveConnectionTime");
    minTimeLenghtWP=par("minTimeLenghtWP");
    passiveNeighbors=par("passiveNeighbors");
 	cascadeMinTime=par("cascadeMinTime");
    cascadeMaxTime=par("cascadeMaxTime");
    cascadeMinNodes=par("cascadeMinNodes");
    cascadeMaxNodes=par("cascadeMaxNodes");
    neighborsTimeOutLimit=par("neighborsTimeOutLimit");
    //  headCascadeNode=par("headCascadeNode");
    maxCascadeNumber=par("maxCascadeNumber");
    diferenceForCascadeMinesforFWD=par("diferenceForCascadeMinesforFWD");
    serverNum = 0;
    RecivedListOfFilm=false;
  	getParentModule()->getParentModule()->setDisplayString("p=240,50;i=device/mainframe_l;i2=block/circle_vs");
	getParentModule()->getParentModule()->setName("CDNP2PTracker");
    for (unsigned int i=0 ; i<6 ; i++)
	{
		loadServers[i] = 0;
	}

    //Initialize topo
	topo.extractByModulePath(cStringTokenizer("**.accessRouter[*] **.backboneRouter[*]").asVector());

}
void CDNP2PTracker::joinOverlay()
{
	setOverlayReady(true);

}
void CDNP2PTracker::handleTimerEvent(cMessage* msg)
{
	if(msg == endReserveRemainNeighborsTimer)
	{

		if(meshStructure==1||meshStructure==2)
		{
			for(int film=0;film<FilmList.numberOfFilms;film++)
			{
				for(int p=0;p<myStructiure[film].lastCascadeNumber+1;p++)
				{
					if(myStructiure[film].cascade[p].reservedirectToServerNodesTimeStart+reserveConnectionTime <simTime().dbl())
						myStructiure[film].cascade[p].reservedirectToServerNodesNumber=myStructiure[film].cascade[p].directToServerNodes.size();
					if(myStructiure[film].cascade[p].reserveheadCascadeNodesTimeStart+reserveConnectionTime <simTime().dbl())
						myStructiure[film].cascade[p].reserveheadCascadeNodesNumber=myStructiure[film].cascade[p].headCascadeNodes.size();;
				}
			}
		}
		scheduleAt(simTime()+1,endReserveRemainNeighborsTimer);
		}
		else if(msg == checkPeersTimeoutTimer)
		{
			checkPeersTimOuts();
			scheduleAt(simTime()+2,checkPeersTimeoutTimer);
		}
}
void CDNP2PTracker::handleUDPMessage(BaseOverlayMessage* msg)
{
	 if (dynamic_cast<EncapSelectFilm*>(msg) != NULL)
		{
		 		 SelectFilm* listOfFilmsMsg =  check_and_cast<SelectFilm*> (msg->decapsulate());
				 if(listOfFilmsMsg->getType()==0 && RecivedListOfFilm==false)
					 {
						FilmList.numberOfFilms=listOfFilmsMsg->getNumberOfFilms();
						for(int i=0;i<FilmList.numberOfFilms;i++)
						{
							FilmList.lenthOfFilmsSec[i]=listOfFilmsMsg->getLenthOfFilmsSec(i);
							FilmList.FrameOfFilms[i]=listOfFilmsMsg->getFrameOfFilms(i);
							FilmList.FPS=listOfFilmsMsg->getFPS(i);
							if(meshStructure==1||meshStructure==2)// its my structure(AGH)
							{
								initiateMystructiure(i,FilmList.lenthOfFilmsSec[i]);
							}
						}
						RecivedListOfFilm=true;
						endReserveRemainNeighborsTimer = new cMessage ("endReserveRemainNeighborsTimer");
						scheduleAt(simTime()+1,endReserveRemainNeighborsTimer);
						checkPeersTimeoutTimer = new cMessage ("checkPeersTimeoutTimer");
						scheduleAt(simTime()+2,checkPeersTimeoutTimer);
					 }
				 delete listOfFilmsMsg;
				 delete msg;
		}
	 else	if (dynamic_cast<TrackerMessage*>(msg) != NULL)
	 {
		TrackerMessage* trackerMsg = check_and_cast<TrackerMessage*>(msg);
		if(trackerMsg->getCommand() == NEIGHBOR_REQUEST)
		{
			if(isInTimeOutPeers(trackerMsg->getSrcNode(),timeoutPeers[trackerMsg->getSelectedFilmNum()]))
			{
				deleteVector(trackerMsg->getSrcNode(),timeoutPeers[trackerMsg->getSelectedFilmNum()]);
			}
			TrackerMessage* NeighborRsp = new TrackerMessage("NeighborRsp");
			NeighborRsp->setCommand(NEIGHBOR_RESPONSE);
			NeighborRsp->setSrcNode(thisNode);

			unsigned int size = 0;
			SetServerNumber(trackerMsg->getSrcNode(),trackerMsg->getSelectedFilmNum(),trackerMsg->getCascadenumber(),selectPolicy,trackerMsg->getConnectedAccess());
			std::vector <TransportAddress> list;
			if(meshStructure==2)// its my structure-AGH
			{
				peerCasadeNumber =trackerMsg->getCascadenumber();
				size =	Structure2MeshFillList(trackerMsg->getHopCount(),peerCasadeNumber,trackerMsg->getNeighborSize(),list,trackerMsg->getSrcNode(),trackerMsg->getSelectedFilmNum(),trackerMsg->getSelectedFilmStartTime(),trackerMsg->getSpecialInteractiveNeghborReq());
				NeighborRsp->setCascadenumber(peerCasadeNumber);
			}
			else if(meshStructure==1)// its my structure-AGH
			{
				peerCasadeNumber =trackerMsg->getCascadenumber();
				size =	Structure1MeshFillList(trackerMsg->getHopCount(),peerCasadeNumber,trackerMsg->getNeighborSize(),list,trackerMsg->getSrcNode(),trackerMsg->getSelectedFilmNum(),trackerMsg->getSelectedFilmStartTime(),trackerMsg->getSpecialInteractiveNeghborReq());
				NeighborRsp->setCascadenumber(peerCasadeNumber);
			}
			else if(meshStructure==0)// simple mesh
			{
				if(meshType==0)
				{
					size = SimpleMeshcalculateSize(trackerMsg->getNeighborSize(), trackerMsg->getSrcNode(),trackerMsg->getSelectedFilmNum(),trackerMsg->getSelectedFilmStartTime());
					SimpleMeshFillList(trackerMsg->getNeighborSize(),list,trackerMsg->getSrcNode(),trackerMsg->getSelectedFilmNum(),size,trackerMsg->getSelectedFilmStartTime());
				}
				else
				{
					size=SimpleMeshFillList(trackerMsg->getNeighborSize(),list,trackerMsg->getSrcNode(),trackerMsg->getSelectedFilmNum(),size,trackerMsg->getSelectedFilmStartTime());
				}
			}
			NeighborRsp->setNeighborsArraySize(size+1);
			int selectedServer = getServerNumber(trackerMsg->getSrcNode(),trackerMsg->getSelectedFilmNum());
			std::multimap <int,CdnServers>::iterator CDNIt = CDNList.begin();
			for(CDNIt = CDNList.begin() ; CDNIt != CDNList.end() ; CDNIt++)
			{
				if(CDNIt->first==selectedServer)
				{
					NeighborRsp->setNeighbors(0,CDNIt->second.tAddress);
					break;
				}
			}
			for(unsigned int i=0 ; i<size ; i++ )
				NeighborRsp->setNeighbors(i+1,list[i]);
			NeighborRsp->setSpecialInteractiveNeghborReq(trackerMsg->getSpecialInteractiveNeghborReq());
			sendMessageToUDP(trackerMsg->getSrcNode(),NeighborRsp);
		}
		else if(trackerMsg->getCommand() == NEIGHBOR_Exit)
		{
			int sfn=trackerMsg->getSelectedFilmNum();
			timeOutPeer tu;
			tu.tAddress=trackerMsg->getSrcNode();
			tu.cascadenumber=trackerMsg->getCascadenumber();
			tu.timeOut=simTime().dbl();
			if(trackerMsg->getIsServer()==true)
			{
				if(meshStructure==0)
				{
					std::multimap <int,currentFilm>::iterator nodeIt = peerList[sfn].begin();
					for(nodeIt = peerList[sfn].begin() ; nodeIt != peerList[sfn].end() ; nodeIt++)
					{
						if(nodeIt->second.nodeInfo.tAddress == tu.tAddress)
						{
							std::multimap <int,CdnServers>::iterator CDNIt = CDNList.begin();
							int serverID = getServerNumber(trackerMsg->getSrcNode(),nodeIt->second.filmnumber);
							for(CDNIt = CDNList.begin() ; CDNIt != CDNList.end() ; CDNIt++)
							{
								if(CDNIt->first==serverID)
								{
									CDNIt->second.cdnfilmStreamed[nodeIt->second.filmnumber]--;
									break;
								}
							}
							peerList[sfn].erase(nodeIt);
							peerServers[sfn].erase(tu.tAddress);
							loadServers[serverID] -= 1;
							// in seldom time peer maybe not exit-AGH
							TrackerMessage* youUnRegMsg =new TrackerMessage(" nodeUnRegistered");
							youUnRegMsg->setCommand(You_UnRegistered);
							sendMessageToUDP(tu.tAddress,youUnRegMsg);
							deleteVector(tu.tAddress,timeoutPeers[sfn]);
							break;
						}
					}
				}
				else if(meshStructure==1||meshStructure==2)
				{
					std::multimap <int,currentFilm>::iterator nodeIt = myStructiure[sfn].cascade[tu.cascadenumber].directToServerNodes.begin();
					for(nodeIt = myStructiure[sfn].cascade[tu.cascadenumber].directToServerNodes.begin() ;nodeIt!= myStructiure[sfn].cascade[tu.cascadenumber].directToServerNodes.end() ; nodeIt++)
					{
						if(nodeIt->second.nodeInfo.tAddress == tu.tAddress)
						{
							std::multimap <int,CdnServers>::iterator CDNIt = CDNList.begin();
							int serverID = getServerNumber(tu.tAddress,nodeIt->second.filmnumber);
							for(CDNIt = CDNList.begin() ; CDNIt != CDNList.end() ; CDNIt++)
							{
								if(CDNIt->first==serverID)
								 {
									CDNIt->second.cdnfilmStreamed[nodeIt->second.filmnumber]--;
									break;
								 }
							}
							myStructiure[sfn].cascade[tu.cascadenumber].directToServerNodes.erase(nodeIt);
							peerServers[sfn].erase(tu.tAddress);
							// in seldom time peer maybe not exit-AGH
							TrackerMessage* youUnRegMsg =new TrackerMessage(" nodeUnRegistered");
							youUnRegMsg->setCommand(You_UnRegistered);
							sendMessageToUDP(tu.tAddress,youUnRegMsg);
							deleteVector(tu.tAddress,timeoutPeers[sfn]);
							break;
						}
					}
				}
			}

			if(!isInTimeOutPeers(tu.tAddress,timeoutPeers[sfn]))
				timeoutPeers[sfn].push_back(tu);
		}
		else if(trackerMsg->getCommand() == ChangeFilm)
		{
				watchedFilm wf;
				wf.nodeInfo.tAddress = trackerMsg->getSrcNode();
				wf.nodeInfo.remainedNeighbor = trackerMsg->getRemainNeighbor();
				wf.filmnumber=trackerMsg->getPreviousFilm();
				wf.startTime=trackerMsg->getPreviousFilmStartTime();
				wf.endTime=trackerMsg->getPreviousFilmEndTime();
				int serverID = getServerNumber(trackerMsg->getSrcNode(),wf.filmnumber);
				if(wf.endTime-wf.startTime>=minTimeLenghtWP && usePreVideo==true)
				{
					peerListWatchpast[wf.filmnumber].insert (std::make_pair<int,watchedFilm>(serverID,wf));
				}
				int sfn=trackerMsg->getPreviousFilm();
				if(meshStructure==0)
				{
					std::multimap <int,currentFilm>::iterator nodeIt = peerList[sfn].begin();
					for(nodeIt = peerList[sfn].begin() ; nodeIt != peerList[sfn].end() ; nodeIt++)
					{
						if(nodeIt->second.nodeInfo.tAddress==wf.nodeInfo.tAddress)
						{
							if(nodeIt->second.nodeInfo.havedirectlinktocdnserver==true)
							{
								 std::multimap <int,CdnServers>::iterator CDNIt = CDNList.begin();
								 for(CDNIt = CDNList.begin() ; CDNIt != CDNList.end() ; CDNIt++)
								 {
									 if(CDNIt->first==serverID)
									 {
										 CDNIt->second.cdnfilmStreamed[sfn]--;
										 // peer send msg to neighbors (and cdn if it is neighbor of that peer)
										 // so NOT NECESSARY to send msg to this for correction values.
									 }
								 }
							}
							peerList[sfn].erase(nodeIt);
							peerServers[sfn].erase(wf.nodeInfo.tAddress);
							loadServers[serverID] -= 1;
							std::cout << "node: " << simTime()<< "  " <<wf.nodeInfo.tAddress << "  changeFilm : "  << std::endl;
							break;
						}
					}
				}
				else if(meshStructure==1||meshStructure==2)
				{
					int cascadeNum=trackerMsg->getCascadenumber();
					if(trackerMsg->getConnectDirectToServer()==true)
					{
						std::multimap <int,currentFilm>::iterator nodeIt = myStructiure[sfn].cascade[cascadeNum].directToServerNodes.begin();
						for(nodeIt = myStructiure[sfn].cascade[cascadeNum].directToServerNodes.begin() ;nodeIt!= myStructiure[sfn].cascade[cascadeNum].directToServerNodes.end() ; nodeIt++)
						{
							if(nodeIt->second.nodeInfo.tAddress.getAddress() == trackerMsg->getSrcNode().getAddress())
							{
								std::multimap <int,CdnServers>::iterator CDNIt = CDNList.begin();
								int serverID = getServerNumber(trackerMsg->getSrcNode(),sfn);
								for(CDNIt = CDNList.begin() ; CDNIt != CDNList.end() ; CDNIt++)
								{
									if(CDNIt->first==serverID)
									{
										CDNIt->second.cdnfilmStreamed[nodeIt->second.filmnumber]--;
									}
								}
								myStructiure[sfn].cascade[cascadeNum].directToServerNodes.erase(nodeIt);
								break;
							}

						}
					}
					else
					{
						std::multimap <int,currentFilm>::iterator nodeIt = myStructiure[sfn].cascade[cascadeNum].nodes.begin();
						for(nodeIt = myStructiure[sfn].cascade[cascadeNum].nodes.begin() ;nodeIt!= myStructiure[sfn].cascade[cascadeNum].nodes.end() ; nodeIt++)
						{
							if(nodeIt->second.nodeInfo.tAddress.getAddress() == trackerMsg->getSrcNode().getAddress())
							{
								myStructiure[sfn].cascade[trackerMsg->getCascadenumber()].nodes.erase(nodeIt);
								break;
							}
						}
					}
					peerServers[sfn].erase(trackerMsg->getSrcNode());
					std::cout << "node: " << simTime()<< "  " <<wf.nodeInfo.tAddress << "  changeFilm : "  << std::endl;
				}
		}
		else if(trackerMsg->getCommand() == SELF_REGISTER)
		{
			try
			{
				if(trackerMsg->getIsServer())
				{
					CdnServers cdn;
					serverNum += 1;
					 for(int i=0;i<10;i++)
					 {
						 cdn.cdnfilmStreamed[i]=0;
					 }
					cdn.tAddress=trackerMsg->getSrcNode();
					cdn.accessAddress=trackerMsg->getConnectedAccess();
					
					ev<<"This recent registered CDN's ip is "<<trackerMsg->getSrcNode()<<endl;
					ev<<"and its connected Access Router's ip is"<<trackerMsg->getConnectedAccess()<<endl;

					cdn.remainedClient=trackerMsg->getRemainNeighbor();
					cdn.timeOut = simTime().dbl();
					CDNList.insert(std::make_pair<int,CdnServers>(serverNum,cdn));
					peerServers[10].insert(std::make_pair<NodeHandle,int>(trackerMsg->getSrcNode(),serverNum));
				}
			  else
				{
					currentFilm cf;
					cf.nodeInfo.tAddress = trackerMsg->getSrcNode();
					cf.nodeInfo.remainedNeighbor = trackerMsg->getRemainNeighbor();
					cf.nodeInfo.reserveRemainedNeighbor = trackerMsg->getRemainNeighbor();
					cf.nodeInfo.reserveTimeStart=0;
					cf.nodeInfo.timeOut = simTime().dbl();
					cf.nodeInfo.havedirectlinktocdnserver=trackerMsg->getConnectDirectToServer();
					cf.nodeInfo.haveChangecascadeForInteractive=trackerMsg->getSpecialInteractiveNeghborReq();
					cf.nodeInfo.isHeadnode=trackerMsg->getIsHeadnode();
					cf.filmnumber=trackerMsg->getSelectedFilmNum();
					cf.nodeInfo.hopCount=trackerMsg->getHopCount();
					int serverID = getServerNumber(trackerMsg->getSrcNode(),cf.filmnumber);
					if(meshStructure==0)
						peerList[cf.filmnumber].insert (std::make_pair<int,currentFilm>(serverID,cf));
					else if(meshStructure==1||meshStructure==2)
					{
						if(trackerMsg->getConnectDirectToServer()==true)
						{
							myStructiure[cf.filmnumber].cascade[trackerMsg->getCascadenumber()].directToServerNodes.insert(std::make_pair<int,currentFilm>(serverID,cf));
						}
						else
						{
							myStructiure[cf.filmnumber].cascade[trackerMsg->getCascadenumber()].nodes.insert(std::make_pair<int,currentFilm>(serverID,cf));
						}
						//if(myStructiure[cf.filmnumber].cascade[trackerMsg->getCascadenumber()].lastHop<trackerMsg->getHopCount())
							//myStructiure[cf.filmnumber].cascade[trackerMsg->getCascadenumber()].lastHop=trackerMsg->getHopCount();
					}
					if(trackerMsg->getConnectDirectToServer()==true)
					  {
						 std::multimap <int,CdnServers>::iterator CDNIt = CDNList.begin();
						 for(CDNIt = CDNList.begin() ; CDNIt != CDNList.end() ; CDNIt++)
							{
								if(CDNIt->first==serverID)
								{
								CDNIt->second.cdnfilmStreamed[cf.filmnumber]++;
								}
							}
					  }
				}
		}
		catch(std::exception& e)
		{
			std::cout << "error on tracker on registeration  " << simTime()<< "  " <<" "<< std::endl;

		}
		}
		else if(trackerMsg->getCommand() == SELF_UNREGISTER)
		{
			if(isInTimeOutPeers(trackerMsg->getSrcNode(),timeoutPeers[trackerMsg->getSelectedFilmNum()]))
			{
				deleteVector(trackerMsg->getSrcNode(),timeoutPeers[trackerMsg->getSelectedFilmNum()]);
			}
			peerUnregister(trackerMsg);
		}
		else if (trackerMsg->getCommand() == REMAIN_NEIGHBOR)
		{
			if(isInTimeOutPeers(trackerMsg->getSrcNode(),timeoutPeers[trackerMsg->getSelectedFilmNum()]))
			{
				deleteVector(trackerMsg->getSrcNode(),timeoutPeers[trackerMsg->getSelectedFilmNum()]);
			}
			if(trackerMsg->getIsServer()==true)
			{
				CdnServers cdn;
				cdn.tAddress=trackerMsg->getSrcNode();
				cdn.remainedClient=trackerMsg->getRemainNeighbor();
				std::multimap <int,CdnServers>::iterator CDNIt = CDNList.begin();
				for(CDNIt = CDNList.begin() ; CDNIt != CDNList.end() ; CDNIt++)
				  if(CDNIt->second.tAddress == cdn.tAddress)
					{
					  CDNIt->second.remainedClient = cdn.remainedClient;
					  CDNIt->second.timeOut =  simTime().dbl();
						break;
					}
			}
			else
			{
				nodeInformation nF;
				nF.tAddress = trackerMsg->getSrcNode();
				nF.remainedNeighbor = trackerMsg->getRemainNeighbor();
				nF.hopCount=trackerMsg->getHopCount();
				nF.havedirectlinktocdnserver=trackerMsg->getConnectDirectToServer();
				//nF.isHeadnode=trackerMsg->getIsHeadnode();
				int sfn=trackerMsg->getSelectedFilmNum();
				if(meshStructure==0)
				{
					std::multimap <int,currentFilm>::iterator nodeIt = peerList[sfn].begin();
					for(nodeIt = peerList[sfn].begin() ; nodeIt != peerList[sfn].end() ; nodeIt++)
						if(nodeIt->second.nodeInfo.tAddress == nF.tAddress)
						{
							nodeIt->second.nodeInfo.remainedNeighbor = nF.remainedNeighbor;
							nodeIt->second.nodeInfo.timeOut =  simTime().dbl();
							if(nF.havedirectlinktocdnserver==true && nodeIt->second.nodeInfo.havedirectlinktocdnserver==false )
							{
								std::multimap <int,CdnServers>::iterator CDNIt = CDNList.begin();
								for(CDNIt = CDNList.begin() ; CDNIt != CDNList.end() ; CDNIt++)
								{
									if(CDNIt->first==nodeIt->first)
									{
								 		CDNIt->second.cdnfilmStreamed[sfn]++;
									}
							 	}
							}
							 nodeIt->second.nodeInfo.havedirectlinktocdnserver=nF.havedirectlinktocdnserver;
							break;
						}
				}
				else if(meshStructure==1||meshStructure==2)
				{
				//	if(myStructiure[sfn].cascade[trackerMsg->getCascadenumber()].lastHop<trackerMsg->getHopCount())
				//		myStructiure[sfn].cascade[trackerMsg->getCascadenumber()].lastHop=trackerMsg->getHopCount();
					if(trackerMsg->getConnectDirectToServer()==true)
					{
						std::multimap <int,currentFilm>::iterator nodeIt = myStructiure[sfn].cascade[trackerMsg->getCascadenumber()].directToServerNodes.begin();
						for(nodeIt = myStructiure[sfn].cascade[trackerMsg->getCascadenumber()].directToServerNodes.begin() ;nodeIt!= myStructiure[sfn].cascade[trackerMsg->getCascadenumber()].directToServerNodes.end() ; nodeIt++)
						{
							if(nodeIt->second.nodeInfo.tAddress == nF.tAddress)
							{
								nodeIt->second.nodeInfo.remainedNeighbor = nF.remainedNeighbor;
								nodeIt->second.nodeInfo.timeOut =  simTime().dbl();
							//in my structure we manage direct to servsr nodes -so we dont need below code /*- AGH
								/*	if(trackerMsg->getConnectDirectToServer()==true && nodeIt->second.nodeInfo.havedirectlinktocdnserver==false )
								{
									std::multimap <int,CdnServers>::iterator CDNIt = CDNList.begin();
									for(CDNIt = CDNList.begin() ; CDNIt != CDNList.end() ; CDNIt++)
									{
										if(CDNIt->first==nodeIt->first)
										{
											CDNIt->second.cdnfilmStreamed[cf.filmnumber]++;
										}
									}
								}*/
								nodeIt->second.nodeInfo.havedirectlinktocdnserver=true;
								nodeIt->second.nodeInfo.isHeadnode=false;
								nodeIt->second.nodeInfo.hopCount=1;
								break;
							}
						}
					}
					else
					{
						std::multimap <int,currentFilm>::iterator nodeIt = myStructiure[sfn].cascade[trackerMsg->getCascadenumber()].nodes.begin();
						for(nodeIt = myStructiure[sfn].cascade[trackerMsg->getCascadenumber()].nodes.begin() ;nodeIt!= myStructiure[sfn].cascade[trackerMsg->getCascadenumber()].nodes.end() ; nodeIt++)
						{
							if(nodeIt->second.nodeInfo.tAddress == nF.tAddress)
							{
								nodeIt->second.nodeInfo.remainedNeighbor = nF.remainedNeighbor;
								nodeIt->second.nodeInfo.timeOut =  simTime().dbl();
								nodeIt->second.nodeInfo.havedirectlinktocdnserver=false;
								nodeIt->second.nodeInfo.isHeadnode=false;
								nodeIt->second.nodeInfo.hopCount=nF.hopCount;
								break;
							}
						}
					}
				}
				std::multimap <int,watchedFilm>::iterator WatchnodeIt = peerListWatchpast[sfn].begin();
				for(WatchnodeIt = peerListWatchpast[sfn].begin() ; WatchnodeIt != peerListWatchpast[sfn].end() ; WatchnodeIt++)
					if(WatchnodeIt->second.nodeInfo.tAddress == nF.tAddress)
					{
						WatchnodeIt->second.nodeInfo.remainedNeighbor = nF.remainedNeighbor;
						WatchnodeIt->second.nodeInfo.timeOut =  simTime().dbl();
						WatchnodeIt->second.nodeInfo.havedirectlinktocdnserver=trackerMsg->getConnectDirectToServer();
						WatchnodeIt->second.nodeInfo.isHeadnode=trackerMsg->getIsHeadnode();
						break;
					}
			}
//			checkPeersTimOuts();
		}
		else if (trackerMsg->getCommand() == Request_ListOfFilm)
		{
			if(RecivedListOfFilm==true)
			{
				SelectFilm* listOfFilms = new SelectFilm("list of films from Tracker");
				listOfFilms->setNumberOfFilms(FilmList.numberOfFilms);
				for (int var = 0; var < FilmList.numberOfFilms; var++)
				{
					listOfFilms->setFrameOfFilms(var,FilmList.FrameOfFilms[var]);
					listOfFilms->setLenthOfFilmsSec(var,FilmList.FrameOfFilms[var]/FilmList.FPS);
					listOfFilms->setFPS(var,FilmList.FPS);// if you want have different FPS you must create different variables of FPS and set it here to each film FPS. Abdollah ghaffari
				}
				EncapSelectFilm* LOF = new EncapSelectFilm ("Encapsulated-list-of-Films");
				char buf[50];
				sprintf(buf,"Encap-%s-from-tracker",listOfFilms->getName());
				LOF->setName(buf);
				listOfFilms->setSrcNode(thisNode);
				NodeHandle Dst = trackerMsg->getSrcNode();
				LOF->setByteLength(ENCAPVIDEOMESSAGE_L(msg)/8);
				LOF->encapsulate(listOfFilms);
				sendMessageToUDP(Dst , LOF);
			}
		}
		else if (trackerMsg->getCommand() == CascadeNumRequest)
		{
			// in this system we only have seek forward so... -AGH
			int sfn=trackerMsg->getSelectedFilmNum();
			bool findinTemp=false;
			bool erase=false;
			int pointer=trackerMsg->getCascadenumber();
			std::map <TransportAddress,Temp>::iterator tempIt = interactiveTemp[sfn].begin();
			for(tempIt = interactiveTemp[sfn].begin() ; tempIt != interactiveTemp[sfn].end() ; tempIt++)
			{
				if(tempIt->first== trackerMsg->getSrcNode())
				{
					if(tempIt->second.startTime==trackerMsg->getSelectedFilmStartTime())
					{
						findinTemp=true;
						pointer=tempIt->second.cascadeNumber;
						break;
					}
					else
					{
						erase=true;
					}
				}
			}
			if(erase==true)
			{
				for(tempIt = interactiveTemp[sfn].begin() ; tempIt != interactiveTemp[sfn].end() ; tempIt++)
				{
					if(tempIt->first== trackerMsg->getSrcNode()&&tempIt->second.startTime!=trackerMsg->getSelectedFilmStartTime() )
					{
						interactiveTemp[sfn].erase(tempIt);// its previouse interactive- its option for future because multi interactive is not enable in TMUVOD-AGH
						break;
					}
				}
			}
			if(findinTemp==false)
			{
				double cascadeDomainTime=0;
				for(int i=trackerMsg->getCascadenumber();i>0;i--)
				{
					cascadeDomainTime=myStructiure[trackerMsg->getSelectedFilmNum()].cascade[i].initiateTime-myStructiure[trackerMsg->getSelectedFilmNum()].cascade[i-1].initiateTime;
					if(simTime().dbl()-myStructiure[trackerMsg->getSelectedFilmNum()].cascade[i].initiateTime < trackerMsg->getSelectedFilmStartTime())
						pointer--;
				}
				double diferenceTime=simTime().dbl()-myStructiure[trackerMsg->getSelectedFilmNum()].cascade[pointer].initiateTime - trackerMsg->getSelectedFilmStartTime();
				double temp=diferenceTime/cascadeDomainTime;
				if(temp<diferenceForCascadeMinesforFWD)
					pointer--;// peer join at end of cascade so is better to assign it to one cascade before-AGH
			//	if(pointer==trackerMsg->getCascadenumber())
			//		pointer--;
				if(pointer<0)
					pointer=0;
				Temp te;
				te.cascadeNumber=pointer;
				te.startTime=trackerMsg->getSelectedFilmStartTime();
				interactiveTemp[sfn].insert(std::make_pair<TransportAddress,Temp>(trackerMsg->getSrcNode(),te));
			}
			TrackerMessage* CascadeNumResponse = new TrackerMessage("CascadeNumRequest");
			CascadeNumResponse->setCommand(CascadeNumRequest);
			CascadeNumResponse->setSrcNode(thisNode);
			CascadeNumResponse->setCascadenumber(pointer);
			if(pointer!=trackerMsg->getCascadenumber())
			{
				if(!findinTemp)
				{
					peerUnregister(trackerMsg);
					SetServerNumber(trackerMsg->getSrcNode(),trackerMsg->getSelectedFilmNum(),pointer,selectPolicy,trackerMsg->getConnectedAccess());
				}
				unsigned int size = 0;
				std::vector <TransportAddress> list;
				if(meshStructure==2)// its my structure-AGH
				{
					peerCasadeNumber =pointer;
					size =	Structure2MeshFillList(-1,peerCasadeNumber,trackerMsg->getNeighborSize(),list,trackerMsg->getSrcNode(),trackerMsg->getSelectedFilmNum(),trackerMsg->getSelectedFilmStartTime(),trackerMsg->getSpecialInteractiveNeghborReq());
					CascadeNumResponse->setCascadenumber(peerCasadeNumber);
				}
				else if(meshStructure==1)// its my structure-AGH
				{
					peerCasadeNumber =pointer;
					size =	Structure1MeshFillList(-1,peerCasadeNumber,trackerMsg->getNeighborSize(),list,trackerMsg->getSrcNode(),trackerMsg->getSelectedFilmNum(),trackerMsg->getSelectedFilmStartTime(),trackerMsg->getSpecialInteractiveNeghborReq());
					CascadeNumResponse->setCascadenumber(peerCasadeNumber);
				}
				CascadeNumResponse->setNeighborsArraySize(size);
				for(unsigned int i=0 ; i<size ; i++ )
					CascadeNumResponse->setNeighbors(i,list[i]);
				CascadeNumResponse->setSpecialInteractiveNeghborReq(trackerMsg->getSpecialInteractiveNeghborReq());
			}
			sendMessageToUDP(trackerMsg->getSrcNode(),CascadeNumResponse);
		}
		delete trackerMsg;
	}
	else
		delete msg;
}
void CDNP2PTracker::peerUnregister(TrackerMessage* trackerMsg)
{
	try
	{
		int sfn=trackerMsg->getSelectedFilmNum();
		std::multimap <int,watchedFilm>::iterator pastnodeIt = peerListWatchpast[sfn].begin();
		for(pastnodeIt = peerListWatchpast[sfn].begin() ; pastnodeIt != peerListWatchpast[sfn].end() ; pastnodeIt++)
		{
			if(pastnodeIt->second.nodeInfo.tAddress== trackerMsg->getSrcNode().getAddress())
			{
				peerListWatchpast[sfn].erase(pastnodeIt);
			}
		}
		if(meshStructure==0)
		{
		std::multimap <int,currentFilm>::iterator nodeIt = peerList[sfn].begin();
			for(nodeIt = peerList[sfn].begin() ; nodeIt != peerList[sfn].end() ; nodeIt++)
			{
				if(nodeIt->second.nodeInfo.tAddress.getAddress() == trackerMsg->getSrcNode().getAddress())
				{
					if(nodeIt->second.nodeInfo.havedirectlinktocdnserver==true)
					  {
						 std::multimap <int,CdnServers>::iterator CDNIt = CDNList.begin();
						 int serverID = getServerNumber(trackerMsg->getSrcNode(),nodeIt->second.filmnumber);
						 for(CDNIt = CDNList.begin() ; CDNIt != CDNList.end() ; CDNIt++)
							{
							  if(CDNIt->first==serverID)
							  {
							  CDNIt->second.cdnfilmStreamed[nodeIt->second.filmnumber]--;
							  loadServers[serverID] -= 1;
							  }
							}
						 
						}

					peerList[sfn].erase(nodeIt);
					
					break;
				}
			}
		}
		else if(meshStructure==1||meshStructure==2)
		{
		//	if((myStructiure[sfn].cascade[trackerMsg->getCascadenumber()].nodes.size()/passiveNeighbors)+2<myStructiure[sfn].cascade[trackerMsg->getCascadenumber()].lastHop)
				//myStructiure[sfn].cascade[trackerMsg->getCascadenumber()].lastHop--;//we do this with worst condition
			int cascadeNum=trackerMsg->getCascadenumber();
			if(trackerMsg->getConnectDirectToServer()==true)
			{
				std::multimap <int,currentFilm>::iterator nodeIt = myStructiure[sfn].cascade[cascadeNum].directToServerNodes.begin();
				for(nodeIt = myStructiure[sfn].cascade[cascadeNum].directToServerNodes.begin() ;nodeIt!= myStructiure[sfn].cascade[cascadeNum].directToServerNodes.end() ; nodeIt++)
				{
					if(nodeIt->second.nodeInfo.tAddress.getAddress() == trackerMsg->getSrcNode().getAddress())
					{
						std::multimap <int,CdnServers>::iterator CDNIt = CDNList.begin();
						int serverID = getServerNumber(trackerMsg->getSrcNode(),nodeIt->second.filmnumber);
						for(CDNIt = CDNList.begin() ; CDNIt != CDNList.end() ; CDNIt++)
						{
							if(CDNIt->first==serverID)
							 {
								CDNIt->second.cdnfilmStreamed[nodeIt->second.filmnumber]--;
							 }
						}
					myStructiure[sfn].cascade[trackerMsg->getCascadenumber()].directToServerNodes.erase(nodeIt);
					break;
					}

				}
			}
			else
			{
				std::multimap <int,currentFilm>::iterator nodeIt = myStructiure[sfn].cascade[cascadeNum].nodes.begin();
				for(nodeIt = myStructiure[sfn].cascade[cascadeNum].nodes.begin() ;nodeIt!= myStructiure[sfn].cascade[cascadeNum].nodes.end() ; nodeIt++)
				{
					if(nodeIt->second.nodeInfo.tAddress.getAddress() == trackerMsg->getSrcNode().getAddress())
					{
						myStructiure[sfn].cascade[trackerMsg->getCascadenumber()].nodes.erase(nodeIt);
						break;
					}
				}
			}
		}
			peerServers[sfn].erase(trackerMsg->getSrcNode());
	}
catch(std::exception& e)
{
	std::cout << simTime()<< "peer unregister error on tracker "<<  std::endl;

}
}
void CDNP2PTracker::finishOverlay()
{
	setOverlayReady(false);
}
void CDNP2PTracker:: initiateMystructiure(int index,int videoLength)
{
	myStructiure[index].cascade=new cascades[maxCascadeNumber];
	myStructiure[index].lastCascadeNumber=0;
	myStructiure[index].cascade[0].initiateTime=-1;
	myStructiure[index].cascade[0].lastHop=-1;
	for(int x=1;x<maxCascadeNumber;x++)
	{
		myStructiure[index].cascade[x].reservedirectToServerNodesNumber=0;
		myStructiure[index].cascade[x].reserveheadCascadeNodesNumber=0;
		myStructiure[index].cascade[x].reservedirectToServerNodesTimeStart=0;
		myStructiure[index].cascade[x].reserveheadCascadeNodesTimeStart=0;
		myStructiure[index].cascade[x].initiateTime=-1;
		myStructiure[index].cascade[0].lastHop=-1;
	}
}

int CDNP2PTracker::Structure1MeshFillList(int peerHopCount,int& peerCasadeNumber,unsigned int neighborSize,std::vector <TransportAddress>& list, TransportAddress& node,int filmNumber,int watchStartTime,bool specialInteractiveNeghborReq)
{
	int selectServer = getServerNumber(node,filmNumber);
	unsigned int sectionSize = 0;
	int lastcascade=myStructiure[filmNumber].lastCascadeNumber;
	std::multimap <int,CdnServers>::iterator CDNIt = CDNList.begin();
	bool cdnServerSelected=false;
	int cascadeNumber=peerCasadeNumber;
	if(peerCasadeNumber==-1)
		cascadeNumber=lastcascade;
	int lastHop=0;
	bool haveNonItractiveNeighbor=false;
	myStructiure[filmNumber].cascade[cascadeNumber].lastHop= (myStructiure[filmNumber].cascade[cascadeNumber].nodes.size()+1);// in worst case-AGH
	lastHop=myStructiure[filmNumber].cascade[cascadeNumber].lastHop;
	if((myStructiure[filmNumber].cascade[cascadeNumber].directToServerNodes.size()<directConnectionToServers )&& ((peerHopCount==-1&&lastHop<=4) || peerHopCount<4))// peer might exit so we must check this connections- we -AGH
	{
		peerCasadeNumber=cascadeNumber;
		//myStructiure[filmNumber].cascade[cascadeNumber].reservedirectToServerNodesNumber++;
		//	myStructiure[filmNumber].cascade[cascadeNumber].reservedirectToServerNodesTimeStart=simTime().dbl();
		for(CDNIt = CDNList.begin() ; CDNIt != CDNList.end() ; CDNIt++)
		{
			if( cdnServerSelected==false && CDNIt->second.cdnfilmStreamed[filmNumber] < CdnClients&& CDNIt->first == selectServer )// a peer only allow to connect to selected server-AGH
			{
				list.push_back(CDNIt->second.tAddress);
				cdnServerSelected=true;  //a peer can only connect to one CDN (AGH)
				sectionSize ++;
			}
		}
	}
	else if(peerCasadeNumber==-1 && ((myStructiure[filmNumber].cascade[lastcascade].nodes.size()>=cascadeMaxNodes)||(myStructiure[filmNumber].cascade[lastcascade].initiateTime+cascadeMaxTime<simTime().dbl() && myStructiure[filmNumber].cascade[lastcascade].nodes.size()>=cascadeMinNodes)))
	{
		selectServer = (selectServer % serverNum)+1;
		for(CDNIt = CDNList.begin() ; CDNIt != CDNList.end() ; CDNIt++)
		{
			if( cdnServerSelected==false && CDNIt->second.cdnfilmStreamed[filmNumber] < CdnClients&& CDNIt->first == selectServer )// a peer only allow to connect to selected server-AGH
			{
				list.push_back(CDNIt->second.tAddress);
				cdnServerSelected=true;  //a peer can only connect to one CDN (AGH)
				sectionSize ++;
				peerCasadeNumber=cascadeNumber;
				myStructiure[filmNumber].lastCascadeNumber++;
				peerCasadeNumber++;
				myStructiure[filmNumber].cascade[peerCasadeNumber].initiateTime=simTime().dbl();
				peerServers[filmNumber].erase(node);
				peerServers[filmNumber].insert(std::make_pair<TransportAddress,int>(node,selectServer));
			}
		}
	}
	else if( sectionSize==0 &&  (peerHopCount>1||peerHopCount==-1))
	{
		bool useSpecialInteractiveNeghborReq=false;
		peerCasadeNumber=cascadeNumber;
		std::multimap <int,currentFilm>::iterator nodeIt = myStructiure[filmNumber].cascade[peerCasadeNumber].nodes.begin();
		std::multimap <int,currentFilm>::iterator directIt =myStructiure[filmNumber].cascade[peerCasadeNumber].directToServerNodes.begin();
		std::multimap <int,watchedFilm>::iterator nodeWatchPastIt = peerListWatchpast[filmNumber].begin();
		if(meshType==0)
		{
			int size=Structure1MeshCalculateSize(peerCasadeNumber,neighborSize,node,filmNumber,watchStartTime, specialInteractiveNeghborReq);
			int bernulli1zero=0;
			int bernulli2one=0;
			int bernulli2zero=0;
			while (list.size() < size)
			{
				// this is simple mesh so peer have same chance to connect to a Direct to server peer or other peer -(AGH)
				int watchsize=myStructiure[filmNumber].cascade[peerCasadeNumber].nodes.size();
				int directprob=bernoulli(directConnectionToServers/(directConnectionToServers+watchsize+peerListWatchpast[filmNumber].size()),0);
				if(peerCasadeNumber>0)
					watchsize+=myStructiure[filmNumber].cascade[peerCasadeNumber-1].nodes.size();
				if(directprob==0)
					bernulli1zero++;
				if(directprob==1 ||bernulli1zero>10)//  sometime  bernoulli(x)=0 in all loops even x>0 so for this  we use bernulli1zero>10-AGH
				{
					for(int remain=passiveNeighbors-1;remain>=0;remain--)
					for(directIt = myStructiure[filmNumber].cascade[peerCasadeNumber].directToServerNodes.begin(); directIt != myStructiure[filmNumber].cascade[peerCasadeNumber].directToServerNodes.end() ; directIt++)
					{
					if(directIt->second.nodeInfo.remainedNeighbor > remain && !isInVector(directIt->second.nodeInfo.tAddress,list) )
						{
							list.push_back(directIt->second.nodeInfo.tAddress);
							sectionSize ++;
							if(directIt->second.nodeInfo.haveChangecascadeForInteractive==false)
								haveNonItractiveNeighbor=true;
						}
					}
				}
				if(watchsize+peerListWatchpast[filmNumber].size()!=0)
				{
					int preWatchprob=bernoulli(peerListWatchpast[filmNumber].size()/(watchsize+peerListWatchpast[filmNumber].size()),0);
					//  sometime  bernoulli(x)=0 in all loops even x>0 so for this  we use bernulli2zero>10-AGH
					bernulli2one++;
					if(bernulli2one>50)
					{
						if(specialInteractiveNeghborReq && bernulli2one<100)
							useSpecialInteractiveNeghborReq=true;
						else
							size--;// in SELDOM times size change- so for prevent loop in the program we use this-AGH
					}
					if(preWatchprob==1||(bernulli2zero>10&&peerListWatchpast[filmNumber].size()>0))
					{
						int randomNum = intuniform(1,peerListWatchpast[filmNumber].size());
						for(int i=1; i<randomNum ; i++)
							++nodeWatchPastIt;
						if(nodeWatchPastIt != peerListWatchpast[filmNumber].end()&& nodeWatchPastIt->second.nodeInfo.tAddress != node && !isInVector(nodeWatchPastIt->second.nodeInfo.tAddress,list)
						&& nodeWatchPastIt->second.nodeInfo.remainedNeighbor > 0 &&!isInTimeOutPeers(nodeWatchPastIt->second.nodeInfo.tAddress,timeoutPeers[filmNumber]))
						{
							list.push_back(nodeWatchPastIt->second.nodeInfo.tAddress);
							sectionSize++;
						}
						nodeWatchPastIt = peerListWatchpast[filmNumber].begin();
					}
					//  sometime  bernoulli(x)=0 in all loops even x>0 so for this  we use bernulli2one>10-AGH
					if(preWatchprob==0 ||( bernulli2one>10&&watchsize>0))
					{
						bernulli2zero++;
						int mycascadesize=myStructiure[filmNumber].cascade[peerCasadeNumber].nodes.size();
						int randomNum = intuniform(1,watchsize);
						if(randomNum<=mycascadesize)
						{
							for(int i=1; i<randomNum ; i++)
								++nodeIt;
							if(nodeIt != myStructiure[filmNumber].cascade[peerCasadeNumber].nodes.end()
									&& nodeIt->second.nodeInfo.tAddress != node && !isInVector(nodeIt->second.nodeInfo.tAddress,list)
									&&!isInTimeOutPeers(nodeIt->second.nodeInfo.tAddress,timeoutPeers[filmNumber])&&( nodeIt->second.nodeInfo.remainedNeighbor > 0||useSpecialInteractiveNeghborReq))
							{
								list.push_back(nodeIt->second.nodeInfo.tAddress);
								sectionSize++;
								if(nodeIt->second.nodeInfo.haveChangecascadeForInteractive==false)
									haveNonItractiveNeighbor=true;
							}
							nodeIt =myStructiure[filmNumber].cascade[peerCasadeNumber].nodes.begin();
						}
						else
						{
							std::multimap <int,currentFilm>::iterator pastCascadenodeIt = myStructiure[filmNumber].cascade[peerCasadeNumber-1].nodes.begin();
							randomNum=randomNum-mycascadesize;
							for(int i=1; i<randomNum ; i++)
								++pastCascadenodeIt;
							if(pastCascadenodeIt != myStructiure[filmNumber].cascade[peerCasadeNumber-1].nodes.end()
									&& pastCascadenodeIt->second.nodeInfo.tAddress != node && !isInVector(pastCascadenodeIt->second.nodeInfo.tAddress,list)
									&&!isInTimeOutPeers(pastCascadenodeIt->second.nodeInfo.tAddress,timeoutPeers[filmNumber])&&( pastCascadenodeIt->second.nodeInfo.remainedNeighbor > 0||useSpecialInteractiveNeghborReq))
							{
								list.push_back(pastCascadenodeIt->second.nodeInfo.tAddress);
								sectionSize++;
								if(pastCascadenodeIt->second.nodeInfo.haveChangecascadeForInteractive==false)
									haveNonItractiveNeighbor=true;
							}

						}

					}
				}
			}
		}
		else if(meshType==1)
		{
			for(int remain=passiveNeighbors-1;remain>=0;remain--)
			{
				for(directIt = myStructiure[filmNumber].cascade[peerCasadeNumber].directToServerNodes.begin(); directIt != myStructiure[filmNumber].cascade[peerCasadeNumber].directToServerNodes.end() ; directIt++)
				{
					if( directIt->second.nodeInfo.remainedNeighbor > remain && !isInVector(directIt->second.nodeInfo.tAddress,list) )
					{
						list.push_back(directIt->second.nodeInfo.tAddress);
						sectionSize ++;
						if(directIt->second.nodeInfo.haveChangecascadeForInteractive==false)
							haveNonItractiveNeighbor=true;
					}
				}
			}
			for(int j=0;j<2;j++)
			{
				if(peerCasadeNumber-j>=0)
				for(nodeIt =  myStructiure[filmNumber].cascade[peerCasadeNumber-j].nodes.begin() ; nodeIt !=  myStructiure[filmNumber].cascade[peerCasadeNumber-j].nodes.end() ; nodeIt++)
				{
					if(nodeIt->second.nodeInfo.tAddress != node && !isInVector(nodeIt->second.nodeInfo.tAddress,list)&& nodeIt->second.nodeInfo.remainedNeighbor > 0 &&!isInTimeOutPeers(nodeIt->second.nodeInfo.tAddress,timeoutPeers[filmNumber]))
					{
						list.push_back(nodeIt->second.nodeInfo.tAddress);
						sectionSize++;
						if(nodeIt->second.nodeInfo.haveChangecascadeForInteractive==false)
							haveNonItractiveNeighbor=true;
					}
				}
			}
			for(nodeWatchPastIt = peerListWatchpast[filmNumber].begin() ; nodeWatchPastIt != peerListWatchpast[filmNumber].end() ; nodeWatchPastIt++)
			{
				if(nodeWatchPastIt->second.nodeInfo.tAddress != node && !isInVector(nodeWatchPastIt->second.nodeInfo.tAddress,list)
					&& nodeWatchPastIt->second.nodeInfo.remainedNeighbor > 0 &&!isInTimeOutPeers(nodeWatchPastIt->second.nodeInfo.tAddress,timeoutPeers[filmNumber]))
				{
					list.push_back(nodeWatchPastIt->second.nodeInfo.tAddress);
						sectionSize++;
				}
			}
			if(sectionSize<neighborSize && specialInteractiveNeghborReq)
			{
				for(int j=0;j<2;j++)
				{
					if(peerCasadeNumber-j>=0)
					if(myStructiure[filmNumber].cascade[peerCasadeNumber-j].nodes.size()>0)
					{
						int exitcheck=0;
						std::multimap <int,currentFilm>::iterator CascadenodeIt = myStructiure[filmNumber].cascade[peerCasadeNumber-j].nodes.begin();
						while (sectionSize< neighborSize && exitcheck<50)
						{
							exitcheck++;
							int randomNum = intuniform(1,myStructiure[filmNumber].cascade[peerCasadeNumber-j].nodes.size());
							for(int i=1; i<randomNum ; i++)
								++CascadenodeIt;
							if(CascadenodeIt != myStructiure[filmNumber].cascade[peerCasadeNumber-j].nodes.end()
									&& CascadenodeIt->second.nodeInfo.tAddress != node && !isInVector(CascadenodeIt->second.nodeInfo.tAddress,list)
									&&!isInTimeOutPeers(CascadenodeIt->second.nodeInfo.tAddress,timeoutPeers[filmNumber])&&( CascadenodeIt->second.nodeInfo.remainedNeighbor > 0||specialInteractiveNeghborReq))
							{
								list.push_back(CascadenodeIt->second.nodeInfo.tAddress);
								sectionSize++;
								if(CascadenodeIt->second.nodeInfo.haveChangecascadeForInteractive==false)
									haveNonItractiveNeighbor=true;
							}
							CascadenodeIt = myStructiure[filmNumber].cascade[peerCasadeNumber-j].nodes.begin();
						}
					}
				}
			}
		}
		/// if we have seek forward we must at minimum have 2 connection to non interactive peer
		if(haveNonItractiveNeighbor==false)
		if(myStructiure[filmNumber].cascade[peerCasadeNumber].nodes.size()>1 && specialInteractiveNeghborReq)
		{
			int counter=0;
			int specialSectionSize=0;
			int limit=2;
			while (specialSectionSize < limit)
			{
				nodeIt = myStructiure[filmNumber].cascade[peerCasadeNumber].nodes.begin();
				counter++;
				if(counter>50 && limit > 1)
					limit--;// maybe we have not non interactive node
				else if(counter>60 && limit >0)
					limit--;// maybe we have not non interactive node
				int randomNum = intuniform(1,myStructiure[filmNumber].cascade[peerCasadeNumber].nodes.size());
				for(int i=1; i<randomNum ; i++)
					++nodeIt;
				if(nodeIt != myStructiure[filmNumber].cascade[peerCasadeNumber].nodes.end()
						&& nodeIt->second.nodeInfo.tAddress != node && !isInVector(nodeIt->second.nodeInfo.tAddress,list)
						&& nodeIt->second.nodeInfo.haveChangecascadeForInteractive==false //&& nodeIt->second.nodeInfo.remainedNeighbor >=-1
						&&!isInTimeOutPeers(nodeIt->second.nodeInfo.tAddress,timeoutPeers[filmNumber]))
				{
					list.push_back(nodeIt->second.nodeInfo.tAddress);
					specialSectionSize++;
					sectionSize++;
				}
			}
		}
		/// end special interactive get non interactive
	}
	if(sectionSize > neighborSize)
	{
		if(specialInteractiveNeghborReq && !haveNonItractiveNeighbor)
			return sectionSize;
		else
			return neighborSize;
	}
		else
		return sectionSize;
}
int CDNP2PTracker::Structure2MeshFillList(int peerHopCount,int& peerCasadeNumber,unsigned int neighborSize,std::vector <TransportAddress>& list, TransportAddress& node,int filmNumber,int watchStartTime,bool specialInteractiveNeghborReq)
{
	int selectServer = getServerNumber(node,filmNumber);
	unsigned int sectionSize = 0;
	int lastcascade=myStructiure[filmNumber].lastCascadeNumber;
	std::multimap <int,CdnServers>::iterator CDNIt = CDNList.begin();
	bool cdnServerSelected=false;
	int cascadeNumber=peerCasadeNumber;
	if(peerCasadeNumber==-1)
		cascadeNumber=lastcascade;
	int lastHop=0;
	bool haveNonItractiveNeighbor=false;
	myStructiure[filmNumber].cascade[cascadeNumber].lastHop= (myStructiure[filmNumber].cascade[cascadeNumber].nodes.size()+1);// in worst case-AGH
	lastHop=myStructiure[filmNumber].cascade[cascadeNumber].lastHop;
	if((myStructiure[filmNumber].cascade[cascadeNumber].directToServerNodes.size()<directConnectionToServers )&& ((peerHopCount==-1&&lastHop<=4) || peerHopCount<4))// peer might exit so we must check this connections- we -AGH
	{
		peerCasadeNumber=cascadeNumber;
		//myStructiure[filmNumber].cascade[cascadeNumber].reservedirectToServerNodesNumber++;
		//	myStructiure[filmNumber].cascade[cascadeNumber].reservedirectToServerNodesTimeStart=simTime().dbl();
		for(CDNIt = CDNList.begin() ; CDNIt != CDNList.end() ; CDNIt++)
		{
			if( cdnServerSelected==false && CDNIt->second.cdnfilmStreamed[filmNumber] < CdnClients&& CDNIt->first == selectServer )// a peer only allow to connect to selected server-AGH
			{
				list.push_back(CDNIt->second.tAddress);
				cdnServerSelected=true;  //a peer can only connect to one CDN (AGH)
				sectionSize ++;
			}
		}
	}
	else if(peerCasadeNumber==-1 && ((myStructiure[filmNumber].cascade[lastcascade].nodes.size()>=cascadeMaxNodes)||(myStructiure[filmNumber].cascade[lastcascade].initiateTime+cascadeMaxTime<simTime().dbl() && myStructiure[filmNumber].cascade[lastcascade].nodes.size()>=cascadeMinNodes)))
	{
		selectServer = (selectServer % serverNum)+1;
		for(CDNIt = CDNList.begin() ; CDNIt != CDNList.end() ; CDNIt++)
		{
			if( cdnServerSelected==false && CDNIt->second.cdnfilmStreamed[filmNumber] < CdnClients&& CDNIt->first == selectServer )// a peer only allow to connect to selected server-AGH
			{
				list.push_back(CDNIt->second.tAddress);
				cdnServerSelected=true;  //a peer can only connect to one CDN (AGH)
				sectionSize ++;
				peerCasadeNumber=cascadeNumber;
				myStructiure[filmNumber].lastCascadeNumber++;
				peerCasadeNumber++;
				myStructiure[filmNumber].cascade[peerCasadeNumber].initiateTime=simTime().dbl();
				peerServers[filmNumber].erase(node);
				peerServers[filmNumber].insert(std::make_pair<TransportAddress,int>(node,selectServer));
			}
		}
	}
	else if( sectionSize==0 &&  (peerHopCount>1||peerHopCount==-1))
	{
		bool useSpecialInteractiveNeghborReq=false;
		peerCasadeNumber=cascadeNumber;
		std::multimap <int,currentFilm>::iterator nodeIt = myStructiure[filmNumber].cascade[peerCasadeNumber].nodes.begin();
		std::multimap <int,currentFilm>::iterator directIt =myStructiure[filmNumber].cascade[peerCasadeNumber].directToServerNodes.begin();
		std::multimap <int,watchedFilm>::iterator nodeWatchPastIt = peerListWatchpast[filmNumber].begin();
		if(meshType==0)
		{
			int size=Structure2MeshCalculateSize(peerCasadeNumber,neighborSize,node,filmNumber,watchStartTime, specialInteractiveNeghborReq);
			int bernulli1zero=0;
			int bernulli2one=0;
			int bernulli2zero=0;
			while (list.size() < size)
			{
				// this is simple mesh so peer have same chance to connect to a Direct to server peer or other peer -(AGH)
				int directprob=bernoulli(2/(2+myStructiure[filmNumber].cascade[peerCasadeNumber].nodes.size()+peerListWatchpast[filmNumber].size()),0);
				if(directprob==0)
					bernulli1zero++;
				if(directprob==1 ||bernulli1zero>10)//  sometime  bernoulli(x)=0 in all loops even x>0 so for this  we use bernulli1zero>10-AGH
				{
					for(int remain=passiveNeighbors-1;remain>=0;remain--)
					for(directIt = myStructiure[filmNumber].cascade[peerCasadeNumber].directToServerNodes.begin(); directIt != myStructiure[filmNumber].cascade[peerCasadeNumber].directToServerNodes.end() ; directIt++)
					{
					if(directIt->second.nodeInfo.remainedNeighbor > remain && !isInVector(directIt->second.nodeInfo.tAddress,list) )
						{
							list.push_back(directIt->second.nodeInfo.tAddress);
							sectionSize ++;
							if(directIt->second.nodeInfo.haveChangecascadeForInteractive==false)
								haveNonItractiveNeighbor=true;
						}
					}
				}
				if(myStructiure[filmNumber].cascade[peerCasadeNumber].nodes.size()+peerListWatchpast[filmNumber].size()!=0)
				{
					int preWatchprob=bernoulli(peerListWatchpast[filmNumber].size()/myStructiure[filmNumber].cascade[peerCasadeNumber].nodes.size()+peerListWatchpast[filmNumber].size(),0);
					//  sometime  bernoulli(x)=0 in all loops even x>0 so for this  we use bernulli2zero>10-AGH
					bernulli2one++;
					if(bernulli2one>50)
					{
						if(specialInteractiveNeghborReq && bernulli2one<100)
							useSpecialInteractiveNeghborReq=true;
						else
							size--;// in SELDOM times size change- so for prevent loop in the program we use this-AGH
					}
					if(preWatchprob==1||(bernulli2zero>10&&peerListWatchpast[filmNumber].size()>0))
					{
						int randomNum = intuniform(1,peerListWatchpast[filmNumber].size());
						for(int i=1; i<randomNum ; i++)
							++nodeWatchPastIt;
						if(nodeWatchPastIt != peerListWatchpast[filmNumber].end()&& nodeWatchPastIt->second.nodeInfo.tAddress != node && !isInVector(nodeWatchPastIt->second.nodeInfo.tAddress,list)
						&& nodeWatchPastIt->second.nodeInfo.remainedNeighbor > 0 &&!isInTimeOutPeers(nodeWatchPastIt->second.nodeInfo.tAddress,timeoutPeers[filmNumber]))
						{
							list.push_back(nodeWatchPastIt->second.nodeInfo.tAddress);
							sectionSize++;
						}
						nodeWatchPastIt = peerListWatchpast[filmNumber].begin();
					}
					//  sometime  bernoulli(x)=0 in all loops even x>0 so for this  we use bernulli2one>10-AGH
					if(preWatchprob==0 ||( bernulli2one>10&&myStructiure[filmNumber].cascade[peerCasadeNumber].nodes.size()>0))
					{
						bernulli2zero++;
						int randomNum = intuniform(1,myStructiure[filmNumber].cascade[peerCasadeNumber].nodes.size());
						for(int i=1; i<randomNum ; i++)
							++nodeIt;
						if(nodeIt != myStructiure[filmNumber].cascade[peerCasadeNumber].nodes.end()
						   && nodeIt->second.nodeInfo.tAddress != node && !isInVector(nodeIt->second.nodeInfo.tAddress,list)
						   &&!isInTimeOutPeers(nodeIt->second.nodeInfo.tAddress,timeoutPeers[filmNumber])&&( nodeIt->second.nodeInfo.remainedNeighbor > 0||useSpecialInteractiveNeghborReq))
						{
							list.push_back(nodeIt->second.nodeInfo.tAddress);
							sectionSize++;
							if(nodeIt->second.nodeInfo.haveChangecascadeForInteractive==false)
								haveNonItractiveNeighbor=true;
						}
						nodeIt =myStructiure[filmNumber].cascade[peerCasadeNumber].nodes.begin();
					}
				}
			}
		}
		else if(meshType==1)
		{
			for(int remain=passiveNeighbors-1;remain>=0;remain--)
			{
				for(directIt = myStructiure[filmNumber].cascade[peerCasadeNumber].directToServerNodes.begin(); directIt != myStructiure[filmNumber].cascade[peerCasadeNumber].directToServerNodes.end() ; directIt++)
				{
					if( directIt->second.nodeInfo.remainedNeighbor > remain && !isInVector(directIt->second.nodeInfo.tAddress,list) )
					{
						list.push_back(directIt->second.nodeInfo.tAddress);
						sectionSize ++;
						if(directIt->second.nodeInfo.haveChangecascadeForInteractive==false)
							haveNonItractiveNeighbor=true;
					}
				}
			}
			for(nodeIt =  myStructiure[filmNumber].cascade[peerCasadeNumber].nodes.begin() ; nodeIt !=  myStructiure[filmNumber].cascade[peerCasadeNumber].nodes.end() ; nodeIt++)
			{
				if(nodeIt->second.nodeInfo.tAddress != node && !isInVector(nodeIt->second.nodeInfo.tAddress,list)&& nodeIt->second.nodeInfo.remainedNeighbor > 0 &&!isInTimeOutPeers(nodeIt->second.nodeInfo.tAddress,timeoutPeers[filmNumber]))
				{
					list.push_back(nodeIt->second.nodeInfo.tAddress);
					sectionSize++;
					if(nodeIt->second.nodeInfo.haveChangecascadeForInteractive==false)
						haveNonItractiveNeighbor=true;
				}
			}
			for(nodeWatchPastIt = peerListWatchpast[filmNumber].begin() ; nodeWatchPastIt != peerListWatchpast[filmNumber].end() ; nodeWatchPastIt++)
			{
				if(nodeWatchPastIt->second.nodeInfo.tAddress != node && !isInVector(nodeWatchPastIt->second.nodeInfo.tAddress,list)
					&& nodeWatchPastIt->second.nodeInfo.remainedNeighbor > 0 &&!isInTimeOutPeers(nodeWatchPastIt->second.nodeInfo.tAddress,timeoutPeers[filmNumber]))
				{
					list.push_back(nodeWatchPastIt->second.nodeInfo.tAddress);
						sectionSize++;
				}
			}
		if(sectionSize<neighborSize && specialInteractiveNeghborReq)
		{
			if(myStructiure[filmNumber].cascade[peerCasadeNumber].nodes.size()>0)
			{
				int exitcheck=0;
				while (sectionSize< neighborSize && exitcheck<50)
				{
					nodeIt = myStructiure[filmNumber].cascade[peerCasadeNumber].nodes.begin();
					exitcheck++;
					int randomNum = intuniform(1,myStructiure[filmNumber].cascade[peerCasadeNumber].nodes.size());
					for(int i=1; i<randomNum ; i++)
						++nodeIt;
					if(nodeIt != myStructiure[filmNumber].cascade[peerCasadeNumber].nodes.end()
							&& nodeIt->second.nodeInfo.tAddress != node && !isInVector(nodeIt->second.nodeInfo.tAddress,list)
							&&!isInTimeOutPeers(nodeIt->second.nodeInfo.tAddress,timeoutPeers[filmNumber])&&( nodeIt->second.nodeInfo.remainedNeighbor > 0||specialInteractiveNeghborReq))
					{
						list.push_back(nodeIt->second.nodeInfo.tAddress);
						sectionSize++;
						if(nodeIt->second.nodeInfo.haveChangecascadeForInteractive==false)
							haveNonItractiveNeighbor=true;
					}
				}
			}

		}
		}
		/// if we have seek forward we must at minimum have 2 connection to non interactive peer
		if(haveNonItractiveNeighbor==false)
		if(myStructiure[filmNumber].cascade[peerCasadeNumber].nodes.size()>1 && specialInteractiveNeghborReq)
		{
			int counter=0;
			int specialSectionSize=0;
			int limit=2;
			while (specialSectionSize < limit)
			{
				counter++;
				nodeIt = myStructiure[filmNumber].cascade[peerCasadeNumber].nodes.begin();
				if(counter>50 && limit > 1)
					limit--;// maybe we have not non interactive node
				else if(counter>60 && limit >0)
					limit--;// maybe we have not non interactive node
				int randomNum = intuniform(1,myStructiure[filmNumber].cascade[peerCasadeNumber].nodes.size());
				for(int i=1; i<randomNum ; i++)
					++nodeIt;
				if(nodeIt != myStructiure[filmNumber].cascade[peerCasadeNumber].nodes.end()
						&& nodeIt->second.nodeInfo.tAddress != node && !isInVector(nodeIt->second.nodeInfo.tAddress,list)
						&& nodeIt->second.nodeInfo.haveChangecascadeForInteractive==false //&& nodeIt->second.nodeInfo.remainedNeighbor >=-1
						&&!isInTimeOutPeers(nodeIt->second.nodeInfo.tAddress,timeoutPeers[filmNumber]))
				{
					list.push_back(nodeIt->second.nodeInfo.tAddress);
					specialSectionSize++;
					sectionSize++;
				}
			}
		}
		/// end special interactive get non interactive
	}
	if(sectionSize > neighborSize)
	{
		if(specialInteractiveNeghborReq && !haveNonItractiveNeighbor)
			return sectionSize;
		else
			return neighborSize;
	}
		else
		return sectionSize;
}
bool CDNP2PTracker::isInVector(TransportAddress& Node, std::vector <TransportAddress> &list)
{
	for (unsigned int i=0; i!=list.size(); i++)
	{
		if (list[i] == Node)
		{
			return true;
		}
	}
	return false;
}
bool CDNP2PTracker::isInTimeOutPeers(TransportAddress& Node, std::vector <timeOutPeer> &timeoutpeers)
{
	for (unsigned int i=0; i!=timeoutpeers.size(); i++)
	{
		if (timeoutpeers[i].tAddress == Node)
		{
			return true;
		}
	}
	return false;

}
int CDNP2PTracker::SimpleMeshFillList(unsigned int neighborSize,std::vector <TransportAddress>& list, TransportAddress& node,int filmNumber, unsigned int size,int watchStartTime)
{
	int selectServer = getServerNumber(node,filmNumber);
	std::multimap <int,currentFilm>::iterator nodeIt = peerList[filmNumber].begin();
	std::multimap <int,CdnServers>::iterator CDNIt = CDNList.begin();
	std::multimap <int,watchedFilm>::iterator nodeWatchPastIt = peerListWatchpast[filmNumber].begin();
	bool cdnServerSelected=false;
	     //a peer can only connect to one CDN (AGH)
	if(meshType==0)
	{
		int bernulli1zero=0;
		int bernulli2zero=0;
		int bernulli2one=0;
		while (list.size() < size)
		{
			// this is simple mesh so peer have same chance to connect to a server or a peer -(AGH)
			if( cdnServerSelected==false)
			{
				int cdnprob=bernoulli(serverNum/(serverNum+peerList[filmNumber].size()+peerListWatchpast[filmNumber].size()),0);
				if(cdnprob==0)
					bernulli1zero++;
				if(cdnprob==1 ||bernulli1zero>10)//  sometime  bernoulli(x)=0 in all loops even x>0 so for this  we use bernulli1zero>10-AGH
				{
					for(CDNIt = CDNList.begin() ; CDNIt != CDNList.end() ; CDNIt++)
					{
						if( CDNIt->second.cdnfilmStreamed[filmNumber] < CDNIt->second.remainedClient && CDNIt->first == selectServer )// a peer only allow to connect to selected server-AGH
						{
							list.push_back(CDNIt->second.tAddress);
							cdnServerSelected=true;
						}
					}
					cdnServerSelected=true; // CDN don't have empty connection-AGH
				}
			}
			if(peerList[filmNumber].size()+peerListWatchpast[filmNumber].size()!=0)
			{
				int preWatchprob=bernoulli(peerListWatchpast[filmNumber].size()/peerList[filmNumber].size()+peerListWatchpast[filmNumber].size(),0);
				//  sometime  bernoulli(x)=0 in all loops even x>0 so for this  we use bernulli2zero>10-AGH
				if(preWatchprob==1||(bernulli2zero>10&&peerListWatchpast[filmNumber].size()>0))
				{
					bernulli2one++;
					int randomNum = intuniform(1,peerListWatchpast[filmNumber].size());
					for(int i=1; i<randomNum ; i++)
						++nodeWatchPastIt;
					if(satisfactionConnected())
					{
						if(nodeWatchPastIt != peerListWatchpast[filmNumber].end()&& nodeWatchPastIt->second.nodeInfo.tAddress != node
						 && !isInVector(nodeWatchPastIt->second.nodeInfo.tAddress,list)&& nodeWatchPastIt->second.nodeInfo.remainedNeighbor > 0
						 && watchStartTime>=nodeWatchPastIt->second.startTime &&  watchStartTime < nodeWatchPastIt->second.endTime+varianceTime
						 &&!isInTimeOutPeers(nodeIt->second.nodeInfo.tAddress,timeoutPeers[filmNumber]))
							list.push_back(nodeWatchPastIt->second.nodeInfo.tAddress);
					}
					else
					{
						if(nodeWatchPastIt != peerListWatchpast[filmNumber].end() && nodeWatchPastIt->first == selectServer && nodeWatchPastIt->second.nodeInfo.tAddress != node
						&& !isInVector(nodeWatchPastIt->second.nodeInfo.tAddress,list)&& nodeWatchPastIt->second.nodeInfo.remainedNeighbor > 0
						 && watchStartTime>=nodeWatchPastIt->second.startTime &&  watchStartTime < nodeWatchPastIt->second.endTime+varianceTime
						 &&!isInTimeOutPeers(nodeIt->second.nodeInfo.tAddress,timeoutPeers[filmNumber]))
							list.push_back(nodeWatchPastIt->second.nodeInfo.tAddress);
					}
					nodeWatchPastIt = peerListWatchpast[filmNumber].begin();
				}
				//  sometime  bernoulli(x)=0 in all loops even x>0 so for this  we use bernulli2one>10-AGH
				if(preWatchprob==0 ||( bernulli2one>10&&peerList[filmNumber].size()>0))
				{
					bernulli2zero++;
					int randomNum = intuniform(1,peerList[filmNumber].size());
					for(int i=1; i<randomNum ; i++)
						++nodeIt;
					if(satisfactionConnected())
					{
						if(nodeIt != peerList[filmNumber].end()
								&& nodeIt->second.nodeInfo.tAddress != node && !isInVector(nodeIt->second.nodeInfo.tAddress,list)
								&& nodeIt->second.nodeInfo.remainedNeighbor > 0&&!isInTimeOutPeers(nodeIt->second.nodeInfo.tAddress,timeoutPeers[filmNumber]))
							list.push_back(nodeIt->second.nodeInfo.tAddress);
					}
					else
					{
						if(nodeIt != peerList[filmNumber].end() && nodeIt->first == selectServer
								&& nodeIt->second.nodeInfo.tAddress != node && !isInVector(nodeIt->second.nodeInfo.tAddress,list)
								&& nodeIt->second.nodeInfo.remainedNeighbor > 0&&!isInTimeOutPeers(nodeIt->second.nodeInfo.tAddress,timeoutPeers[filmNumber]))
							list.push_back(nodeIt->second.nodeInfo.tAddress);
					}
					nodeIt = peerList[filmNumber].begin();
				}
			}
		}
		return 0;//only for warning
	}
	else if(meshType==1)
	{
		unsigned int sectionSize = 0;
		if(satisfactionConnected())
		{
			for(CDNIt = CDNList.begin() ; CDNIt != CDNList.end() ; CDNIt++)
			{
				if( cdnServerSelected==false && CDNIt->second.cdnfilmStreamed[filmNumber] < CDNIt->second.remainedClient )
				{
					list.push_back(CDNIt->second.tAddress);
					cdnServerSelected=true;
					sectionSize ++;
				}
			}
			for(nodeIt = peerList[filmNumber].begin() ; nodeIt != peerList[filmNumber].end() ; nodeIt++)
			{
				if( nodeIt->second.nodeInfo.tAddress != node && nodeIt->second.nodeInfo.remainedNeighbor > 0 &&!isInVector(nodeIt->second.nodeInfo.tAddress,list)&&!isInTimeOutPeers(nodeIt->second.nodeInfo.tAddress,timeoutPeers[filmNumber]))
				{
					list.push_back(nodeIt->second.nodeInfo.tAddress);
					sectionSize ++;
				}
			}
			for(nodeWatchPastIt = peerListWatchpast[filmNumber].begin() ; nodeWatchPastIt != peerListWatchpast[filmNumber].end() ; nodeWatchPastIt++)
			{
				if(nodeWatchPastIt->second.nodeInfo.tAddress != node && nodeWatchPastIt->second.nodeInfo.remainedNeighbor > 0 &&!isInVector(nodeWatchPastIt->second.nodeInfo.tAddress,list)
				 && watchStartTime>=nodeWatchPastIt->second.startTime &&  watchStartTime < nodeWatchPastIt->second.endTime+varianceTime &&!isInTimeOutPeers(nodeIt->second.nodeInfo.tAddress,timeoutPeers[filmNumber]))
				{
					list.push_back(nodeWatchPastIt->second.nodeInfo.tAddress);
					sectionSize ++;
				}
			}
		}
		else
		{
			for(CDNIt = CDNList.begin() ; CDNIt != CDNList.end() ; CDNIt++)
			{
				if(cdnServerSelected==false && CDNIt->second.cdnfilmStreamed[filmNumber] < CDNIt->second.remainedClient && CDNIt->first == selectServer )
				{
					list.push_back(CDNIt->second.tAddress);
					cdnServerSelected=true;
					sectionSize ++;
				}
			}
			for(nodeIt = peerList[filmNumber].begin() ; nodeIt != peerList[filmNumber].end() ; nodeIt++)
			{
				if(nodeIt->first == selectServer && nodeIt->second.nodeInfo.tAddress != node && nodeIt->second.nodeInfo.remainedNeighbor > 0 && !isInVector(nodeIt->second.nodeInfo.tAddress,list)&&!isInTimeOutPeers(nodeIt->second.nodeInfo.tAddress,timeoutPeers[filmNumber]))
				{
					list.push_back(nodeIt->second.nodeInfo.tAddress);
					sectionSize ++;
				}
			}
			for(nodeWatchPastIt = peerListWatchpast[filmNumber].begin() ; nodeWatchPastIt != peerListWatchpast[filmNumber].end() ; nodeWatchPastIt++)
			{
				if(nodeWatchPastIt->second.nodeInfo.tAddress != node && nodeWatchPastIt->second.nodeInfo.remainedNeighbor > 0&&!isInVector(nodeWatchPastIt->second.nodeInfo.tAddress,list)
				&& watchStartTime>=nodeWatchPastIt->second.startTime &&  watchStartTime < nodeWatchPastIt->second.endTime+varianceTime &&!isInTimeOutPeers(nodeIt->second.nodeInfo.tAddress,timeoutPeers[filmNumber]))
				{
					list.push_back(nodeWatchPastIt->second.nodeInfo.tAddress);
					sectionSize ++;
				}
			}
		}
		if(sectionSize > neighborSize)
			return neighborSize;
		else
			return sectionSize;
	}
}
int CDNP2PTracker::Structure1MeshCalculateSize(int peerCasadeNumber,unsigned int neighborSize, TransportAddress& sourceNode,int filmNumber,int watchStartTime,bool specialInteractiveNeghborReq)
{
		unsigned int sectionSize = 0;
		std::multimap <int,currentFilm>::iterator nodeIt =myStructiure[filmNumber].cascade[peerCasadeNumber].nodes.begin();
		std::multimap <int,currentFilm>::iterator DirectIt = myStructiure[filmNumber].cascade[peerCasadeNumber].directToServerNodes.begin();
		std::multimap <int,watchedFilm>::iterator nodeWatchPastIt = peerListWatchpast[filmNumber].begin();
		for(DirectIt =myStructiure[filmNumber].cascade[peerCasadeNumber].directToServerNodes.begin(); DirectIt != myStructiure[filmNumber].cascade[peerCasadeNumber].directToServerNodes.end() ; DirectIt++)
		{
			if( DirectIt->second.nodeInfo.remainedNeighbor > 0  )
				sectionSize ++;
		}
		for(int j=0;j<2;j++)
		{
			if(peerCasadeNumber-j>=0)
				for(nodeIt = myStructiure[filmNumber].cascade[peerCasadeNumber-j].nodes.begin(); nodeIt != myStructiure[filmNumber].cascade[peerCasadeNumber-j].nodes.end() ; nodeIt++)
				{
					if(nodeIt->second.nodeInfo.tAddress != sourceNode && nodeIt->second.nodeInfo.remainedNeighbor > 0  &&!isInTimeOutPeers(nodeIt->second.nodeInfo.tAddress,timeoutPeers[filmNumber]))
						sectionSize ++;
				}
		}
		for(nodeWatchPastIt = peerListWatchpast[filmNumber].begin() ; nodeWatchPastIt != peerListWatchpast[filmNumber].end() ; nodeWatchPastIt++)
		{
			if(nodeWatchPastIt->second.nodeInfo.tAddress != sourceNode && nodeWatchPastIt->second.nodeInfo.remainedNeighbor > 0
				&& watchStartTime>=nodeWatchPastIt->second.startTime &&  watchStartTime < nodeWatchPastIt->second.endTime+varianceTime
				&&!isInTimeOutPeers(nodeIt->second.nodeInfo.tAddress,timeoutPeers[filmNumber]))
				sectionSize ++;
		}
		if(specialInteractiveNeghborReq)
			sectionSize=neighborSize;
		if(sectionSize > neighborSize)
				return neighborSize;
			else
				return sectionSize;
}
int CDNP2PTracker::Structure2MeshCalculateSize(int peerCasadeNumber,unsigned int neighborSize, TransportAddress& sourceNode,int filmNumber,int watchStartTime,bool specialInteractiveNeghborReq)
{
		unsigned int sectionSize = 0;
		std::multimap <int,currentFilm>::iterator nodeIt =myStructiure[filmNumber].cascade[peerCasadeNumber].nodes.begin();
		std::multimap <int,currentFilm>::iterator DirectIt = myStructiure[filmNumber].cascade[peerCasadeNumber].directToServerNodes.begin();
		std::multimap <int,watchedFilm>::iterator nodeWatchPastIt = peerListWatchpast[filmNumber].begin();
		for(DirectIt =myStructiure[filmNumber].cascade[peerCasadeNumber].directToServerNodes.begin(); DirectIt != myStructiure[filmNumber].cascade[peerCasadeNumber].directToServerNodes.end() ; DirectIt++)
		{
			if( DirectIt->second.nodeInfo.remainedNeighbor > 0  )
				sectionSize ++;
		}
		for(nodeIt = myStructiure[filmNumber].cascade[peerCasadeNumber].nodes.begin(); nodeIt != myStructiure[filmNumber].cascade[peerCasadeNumber].nodes.end() ; nodeIt++)
		{
			if(nodeIt->second.nodeInfo.tAddress != sourceNode && nodeIt->second.nodeInfo.remainedNeighbor > 0  &&!isInTimeOutPeers(nodeIt->second.nodeInfo.tAddress,timeoutPeers[filmNumber]))
				sectionSize ++;
		}
		for(nodeWatchPastIt = peerListWatchpast[filmNumber].begin() ; nodeWatchPastIt != peerListWatchpast[filmNumber].end() ; nodeWatchPastIt++)
		{
			if(nodeWatchPastIt->second.nodeInfo.tAddress != sourceNode && nodeWatchPastIt->second.nodeInfo.remainedNeighbor > 0
				&& watchStartTime>=nodeWatchPastIt->second.startTime &&  watchStartTime < nodeWatchPastIt->second.endTime+varianceTime
				&&!isInTimeOutPeers(nodeIt->second.nodeInfo.tAddress,timeoutPeers[filmNumber]))
				sectionSize ++;
		}
		if(specialInteractiveNeghborReq)
			sectionSize=neighborSize;
		if(sectionSize > neighborSize)
				return neighborSize;
			else
				return sectionSize;
}
int CDNP2PTracker::SimpleMeshcalculateSize(unsigned int neighborSize, TransportAddress& sourceNode,int filmNumber,int watctStartTime)
{
	int selectServer = getServerNumber(sourceNode,filmNumber);
	unsigned int sectionSize = 0;
	std::multimap <int,currentFilm>::iterator nodeIt = peerList[filmNumber].begin();
	std::multimap <int,CdnServers>::iterator CDNIt = CDNList.begin();
	std::multimap <int,watchedFilm>::iterator nodeWatchPastIt = peerListWatchpast[filmNumber].begin();
	bool cdnServerSelected=false;
	if(satisfactionConnected())
	{
		for(CDNIt = CDNList.begin() ; CDNIt != CDNList.end() ; CDNIt++)
		{
			if(cdnServerSelected==false && CDNIt->second.cdnfilmStreamed[filmNumber] < CDNIt->second.remainedClient && CDNIt->first == selectServer) // a peer only allow to connect to selected server-AGH
			{
				sectionSize ++;
				cdnServerSelected=true;
			}
		}
		for(nodeIt = peerList[filmNumber].begin() ; nodeIt != peerList[filmNumber].end() ; nodeIt++)
		{
			 if(nodeIt->second.nodeInfo.tAddress != sourceNode && nodeIt->second.nodeInfo.remainedNeighbor > 0 && !isInTimeOutPeers(nodeIt->second.nodeInfo.tAddress,timeoutPeers[filmNumber]) )
				sectionSize ++;
		}
		for(nodeWatchPastIt = peerListWatchpast[filmNumber].begin() ; nodeWatchPastIt != peerListWatchpast[filmNumber].end() ; nodeWatchPastIt++)
		{
			if(nodeWatchPastIt->second.nodeInfo.tAddress != sourceNode && nodeWatchPastIt->second.nodeInfo.remainedNeighbor > 0
			  && watctStartTime>=nodeWatchPastIt->second.startTime &&  watctStartTime < nodeWatchPastIt->second.endTime+varianceTime
			  &&!isInTimeOutPeers(nodeIt->second.nodeInfo.tAddress,timeoutPeers[filmNumber]))
				 sectionSize ++;
		}
		if(sectionSize > neighborSize)
			return neighborSize;
		else
			return sectionSize;
	}
	else
	{
		for(CDNIt = CDNList.begin() ; CDNIt != CDNList.end() ; CDNIt++)
		{
			if(CDNIt->first == selectServer && cdnServerSelected==false&& CDNIt->second.cdnfilmStreamed[filmNumber] <CDNIt->second.remainedClient )
			{
				sectionSize ++;
				cdnServerSelected=true;
			}
		}

		for(nodeIt = peerList[filmNumber].begin() ; nodeIt != peerList[filmNumber].end() ; nodeIt++)
		{
			if(nodeIt->first == selectServer && nodeIt->second.nodeInfo.tAddress != sourceNode && nodeIt->second.nodeInfo.remainedNeighbor > 0  &&!isInTimeOutPeers(nodeIt->second.nodeInfo.tAddress,timeoutPeers[filmNumber]))
					sectionSize ++;
		}
		for(nodeWatchPastIt = peerListWatchpast[filmNumber].begin() ; nodeWatchPastIt != peerListWatchpast[filmNumber].end() ; nodeWatchPastIt++)
		{
			if(nodeWatchPastIt->first == selectServer && nodeWatchPastIt->second.nodeInfo.tAddress != sourceNode && nodeWatchPastIt->second.nodeInfo.remainedNeighbor > 0
			 && watctStartTime>=nodeWatchPastIt->second.startTime &&  watctStartTime < nodeWatchPastIt->second.endTime+varianceTime
			 &&!isInTimeOutPeers(nodeIt->second.nodeInfo.tAddress,timeoutPeers[filmNumber]))
				sectionSize ++;
		}

		if(sectionSize > neighborSize)
			return neighborSize;
		else
			return sectionSize;
	}
}
bool CDNP2PTracker::satisfactionConnected()
{
	if(connectedMesh)
	{
		int peerlistsize=0;
		for(int i=0;i<10;i++)
		{
			peerlistsize+=peerList[i].size();

		}
		if(5*(serverNum+2) > peerlistsize+serverNum)
			return false;
		else
			return true;
	}
	else
		return false;
}
int CDNP2PTracker::getServerNumber(TransportAddress& node, int filmnumber)
{
	std::map <TransportAddress,int>::iterator serverIt = peerServers[filmnumber].begin();
	serverIt = peerServers[filmnumber].find(node);
	return serverIt->second;
}


void CDNP2PTracker::SetServerNumber(TransportAddress& node,int filmnumber,int cascadeNum,int selectPolicy,IPvXAddress& accessRouter)
{

	// Total Load Balancing Policy
	// Fix Unregister scenario!!!
	if(selectPolicy == 2)
	{	
		int index = 0;
		// Initiate an iterator to check if the peer has already been assigned
		std::map <TransportAddress,int>::iterator serverIt = peerServers[filmnumber].begin();
		// Check if the peer is already assigned to any server
		serverIt = peerServers[filmnumber].find(node);
		
		

		if(serverIt == peerServers[filmnumber].end() || cascadeNum == -1)
		{

			// find a server with the lowest load
			index = filmnumber%serverNum+1; // for load balancing in multiple videos scenario
			int min = loadServers[index];
			for(unsigned int i = 1 ; i<serverNum+1 ; i++)
			{
				if(min > loadServers[i])
				{
					min = loadServers[i];
					index = i;
				}
			}
			loadServers[index] += 1;
			peerServers[filmnumber].insert(std::make_pair<TransportAddress,int>(node,index));
		}
		else// where a peer seek forward and change its cascade-AGH
		{
			index =((cascadeNum+filmnumber)%serverNum)+1;
		}
		
	}
	

	// Load Balancing Policy
	if(selectPolicy == 0)
	{
		
		std::map <TransportAddress,int>::iterator serverIt = peerServers[filmnumber].begin();
        // Check and see if the requesting node is already assigned to a server yet.
		serverIt = peerServers[filmnumber].find(node); 	

		if(serverIt == peerServers[filmnumber].end()||cascadeNum==-1) 
		{
			// initialize the index
			int index = 1;
			// in simplemesh
			if(cascadeNum==-1||meshStructure==0)
			{
				if(meshStructure==0)
				{
					int *serverPopulation = new int[serverNum+1];			// Load Balancing
					for(unsigned int i =1 ; i<serverNum+1 ; i++)
						serverPopulation[i]=0;
					for(serverIt = peerServers[filmnumber].begin(); serverIt != peerServers[filmnumber].end() ; serverIt++)
						serverPopulation[serverIt->second]++;
					index=filmnumber%serverNum+1;// for load balancing in multi channel(multi videos)- AGH
					int min = serverPopulation[index];
					for(unsigned int i =1 ; i<serverNum+1 ; i++)
					{
						if(min > serverPopulation[i])
						{
							min = serverPopulation[i];
							index = i;
						}
					}
					delete [] serverPopulation;
				}
				else
				{
					index =((myStructiure[filmnumber].lastCascadeNumber+filmnumber)%serverNum)+1;
				}
			}
			else// where a peer seek forward and change its cascade-AGH
			{
				index =((cascadeNum+filmnumber)%serverNum)+1;
			}
			peerServers[filmnumber].insert(std::make_pair<TransportAddress,int>(node,index));
		}
		
	}
	// Closest Policy
	if(selectPolicy == 1)
	{
		// Store requesting node whereabout
		cTopology::Node *requestNode = topo.getNodeFor(findNodeInTopo(accessRouter));
		// Initialize an iterator for peer-server map
		std::map <TransportAddress,int>::iterator serverIt = peerServers[filmnumber].begin();
        // Check and see if the requesting node is already assigned to a server yet.
		serverIt = peerServers[filmnumber].find(node); 
        // Initialize an iterator for CDN map
		std::multimap <int,CdnServers>::iterator CDNIt = CDNList.begin();
		// Calculate shortest path from all access routers to the request node by Djikstra's algorithm
		topo.calculateUnweightedSingleShortestPathsTo(requestNode);
		

		if(serverIt == peerServers[filmnumber].end() || cascadeNum == -1)
		{
			// Initialize index
			int index = 1;
			if(cascadeNum == -1 || meshStructure == 0)
			{
				if (meshStructure == 0)
				{
					// List out distance from each server to the requesting node
					// Initialize temporary container to collect hop counts
					int *hopToClient = new int[serverNum+1];
					for(unsigned int i=1 ; i<serverNum+1 ;i++)
					{
						hopToClient[i]=0;
					}
					for(CDNIt = CDNList.begin() ; CDNIt != CDNList.end() ; CDNIt++)
					{
						
						cTopology::Node *targetNode=topo.getNodeFor(findNodeInTopo(CDNIt->second.accessAddress));

						hopToClient[CDNIt->first]=targetNode->getDistanceToTarget();					
					}
					// Load Balancing for equal distance servers
					int *serverPopulation = new int[serverNum+1];
					for(unsigned int i =1 ; i<serverNum+1 ; i++)
						serverPopulation[i]=0;
					for(serverIt = peerServers[filmnumber].begin(); serverIt != peerServers[filmnumber].end() ; serverIt++)
						serverPopulation[serverIt->second]++;
					
					// Initialize minimal parameters
					index = filmnumber%serverNum+1;
					int minDistance = hopToClient[index];
					int minPopulation = serverPopulation[index];
					// Start a loop to check the minimal distance and population
					for (unsigned int i =1; i<serverNum+1; i++)
					{
						if(minDistance >= hopToClient[i])
						{
							if(minDistance > hopToClient[i])
							{
								minDistance = hopToClient[i];
								minPopulation = serverPopulation[i];
								index = i;
							}							
							else if(minDistance = hopToClient[i])
							{
								if(minPopulation > serverPopulation[i])
								{
									minDistance = hopToClient[i];
									minPopulation = serverPopulation[i];
									index = i;
								}
							}
						}
					}
					delete [] serverPopulation;
					delete [] hopToClient;
				}
			    else
				{
					index =((myStructiure[filmnumber].lastCascadeNumber+filmnumber)%serverNum)+1;
				}
			}
			else// where a peer seek forward and change its cascade-AGH
			{
				index =((cascadeNum+filmnumber)%serverNum)+1;
			}
			peerServers[filmnumber].insert(std::make_pair<TransportAddress,int>(node,index));		
		}
        
	}
	
}
void CDNP2PTracker::checkPeersTimOuts()
{
	for(unsigned int j=0; j < FilmList.numberOfFilms; j++)
	{
		int n=0;
		for (unsigned int i=0; i < timeoutPeers[j].size(); i++)
		{
			bool isExit=false;
			if(meshStructure==0)
			{
				std::multimap <int,currentFilm>::iterator nodeIt = peerList[j].begin();
				for(nodeIt = peerList[j].begin() ; nodeIt != peerList[j].end() ; nodeIt++)
				{
					if(nodeIt->second.nodeInfo.tAddress == timeoutPeers[j][i+n].tAddress)
					{
						if(simTime().dbl()-nodeIt->second.nodeInfo.timeOut> (2*neighborsTimeOutLimit))
						{
							peerList[j].erase(nodeIt);
							isExit=true;
						}
						break;
					}
				}
			}
			int cascadeNum=timeoutPeers[j][i+n].cascadenumber;
			if( meshStructure==1||meshStructure==2)
			{

				std::multimap <int,currentFilm>::iterator nodeIt = myStructiure[j].cascade[cascadeNum].nodes.begin();
				for(nodeIt = myStructiure[j].cascade[cascadeNum].nodes.begin() ;nodeIt!= myStructiure[j].cascade[cascadeNum].nodes.end() ; nodeIt++)
				{
					if(nodeIt->second.nodeInfo.tAddress == timeoutPeers[j][i+n].tAddress)
					{
						if(simTime().dbl()-nodeIt->second.nodeInfo.timeOut>(2*neighborsTimeOutLimit))
						{
							myStructiure[j].cascade[cascadeNum].nodes.erase(nodeIt);
							isExit=true;
						}
						break;
					}
				}

			}
			if(isExit)
			{
				std::multimap <int,watchedFilm>::iterator pastnodeIt = peerListWatchpast[j].begin();
				for(pastnodeIt = peerListWatchpast[j].begin() ; pastnodeIt != peerListWatchpast[j].end() ; pastnodeIt++)
				{
					if(pastnodeIt->second.nodeInfo.tAddress== timeoutPeers[j][i+n].tAddress)
					{
						peerListWatchpast[j].erase(pastnodeIt);
					}
				}
				peerServers[j].erase(timeoutPeers[j][i+n].tAddress);
				// in seldom time peer maybe not exit-AGH
				TrackerMessage* youUnRegMsg =new TrackerMessage(" nodeUnRegistered");
				youUnRegMsg->setCommand(You_UnRegistered);
				sendMessageToUDP(timeoutPeers[j][i+n].tAddress,youUnRegMsg);
				deleteVector(timeoutPeers[j][i+n].tAddress,timeoutPeers[j]);
				n--;
			}
		}
	}
}
void CDNP2PTracker::deleteVector(TransportAddress Node,std::vector <timeOutPeer> &timeoutpeers)
{
	try
	{
		for (unsigned int i=0; i!=timeoutpeers.size(); i++)
		{
			if(timeoutpeers[i].tAddress.isUnspecified())
			{
				timeoutpeers.erase(timeoutpeers.begin()+i,timeoutpeers.begin()+1+i);
				break;
			}
		}
		//if(Node.isUnspecified())
			//return;
		for (unsigned int i=0; i!=timeoutpeers.size(); i++)
		{
			if (Node == timeoutpeers[i].tAddress)
			{
				timeoutpeers.erase(timeoutpeers.begin()+i,timeoutpeers.begin()+1+i);
				break;
			}
		}
	}
	catch(std::exception& e)
	{
		std::cout << "time: " << simTime()<< "deleteVector error in Peers mesh "<<getParentModule()->getParentModule()->getFullName() <<  std::endl;
	}

}

cModule *CDNP2PTracker::findNodeInTopo(const IPvXAddress& addr)
{
	//ev << "The infrastructure consists of " << topo.getNumNodes() << " nodes.";
	for (int i=0; i<topo.getNumNodes();i++)
	{
		cTopology::Node *node = topo.getNode(i);
			if(IPAddressResolver().resolve(node->getModule()->getFullName()) == addr)
			{
				cModule *mod = topo.getNode(i)->getModule();
				return mod;
			}


	}

}
