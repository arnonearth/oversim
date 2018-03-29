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
 * @file BufferMap.cc
 * @author Abdollah Ghaffari Sheshjavani(AGH), Behnam Ahmadifar, Yasser Seyyedi
 */

#include "BufferMap.h"

#include <iostream>

void BufferMap::setLastSetChunk(int LastSetChunk)
{
	lastSetChunk = LastSetChunk;
}
void BufferMap::setValues(int BufferSize)
{
	bufferSize = BufferSize;
	lastSetChunk = 0;
	buffermap = new bool[bufferSize];
	chunkNumbers = new  int[bufferSize];
	for(int i = 0 ; i < bufferSize ; i++)
	{
		buffermap[i] = false;
		chunkNumbers[i]=i;
	}
	initiate=true;
	interactiveChunkStart=-1;
	seekChuncknumbers=0;

}
void BufferMap::setinteractive(int ChunkStart, int sChuncknumbers)
{
	 interactiveChunkStart=ChunkStart;
	 seekChuncknumbers=sChuncknumbers;
	 for(int i = ChunkStart-chunkNumbers[0] ; i < bufferSize ; i++)
	 {
		 chunkNumbers[i]=i+seekChuncknumbers;
	 }

}
void BufferMap::setChunk(int index,int chunkNumber,bool exist)
{
	if(index < bufferSize )
	{
		buffermap[index]=exist;
		chunkNumbers[index]=chunkNumber;
	}

	else
		std::cout << "!--"<<index<<" is larger than "<<bufferSize<< " set chunk error "<<  std::endl;

}
bool BufferMap::getChunk(int index)
{
	if(index<bufferSize)
		{
			if(buffermap[index]==true)
				return true;
			else
				return false;
		}
		else
			std::cout << "!--"<<index<<"is larger than"<<bufferSize<< " get chunk error "<< std::endl;

}
int BufferMap::getChunkNumber(int index)
{
	if(index<bufferSize)
			{
				return chunkNumbers[index];
			}
			else
				std::cout <<"!--"<<index<<"is larger than"<<bufferSize<< " get chunk Number error "<<std::endl;

}

bool BufferMap::findChunk(int ChunkNumber,bool limitedBuffer)
{
	if (initiate==false)
	{
		return false;
	}

	if(limitedBuffer || interactiveChunkStart<=0)
	{
		//	int x=	chunkNumbers[0];
		if(ChunkNumber - chunkNumbers[0] >= bufferSize || ChunkNumber - chunkNumbers[0] < 0)
			return false;
		if(buffermap[ChunkNumber - chunkNumbers[0]])
				return true;
			else
				return false;

	}
	else if(interactiveChunkStart>0)
	{
		if(ChunkNumber-seekChuncknumbers - chunkNumbers[0] >= bufferSize || ChunkNumber-seekChuncknumbers - chunkNumbers[0] < 0)
			return false;
		if(buffermap[ChunkNumber -seekChuncknumbers- chunkNumbers[0]])
			return true;
		else
			return false;
	}
}
std::ostream& operator<<(std::ostream& os, const BufferMap& b)
{
	for(int i = 0 ; i < b.bufferSize ; i++)
	{
		os << b.chunkNumbers[i] << ": " << b.buffermap[i] << "  ";
		if((i+1)%12 == 0)
			os << std::endl;
	}
	return os;
}
int BufferMap::getNextUnsetChunk(std::vector<requestedFrames>& sendframes ,
				std::vector<int>& notInNeighbors, int playbackPoint,bool retryReq,int deadlineframe,int retryReqTime,PlayingState playState,int numberOfFrameRequested)// in this function playbackPoint is chunk- we call: getNextUnsetChunk(...(playbackPoint+1)/chunkSize[currentFilmId]...)-AGH
{
	int unSetChunk = -1;
	bool c1 = true;
	bool c2 = true;
	for(int i=0 ; i<bufferSize ; i++)
	{
		if (playbackPoint>chunkNumbers[i])
		{
			i+=playbackPoint-chunkNumbers[i];
			unSetChunk=playbackPoint;
		}
		else
			unSetChunk = chunkNumbers[i];
		//if(playbackPoint > unSetChunk)
		//	continue;
		c1=true;
		c2=true;
		if(!buffermap[i])
		{
			for (unsigned int k=0; k!=sendframes.size(); k++)
			{
				if (sendframes[k].frameNum == unSetChunk)
				{
					int delay=simTime().dbl()-sendframes[k].requestTime;
					if(retryReq && delay>retryReqTime && (sendframes[k].frameNum>deadlineframe ||playState==BUFFERING))
					{
						sendframes.erase(sendframes.begin()+k,sendframes.begin()+1+k);
						numberOfFrameRequested--;// when retry we must notice to rate control-AGH
					}
					else
						c1=false;
					break;
				}
			}
			if(c1)
				for (unsigned int k=0; k!=notInNeighbors.size(); k++)
				{
					if (notInNeighbors[k] == unSetChunk)
					{
						c2=false;
						break;
					}
				}
			if(c1 && c2)
				return unSetChunk;
		}
	}
	return -1;
}
int BufferMap::getBitLength()
{
	return bufferSize+16+16+16;
}
