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
 * @file CDNP2PTracker.h
 * @author Abdollah Ghaffari Sheshjavani (AGH)
 */

#ifndef CDNP2PTRACKER_H_
#define CDNP2PTRACKER_H_

#include <BaseOverlay.h>
#include "TrackerMessage_m.h"
#include "VideoMessage_m.h"
#include "MeshOverlayMessage_m.h"
#include "IPAddressResolver.h"



/**<
 * a struct that contains node information
 *
 */
struct ListOfFilms
{
	int numberOfFilms;//
	int FrameOfFilms[10];//cdn max 10
	int lenthOfFilmsSec[10];//cdn max 10
	int FPS;//cdn max 10

};
struct Temp
{
	int cascadeNumber;
	int startTime;
};
struct nodeInformation
{
	TransportAddress tAddress;		/**< address of node */
    IPvXAddress accessAddress; /**< address of connected Access Router */
	int remainedNeighbor;/**< number of remained neighbor that node could accept*/
	int reserveRemainedNeighbor;/**< number of remained neighbor that node could accept we reserve this for a peer for a few time in my structure-AGH*/
	int reserveTimeStart;//AGH
	double timeOut;             /**< Time out for neighbor notification*/
	bool havedirectlinktocdnserver;//for peer (because where one peer go out from system we can understand)(AGH)
	bool isHeadnode;
	bool haveChangecascadeForInteractive;
	int hopCount;//AGH
};
struct CdnServers
{
	TransportAddress tAddress;		/**< address of CDN server */
    IPvXAddress accessAddress; /**< address of connected Access Router */
	int remainedClient;
	double timeOut;
	int cdnfilmStreamed[10];// for CDN server

};
struct watchedFilm
{
	  /*    * second integer is for film number that this node want to have it or whach it in past(AGH)
	         *  third integer is for start time of film that this node watch in past(AGH)
	         * forth integer is for end time of film that this node watch in past(AGH)
	*/
	nodeInformation nodeInfo;
	int filmnumber;
	int startTime;
	int endTime;

};

