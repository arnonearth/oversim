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
 * @file PeersMesh.h
 * @author Abdollah Ghaffari Sheshjavani(AGH)
 */

#ifndef PEERSMESH_H_
#define PEERSMESH_H_

#include <omnetpp.h>
#include <OverlayKey.h>
#include <TransportAddress.h>
#include <GlobalNodeList.h>
#include "MeshMessage_m.h"
#include <MeshOverlay.h>
#include <IPAddressResolver.h>


class PeersMesh : public MeshOverlay
{
protected:
	int numberOfVideos;
	int meshStructure;//0 is simple mesh 1 is CTMVOD//  2 is TMVOD...(AGH)
 	int isStable;// if is true peer do not exit from system-AGH
	int simpleMeshType;
	int cascadeNumber;// use in my structure-AGH
	int hopCount;// we use this for my structure-AGH
	bool isSource;/**< whether this node is server*/
    int activeNeighbors;/**< Neighbor that node connects to them*/
    int passiveNeighbors; /**< Neighbors that node let them connect to it*/
    int neighborNum; /**< Sum of active and passive neighbors*/
   // int currentFilmNum;// film that peer watch it (AGH)
    int startTime;// start time of current film (AGH)
    int expireTime;
    int interactiveTime;
    int startIntractiveSimTime; //sim time that after this time peer can interactive-AGH
    bool haveInteractive;
    double percentofExit;
    double percentOfNotGracefullExit;// its between 0 and 1-AGH
    bool checkNeighborsTimeOut; //when we have peers that Not Gracefull Exit- it must be true-AGH
    double neighborsTimeOutLimit;// for check not graceful exit
   	int startExitSimTime;
    double percentofInteractive; //if is 1 peer dont exit from system -2 it can seek forward-3 can exit with exit prob-4 can exit and seek forward  -0 (real world)is base on result of paper"can vod profitable"-AGH
    int Timer; // we use this in meshJoinRequestTimer for scheduling different timer.(AGH)
    double neighborNotificationPeriod; /**< period to notify tracker about number of current neighbor*/
    double videoAverageRate[10]; /**< average video bit rate for calculating neighbor num*/
    double changeProbability;
    bool adaptiveNeighboring; /**< true if we want to have adaptive neighbors*/
    bool isRegistered; /**< if this node registered in the tracker */
    bool recivelistfilm;
    bool connectDirectToServer;
    bool isheadnode;// use in my structure - AGH
    bool connectToHeadNode;// use in my structure - AGH
    bool neighborSelelctionStart;
   // bool peerExit;//AGH
    bool improvmentStructureStart;//AGH
    bool emergency;
    bool specialInteractiveNeghborReq;
    bool usingSpecialInteractiveNeghborReq;
    bool haveChangecascadeForInteractive;
    //int cdnClients;//in this array holding the max number of connection for stream per film that cdn must accept.
    int nof;// number of films (AGH)
   // int filmStreamed[10][100];//in this array holding the number of connection for stream per film and per cascade that cdn  accepted.(AGH)
    int watchFilmEndTime[10];// in this array holding end time of Film watched in past .(AGH) // max 10 film
    bool usePreVideo;
    double improvementProcessTimeLenght;
    TransportAddress trackerAddress; /**< Transport address of tracker node */
    TransportAddress serverAddress; /**< Transport address of server node */
    IPvXAddress accessAddress; /** <IP address of connected Access Router */
    std::vector <neighbor> requestedNeighbors; /**< when node have graceful exit- it must disconnect from nodes that request from them but not response yet-AGH */
   // std::map <TransportAddress,double> neighborTimeOut; /**< */
    /**
     * Register node in the tracker
     */
    int minimum(int A,int B);
    void selfRegister(bool connectDirecttoServer);
    /**
     * Unregister node in the tracker
     */
    void selfUnRegister();
    /**
     * for leaving this method do this process for node that want to leave with notification
     * @param TransportAdress
     */
    void disconnectProcess(TransportAddress node,int filmId,int cascadeNumber, int disconnectType);
    void disconnectFromNode(TransportAddress Node,int cascadeNumber);

    //
    //selfMessages
    cMessage* meshJoinRequestTimer; /**< self message for scheduling neighboring*/
    cMessage* remainNotificationTimer; /**< self message for scheduling send notification to server*/
     cMessage* improvementProcessTimeLenghtTimer; /**< time that the improvement process start to end-AGH  */

    // statistics
	uint32_t stat_TotalUpBandwidthUsage;
	uint32_t stat_TotalDownBandwidthUsage;

