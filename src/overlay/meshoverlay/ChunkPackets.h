//
// Copyright (C) 2010 DenaCast All Rights Reserved.
// http://www.denacast.com
// The DenaCast was designed and developed at the DML(Digital Media Lab http://dml.ir/)
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
 * @file ChunkPackets.h
 * @author Behnam Ahmadifar, Yasser Seyyedi
 */

#ifndef CHUNKPACKETS_H_
#define CHUNKPACKETS_H_

#include <Chunk.h>

class ChunkPackets
{
	int packetUnit; /**<maximum length of each packet*/
	Chunk chunk; /**<an object of class Video frame to determine which frame this packet belongs*/
	int videoMsgLength; /**< length of video message*/
	int redundantNum; /**< number of redundant packets that generated for FEC*/
public:
	int segmentNumber; /**<each chunk divided to segments and each segment has a identifier*/
	/**
	 * Base class constructor
	 *
	 * @param packetunit the size of each packet
	 * @param Chunk the video chunk
	 * @param videomsglength the length of original response frame message
	 */
	ChunkPackets(int packetunit,Chunk CHunk,int videomsglength,int RedundantNum);

	/**
	 * gives out the packet Unit
	 * @return integer packet unit
	 */
	int getPacketUnit(){return packetUnit;}
	/**
	 * set the class video frame to video response msg frame
	 * @param frame the video frame
	 */
	void setChunk(Chunk CHunk);

	/**
	 * gives out video chunk for current situation of packetization
	 * @return Chunk
	 */
	Chunk getChunk(){return chunk;}

	/**
	 * check if all of the dependent packets to current chunk is received
	 * @return boolean true if all packets are recieved
	 */
	bool isComplete();
	/**
	 * check whether enough number of FEC packets are recieved
	 */
	bool FECSatisfaction();

	bool *fragmentsState; /**<array of segment status*/
	bool *errorState; /**< array of segment error status*/
};

#endif /* CHUNKPACKETS_H_ */
