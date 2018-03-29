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
 * @file VideoBuffer.h
 * @author Abdollah Ghaffari Sheshjavani (AGH), Yasser Seyyedi, Behnam Ahmadifar
 */

#ifndef VIDEOBUFFER_H_
#define VIDEOBUFFER_H_

#include "Chunk.h"
#include "BufferMap.h"

class VideoBuffer
{
protected:
	short int numOfBFrame; /**<Number of B frames between 'I' and 'P' or between two 'P' frames; */
	int bufferSize; /**< number of chunk available in the buffer*/
	short int chunkSize; /**< number of frames available in a chunk*/
public:
	int interactiveChunkStart;
	int seekChuncknumbers;

	int lastSetChunk; /**< last chunk that recently set*/
	int lastSetFrame; /**< last frame that recently set*/
	short int gopSize; /**< Number of frames per Group of Picture */
	Chunk* chunkBuffer; /**< video buffer that keeps some number of chunks*/

	/**
	 * class constructor
	 * @param NumOfBFrame number of B-frame between I and P or two P-frames
	 * @param BufferSize number of chunk available in the buffer
	 * @param ChunkSize number of frame available in a chunk
	 */
	VideoBuffer(int NumOfBFrame,int BufferSize, int ChunkSize, int GopSize);
	/**
	 * class distructor
	 */
	~VideoBuffer();
	/**
	 * set BufferMap array according to what is available in the buffer
	 * @param BMap BufferMap to set
	 */
	void setinteractive(int ChunkStart, int Chuncknumbers);
	void updateBufferMap(BufferMap* BMap);
	void updateBufferMapXchange(BufferMap* BufferMap,int start,int lenght, bool limitedbuffer, int redundantSize);
	/**
	 * Shift the buffer one chunk ( one chunk will be deleted and new empty chunk will add)
	 */
	void shiftChunkBuf();
	/**
	 *	gives out the type of a frame based on its number
	 *	@param FrameNumber frame number to check
	 *	@return character type of the frame
	 */
	char getFrameType(int FrameNumber);
	/**
	 * set a frame in the buffer according to its frame number
	 * @param vFrame video frame to set
	 */
	void setFrame(bool isServer,VideoFrame vFrame,bool limitedBuffer);
	/**
	 * gives out a video frame according to given frame number
	 * @param FrameNumber frame number to retrieve
	 * @return VideoFrame video frame
	 */
	VideoFrame getFrame(int FrameNumber,bool limitedBuffer);
	/**
	 * gives out a chunk according to given chunk number
	 * @param ChunkNumber the chunk number to find
	 * @return Chunk video chunk to retreive
	 */
	Chunk getChunk(int ChunkNumber,bool limitedBuffer);
	/**
	 * set a chunk that receive by this node in the buffer
	 * @param InputChunk the chunk that should be set in the buffer
	 */
	void setChunk(Chunk InputChunk,bool limitedBuffer);
    /**
     * standard output stream for VideoBuffer,
     * gives out all contained chunks and frames
     *
     * @param os the ostream
     * @param v the VideoBuffer
     * @return the output stream
     */
	friend std::ostream& operator<<(std::ostream& os, const VideoBuffer& v);
};

#endif /* VIDEOBUFFER_H_ */