    uint32_t stat_joinREQ; /**< number of sent join request messages */
	uint32_t stat_joinREQBytesSent;  /**< number of sent bytes of join request messages */
	uint32_t stat_joinRSP; /**< number of sent join response messages */
	uint32_t stat_joinRSPBytesSent; /**< number of sent bytes of join response messages */
	uint32_t stat_joinACK; /**< number of sent join acknowledge messages */
	uint32_t stat_joinACKBytesSent; /**< number of sent bytes of join acknowledge messages */
	uint32_t stat_joinDNY; /**< number of sent join deny messages */
	uint32_t stat_joinDNYBytesSent; /**< number of sent bytes of join deny messages */
	uint32_t stat_disconnectMessages; /**< number of sent disconnect messages */
	uint32_t stat_disconnectMessagesBytesSent; /**< number of sent bytes of disconnect messages */
	uint32_t stat_addedNeighbors; /**< number of added neighbors during life cycle of this node */
	uint32_t stat_nummeshJoinRequestTimer; /**< number of meshJoinRequestTimer self messages */
	//improve parameters-AGH
	uint32_t stat_improvementREQ; /**< number of sent improvement request messages */
	uint32_t stat_improvementREQBytesSent;  /**< number of sent bytes of improvement request messages */
	uint32_t stat_improvementRSP; /**< number of sent improvement response messages */
	uint32_t stat_improvementRSPBytesSent; /**< number of sent bytes of improvement response messages */
	uint32_t stat_improvementACK; /**< number of sent improvement acknowledge messages */
	uint32_t stat_improvementACKBytesSent; /**< number of sent bytes of improvement acknowledge messages */
	uint32_t stat_improvementDNY; /**< number of sent improvement deny messages */
	uint32_t stat_improvementDNYBytesSent; /**< number of sent bytes of improvement deny messages */
	int stat_numberOfImprovement;//AGH
	//change place for improvement parameters-AGH
	uint32_t stat_ChangePlaceREQ; /**< number of sent ChangePlace request messages */
	uint32_t stat_ChangePlaceREQBytesSent;  /**< number of sent bytes of ChangePlace request messages */
	uint32_t stat_ChangePlaceRSP; /**< number of sent ChangePlace response messages */
	uint32_t stat_ChangePlaceRSPBytesSent; /**< number of sent bytes of ChangePlace response messages */
	uint32_t stat_ChangePlaceACK; /**< number of sent ChangePlace acknowledge messages */
	uint32_t stat_ChangePlaceACKBytesSent; /**< number of sent bytes of ChangePlace acknowledge messages */
	uint32_t stat_ChangePlaceDNY; /**< number of sent ChangePlace deny messages */
	uint32_t stat_ChangePlaceDNYBytesSent; /**< number of sent bytes of improvement deny messages */



public:

    /**
     * initializes base class-attributes
     *
     * @param stage the init stage
     */
	virtual void initializeOverlay(int stage);
    /**
     * Writes statistical data and removes node from bootstrap oracle
     */
	virtual void finishOverlay();
	virtual void handleTimerEvent(cMessage* msg);
	virtual void handleUDPMessage(BaseOverlayMessage* msg);
	virtual void handleAppMessage(cMessage* msg);
	virtual void checkNeighborsTimOuts();
	virtual void joinOverlay();
	virtual void changeFilm();
	virtual void neighborRequest(int neighborSize);//AGH
	virtual void joinResponse(bool isServer,int FilmEndTime,TransportAddress& destinationNode,bool specialInteractiveNeghborReq);//AGH
	virtual void joinDeny(bool isserver,TransportAddress& destinationNode);//AGH
	virtual void setStatistics();//AGH
	virtual void disconnectFromPreviouseNeighbors();//AGH
	virtual void disconnectFromPreviouseActiveNeighbors();//AGH
	virtual void requesterJoinAck(MeshMessage* Meshmsg);//AGH
	int findNeighborIndex(TransportAddress& Node, std::vector <neighbor> &neighbors);
	// its for select  one film (AGH)
   	int selectingOneFilm(int min, int max);
   	// its for find time that peer watching one film(in gamma distribution)-AGH
   	virtual void timeWatchingFilm(int FilmLenghtSecond);
   // its for changing Film or exit system


    /*  First integer is for film number that this node want to have it or whach it in past(AGH)
            *  second integer is for start time of film that this node watch in past(AGH)
            * third integer is for end time of film that this node watch in past(AGH)
         */
       std::multimap <int,int,int> FilmListWatchpast;


       /**
     *notify its neighbors that it is going to leave the mesh
     */
	virtual void handleNodeGracefulLeaveNotification();
	//not graceful nodes
	virtual void nodeLeave();
	//notify its neighbors that it is going to leave the film
	virtual void handleNodeGracefulChangeFilmNotification();

    /**
     * Search neighbor list with specific TransportAddress to see
     * if they are neighbor or not
     *
     * @param Node the TransportAddress
     * @param vector<TransportAddress> neighbor list
     */
    bool isInVector(TransportAddress& Node, std::vector <neighbor> &neighbors);

    /**
     * Delete the node from its neighbors list
     *
     * @param Node the TransportAddress
     * @param vector<TransportAddress> neighbor list
     */
    void deleteVector(TransportAddress Node,std::vector <neighbor> &neighbors);

    void introduceItself();
};

#endif /* PEERSMESH_H_ */

