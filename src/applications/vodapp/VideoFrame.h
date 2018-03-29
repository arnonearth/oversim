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
 * @file VideoFrame.h
 * @author Abdollah Ghaffari(AGH), Behnam Ahmadifar, Yasser Seyyedi
 */

#ifndef __VIDEOFRAME_H_
#define __VIDEOFRAME_H_

#include <iostream>

class VideoFrame
{
public:
	/**
	 * Checks whether the VideoFrame object's value are set.
	 *
	 * @return boolean true if set else false
	 */
	bool isSet();
	/**
	 * VideoFrame base class-constructor.
	 */
	VideoFrame();
	/**
	 * VideoFrame alternative constructor
	 */
	VideoFrame(int FrameNumber, int FrameLength, char FrameType);
	/**
	 * set the VideoFrame object's value.
	 *
	 * @param FrameNumber the video frame number
	 * @param FrameLength the video frame size (Byte)
	 * @param FrameType the video frame type (I,P or B)
	 */
	void setFrame(VideoFrame vFrame);
	/**
	 * Set the number of the following frame
	 *
	 * @param FrameNumber the input frame number to set
	 */
	void setFrameNumber(int FrameNumber);
	/**
	 * function to access frametype data member
	 *
	 * @return character type of the VideoFrame object.
	 */
	//int getFilmId(){return filmId;}
	char getFrameType(){return frameType;}
	/**
	 * function to access frameNumber data member
	 *
	 * @return integer frame number of the VideoFrame object.
	 */
	int getFrameNumber(){return frameNumber;}
	/**
	 * function to access frameLength data member
	 *
	 * @return integer size of VideoFrame object
	 */
	int getFrameLength(){return frameLength;}
	/**
	 * function to access creationTime data member
	 *
	 * @return double that represent the time that frame is created
	 */
	double getCreationTime(){return creationTime;}
	/**
	 * return an object equal to the current object
	 *
	 * @return VideoFrame the similar VideoFrame
	 */
	VideoFrame getVFrame();


protected:
	//int filmId;// vod have several film, so we must set its what film!(AGH)
	char frameType;/**<Type of video frame*/
	int frameNumber;/**<Frame number (it is unique identifier for each video frame)*/
	short int frameLength;/**<Size of video frame in byte*/
	bool initiate;
	float creationTime;/**<The SimTime that this frame is created*/
	/**
	 * set the frame properties
	 *
	 * @param FrameNumber the frame number
	 * @param FrameLength the frame size (in byte)
	 * @param FrameType type of the frame (could be I,P or B)
	 * @param CreationTime time that frame is created
	 */
	void setFrame(int FrameNumber, int FrameLength, char FrameType);
    /**
     * standard output stream for VideoFrame,
     * gives out all information in a video frame
     *
     * @param os the ostream
     * @param v the VideoFrame
     * @return the output stream
     */
	friend std::ostream& operator<<(std::ostream& os, const VideoFrame& v);
};
#endif
