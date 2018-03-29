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
 * @file LocalVariables.h
 * @author Abdollah Ghaffari Sheshjavani (AGH),Yasser Seyyedi, Behnam Ahmadifar
 */

#ifndef LOCALVARIABLES_H_
#define LOCALVARIABLES_H_

#include <omnetpp.h>
#include "VideoBuffer.h"
#include <TransportAddress.h>

struct neighbor
{
	short type; // 0 is a active - 1 is passive
	TransportAddress address;
	int endTime;
	double lastnotifyed;
	int videoId;//used only on CDN server
	int cascadeNumber;
};
class LocalVariables: public cSimpleModule
{
public:
	/**
	 * For initiation of the variables
	 */
	virtual void initialize();
    /**
     * gives out current node download bandwidth
     *
     * @return download bandwidth of this node
     */
    double getDownBandwidth(){return downBandwidth;}
    /**
     * gives out current node upload bandwidth
     *
     * @return upload bandwidth of this node
     */
    double getUpBandwidth(){return upBandwidth;}
    /**
     * Update the node BufferMap based on video buffer
     */
    void updateLocalBufferMap(int filmId);
    /**
     * set download bandwidth (by overlay)
     * @param DB download bandwidth
     */
    void setDownBandwidth(double DB);
    /**
     * set upload bandwidth (by overlay)
     */
    void setUpBandwidth(double UB);
    /**
     * get the value of late arrival loss (by player)
     * @return double late arrival loss size
     */
	virtual double getLateArivalLoss(){return stat_lateArrivalLossSize;}
	/**
     * get the value of availability and rate control loss (by player)
     * @return double sum of availability and rate control loss size
	 */
	virtual double getAvailability_RateControlLoss(){return stat_availability_RateControlLoss;}
	/**
	 * add values to statistics of loss due to availability and rate control in APP
	 * @param double Avail_RateControl availability rate control parameter
	 */
	virtual void addToAvailability_RateControlLoss(double Avail_RateControl);
	/**
	 * add value to statistics of loss due to late arrival
	 * @param LateArrivalLoss
	 */
	virtual void addToLateArivalLoss(double LateArivalLoss);

	bool watchFilm[10];// in this array holding true if Film watched in past .(AGH) // max 10 film
	VideoBuffer* videoBuffer[10]; /**<Create new buffer for existing host to keep latest Chunks max 10 film*/
    BufferMap* hostBufferMap[10]; /**<Create our own Buffer map to announce other neighbors max 10 film */
    std::vector <neighbor> neighbors;	/**< Vector in which keeps node neighbors' TransportAddresses*/
    bool WatchExpired;
    bool isFreeRider;
    int playbackpoint;
    int currentFilmId;
    int recieveTime;
    int hopCount;
    bool needNewneighbor;
    bool interactive;
    int cascadeNumber;
    bool StructureImproving;
    TransportAddress improveNeighbor;
    TransportAddress previousimproveNeighbor;
    TransportAddress ppreviousimproveNeighbor;
    short improveScore;
    int seekTime;
    bool emergency;
    bool improveLock;
    bool improveDeny;
    bool peerExit;


protected:

    int windowOfIntrest; /**< size of window of interest in second*/
	int Fps; /**< Frame per second*/
	int chunkSize; /**< number of frames in a chunk*/
	int gopSize; /**<Number of frames per Group of Picture */
    int numOfBFrame; /**<Number of B frames between 'I' and 'P' or between two 'P' frames */
    int bufferSize; /** number of chunks in the video buffer */
    double downBandwidth; /**< The download bandwidth that node initially has*/
    double upBandwidth; /**< The upload bandwidth that node initially has*/
	double stat_lateArrivalLossSize; /**< statistic that is all losses due late arrival */
	double stat_availability_RateControlLoss; /**< statistics that is all losses due to availability or rate control */
};

#endif /* LOCALVARIABLES_H_ */
