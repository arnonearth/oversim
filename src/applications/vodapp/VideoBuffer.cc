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
 * @file VideoBuffer.cc
 * @author Abdollah Ghaffari Sheshjavani (AGH), Yasser Seyyedi, Behnam Ahmadifar
 */

#include "VideoBuffer.h"

VideoBuffer::VideoBuffer(int NumOfBFrame,int BufferSize, int ChunkSize, int GopSize)
{
	gopSize = GopSize;
	numOfBFrame = NumOfBFrame;
	bufferSize = BufferSize;
	chunkSize = ChunkSize;
	lastSetChunk = 0;
	lastSetFrame = 0;
	interactiveChunkStart=-1;
	seekChuncknumbers=0;
	chunkBuffer = new Chunk[bufferSize];
	for(int i = 0 ; i < bufferSize ; i++)
	{
		chunkBuffer[i].setValues(chunkSize);
		chunkBuffer[i].setChunkNumber(i);
	}
}
VideoBuffer::~VideoBuffer()
{
	delete[] chunkBuffer;
}
void VideoBuffer::setinteractive(int ChunkStart, int Chuncknumbers)
{
	 interactiveChunkStart=ChunkStart;
	 seekChuncknumbers=Chuncknumbers;
	 for(int i = ChunkStart-chunkBuffer[0].getChunkNumber() ; i < bufferSize ; i++)
	 {
	  	chunkBuffer[i].setChunkNumber(i+Chuncknumbers);
	 }

}
void VideoBuffer::updateBufferMapXchange(BufferMap* BufferMap,int start,int lenght, bool limitedbuffer, int redundantSize)
{
	// It is adaptive buffer map exchange(AGH)

	start-=redundantSize;
	int point=start-chunkBuffer[0].getChunkNumber();

		for(int i=0 ; i < lenght ; i++)
		{
			if(!limitedbuffer && i+start>interactiveChunkStart+seekChuncknumbers && interactiveChunkStart>0)
				point=start-seekChuncknumbers;
			if(  i+point>=0 && i+point < bufferSize)
			{
				if(!limitedbuffer && interactiveChunkStart<i+start && i+start<interactiveChunkStart+seekChuncknumbers &&interactiveChunkStart>0)
					BufferMap->buffermap[i] = false;
				else if(chunkBuffer[i+point].isComplete())
					BufferMap->buffermap[i] = true;
				else
					BufferMap->buffermap[i] = false;
				BufferMap->chunkNumbers[i] = chunkBuffer[i+point].getChunkNumber();
			}
			else
			{
				BufferMap->buffermap[i] = false;
				BufferMap->chunkNumbers[i] =i+point;
			}
		}
		if(lenght<=1)
		{
			BufferMap->setLastSetChunk(0);
		}
		//else if(bufferSize>start+lenght-1)
		else if(bufferSize>point+lenght-1)
		{
			//if(lastSetChunk>chunkBuffer[start+lenght-1].getChunkNumber())
			if(lastSetChunk>chunkBuffer[point+lenght-1].getChunkNumber())
				BufferMap->setLastSetChunk(chunkBuffer[point+lenght-1].getChunkNumber());
			else
				BufferMap->setLastSetChunk(lastSetChunk);
		}
		else
			BufferMap->setLastSetChunk(lastSetChunk);
		//if(BufferMap->getLastSetChunk()<0)
		//	BufferMap->setLastSetChunk(0);
}

