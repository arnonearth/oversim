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
 * @file BaseVodStream.h
 * @author  Abdollah Ghaffari Sheshjavani (AGH), Yasser Seyyedi, Behnam Ahmadifar
 */
#ifndef BASEVODSTREAM_H_
#define BASEVODSTREAM_H_


#include <omnetpp.h>
#include <TransportAddress.h>
#include <iostream>
#include <string>
#include <fstream>
#include "VideoMessage_m.h"
#include <BaseApp.h>
#include <GlobalNodeList.h>
#include "LocalVariables.h"

struct VideoFilm
{
	// 10 is max number of trace files (AGH)
	std::string filmPath;
	unsigned short int numOfBFrame; /**<Number of B frames between 'I' and 'P' or between two 'P' frames */
	unsigned short int chunkSize; /**< Number of frames in a chunk - */
	unsigned short int gopSize; /**< number of frame available in one GoP (Group of picture)*/
	int bufferSize; /**<number of chunk in buffer */
	int videolenght;// length of videos that peer watching them hold in this variable (AGH)
	unsigned short int Fps; /**< Store parameter Fps, Frame per second of the video*/


};

class BaseVodStream : public BaseApp
{
public:
    // application routines
	virtual void initializeApp(int stage);
    virtual void handleTimerEvent(cMessage* msg);
    virtual void finishApp();
   	/**
	 * base class-destructor
	 */
	~BaseVodStream();
    /**
     * Read first line that contains frame information
     * @param file handle that opened for reading
     */
	virtual int readFrames(std::ifstream * file, int filmno);
	//virtual void sendFrame(TransportAddress peerNode,int framenumber,int filmno);
	/**
	 * update time substring on top of module
	 *
	 * @param frameNo current streaming frame number
	 */
	virtual void updateTime(int filmNumber,int long frameNo);
	virtual int getLastFrameStreamed(){return lastFrameStreamed;}
	std::map <int,int> frameInfo;	// frame number, frame size

protected:

	//self messages
	cMessage* TraceFileInit; /**< self message for start streaming*/

	int lastFrameStreamed; /**< last frame that streamed*/
	int nof;/** number of films()(AGH)*/


	std::vector <VideoFilm> traceFile;/**< address of trace file in the computer(AGH)*/
	std::ifstream baseFile; /**< stream reader for reading file*/
	long int totalBit;
	std::vector<VideoFrame> tempVideoBuffer;
	int tempPointer;
	LocalVariables* LV; /** < a pointer to LocalVariables module*/

	//statistics
	uint32_t stat_totalBytesend; /**< number of bits that received */
	uint32_t stat_totalFramesent; /**< number of frames that sent */
	uint32_t stat_totalIframesent; /**< number of I frames that sent */
	uint32_t stat_totalBframesent; /**< number of B frames that sent */
	uint32_t stat_totalPframesent; /**< number of P frames that sent */
	double stat_startStreamingTime; /**<start streaming time of this node */
};

#endif /* BASECAMERA_H_ */
