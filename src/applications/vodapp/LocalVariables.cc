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
 * @file LocalVariables.cc
 * @author Yasser Seyyedi, Behnam Ahmadifar
 */

#include "LocalVariables.h"

Define_Module(LocalVariables);

void LocalVariables::initialize()
{
	/*windowOfIntrest = par ("windowOfIntrest");
	Fps = par("Fps");
	chunkSize = par("chunkSize");
	gopSize = par("gopSize");
	numOfBFrame = par("numOfBFrame");
    bufferSize = windowOfIntrest*Fps/chunkSize;
    if(bufferSize%chunkSize > 0)
    		bufferSize += chunkSize - (bufferSize%chunkSize);

    //initialize objects
    videoBuffer = new VideoBuffer(numOfBFrame,bufferSize,chunkSize,gopSize);
    hostBufferMap = new BufferMap();
    hostBufferMap->setValues(bufferSize);
    videoBuffer->updateBufferMap(hostBufferMap);
    */
}
void LocalVariables::updateLocalBufferMap(int filmId)
{
	videoBuffer[filmId]->updateBufferMap(hostBufferMap[filmId]);
}
void LocalVariables::setDownBandwidth(double DB)
{
	downBandwidth = DB;
}
void LocalVariables::setUpBandwidth(double UB)
{
	upBandwidth = UB;
}
void LocalVariables::addToAvailability_RateControlLoss(double Avail_RateControl)
{
	stat_availability_RateControlLoss += Avail_RateControl;
}
void LocalVariables::addToLateArivalLoss(double LateArivalLoss)
{
	stat_lateArrivalLossSize += LateArivalLoss;
}
