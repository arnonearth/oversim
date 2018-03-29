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
 * @file VideoFrame.cc
 * @author Abdollah Ghaffari(AGH),Behnam Ahmadifar, Yasser Seyyedi
 */

#include "VideoFrame.h"

VideoFrame::VideoFrame()
{
	initiate=true;
	setFrame(-1,-1, 'N');
}
VideoFrame::VideoFrame(int FrameNumber, int FrameLength, char FrameType)
{
	initiate=true;
	setFrame(FrameNumber, FrameLength,FrameType);
}
bool VideoFrame::isSet()
{
	if(initiate==true)
	{
	if(frameType == 'N' || frameLength == -1)
		return false;
	else
		return true;
	}
	else
		return false;
}
void VideoFrame::setFrame(VideoFrame vFrame)
{
	initiate=true;
	frameNumber = vFrame.getFrameNumber();
	frameLength = vFrame.getFrameLength();
	frameType = vFrame.getFrameType();

}

VideoFrame VideoFrame::getVFrame()
{
	VideoFrame vFrame;
	vFrame.setFrame(frameNumber,frameLength,frameType);
	return vFrame;
}
void VideoFrame::setFrame(int FrameNumber, int FrameLength, char FrameType)
{
	initiate=true;
	frameNumber = FrameNumber;
	frameLength = FrameLength;
	frameType = FrameType;

}
void VideoFrame::setFrameNumber(int FrameNumber)
{
	frameNumber = FrameNumber;
}
std::ostream& operator<<(std::ostream& os, const VideoFrame& v)
{
	os << v.frameNumber<<","
	<< v.frameType << ","
	<<v.frameLength;
	return os;
}

