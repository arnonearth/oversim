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
 * @author Abdollah Ghaffari Sheshjavani (AGH),Behnam Ahmadifar, Yasser Seyyedi
 */

#ifndef CHUNK_H_
#define CHUNK_H_
#include "VideoFrame.h"

class Chunk
{
protected:

	int chunkNumber; /**< The chunk number of the created object */
	short int filmNumber;  // Film number of chunk- for vod -(AGH)
	short int chunkSize; /**< Number of frames in the chunk object*/
	short int hopCount; /**< Hop count of the chunk that traverse through the network*/
	double creationTime; /**< Time that the chunk is created. It is use for calculating end to end delay*/
public:
	VideoFrame* chunk; /**< array that keeps video frames as a chunk*/
	/**
	 * This function is used in order to initialize the chunk class
	 *
	 * @param ChunkSize number of frame available in this chunk
	 */
	void setValues(int ChunkSize);
	/**
	 * Base class constructor
	 */
	Chunk();
	/**
	 * Base class distructor
	 */
	~Chunk();
	/**
	 * set a frame in chunk array acording to its number
	 *
	 * @param VideoFrame a frame to place in this chunk
	 */
	void setFrame(bool isServer,VideoFrame vFrame);
	/**
	 * check whether this chunk fill with its frames
	 *
	 * @return boolean true if it is contained all frames
	 */
	bool isComplete();
	/**
	 * set the Film id of this chunk-AGH
	 *
	* @param FilmNumber is id of film-AGH
	*/
	void setFilmNumber(int FilmNumber);

	/**
	 * set the number of this chunk
	 *
	 * @param ChunkNumber the number to set for chunk number
	 */
	void setChunkNumber(int ChunkNumber);

	/* * set the Film id of this chunk-AGH
	 *
	* @param FilmNumber is id of film-AGH
	*/
	 int getFilmNumber(){return filmNumber;};
	/**
	 * get the chunk number of this chunk
	 *
	 * @return integer the chunk number
	 */
	 int getChunkNumber(){return chunkNumber;}
	/**
	 * get the byte size of the chunk based on contained frames and other variables
	 *
	 * @return integer byte size of this chunk
	 */
	int getChunkByteLength();
	/**
	 * get the value of the hop that the chunk traverse
	 *
	 * @return integer hop count number
	 */
	int getHopCount(){return hopCount;}
	/**
	 * for geting the time of creating this chunk
	 * @return double creation time value
	 */
	double getCreationTime();
	/**
	 * set hop count number
	 *
	 * @param HopCount to be set
	 */
	void setHopCout(int HopCount);
    /**
     * standard output stream for Chunk,
     * gives out all contained frames
     *
     * @param os the ostream
     * @param c the Chunk
     * @return the output stream
     */
	friend std::ostream& operator<<(std::ostream& os, const Chunk& c);
	/**
	 * gives the last frame number that is insert in this chunk
	 *
	 * @return integer las frame numbers
	 */
	int getLastFrameNo();
	/**
	 * calculate the size of frame that will lost due to late arrival
	 *
	 * @param playbackPoint frame number that is currently is playing
	 * @return integer size of late arrival frames
	 */
	int getLateArrivalLossSize(int playBackPoint);
};
#endif /* CHUNK_H_ */
