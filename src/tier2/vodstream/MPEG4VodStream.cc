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
 * @file MPEG4VodStream.cc
 * @author Abdollah Ghaffari Sheshjavani, Behnam Ahmadifar, Yasser Seyyedi
 */


#include "MPEG4VodStream.h"
#include <GlobalStatistics.h>

Define_Module(MPEG4VodStream);



int MPEG4VodStream::readFrames(std::ifstream * file,int filmno)
{

		char Parameter[4][20];
		int frame_num = 0;
		int counter=0;

  while (!baseFile.eof())
  {
	(* file) >> Parameter[0] >> Parameter[1] >> Parameter[2] >> Parameter[3] ;

	updateTime(filmno,atol(Parameter[0]));
	frame_num = atoi(Parameter[0]);
	char frame_type=Parameter[1][0];
	int frame_length  = atoi(Parameter[3]); // the traces contains size in bytes
	//change  frame numbers in time order
	if(frame_num==1)
	{
		frame_num-=1;

	}
	else if(frame_type=='I' && frame_num !=0)
	{
		frame_num+=1;

	}
	else if( frame_type == 'P')
	{
		frame_num+=1;

	}
	else if(frame_type=='B')
	{
		frame_num-=2;

	}

	VideoFrame vf(frame_num,frame_length, frame_type);
	tempVideoBuffer.push_back(vf);
	counter++;
  	if(frame_type=='I')
	{
		stat_totalIframesent +=1;
	}
	else if(frame_type=='P')
	{
		stat_totalPframesent += 1;

	}
	else if(frame_type=='B')
	{
		stat_totalBframesent += 1;
	}
	stat_totalBytesend += frame_length;
	stat_totalFramesent += 1;

  }

	  traceFile[filmno].videolenght=counter;
	  traceFile[filmno].bufferSize = counter/traceFile[filmno].chunkSize;
 	  if(traceFile[filmno].bufferSize%traceFile[filmno].chunkSize > 0)
 		 traceFile[filmno].bufferSize += traceFile[filmno].chunkSize - (traceFile[filmno].bufferSize%traceFile[filmno].chunkSize)+traceFile[filmno].chunkSize;

 	  LV->videoBuffer[filmno] = new VideoBuffer(traceFile[filmno].numOfBFrame,traceFile[filmno].bufferSize,traceFile[filmno].chunkSize,traceFile[filmno].gopSize);
 	  LV->hostBufferMap[filmno] = new BufferMap();
 	  LV->hostBufferMap[filmno]->setValues(traceFile[filmno].bufferSize);

 	  for(int i=0;i<counter;i++)
 	  {
 		  LV->videoBuffer[filmno]->setFrame(true,tempVideoBuffer[tempPointer],false);
 		  tempPointer++;
 	  }
 	 //LV->videoBuffer[filmno]->updateBufferMap(LV->hostBufferMap[filmno]);
 	  LV->updateLocalBufferMap(filmno);

 	  return counter;
}


