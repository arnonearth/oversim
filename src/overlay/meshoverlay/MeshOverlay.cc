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
 * @file MeshOverlay.cc
 * @author Abdollah Ghaffari Sheshjavani (AGH), Behnam Ahmadifar, Yasser Seyyedi
 */

#include "MeshOverlay.h"
#include <GlobalStatistics.h>
#include <math.h>
#include <GlobalNodeList.h>
#include <SimpleInfo.h>
#include "MeshMessage_m.h"

Define_Module(MeshOverlay);

void MeshOverlay::initializeOverlay(int stage)
{
    if(stage != MIN_STAGE_OVERLAY)
        return;
    firstBufferMapSending = 0.0;
    bufferMapExchangeStart = false;
    packetUnit = par("packetUnit");
    FEC = par("FEC");
    ARQ = par("ARQ");
    pMax = par("pMax");
    limitedBuffer=par("limitedBuffer");
	sourceBandwidth = par("sourceBandwidth");
	videoAverageRate[0] = par("videoAverageRate1");
	videoAverageRate[1] = par("videoAverageRate2");
	videoAverageRate[2] = par("videoAverageRate3");
	videoAverageRate[3] = par("videoAverageRate4");
	videoAverageRate[4] = par("videoAverageRate5");
	videoAverageRate[5] = par("videoAverageRate6");
	videoAverageRate[6] = par("videoAverageRate7");
	videoAverageRate[7] = par("videoAverageRate8");
	videoAverageRate[8] = par("videoAverageRate9");
	videoAverageRate[9] = par("videoAverageRate10");
	if(globalNodeList->getPeerInfo(thisNode.getAddress())->getTypeID() == 2)
    	isSource = true;
    else
    	isSource = false;
	if(isSource)
		getParentModule()->getParentModule()->setName("CDN-Server");

    LV = check_and_cast<LocalVariables*>(getParentModule()->getParentModule()->getSubmodule("tier1")->getSubmodule("localvariables"));
    // Free Rider-AGH
    	percentOfFreeRiders= par("percentOfFreeRiders");
    	int prob=bernoulli(percentOfFreeRiders,0);
    	if(prob==0 || isSource)
    		LV->isFreeRider=false;
    	else
    		LV->isFreeRider=true;
       // Free Rider End-AGH
    setBandwidth();
    stat_TotalReceived = 0;
    stat_TotalErrorReceived = 0;
    stat_TotalByte = 0;
    stat_FECRedundent = 0;
    //stat_UpBandwidthUtilization=0;
    stat_TotalSendByte=0;
    stat_TotalDataSendByte=0;
    stat_TotalControlSendByte=0;
    stat_PeerEntranceTime=simTime().dbl();
	stat_TotalEndtoEndDelay = 0;
	stat_EndtoEndCount = 0;
	

}
MeshOverlay::~MeshOverlay()
{
	chunkBuffer.empty();
}
void MeshOverlay::handleAppMessage(cMessage* msg)
{
	if (dynamic_cast<BufferMapMessage*>(msg) != NULL)
	{
		BufferMapMessage* bufferMapmsg = dynamic_cast<BufferMapMessage*>(msg);
		EncapBufferMap* denaCastOvelayMsg = new EncapBufferMap ("Encapsulated-BufferMap");
		bufferMapmsg->setSrcNode(thisNode);
		denaCastOvelayMsg->setByteLength(ENCAPBUFFERMAP_L(msg)/8);
		bufferMapmsg->setTotalBandwidth(getUpBandwidth()/1024);
		bufferMapmsg->setDownBandwith(downBandwidth/1024);
		denaCastOvelayMsg->encapsulate(bufferMapmsg);
		if(!bufferMapExchangeStart)
		{
			bufferMapExchangeStart = true;
			firstBufferMapSending = simTime().dbl();
		}
		if(bufferMapmsg->getUnicast()==true)
		{
			sendMessageToUDP(bufferMapmsg->getDstNode(), denaCastOvelayMsg->dup());
			stat_TotalSendByte+=denaCastOvelayMsg->getByteLength();
			stat_TotalControlSendByte+=denaCastOvelayMsg->getByteLength();
		}
		else
		{
			for (unsigned int i=0 ; i!=LV->neighbors.size(); i++)
			{
				sendMessageToUDP(LV->neighbors[i].address, denaCastOvelayMsg->dup());
				stat_TotalSendByte+=denaCastOvelayMsg->getByteLength();
				stat_TotalControlSendByte+=denaCastOvelayMsg->getByteLength();
			}
		}
		delete denaCastOvelayMsg;
	}
	else if (dynamic_cast<MeshMessage*>(msg) != NULL)
	{
			MeshMessage* disconnect=dynamic_cast<MeshMessage*>(msg);
			if(disconnect->getCommand()==DISCONNECT)
			{
				disconnect->setSrcNode(thisNode);
				sendMessageToUDP(disconnect->getOtherNode(),disconnect->dup());
				stat_TotalSendByte+=disconnect->getByteLength();
				stat_TotalControlSendByte+=disconnect->getByteLength();
			}
			delete disconnect;
	}
	else if(dynamic_cast<SelectFilm*>(msg) != NULL)
	{
		SelectFilm* SelectVideoMsg=dynamic_cast<SelectFilm*>(msg);
		trackerAddress  = *globalNodeList->getRandomAliveNode(1);
		//ev << "I'm sending encapsulated list of films" << endl;
		EncapSelectFilm* LOF = new EncapSelectFilm ("Encapsulated-list-of-Films");
		char buf[50];
		sprintf(buf,"Encap-%s",SelectVideoMsg->getName());
		LOF->setName(buf);
		SelectVideoMsg->setSrcNode(thisNode);
		NodeHandle Dst = trackerAddress;
		LOF->setByteLength(ENCAPVIDEOMESSAGE_L(msg)/8);
		LOF->encapsulate(SelectVideoMsg);
		if (SelectVideoMsg->getType()== 0)
		{
			sendMessageToUDP(Dst , LOF->dup());
			stat_TotalSendByte+=LOF->getByteLength();
			stat_TotalControlSendByte+=LOF->getByteLength();
		}

		delete LOF;

	}

	else if (dynamic_cast<VideoProperty*>(msg) != NULL)
	{
		VideoProperty* VideoPropertymsg=dynamic_cast<VideoProperty*>(msg);
		send(VideoPropertymsg,"appOut");
	}
	else if (dynamic_cast<CreateStream*>(msg) != NULL)
	{
		CreateStream* CreateStreammsg=dynamic_cast<CreateStream*>(msg);
		send(CreateStreammsg,"appOut");
	}
	else if (dynamic_cast<VideoMessage*>(msg) != NULL)
	{
		VideoMessage* videoBufferMsgApp=dynamic_cast<VideoMessage*>(msg);

		if(videoBufferMsgApp->getCommand() == CHUNK_RSP)
		{
			packetizeAndSendToUDP(videoBufferMsgApp);
		}
		else
		{
			EncapVideoMessage* denaCastOvelayMsg = new EncapVideoMessage ("Encapsulated-VideoMsg");
			char buf[50];
			sprintf(buf,"Encap-%s",videoBufferMsgApp->getName());
			//ev << "\nHi.\n" << endl;
			denaCastOvelayMsg->setName(buf);
			videoBufferMsgApp->setSrcNode(thisNode);
			NodeHandle Dst = videoBufferMsgApp->getDstNode();
			denaCastOvelayMsg->setByteLength(ENCAPVIDEOMESSAGE_L(msg)/8);
			denaCastOvelayMsg->encapsulate(videoBufferMsgApp);
			sendMessageToUDP(Dst , denaCastOvelayMsg);
			stat_TotalSendByte+=denaCastOvelayMsg->getByteLength();
			stat_TotalControlSendByte+=denaCastOvelayMsg->getByteLength();
			//sendMessageToUDP(Dst , denaCastOvelayMsg->dup());
			//delete denaCastOvelayMsg;
		}
	}

	else
		delete msg;
}
void MeshOverlay::handleUDPMessage(BaseOverlayMessage* msg)
{

	if (dynamic_cast<EncapBufferMap*>(msg) != NULL)
	{
		EncapBufferMap* UDPmsg=dynamic_cast<EncapBufferMap*>(msg);
		BufferMapMessage* bufferMapmsg = dynamic_cast<BufferMapMessage*>(UDPmsg->decapsulate()) ;
		send(bufferMapmsg,"appOut");
		delete UDPmsg;
	}
	else if (dynamic_cast<EncapVideoMessage*>(msg) != NULL)
	{
		EncapVideoMessage* denaCastOvelayMsg=dynamic_cast<EncapVideoMessage*>(msg);
		VideoMessage* videoMsgUDP=  check_and_cast<VideoMessage*> (msg->decapsulate());
        
		

		if(videoMsgUDP->getCommand() == CHUNK_RSP)
		{

			if (opp_strcmp(this->getFullName(),"peersmesh") == 0 || opp_strcmp(this->getName(),"peersmesh") == 0)
			{	
				ev << "I'm a client." << endl;
				ev << "I receive something." << endl;
				ev << "This set of packets firstly created at " << denaCastOvelayMsg->getCreationTime() << endl;
				ev << "This packet is " << denaCastOvelayMsg->getSeqNo() << " out of " << denaCastOvelayMsg->getSegmentSize() << endl;
				if (denaCastOvelayMsg->getSeqNo() == denaCastOvelayMsg->getSegmentSize())
				{
					ev << "This is the Last packet for this chunk." << endl;
					stat_TotalEndtoEndDelay = simTime().dbl() - denaCastOvelayMsg->getCreationTime();

					
					globalStatistics->addStdDev("MeshOverlay: end to end delay", stat_TotalEndtoEndDelay);

					ev << "End to end delay is " << stat_TotalEndtoEndDelay << " secs." << endl;
				}
			}
			
			if(FEC)
				bufferAndSendToApp(videoMsgUDP, denaCastOvelayMsg->hasBitError(),
						denaCastOvelayMsg->getLength(), denaCastOvelayMsg->getSeqNo(),
						denaCastOvelayMsg->getRedundant());
			else if(ARQ && msg->hasBitError())
				handleARQ(videoMsgUDP,
						denaCastOvelayMsg->getLength(), denaCastOvelayMsg->getSeqNo());
			else
				bufferAndSendToApp(videoMsgUDP, denaCastOvelayMsg->hasBitError(),
										denaCastOvelayMsg->getLength(), denaCastOvelayMsg->getSeqNo(),
										denaCastOvelayMsg->getRedundant());
		}
		else
		{
			send(videoMsgUDP,"appOut");
		}
		delete denaCastOvelayMsg;
	}
	else if(dynamic_cast<ErrorRecoveryMessage*>(msg) != NULL)
	{
		ErrorRecoveryMessage* ARQMsg = dynamic_cast<ErrorRecoveryMessage*>(msg);
		retransmitPAcket(ARQMsg);
	}
	else
		delete msg;
}
void MeshOverlay::packetizeAndSendToUDP(VideoMessage* videoMsgApp)
{
	EncapVideoMessage* denaCastOvelayMsg = new EncapVideoMessage ("Encap-FrameResponse");
	videoMsgApp->setSrcNode(thisNode);
	NodeHandle Dst = videoMsgApp->getDstNode();
	denaCastOvelayMsg->setLength(videoMsgApp->getByteLength());

	int byteleft = videoMsgApp->getByteLength();
	int segmentNumber;
	if(byteleft%packetUnit == 0)
		segmentNumber = byteleft/packetUnit ;
	else
		segmentNumber = byteleft/packetUnit +1;
	int redundantNo = 0;
	if(FEC)
		redundantNo = getNumFECRedundantPackets(segmentNumber);
	byteleft += redundantNo*packetUnit;
	stat_FECRedundent += redundantNo*packetUnit + redundantNo*20;
	int seqNo = 1;
	int ByteLength;
	VideoMessage* videoBufferMsgTmp;
	while (byteleft != 0)
	{
		if(byteleft>packetUnit)
		{
			ByteLength = packetUnit;
			byteleft -= packetUnit;
		}
		else
		{
			ByteLength = byteleft;
			byteleft = 0;
		}
		char buf[50];
		ev << "\n Hi Number 2 \n" << endl;
		if(seqNo == 1)
		{
			ev <<"This is the first packet" << endl;
			denaCastOvelayMsg->setCreationTime(simTime().dbl());
		}
		if(seqNo == segmentNumber)
		{
			ev <<"This is the last packet" << endl;
		}
		sprintf(buf,"Encap-Packet-#%d",seqNo);
		videoMsgApp->setByteLength(ByteLength);
		denaCastOvelayMsg->setSegmentSize(segmentNumber);
		denaCastOvelayMsg->setSeqNo(seqNo);
		denaCastOvelayMsg->setRedundant(redundantNo);
		denaCastOvelayMsg->setByteLength(ENCAPVIDEOMESSAGE_PACKET_L(msg)/8);
		denaCastOvelayMsg->setName(buf);
		denaCastOvelayMsg->encapsulate(videoMsgApp->dup());
		stat_TotalByte += denaCastOvelayMsg->getByteLength();
		sendMessageToUDP(Dst , denaCastOvelayMsg->dup());
		stat_TotalSendByte+=denaCastOvelayMsg->getByteLength();
		stat_TotalDataSendByte+=denaCastOvelayMsg->getByteLength();
		videoBufferMsgTmp=  check_and_cast<VideoMessage*> (denaCastOvelayMsg->decapsulate());
		delete videoBufferMsgTmp;
		seqNo++;
	}
	delete videoMsgApp;
	delete denaCastOvelayMsg;
}