struct currentFilm
{
	 /*         * second integer is for film number that this node want to have it or whach it in past(AGH)
	*/
	nodeInformation nodeInfo;
	int filmnumber;
	int startTime;
};
struct timeOutPeer
{
	TransportAddress tAddress;		/**< address of node */
	int cascadenumber;//AGH
	double timeOut;
};
struct cascades
{
	std::multimap <int,currentFilm> directToServerNodes;
	int reservedirectToServerNodesNumber;
	int reservedirectToServerNodesTimeStart;
	std::multimap <int,currentFilm> headCascadeNodes;
	int reserveheadCascadeNodesNumber;
	int reserveheadCascadeNodesTimeStart;
	std::multimap <int,currentFilm> nodes;
	//TransportAddress* directToServerNodes;
	//TransportAddress* headCascadeNodes;
	//TransportAddress* nodes;
	int initiateTime;
	int lastHop;
};
struct structiureMesh
{
cascades* cascade;
int lastCascadeNumber;
};
class CDNP2PTracker : public BaseOverlay
{
public:
	virtual void initializeOverlay(int stage);
	virtual void finishOverlay();
	virtual void handleTimerEvent(cMessage* msg);
	virtual void handleUDPMessage(BaseOverlayMessage* msg);
	virtual void joinOverlay();
	virtual void peerUnregister(TrackerMessage* trackerMsg);

protected:
    int loadServers[6]; // Total load of each server for 5 server
    // bool assignedFlag; // Flag for checking if the peer is already assigned to any server or not
    int selectPolicy; // 0 is Load balancing // 1 is Closest // 2 is Fastest // 3 is Interest-based
	int meshStructure;// 0 is simple mesh // 1 is CTMVOD//  2 is TMVOD...(AGH)
	int directConnectionToServers;
	int meshType;//0 is randomly and 1 is ORDINAL -AGH
    bool connectedMesh;	/**< if we have multiple server connects mesh that form under them*/
    bool usePreVideo; // if its true peer that watch a video in past can serve this video when he dont wach this(AGH).
    int CdnClients;
    int varianceTime;// this is used for connect watchPast peer to other peer-max difference(AGH)
    int reserveConnectionTime;// its in second-AGH
    int minTimeLenghtWP;// this is min time that peer must watch film in past to can serve that film in future
    int passiveNeighbors;//AGH
    int cascadeMinTime;//AGH
    int cascadeMaxTime;//AGH
    int cascadeMinNodes;//AGH
    int cascadeMaxNodes;//AGH
    int maxCascadeNumber;//AGH
    int peerCasadeNumber;
    double neighborsTimeOutLimit;// for check not graceful exit-AGH
    double diferenceForCascadeMinesforFWD;// for seek forward-AGH
    structiureMesh myStructiure[10];// we have one structure per video- max 10 video
    //selfMessages
    cMessage* endReserveRemainNeighborsTimer; /**< self message for scheduling send notification to server*/
    cMessage* checkPeersTimeoutTimer; /**< self message for check Peers Timeout */
    /**
     * fill the list with node that are going to send to requested peer
     * @param list list to be sent
     * @param node the node that request in order to exclude from list
     * @param size how many peer should be insert in the list
     */
    ListOfFilms FilmList;// hold list of films(number of films and number of frame of each film and duration of films in second(AGH)
    VideoMessage* listOfFilmsMsg;// because this msg must send to all peer then we hold this. (AGH)
    bool RecivedListOfFilm;// when cdn send list of films, it set to true.(AGH)
     int Structure1MeshFillList(int peerHopCount,int& peerCasadeNumber,unsigned int neighborSize,std::vector <TransportAddress>& list, TransportAddress& node,int filmNumber,int watchStartTime,bool specialInteractiveNeghborReq);//AGH
     int Structure2MeshFillList(int peerHopCount,int& peerCasadeNumber,unsigned int neighborSize,std::vector <TransportAddress>& list, TransportAddress& node,int filmNumber,int watchStartTime,bool specialInteractiveNeghborReq);//AGH
     int Structure1MeshCalculateSize(int peerCasadeNumber,unsigned int neighborSize, TransportAddress& sourceNode,int filmNumber,int watchStartTime,bool specialInteractiveNeghborReq);//AGH
     int Structure2MeshCalculateSize(int peerCasadeNumber,unsigned int neighborSize, TransportAddress& sourceNode,int filmNumber,int watchStartTime,bool specialInteractiveNeghborReq);//AGH
     int SimpleMeshFillList(unsigned int neighborSize,std::vector <TransportAddress>& list, TransportAddress& node,int filmNumber, unsigned int size,int watchStartTime);
 	 virtual void initiateMystructiure(int index,int videoLength);
 	 void deleteVector(TransportAddress Node,std::vector <timeOutPeer> &timeoutpeers);
 	 bool isInTimeOutPeers(TransportAddress& Node, std::vector <timeOutPeer> &timeoutpeers);
     /*
     * check a given vector that contains a node
     * @param node the given node
     * @param list the given list
     * @return Boolean true if the list contains node
     */
    bool isInVector(TransportAddress& Node, std::vector <TransportAddress> &list);
    /*
     * calculate the size of node that tracker should retrun to requested node
     * @param neighborSize number of node that a peer requested
     * @param sourceNode the server that node is member of
     * @return integer number of node that a sever could return
     */
    int SimpleMeshcalculateSize(unsigned int neighborSize, TransportAddress& sourceNode,int filmNumber,int watctStartTime);
    /**
     * specify whether it is the time to connect meshes under servers (time to return nodes from other meshes)
     * @return Boolean true if it is time
     */
    bool satisfactionConnected();
    /**
     * find the server number that a node is member of
     * @param node the address of node to check
     * @return integer the server index number
     */
    int getServerNumber(TransportAddress& node, int filmnumber);
    /*
     * function to assign a server to the given node
     * @param node the given node
     */
    void SetServerNumber(TransportAddress& node, int filmnumber,int cascadeNum,int selectPolicy,IPvXAddress& accessRouter);
    /*
     *
     */
    void checkPeersTimOuts();
    /**< map contains all peer that send at least one message to server
     * first integer = serverID, TransportAddress = peer TransportAddress
	      */
    std::multimap <int,currentFilm> peerList[10];
    std::vector<timeOutPeer> timeoutPeers[10];

    /**< map contains all CDN Servers (AGH)
         * first integer = serverID, TransportAddress = peer TransportAddress
    	      */
        std::multimap <int,CdnServers> CDNList;

    /**< map contains all peer that watch one or different films in past
         first integer = serverID, TransportAddress = peer TransportAddress
	          */
    std::multimap <int,watchedFilm> peerListWatchpast[10];
    /**< map contains all peer that send at least one message to server
     * TransportAddress = Peer TransportAddress , integer = serverID
     * */
    std::map <TransportAddress,int> peerServers[11];
    unsigned int serverNum; /**< number of servers currently in the network*/

    /*< map contains all peer that do interactive but not register yet
     second parameter: integer is cascade number
   /TransportAddress = Peer TransportAddress */
    std::multimap <TransportAddress,Temp> interactiveTemp[10];

    virtual cModule *findNodeInTopo(const IPvXAddress& addr);

    // topology structure for shortest path finding
    cTopology topo;

    // set zero to server load, called at the tracker's initialization
    //void iniLoad();

};
#endif /* CDNP2PTRACKER_H_ */