void VideoBuffer::updateBufferMap(BufferMap* BMap)
{
	// It is Not adaptive buffer map exchange(AGH)
	for(int i=0 ; i < bufferSize ; i++)
	{
		if(chunkBuffer[i].isComplete())
			BMap->buffermap[i] = true;
		else
			BMap->buffermap[i] = false;
		BMap->chunkNumbers[i] = chunkBuffer[i].getChunkNumber();
	}
	BMap->setLastSetChunk(lastSetChunk);
}
std::ostream& operator<<(std::ostream& os, const VideoBuffer& v)
{
	for (int i=0 ; i < v.bufferSize ; i++)
	{
		os << v.chunkBuffer[i];
		if((i+1)%v.gopSize == 0)
			os << std::endl;
	}
	return os;
}
void VideoBuffer::shiftChunkBuf()
{
	for(int j=0 ; j < 1 ; j++)
	{
		for(int i=0 ; i < bufferSize-1 ; i++)
		{
			chunkBuffer[i] = chunkBuffer[i+1];
		}
		Chunk ch;
		ch.setValues(chunkSize);
		ch.setChunkNumber(chunkBuffer[bufferSize-2].getChunkNumber()+1);
		chunkBuffer[bufferSize-1] = ch;
	}
}
void VideoBuffer::setFrame(bool isServer,VideoFrame vFrame, bool limitedBuffer)
{
	if (isServer==true)
	{
		int ExtractedChunkNum = vFrame.getFrameNumber()/(chunkSize);
		chunkBuffer[ExtractedChunkNum].setFrame(true,vFrame);
		if(lastSetFrame < vFrame.getFrameNumber())
				lastSetFrame = vFrame.getFrameNumber();
		if(chunkBuffer[ExtractedChunkNum].isComplete())
				lastSetChunk = ExtractedChunkNum;

	}

	else
	{
		int ExtractedChunkNum = vFrame.getFrameNumber()/(chunkSize);
		//while(ExtractedChunkNum > chunkBuffer[bufferSize-2].getChunkNumber())
			//	shiftChunkBuf();

		if(ExtractedChunkNum >=  chunkBuffer[0].getChunkNumber() &&
				ExtractedChunkNum <= chunkBuffer[bufferSize-1].getChunkNumber())
		{
			if( !limitedBuffer && ExtractedChunkNum>interactiveChunkStart && interactiveChunkStart>0 && chunkBuffer[0].getChunkNumber()<interactiveChunkStart)
				chunkBuffer[ExtractedChunkNum - chunkBuffer[0].getChunkNumber()-seekChuncknumbers].setFrame(false,vFrame);
			else
				chunkBuffer[ExtractedChunkNum - chunkBuffer[0].getChunkNumber()].setFrame(false,vFrame);

			if(lastSetFrame < vFrame.getFrameNumber())
				lastSetFrame = vFrame.getFrameNumber();
			if(chunkBuffer[ExtractedChunkNum - chunkBuffer[0].getChunkNumber()].isComplete())
				lastSetChunk = ExtractedChunkNum;
		}
		else
			std::cout << "(ChunkBuffer::setFrame) ChunkBuffer Out of boundary!!!!!!!!" << std::endl;
	}
}
VideoFrame VideoBuffer::getFrame(int FrameNumber,bool limitedBuffer)
{
	int ExtractedChunkNum = FrameNumber/(chunkSize);
	if(ExtractedChunkNum >=  chunkBuffer[0].getChunkNumber() &&
			ExtractedChunkNum <= chunkBuffer[bufferSize-1].getChunkNumber())
	{
		if(!limitedBuffer && ExtractedChunkNum>interactiveChunkStart && interactiveChunkStart>0 && chunkBuffer[0].getChunkNumber()<interactiveChunkStart )
		{
			if(ExtractedChunkNum - chunkBuffer[0].getChunkNumber()-seekChuncknumbers>=0)
				return chunkBuffer[ExtractedChunkNum - chunkBuffer[0].getChunkNumber()-seekChuncknumbers]
			                   .chunk[FrameNumber%chunkSize].getVFrame();
			else
				return chunkBuffer[ExtractedChunkNum - chunkBuffer[0].getChunkNumber()]
						                   .chunk[FrameNumber%chunkSize].getVFrame();
		}

		else
		{
		return chunkBuffer[ExtractedChunkNum - chunkBuffer[0].getChunkNumber()]
		                   .chunk[FrameNumber%chunkSize].getVFrame();
		}
	}
	else
	{

		std::cout << chunkBuffer[0].getChunkNumber() << "  to  "<< chunkBuffer[bufferSize-1].getChunkNumber()<< std::endl;
		std::cout << "ExtractedChunkNum : "<<ExtractedChunkNum <<std::endl;
		std::cout << "(VideoBuffer::getFrame) ChunkBuffer Out of boundary!!!!!!!!" << std::endl;
	}
}
Chunk VideoBuffer::getChunk(int ChunkNumber,bool limitedBuffer)
{
	try
	{
	if(ChunkNumber >=  chunkBuffer[0].getChunkNumber() &&
			ChunkNumber <= chunkBuffer[bufferSize-1].getChunkNumber())
	{
		if(!limitedBuffer && ChunkNumber>interactiveChunkStart && interactiveChunkStart>0 &&chunkBuffer[0].getChunkNumber()<interactiveChunkStart)
		{
			if(ChunkNumber - chunkBuffer[0].getChunkNumber()-seekChuncknumbers>=0)
				return chunkBuffer[ChunkNumber - chunkBuffer[0].getChunkNumber()-seekChuncknumbers];
			else
					return chunkBuffer[ChunkNumber - chunkBuffer[0].getChunkNumber()];
		}
		else
			return chunkBuffer[ChunkNumber - chunkBuffer[0].getChunkNumber()];
	}
	else
	{
		std::cout << chunkBuffer[0].getChunkNumber() << "  to  "<< chunkBuffer[bufferSize-1].getChunkNumber()<< std::endl;
		std::cout << "ChunkNum : "<<ChunkNumber <<std::endl;
		std::cout << "(ChunkBuffer:getChunk) Chunk Not Found !!!!!"<< std::endl;
	}
	}
	catch (std::exception& e)
	{
		std::cout << "(ChunkBuffer:getChunk) Chunk Not Found !!!!!"<< std::endl;
	}
}
void VideoBuffer::setChunk(Chunk InputChunk,bool limitedBuffer)
{
	int index = InputChunk.getChunkNumber() - chunkBuffer[0].getChunkNumber();
	if(InputChunk.getChunkNumber()>interactiveChunkStart && interactiveChunkStart>0 &&chunkBuffer[0].getChunkNumber()<interactiveChunkStart)
	{
		if (InputChunk.getChunkNumber()<seekChuncknumbers+interactiveChunkStart)
		{
			// when a peer seek forward it may receive some chunk that it  request them previous-so this chunk deleted-AGH
			return;
		}
		if(!limitedBuffer)
			index-=seekChuncknumbers;
	}
	if(InputChunk.getChunkNumber() >=  chunkBuffer[0].getChunkNumber() &&
				InputChunk.getChunkNumber() <= chunkBuffer[bufferSize-1].getChunkNumber())
	{
		chunkBuffer[index] = InputChunk;
		//if(lastSetChunk < chunkBuffer[index].getChunkNumber())
			lastSetChunk = chunkBuffer[index].getChunkNumber();
	}
	else
		std::cout << "(ChunkBuffer::setChunk) ChunkBuffer Out of boundary!!!!!!!!" << std::endl;
}