void MeshOverlay::handleARQ(VideoMessage* videoMsgUDP, int length, int seq)
{
	ErrorRecoveryMessage* ARQMsg = new ErrorRecoveryMessage("retransmission");
	ARQMsg->setSeqNo(seq);
	ARQMsg->setChunkNo(videoMsgUDP->getChunk().getChunkNumber());
	ARQMsg->setLength(length);
	ARQMsg->setFilmId(videoMsgUDP->getChunk().getFilmNumber());
	ARQMsg->setPacketLength(videoMsgUDP->getByteLength());
	ARQMsg->setSrcNode(thisNode);
	ARQMsg->setByteLength(ERRORRECOVERYMESSAGE_L(msg)/8);
	sendMessageToUDP(videoMsgUDP->getSrcNode(),ARQMsg);
	stat_TotalSendByte+=ARQMsg->getByteLength();
	stat_TotalDataSendByte+=ARQMsg->getByteLength();

	//sendMessageToUDP(videoMsgUDP->getSrcNode(),ARQMsg->dup());
	//delete ARQMsg;
	delete videoMsgUDP;
}
void MeshOverlay::retransmitPAcket(ErrorRecoveryMessage* ARQMsg)
{
	Chunk cH = LV->videoBuffer[ARQMsg->getFilmId()]->getChunk(ARQMsg->getChunkNo(),limitedBuffer);
	VideoMessage* ChunkResponse = new VideoMessage("Chunk_Rsp");
	ChunkResponse->setCommand(CHUNK_RSP);
	ChunkResponse->setSrcNode(thisNode);
	ChunkResponse->setByteLength(ARQMsg->getLength());
	ChunkResponse->setSelectedFilms(ARQMsg->getFilmId());
	ChunkResponse->setChunk(cH);

	EncapVideoMessage* denaCastOvelayMsg = new EncapVideoMessage ("retransmit");
	denaCastOvelayMsg->setSeqNo(ARQMsg->getSeqNo());
	denaCastOvelayMsg->setLength(ARQMsg->getLength());
	denaCastOvelayMsg->setRedundant(0);
	denaCastOvelayMsg->setByteLength(ENCAPVIDEOMESSAGE_L(msg)/8);
	char buf[50];
	sprintf(buf,"Encap-Retransmit-#%d",ARQMsg->getSeqNo());
	denaCastOvelayMsg->setName(buf);
	denaCastOvelayMsg->encapsulate(ChunkResponse);
	sendMessageToUDP(ARQMsg->getSrcNode(),denaCastOvelayMsg);
	stat_TotalSendByte+=denaCastOvelayMsg->getByteLength();
	stat_TotalDataSendByte+=denaCastOvelayMsg->getByteLength();
	//sendMessageToUDP(ARQMsg->getSrcNode(),denaCastOvelayMsg->dup());
	//delete denaCastOvelayMsg;
	delete ARQMsg;
}
void MeshOverlay::bufferAndSendToApp(VideoMessage* videoMsgUDP, bool hasBitError, int length, int seqNo, int redundant)
{
	stat_TotalReceived++;
	if(hasBitError)
		stat_TotalErrorReceived++;
	unsigned int i=0;
	for(i=0 ;i<chunkBuffer.size();i++)
		if(chunkBuffer[i].getChunk().getChunkNumber() == videoMsgUDP->getChunk().getChunkNumber())
			break;
	if(i==chunkBuffer.size())
	{
		Chunk cH = videoMsgUDP->getChunk();

		ChunkPackets* cHp = new ChunkPackets(packetUnit,cH,length,redundant);
		chunkBuffer.push_back(*cHp);
		chunkBuffer[i].fragmentsState[seqNo-1] = true;
		if(hasBitError && FEC)
			chunkBuffer[i].errorState[seqNo-1] = true;
		if(chunkBuffer[i].FECSatisfaction())
		{
			videoMsgUDP->setByteLength(length);
			send(videoMsgUDP,"appOut");
		}
		else
			delete videoMsgUDP;
		if(chunkBuffer[i].isComplete())
			chunkBuffer.erase(chunkBuffer.begin()+i,chunkBuffer.begin()+1+i);

	}
	else
	{
		chunkBuffer[i].fragmentsState[seqNo-1] = true;
		if(hasBitError && FEC)
			chunkBuffer[i].errorState[seqNo-1] = true;
		if(chunkBuffer[i].FECSatisfaction())
		{
			videoMsgUDP->setByteLength(length);
			send(videoMsgUDP,"appOut");
		}
		else
			delete videoMsgUDP;
		if(chunkBuffer[i].isComplete())
			chunkBuffer.erase(chunkBuffer.begin()+i,chunkBuffer.begin()+1+i);
	}
}
int MeshOverlay::factorial(int x)
{
	int fact = 1;
	for(int i=2; i<=x; i++)
		fact *= i;
	return fact;
}
int MeshOverlay::getNumFECRedundantPackets(int fragmentNo)
{
	int redundentNo = 0;
	double PER = 0;    // Packet Error Rate
	double epsilon = 0;
	if(stat_TotalReceived != 0)
		PER = (double)stat_TotalErrorReceived/(double)stat_TotalReceived;
	if(PER == 0)
		PER = 0.07;
	do
	{
		epsilon = 0;
		redundentNo++;
		for(int k = redundentNo+1; k <= fragmentNo+redundentNo ; k++)
		{
			epsilon += (double)nCr(fragmentNo+redundentNo,k)*pow(PER,k)
			*pow(1-PER,fragmentNo+redundentNo-k)
			*(double)k /(double)(fragmentNo + redundentNo);
		}
	}
	while (epsilon > pMax);
	return redundentNo;
}
void MeshOverlay::finishOverlay()
{
	if(stat_TotalByte != 0)
		globalStatistics->addStdDev("DenaCastOverlay: FEC Overhead", 100*(double)stat_FECRedundent/(double)stat_TotalByte);
	setOverlayReady(false);
}

