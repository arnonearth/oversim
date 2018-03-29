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
 * @file BufferMap.h
 * @author Abdollah Ghaffari Sheshjavani(AGH),Behnam Ahmadifar, Yasser Seyyedi
 */

#ifndef BUFFERMAP_H_
#define BUFFERMAP_H_


#include <vector>
#include <iostream>

#include <TransportAddress.h>

struct requestedFrames
{
	int frameNum;
	double requestTime;
	TransportAddress destNode;
};
enum PlayingState
{
    PLAYING = 0,
    BUFFERING = 1,
    STOP = 2
};
class BufferMap
{
protected:
	int lastSetChunk; /** < The last chunk that is set in the buffer */
	int bufferSize; /** < number of chunks that exist in the video buffer*/

public:
	bool* buffermap;  /** < array of boolean that shows the availability of chunks in the video buffer*/
	int* chunkNumbers; /** < array of number associated with each bit of buffermap*/
	bool initiate;
	int	interactiveChunkStart;
	int	seekChuncknumbers;

	/*
	 * Find a chunk based on its number
	 *
	 * @param ChunkNumber chunk number to check
	 * @return boolean true if it finds
	 */
	virtual bool findChunk(int ChunkNumber,bool limitedBuffer);
	void setChunk(int index,int chunkNumber,bool exist);
	bool getChunk(int index);
	int getChunkNumber(int index);
    /**
     * standard output stream for BufferMap,
     * print buffermap as well as its associated chunk number
     *
     * @param os the ostream
     * @param b the BufferMap
     * @return the output stream
     */
	friend std::ostream& operator<<(std::ostream& os, const BufferMap& b);
	/**
	 * initialize default values of vairables
	 *
	 * @param BufferSize number of chunk that is available in the video buffer
	 */
	void setValues(int BufferSize);
	void setinteractive(int ChunkStart, int sChuncknumbers);
	int getBufferSize(){return bufferSize;}
	/**
	 * retrieve the next chunk that is not set in playing order
	 *
	 * @param sendFrames a vector that keeps the chunks that requested but it is not in buffer (they should be excluded)
	 * @param notInNeighbors the chunks that return before but they are not available in neighbors
	 * @param playbackPoint the frame that is playing now
	 * @return integer the chunk number to check for request
	 */
	/**<
	 *  Vector in which keeps the sending frames
	 * to prevent duplicate request.
	 */
	int getNextUnsetChunk(std::vector <requestedFrames>& sendframes ,
				std::vector<int>& notInNeighbors, int playbackPoint,bool retryReq,int deadlineframe,int retryReqTime,PlayingState playState,int numberOfFrameRequested);
	/**
	 * Get the last chunk that is set in the video buffer
	 *
	 * @return integer last chunk that is set in video buffer
	 */
	int getLastSetChunk(){return lastSetChunk;}
	/**
	 * set the value of last chunk
	 *
	 * @param LastSetChunk value to set for last set chunk
	 */
	void setLastSetChunk(int LastSetChunk);
	/**
	 * caclute the size of BufferMap object for setting in packets
	 *
	 * @return integer size of bits
	 */
	int getBitLength();
};
#endif /* BUFFERMAP_H_ */
