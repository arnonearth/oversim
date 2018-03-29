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
 * @file ChunkPackets.cc
 * @author Behnam Ahmadifar, Yasser Seyyedi
 */

#include "ChunkPackets.h"

ChunkPackets::ChunkPackets(int packetunit,Chunk CHunk,int videomsglength,int RedundantNum)
{
	packetUnit = packetunit;
	videoMsgLength = videomsglength;
	redundantNum = RedundantNum;
	setChunk(CHunk);
	if(videoMsgLength%packetunit == 0)
		segmentNumber = videomsglength/packetunit ;
	else
		segmentNumber = videomsglength/packetunit +1;
	fragmentsState = new bool[segmentNumber + redundantNum];
	errorState = new bool [segmentNumber + redundantNum];
	for (int i=0; i<segmentNumber + redundantNum ; i++)
	{
		fragmentsState[i]=false;
		errorState[i] = false;
	}
}
bool ChunkPackets::isComplete()
{
	for(int i=0;i< segmentNumber + redundantNum ;i++)
		if(!fragmentsState[i])
		{
			return false;
		}
	return true;
}
bool ChunkPackets::FECSatisfaction()
{
	int total = 0;
	for(int i=0 ; i< segmentNumber + redundantNum ;i++)
		if(!errorState[i] && fragmentsState[i])
			total++;
	if(total >= segmentNumber)
		return true;
	else
		return false;
}
void ChunkPackets::setChunk(Chunk CHunk)
{
	chunk = CHunk;
}