void MeshOverlay::setBandwidth()
{
	cModule* nodeModule = getParentModule()->getParentModule();
	if(!nodeModule->hasGate("pppg$i"))  //if SimpleUderlay
	{
		upBandwidth = dynamic_cast<SimpleInfo*>(globalNodeList->getPeerInfo(thisNode))->getEntry()->getUpBandwidth();
		downBandwidth = dynamic_cast<SimpleInfo*>(globalNodeList->getPeerInfo(thisNode))->getEntry()->getDownBandwidth();

	}
	else //if InetUnderlay
	{
		cGate* currentGateDownload = nodeModule->gate("pppg$i",0);
		cGate* currentGateUpload = nodeModule->gate("pppg$o",0);
		if(!isSource)
		{	if (currentGateDownload->isConnected() && currentGateUpload->isConnected())
            {
            	downBandwidth = check_and_cast<cDatarateChannel *>
                    (currentGateDownload->getPreviousGate()->getChannel())->getDatarate();
				upBandwidth = check_and_cast<cDatarateChannel *>
					(currentGateUpload->getChannel())->getDatarate();
            }
		}
		else
		{
			if (currentGateUpload-> getPreviousGate()->isConnected())
			{
				upBandwidth = sourceBandwidth*1024*1024;
				check_and_cast<cDatarateChannel *>
				(currentGateUpload->getChannel())->setDatarate(upBandwidth);
			}
			if (currentGateDownload->isConnected())
			{
				downBandwidth = sourceBandwidth*1024*1024;
				check_and_cast<cDatarateChannel *>
				(currentGateDownload->getPreviousGate()->getChannel())->setDatarate(downBandwidth);
			}
		}
	}
	if(LV->isFreeRider==true)
		upBandwidth=intuniform(51200,102000);

	LV->setUpBandwidth(upBandwidth);
	LV->setDownBandwidth(downBandwidth);

	std::cout << "upBandwidth:  " << upBandwidth<< "  downBandwidth:  " << downBandwidth << std::endl;

}


