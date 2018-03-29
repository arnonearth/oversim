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
 * @file CdnServer.h
 * @author Abdollah Ghaffari Sheshjavani(AGH)
 */

#ifndef CDNSERVER_H_
#define CDNSERVER_H_

#include <omnetpp.h>
#include <OverlayKey.h>
#include <TransportAddress.h>
#include <GlobalNodeList.h>
#include "MeshMessage_m.h"
#include <MeshOverlay.h>
#include "IPAddressResolver.h"


class CdnServer : public MeshOverlay
{
protected:
	int meshStructure;// 0 is simple mesh// 1 is CTMVOD//  2 is TMVOD...(AGH)
	int directConnectionToServers;
	int meshType; // 0 is randomly and 1 is ORDINAL -AGH
	int hopCount;// we use this for my structure-AGH
	bool isSource;/**< whether this node is server*/
    int activeNeighbors;/**< Neighbor that node connects to them*/
    int passiveNeighbors; /**< Neighbors that node let them connect to it*/
    int neighborNum; /**< Sum of active and passive neighbors*/
    //int currentFilmNum;// film that peer watch it (AGH)
    double neighborNotificationPeriod; /**< period to notify tracker about number of current neighbor*/
    double videoAverageRate[10]; /**< average video bit rate for calculating neighbor num*/
    bool isRegistered; /**< if this node registered in the tracker */
    bool serverGradualNeighboring; /**< true if gradual neighbor is required for source node*/
    int serverGradualNeighboringAverageTime;
    bool recivelistfilm;
    int cdnClients;//in this array holding the max number of connection for stream per film that cdn must accept.
    int nof;// number of films (AGH)
    int serverStress[10];
    int filmStreamed[10][100];//in this array holding the number of connection for stream per film and per cascade that cdn  accepted.(AGH)
    TransportAddress trackerAddress; /**< Transport address of tracker node */
    double neighborsTimeOutLimit;// for check not graceful exit
    bool checkNeighborsTimeOut; //when we have peers that Not Gracefull Exit- it must be true-AGH
    IPvXAddress accessAddress; //connected accessrouter address
  //  std::map <TransportAddress,double> neighborTimeOut; /**< */
    /**
     * Register node in the tracker
     */
    void selfRegister(bool connectDirecttoServer);
    /**
     * Unregister node in the tracker
     */
    void selfUnRegister();
    /**
     * for leaving this method do this process for node that want to leave with notification
     * @param TransportAdress
     */
    void disconnectProcess(TransportAddress node,int filmId,int cascadeNumber);
     //
    //selfMessages
    cMessage* serverStressTimer; /**< self message for scheduling neighboring*/
    cMessage* remainNotificationTimer; /**< self message for scheduling send notification to server*/
    cMessage* serverNeighborTimer; /**< for gradual neighboring this self message plan for this job */

    

    // statistics
	uint32_t stat_TotalUpBandwidthUsage;
	uint32_t stat_TotalDownBandwidthUsage;
  	uint32_t stat_joinRSP; /**< number of sent join response messages */
	uint32_t stat_joinRSPBytesSent; /**< number of sent bytes of join response messages */
	uint32_t stat_joinDNY; /**< number of sent join deny messages */
	uint32_t stat_joinDNYBytesSent; /**< number of sent bytes of join deny messages */
	uint32_t stat_disconnectMessages; /**< number of sent disconnect messages */
	uint32_t stat_disconnectMessagesBytesSent; /**< number of sent bytes of disconnect messages */
	uint32_t stat_addedNeighbors; /**< number of added neighbors during life cycle of this node */
	//change place for improvement parameters-AGH
	uint32_t stat_ChangePlaceRSP; /**< number of sent ChangePlace response messages */
	uint32_t stat_ChangePlaceRSPBytesSent; /**< number of sent bytes of ChangePlace response messages */
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
	virtual void joinOverlay();
	virtual void joinResponse(bool isServer,int FilmEndTime,TransportAddress& destinationNode);//AGH
	virtual void joinDeny(bool isserver,TransportAddress& destinationNode);//AGH
	virtual void setStatistics();//AGH
	virtual void checkNeighborsTimOuts();
	/*  First integer is for film number that this node want to have it or whach it in past(AGH)
        *  second integer is for start time of film that this node watch in past(AGH)
        * third integer is for end time of film that this node watch in past(AGH)
     */
     std::multimap <int,int,int> FilmListWatchpast;
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

#endif /* CDNSERVER_H_ */

