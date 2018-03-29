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
 * @file Chunk.h
 * @author Abdollah Ghaffari Sheshjavani(AGH), Behnam Ahmadifar, Yasser Seyyedi
 */

#include "Chunk.h"

Chunk::Chunk()
{
	chunkNumber = -1;
}
Chunk::~Chunk()
{
}
void Chunk::setValues(int ChunkSize)
{
	chunkSize = ChunkSize;
	chunk = new VideoFrame[chunkSize];
}
bool Chunk::isComplete()
{
	for(int i = 0 ; i<chunkSize ; i++)
		if(!chunk[i].isSet())
			return false;
	return true;
}
void Chunk::setFilmNumber(int FilmNumber)
{
	filmNumber=FilmNumber;
}
void Chunk::setChunkNumber(int ChunkNumber)
{
	chunkNumber = ChunkNumber;
}
void Chunk::setFrame(bool isServer,VideoFrame vFrame)
{
	int ExtractedFrameNum = vFrame.getFrameNumber()%chunkSize;
	chunk[ExtractedFrameNum].setFrame(vFrame);

}
int Chunk::getChunkByteLength()
{
	int totalSize = 0;
	if(isComplete())
	{
		for(int i = 0 ; i < chunkSize ; i++)
			totalSize += chunk[i].getFrameLength();
		return totalSize;
	}
	else
		return -1;
}
std::ostream& operator<<(std::ostream& os, const Chunk& c)
{
	os << c.chunkNumber<< ": ";
	for(int i = 0 ; i < c.chunkSize ; i++)
		os << c.chunk[i] << " ";
	return os;
}
void Chunk::setHopCout(int HopCount)
{
	hopCount = HopCount;
}
double Chunk::getCreationTime()
{
	return chunk[chunkSize -1].getCreationTime();
}
int Chunk::getLastFrameNo()
{
	return chunk[chunkSize-1].getFrameNumber();
}
int Chunk::getLateArrivalLossSize(int playBackPoint)
{
	int totalSize = 0;
	for(int i = 0 ; i < chunkSize ;i++)
		if(chunk[i].getFrameNumber() > playBackPoint)
			totalSize += chunk[i].getFrameLength();
	return totalSize;
}
