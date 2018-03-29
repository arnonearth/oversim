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
 * @file Scheduling.cc
 * @author Yasser Seyyedi, Behnam Ahmadifar, Abdollah Ghaffari Sheshjavani (AGH)
 */

#include "VodApp.h"
#include "BufferMap.h"
#include <GlobalStatistics.h>
#include "MeshMessage_m.h"

void VodApp::coolStreamingScheduling()
{
	//resetFreeBandwidth();
	notInNeighbors.clear();
	bool neiborUsed[neighborsBufferMaps.size()];
	for(unsigned int j=0;j< neighborsBufferMaps.size();j++)
		neiborUsed[j]=false;
	std::multimap <int,chunkPopulation> requestingWindow;
	int nextChunk = getNextChunk();
	bool serverExist=false;
	while(notInNeighbors.size() - requestingWindow.size() < 4 && requestingWindow.size() < 30)
	{
		if(nextChunk*chunkSize>=playbackPoint+maxDownloadFurtherPlayPoint*Fps)
		{
			//std::cout << "time: " << simTime()<< "ACHIEVE  maxDownloadFurtherPlayPoint "<<getParentModule()->getParentModule()->getFullName() <<  std::endl;
			break;
		}

		while(nextChunk > LV->videoBuffer[LV->currentFilmId]->chunkBuffer[bufferSize[LV->currentFilmId]-2].getChunkNumber())
			LV->videoBuffer[LV->currentFilmId]->shiftChunkBuf();
		LV->updateLocalBufferMap(LV->currentFilmId);

		if(rateControl)
		{
			double maxFrameRequest=	(double)((LV->getDownBandwidth()/(1024*videoAverageRate[LV->currentFilmId]))*Fps*bufferMapExchangePeriod);
			double bufferedTime = (double)(LV->videoBuffer[LV->currentFilmId]->lastSetChunk*chunkSize - playbackPoint);
			bufferedTime = bufferedTime/Fps;
			if( playingState==PLAYING)
			{
				if(LV->emergency==false && bufferedTime > startUpBuffering && numberOfFrameRequested>maxFrameRequestPerBufferMapExchangePeriod)
					break;
				else
				{
					if(maxFrameRequest>2*maxFrameRequestPerBufferMapExchangePeriod )
						maxFrameRequest=2*maxFrameRequestPerBufferMapExchangePeriod;
					if(bufferedTime <= startUpBuffering && numberOfFrameRequested>maxFrameRequest)
					break;
				}
			}
			else if( playingState==BUFFERING)
			{
				if(maxFrameRequest>3*maxFrameRequestPerBufferMapExchangePeriod )
					maxFrameRequest=3*maxFrameRequestPerBufferMapExchangePeriod;
				if(bufferedTime <= startUpBuffering && numberOfFrameRequested>maxFrameRequest)
					break;
			}
		}
		chunkPopulation cP;
		cP.chunkNum = nextChunk;

		for (unsigned int i=0 ; i < neighborsBufferMaps.size();i++)
		{

			if(neighborsBufferMaps[i].isServer==true)
			{
				cP.supplierIndex.push_back(i);
				neiborUsed[i]=true;
				serverExist=true;
			}
			if(!serverExist)
			{
				if (neighborsBufferMaps[i].buffermap.findChunk(nextChunk,limitedBuffer) && neighborsBufferMaps[i].freeBandwidth>0)
				{
					cP.supplierIndex.push_back(i);
					neiborUsed[i]=true;
					if(neighborsBufferMaps[i].totalBandwidth<100)// only for free-riders-AGH
						neighborsBufferMaps[i].freeBandwidth -= (averageChunkLength/5);
				}
				else if(neighborsBufferMaps[i].watchInPast==true && neiborUsed[i]==false)
				{
					if(neighborsBufferMaps[i].videoBufferlastSetChunk<nextChunk)
					{
						MeshMessage* disconnect=new MeshMessage("DISCONNECT");
						disconnect->setCommand(DISCONNECT);
						//disconnect->setSrcNode(thisNode);
						disconnect->setFilmId(LV->currentFilmId);
						disconnect->setDisconnecthopCount(1);
						disconnect->setOtherNode(neighborsBufferMaps[i].tAddress);
						send (disconnect,"to_lowerTier");
						deleteNeighbor(neighborsBufferMaps[i].tAddress);
					}
				}
			}
		}

		if(cP.supplierIndex.size() != 0)
		{
			requestingWindow.insert(std::make_pair<int,chunkPopulation>(cP.supplierIndex.size(),cP));
			numberOfFrameRequested+=chunkSize;
		}
			notInNeighbors.push_back(nextChunk);
		nextChunk = getNextChunk();
	}
	recivePoint=nextChunk-(1+ notInNeighbors.size() - requestingWindow.size());
	if(requestingWindow.size() < 15)
		return;
	std::multimap <int,chunkPopulation>::iterator supplierIt = requestingWindow.begin();
	while(supplierIt != requestingWindow.end())
	{

		if(supplierIt->first == 1)
		{

				VideoMessage *ChunkReq = new VideoMessage("Chunk_Req");
				ChunkReq->setCommand(CHUNK_REQ);
				Chunk CH;
				CH.setValues(chunkSize);
				CH.setFilmNumber(LV->currentFilmId);
				CH.setChunkNumber(supplierIt->second.chunkNum);
				ChunkReq->setChunk(CH);
				ChunkReq->setDstNode(neighborsBufferMaps[supplierIt->second.supplierIndex[0]].tAddress);
				ChunkReq->setBitLength(VIDEOMESSAGE_L(msg));
				requestedFrames rf;
				rf.destNode=neighborsBufferMaps[supplierIt->second.supplierIndex[0]].tAddress;
				rf.frameNum=supplierIt->second.chunkNum;
				rf.requestTime=simTime().dbl();
				sendFrames.push_back(rf);
				send(ChunkReq,"to_lowerTier");
				if(neighborsBufferMaps[supplierIt->second.supplierIndex[0]].totalBandwidth>=100)
					neighborsBufferMaps[supplierIt->second.supplierIndex[0]].freeBandwidth -= averageChunkLength;

		}
		else
		{
			int maxIndex = supplierIt->second.supplierIndex[0];
			for(int i = 1 ; i < supplierIt->first ; i++)
				if(neighborsBufferMaps[supplierIt->second.supplierIndex[i]].freeBandwidth > neighborsBufferMaps[maxIndex].freeBandwidth)
					maxIndex = supplierIt->second.supplierIndex[i];

				VideoMessage *ChunkReq = new VideoMessage("Chunk_Req");
				ChunkReq->setCommand(CHUNK_REQ);
				Chunk CH;
				CH.setValues(chunkSize);
				CH.setFilmNumber(LV->currentFilmId);
				CH.setChunkNumber(supplierIt->second.chunkNum);
				ChunkReq->setChunk(CH);
				ChunkReq->setDstNode(neighborsBufferMaps[maxIndex].tAddress);
				ChunkReq->setBitLength(VIDEOMESSAGE_L(msg));
				requestedFrames rf;
				rf.destNode=neighborsBufferMaps[maxIndex].tAddress;
				rf.frameNum=supplierIt->second.chunkNum;
				rf.requestTime=simTime().dbl();
				sendFrames.push_back(rf);
				send(ChunkReq,"to_lowerTier");
				if(neighborsBufferMaps[supplierIt->second.supplierIndex[0]].totalBandwidth>=100)
					neighborsBufferMaps[maxIndex].freeBandwidth -= averageChunkLength;


		}
		supplierIt++;
	}

}
void VodApp::recieverSideScheduling2()
{
	//resetFreeBandwidth();
	notInNeighbors.clear();
	int upHead = 0;
	int requestNum = 0;
	bool neiborUsed[ neighborsBufferMaps.size()];
		for(unsigned int j=0;j< neighborsBufferMaps.size();j++)
			neiborUsed[j]=false;
	if(chunkSize >= gopSize)
	{
		upHead = 12;
		requestNum = 5;
	}
	else
	{
		upHead = 40;
		requestNum = 12;
	}
	std::multimap <int,chunkPopulation> requestingWindow;
	int nextChunk = getNextChunk();
	bool serverExist=false;
	while(notInNeighbors.size() - requestingWindow.size() < 4 && requestingWindow.size() < 30)
	{
		if(nextChunk*chunkSize>=playbackPoint+maxDownloadFurtherPlayPoint*Fps)
		{
			//std::cout << "time: " << simTime()<< "ACHIEVE  maxDownloadFurtherPlayPoint "<<getParentModule()->getParentModule()->getFullName() <<  std::endl;
			break;
		}
		if(rateControl)
		{
			double maxFrameRequest=	(double)((LV->getDownBandwidth()/(1024*videoAverageRate[LV->currentFilmId]))*Fps*bufferMapExchangePeriod);
			double bufferedTime = (double)(LV->videoBuffer[LV->currentFilmId]->lastSetChunk*chunkSize - playbackPoint);
			bufferedTime = bufferedTime/Fps;
			if( playingState==PLAYING)
			{
				if(bufferedTime > startUpBuffering && numberOfFrameRequested>maxFrameRequestPerBufferMapExchangePeriod)
					break;
				else
				{
					if(maxFrameRequest>2*maxFrameRequestPerBufferMapExchangePeriod )
						maxFrameRequest=2*maxFrameRequestPerBufferMapExchangePeriod;
					if(bufferedTime <= startUpBuffering && numberOfFrameRequested>maxFrameRequest)
						break;
				}
			}
			else if( playingState==BUFFERING)
			{
				if(maxFrameRequest>3*maxFrameRequestPerBufferMapExchangePeriod )
					maxFrameRequest=3*maxFrameRequestPerBufferMapExchangePeriod;
				if(bufferedTime <= startUpBuffering && numberOfFrameRequested>maxFrameRequest)
					break;
			}
		}
		chunkPopulation cP;
		cP.chunkNum = nextChunk;

		for (unsigned int i=0 ; i < neighborsBufferMaps.size();i++)
		{

			if(neighborsBufferMaps[i].isServer==true)
			{
				cP.supplierIndex.push_back(i);
				neiborUsed[i]=true;
				serverExist=true;
			}

			if(!serverExist)
			{
				if (neighborsBufferMaps[i].buffermap.findChunk(nextChunk,limitedBuffer)&& neighborsBufferMaps[i].freeBandwidth>0)
				{
					cP.supplierIndex.push_back(i);
					neiborUsed[i]=true;
					if(neighborsBufferMaps[i].totalBandwidth<100)
						neighborsBufferMaps[i].freeBandwidth -= (averageChunkLength/5);
				}
				else if(neighborsBufferMaps[i].watchInPast==true && neiborUsed[i]==false)
				{
					if(neighborsBufferMaps[i].videoBufferlastSetChunk<nextChunk)
					{
						MeshMessage* disconnect=new MeshMessage("DISCONNECT");
						disconnect->setCommand(DISCONNECT);
						//disconnect->setSrcNode(thisNode);
						disconnect->setFilmId(LV->currentFilmId);
						disconnect->setDisconnecthopCount(1);
						disconnect->setOtherNode(neighborsBufferMaps[i].tAddress);
						send (disconnect,"to_lowerTier");
						deleteNeighbor(neighborsBufferMaps[i].tAddress);
					}
				}
			}
		}
		if(cP.supplierIndex.size() != 0)
		{
			requestingWindow.insert(std::make_pair<int,chunkPopulation>(nextChunk,cP));
			numberOfFrameRequested+=chunkSize;
		}
		notInNeighbors.push_back(nextChunk);
		nextChunk = getNextChunk();
	}
	recivePoint=nextChunk-(1+ notInNeighbors.size() - requestingWindow.size());
	if(requestingWindow.size() < upHead)
		return;
	std::multimap <int,chunkPopulation>::iterator supplierIt = requestingWindow.begin();
	int counter = 0;
	while(supplierIt != requestingWindow.end() && counter < requestNum)
	{
		counter++;

		int maxIndex = supplierIt->second.supplierIndex[0];
		for(unsigned int i = 1 ; i < supplierIt->second.supplierIndex.size() ; i++)
			if(neighborsBufferMaps[supplierIt->second.supplierIndex[i]].freeBandwidth > neighborsBufferMaps[maxIndex].freeBandwidth)
				maxIndex = supplierIt->second.supplierIndex[i];

			VideoMessage *ChunkReq = new VideoMessage("Chunk_Req");
			ChunkReq->setCommand(CHUNK_REQ);
			Chunk CH;
			CH.setValues(chunkSize);
			CH.setFilmNumber(LV->currentFilmId);
			CH.setChunkNumber(supplierIt->second.chunkNum);
			ChunkReq->setChunk(CH);
			ChunkReq->setDstNode(neighborsBufferMaps[maxIndex].tAddress);
			ChunkReq->setBitLength(VIDEOMESSAGE_L(msg));
			requestedFrames rf;
			rf.destNode=neighborsBufferMaps[maxIndex].tAddress;
			rf.frameNum=supplierIt->second.chunkNum;
			rf.requestTime=simTime().dbl();
			sendFrames.push_back(rf);
			send(ChunkReq,"to_lowerTier");
			if(neighborsBufferMaps[supplierIt->second.supplierIndex[0]].totalBandwidth>=100)
				neighborsBufferMaps[maxIndex].freeBandwidth -= averageChunkLength;
			supplierIt++;

	}

}
void VodApp::recieverSideScheduling3()
{
	notInNeighbors.clear();
	int nextChunk = -1;
	bool neiborUsed[neighborsBufferMaps.size()];
		for(unsigned int j=0;j< neighborsBufferMaps.size();j++)
			neiborUsed[j]=false;
	while (nextChunk == -1)
	{
		nextChunk = LV->hostBufferMap[LV->currentFilmId]->getNextUnsetChunk(sendFrames,notInNeighbors,playbackPoint/chunkSize,retryReq,deadLineFrame*Fps,retryframeReqTime,playingState,numberOfFrameRequested);
		if(nextChunk == -1)
		{
			LV->videoBuffer[LV->currentFilmId]->shiftChunkBuf();
			LV->updateLocalBufferMap(LV->currentFilmId);
		}
	}
	std::vector < int > frameSuppliers;
	bool serverExist=false;
	while(notInNeighbors.size() < chunkSize*5)
	{
		if(nextChunk*chunkSize>=playbackPoint+maxDownloadFurtherPlayPoint*Fps)
		{
			std::cout << "time: " << simTime()<< "ACHIEVE  maxDownloadFurtherPlayPoint "<<getParentModule()->getParentModule()->getFullName() <<  std::endl;
			break;
		}
		if(rateControl)
		{
			double maxFrameRequest=	(double)((LV->getDownBandwidth()/(1024*videoAverageRate[LV->currentFilmId]))*Fps*bufferMapExchangePeriod);
			double bufferedTime = (double)(LV->videoBuffer[LV->currentFilmId]->lastSetChunk*chunkSize - playbackPoint);
			bufferedTime = bufferedTime/Fps;
			if( playingState==PLAYING)
			{
				if(bufferedTime > startUpBuffering && numberOfFrameRequested>maxFrameRequestPerBufferMapExchangePeriod)
					break;
				else
				{
					if(maxFrameRequest>2*maxFrameRequestPerBufferMapExchangePeriod )
						maxFrameRequest=2*maxFrameRequestPerBufferMapExchangePeriod;
					if(bufferedTime <= startUpBuffering && numberOfFrameRequested>maxFrameRequest)
						break;
				}
			}
			else if( playingState==BUFFERING)
			{
				if(maxFrameRequest>3*maxFrameRequestPerBufferMapExchangePeriod )
					maxFrameRequest=3*maxFrameRequestPerBufferMapExchangePeriod;
				if(bufferedTime <= startUpBuffering && numberOfFrameRequested>maxFrameRequest)
					break;
			}
		}
		for (unsigned int i=0 ; i < neighborsBufferMaps.size();i++)
		{

			if(neighborsBufferMaps[i].isServer==true)
			{
				frameSuppliers.push_back(i);
				neiborUsed[i]=true;
				serverExist=true;
			}
			if(!serverExist)
			{
				if (neighborsBufferMaps[i].buffermap.findChunk(nextChunk,limitedBuffer)&& neighborsBufferMaps[i].freeBandwidth>0)
				{
					frameSuppliers.push_back(i);
					neiborUsed[i]=true;
					if(neighborsBufferMaps[i].totalBandwidth<100)
						neighborsBufferMaps[i].freeBandwidth -= (averageChunkLength/5);
				}
				else if(neighborsBufferMaps[i].watchInPast==true && neiborUsed[i]==false)
				{
					if(neighborsBufferMaps[i].videoBufferlastSetChunk<nextChunk)
					{
						MeshMessage* disconnect=new MeshMessage("DISCONNECT");
						disconnect->setCommand(DISCONNECT);
						//disconnect->setSrcNode(thisNode);
						disconnect->setFilmId(LV->currentFilmId);
						disconnect->setDisconnecthopCount(1);
						disconnect->setOtherNode(neighborsBufferMaps[i].tAddress);
						send (disconnect,"to_lowerTier");
						deleteNeighbor(neighborsBufferMaps[i].tAddress);
					}
				}
			}
		}
		if(frameSuppliers.size() != 0)
		{
			numberOfFrameRequested+=chunkSize;
			break;
		}
		else
		{
			notInNeighbors.push_back(nextChunk);
			nextChunk = LV->hostBufferMap[LV->currentFilmId]->getNextUnsetChunk(sendFrames,notInNeighbors,playbackPoint/chunkSize,retryReq,deadLineFrame*Fps,retryframeReqTime,playingState,numberOfFrameRequested);
		}
	}
	recivePoint=nextChunk-(1+notInNeighbors.size());
	if(notInNeighbors.size() >= chunkSize*5 )    //gopSize*2 is number of forward frames
		scheduleAt(simTime()+getRequestFramePeriod(),requestChunkTimer);
	else
	{
		globalStatistics->addStdDev("DenaCastVodApp: Number of suppliers", frameSuppliers.size());
		int neighborIndex = intuniform(0,frameSuppliers.size()-1);
			neighborsBufferMaps[frameSuppliers[neighborIndex]].requestCounter += 1;//framesList.size();

			VideoMessage *ChunkRequest = new VideoMessage("Chunk_Req");
			ChunkRequest->setCommand(CHUNK_REQ);
			Chunk cH;
			cH.setFilmNumber(LV->currentFilmId);
			cH.setChunkNumber(nextChunk);
			ChunkRequest->setChunk(cH);
			ChunkRequest->setDstNode(neighborsBufferMaps[frameSuppliers[neighborIndex]].tAddress);
			ChunkRequest->setBitLength(VIDEOMESSAGE_L(msg));
			ChunkRequest->setDeadLine((double)(nextChunk - playbackPoint)/(double)Fps + simTime().dbl());
			requestedChunks.push_back(nextChunk);
			requestedFrames rf;
			rf.destNode=neighborsBufferMaps[frameSuppliers[neighborIndex]].tAddress;
			rf.frameNum=nextChunk;
			rf.requestTime=simTime().dbl();
			sendFrames.push_back(rf);
			scheduleAt(simTime()+getRequestFramePeriod(),requestChunkTimer);
			send(ChunkRequest,"to_lowerTier");
	}
}
double VodApp::getRequestFramePeriod()
{
	if(playingState == BUFFERING)
		return 1/(double)Fps;
	else if(playingState == PLAYING)
		return 1/(double)Fps - 0.01;
	else
		return 1000;
}
/*int VodApp::requestRateState()
{
	if(!rateControl)
		return 0;
	if(playingState == BUFFERING)
		return 0;
	double bufferedTime = (double)(LV->videoBuffer[LV->currentFilmId]->lastSetFrame - playbackPoint);
	bufferedTime = bufferedTime/Fps[LV->currentFilmId];
	if(bufferedTime > startUpBuffering*2/5)
		return 0;
	if(bufferedTime <= startUpBuffering*2/5 &&  bufferedTime > startUpBuffering/4)
		return 1;
	else
		return 2;

}*/
void VodApp::senderSideScheduling1()
{
	//ev << "Here the sender side scheduling number 1 is called." << endl;
	if(LV->watchFilm[senderqueue.begin()->second.filmId]==true)
		handleChunkRequest(senderqueue.begin()->second.tAddress,senderqueue.begin()->second.filmId,senderqueue.begin()->second.chunkNo,false);
		//ev << "Now I will call the handleChunkRequest method." << endl;
	senderqueue.erase(senderqueue.begin());
}
void VodApp::senderSideScheduling2()
{
	if(sendFrameTimer->isScheduled())
		return;
	else
	{
		if(senderqueue.size() != 0)
		{
			if(LV->watchFilm[senderqueue.begin()->second.filmId]==true)
			{
				handleChunkRequest(senderqueue.begin()->second.tAddress,senderqueue.begin()->second.filmId,senderqueue.begin()->second.chunkNo,false);
				Chunk CH = LV->videoBuffer[LV->currentFilmId]->getChunk(senderqueue.begin()->second.chunkNo,limitedBuffer);
				scheduleAt(simTime() + 8*(CH.getChunkByteLength()+29)/LV->getUpBandwidth(),sendFrameTimer);
			}
			senderqueue.erase(senderqueue.begin());
		}
		else
			return;
	}
}
void VodApp::selectRecieverSideScheduling()
{ // Receive
	if(meshTestwithoutStreaming==true)
		return;
	switch(receiverSideSchedulingNumber)
	{
	case 1:
		coolStreamingScheduling();
		break;
	case 2:
		recieverSideScheduling2();
		break;
	case 3:
		recieverSideScheduling3();
		break;
	default:
		recieverSideScheduling2();
		break;
	}
}
void VodApp::selectSenderSideScheduling()
{
	//ev << "Now the sender side scheduling is called." << endl;
	if(LV->isFreeRider==true)
	{
		if(FreeRideUpload < LV->getUpBandwidth())
		{
			switch(senderSideSchedulingNumber)
				{
					case 1:
						senderSideScheduling1();
						break;
					case 2:
						senderSideScheduling2();
						break;
					default:
						senderSideScheduling1();
						break;

				}
		}
		else
			senderqueue.erase(senderqueue.begin());
		FreeRideUpload+=chunkSize*1024*videoAverageRate[LV->currentFilmId]/Fps;
	}
	else
		{
		switch(senderSideSchedulingNumber)
		{
			case 1:
			    //ev << "You set scheduling number = 1" << endl;
				senderSideScheduling1();
				break;
			case 2:
				senderSideScheduling2();
				break;
			default:
				senderSideScheduling1();
				break;

		}
	}
}
void VodApp::handleChunkRequest(TransportAddress& SrcNode,int FilmId,int chunkNo, bool push)
{
	//ev << "Now the handleChunkRequest is successfully called." << endl;
	if(!LV->peerExit)
	{
		//ev << "I will create a Chunk response message which its type is video message." << endl;
		VideoMessage *chunkRsp = new VideoMessage("Chunk_Rsp");
		chunkRsp->setDstNode(SrcNode);
		chunkRsp->setSelectedFilms(LV->currentFilmId);//we want to see that this node is currently watch this video or it watch video in past-AGH
		countRequest(SrcNode);
		if(LV->hostBufferMap[FilmId]->findChunk(chunkNo,limitedBuffer))
		{
			Chunk CH = LV->videoBuffer[FilmId]->getChunk(chunkNo,limitedBuffer);
			if(isVideoServer)
				CH.setHopCout(0);
			CH.setFilmNumber(FilmId);
			//ev << "I want to cound how many this line is called during the data transfer." << endl;
			chunkRsp->setCommand(CHUNK_RSP);
			chunkRsp->setChunk(CH);
			chunkRsp->setSrcNode(thisNode);
			//ev << "Get Last Frame " << chunkRsp->getChunk().getLastFrameNo() << endl;
			chunkRsp->setByteLength(CH.getChunkByteLength()+ VIDEOMESSAGE_L(msg)/8);
			send(chunkRsp,"to_lowerTier");
		}
		else
			delete chunkRsp;
	}
}
void VodApp::resetFreeBandwidth()
{
	FreeRideUpload=0;
	for(unsigned int i = 0; i< neighborsBufferMaps.size(); i++)
	{
			neighborsBufferMaps[i].freeBandwidth = neighborsBufferMaps[i].totalBandwidth;
	}
}
int VodApp::getNextChunk()
{
	//int	deadLineFrame = playbackPoint+2*gopSize[LV->currentFilmId]; we pause in vod - AGH
	int nextChunk = -1;
	while (nextChunk == -1)
	{
		nextChunk = LV->hostBufferMap[LV->currentFilmId]->getNextUnsetChunk(sendFrames,notInNeighbors,(playbackPoint+1)/chunkSize,retryReq,deadLineFrame*Fps,retryframeReqTime,playingState,numberOfFrameRequested);
		if(nextChunk == -1)
		{
			LV->videoBuffer[LV->currentFilmId]->shiftChunkBuf();
			LV->updateLocalBufferMap(LV->currentFilmId);
		}
	}
	return nextChunk;
}
