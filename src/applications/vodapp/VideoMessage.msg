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

//
//@author Abdollah Ghaffari Sheshjavani(AGH),Behnam Ahmadifar, Yasser Seyyedi
//
cplusplus
{{
#include <BufferMap.h>
#include <TransportAddress.h>
#include "VideoFrame.h"
#include "Chunk.h"
   
// constants for message length in bit


static const int CHUNKNUMBER_L = 16;
static const int FRAMENUMBER_L = 16;
static const int FRAMETYPE_L = 2;
static const int FRAMELENGTH_L = 16;
static const int TOTALBANDWIDTH_L = 32;
static const int VIDEOCOMMAND_L = 3;

#define VIDEOMESSAGE_L(msg) (VIDEOCOMMAND_L + CHUNKNUMBER_L + 2*TRANSPORTADDRESS_L+9)
#define BUFFERMAPMESSAGE_L(msg) (2*TRANSPORTADDRESS_L + TOTALBANDWIDTH_L+6*CHUNKNUMBER_L+5)
}}

class noncobject BufferMap;
class noncobject TransportAddress;
class noncobject VideoFrame;
class noncobject Chunk;

//
// Some constants for Command-Field
// @author Abdollah Ghaffari Sheshjavani (AGH), Behnam Ahmadifar, Yasser Seyyedi
//
enum VideoCommand
{
    CAMERA_MSG=0;	// Message from Camera to VideoSource
    PLAYER_MSG=1;	// Message from VideoSource to Player
    CHUNK_REQ=2;	// Message for Requesting a frame
    CHUNK_RSP=3;	// Message for Responsing a frame
    NEIGHBOR_LEAVE=4;
    LEAVING=5;
    select_film=6;
    
}
//
// Main VideoBuffer-Packet definition
// @author Abdollah Ghaffari Sheshjavani(AGH), Behnam Ahmadifar, Yasser Seyyedi
//
packet VideoMessage
{
	short type;// 0 if cdn send it and 1 if client request(AGH)- 1 bit need
    int command enum(VideoCommand);
    TransportAddress dstNode;	// detination node
	TransportAddress srcNode;	// source node
	double deadLine;			// deadline of chunk
	Chunk chunk;			// Chunk object
	VideoFrame VFrame;		// VideoFrame object
    short selectedFilms;//client(AGH)
    bool firstRequest = false; // Flag for the first request message
   
 	
}
// for select films
//@author Abdollah Ghaffari Sheshjavani(AGH)
packet SelectFilm
{
	short type;// 0 if exit peer and 1 if change video (AGH)
	TransportAddress srcNode;	// source node
	short numberOfFilms;//cdn(AGH)
    int FrameOfFilms[10];//cdn max 10(AGH)
    int lenthOfFilmsSec[10];//cdn max 10(AGH)
  	int selectedStartTimeSec;//client(AGH)
 	int FPS[10];//(AGH)
 	short selectedFilms;//client(AGH)
}
//
// BufferMap exchange packet for exchange to neighbors (pull-based)
// @author Abdollah Ghaffari Sheshjavani(AGH),Yasser Seyyedi, Behnam Ahmadifar
//
packet BufferMapMessage
{
	TransportAddress srcNode;		// Source Node handle
	short filmId;		// film Id
    double totalBandwidth;   // Free Bandwidth
    int downBandwith;
    int receivePoint;
    int playbackPoint; // its use only for structure improving-AGH
    int lastBufferPoint;
    bool isServer;// its true if is server else is false -AGH
    bool unicast;// if its false send buffer map to all neighbor
    // double myHopCount;	// hop count
   	TransportAddress DstNode;   // destination peer
    BufferMap buffermap;	// BufferMap Object
    int myVideoBufferLastSetChunk;
    bool whatchInPast;//(AGH)
    bool firstBufferMap;//AGH
    bool pleaseSendBigbufferMap;//AGH
    short hopCount;//AGH
    short redundantsize; // only uused for evaluating buffer-map ex mechanism- Not added to overhead or msg length
   
}

//@author Abdollah Ghaffari Sheshjavani(AGH)    
packet VideoProperty
{
    int lenthOfFilmsSec;
  	int selectedStartTimeSec;
 	int FPS;
 	int selectedFilms;
 	int EndTimeWatching;
 	int interactiveTime;
}
//@author Abdollah Ghaffari Sheshjavani(AGH) 
packet CreateStream
{
    TransportAddress dstNode;	// detination node
	TransportAddress srcNode;	// source node
	int selectedFilm;//client(AGH)
	int selectedStartTimeSec;
	int downBandwith;//AGH
	double delay;// delay for sending in sender to recive in reciever-AGH
}