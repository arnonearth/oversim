//
//Copyright (C) 2012 Tarbiat Modares University All Rights Reserved.
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
 * @file BaseVodStream.cc
 * @author Abdollah Ghaffari Sheshjavani(AGH), Yasser Seyyedi, Behnam Ahmadifar
 */

#include "BaseVodStream.h"
#include <GlobalStatistics.h>
#include <LocalVariables.h>

Define_Module(BaseVodStream);


void BaseVodStream::initializeApp(int stage)
{
    if (stage != MIN_STAGE_APP) return;
    nof=par("nof");
    LV = check_and_cast<LocalVariables*>(getParentModule()->getParentModule()->getSubmodule("tier1")->getSubmodule("localvariables"));
    std::string traceFilePath[nof];
    for (int i=0;i<nof;i+=10)
    	    {
			traceFilePath[i] = par ("traceFile1").stringValue();
    		if (nof>i+1)
			traceFilePath[i+1] = par ("traceFile2").stringValue();
    		if (nof>i+2)
    		traceFilePath[i+2] = par ("traceFile3").stringValue();
    		if (nof>i+3)
    		traceFilePath[i+3] = par ("traceFile4").stringValue();
    		if (nof>i+4)
    		traceFilePath[i+4] = par ("traceFile5").stringValue();
    		if (nof>i+5)
    		traceFilePath[i+5] = par ("traceFile6").stringValue();
    		if (nof>i+6)
    		traceFilePath[i+6] = par ("traceFile7").stringValue();
    		if (nof>i+7)
    		traceFilePath[i+7] = par ("traceFile8").stringValue();
    		if (nof>i+8)
    		traceFilePath[i+8] = par ("traceFile9").stringValue();
    		if (nof>i+9)
    		traceFilePath[i+9] = par ("traceFile10").stringValue();

    	    }

 VideoFilm vFilm;

     for (int i=0;i<nof;i++)
	    {
	    	vFilm.filmPath=traceFilePath[i].c_str();
			vFilm.Fps = par("Fps");
	    	vFilm.gopSize = par("gopSize");
	    	vFilm.chunkSize = par ("chunkSize");
	    	vFilm.numOfBFrame = par("numOfBFrame");
	    	vFilm.videolenght=0;
	    	traceFile.push_back(vFilm);
	     }
    // delete traceFilePath;
	TraceFileInit = new cMessage("TraceFileInit");
	scheduleAt(simTime(),TraceFileInit);
	lastFrameStreamed = 0;
	totalBit = 0;
	tempPointer=0;
	//statistics
	stat_totalBytesend = 0;
	stat_totalFramesent = 0;
	stat_totalIframesent = 0;
	stat_totalPframesent = 0;
	stat_totalBframesent =0;
	stat_startStreamingTime = 0.0;

}
BaseVodStream::~BaseVodStream()
{
	baseFile.close();
}


void BaseVodStream::handleTimerEvent(cMessage* msg)
{

	if (msg == TraceFileInit)
	{


		////////////////////////// read all film start/////////////////////////////
		for(int t=0;t<nof;t++)
		{
			baseFile.open(traceFile[t].filmPath.c_str(), std::ios::in);

			if (baseFile.bad() || !baseFile.is_open())
			{
				opp_error("Error while opening Trace file\n");
			}
			traceFile[t].videolenght=readFrames(&baseFile,t);
			baseFile.close();
		}


///////////////////////// read all film complete /////////////////////////////////////////

	// send list of films to tracker
		SelectFilm* listOfFilms = new SelectFilm("list of films");
		listOfFilms->setType(0);
		listOfFilms->setNumberOfFilms(nof);
		for (int var = 0; var < nof; var++)
		{
			listOfFilms->setFrameOfFilms(var,traceFile[var].videolenght);
			listOfFilms->setLenthOfFilmsSec(var,traceFile[var].videolenght/traceFile[var].Fps);
			listOfFilms->setFPS(var,traceFile[var].Fps);
			// if you want have different FPS you must create different variables of FPS and set it here to each film FPS. AGH
		}

		send(listOfFilms,"to_lowerTier");
		delete msg;
	// end send list of films to tracker


	}


}


int BaseVodStream::readFrames(std::ifstream *file, int filmno)
{
}

void BaseVodStream::updateTime(int filmNumber,int long frameNo)
{
	int hour=0,min=0,sec=0;
	if (frameNo > traceFile[filmNumber].Fps-1)
	{
		sec=frameNo/traceFile[filmNumber].Fps;
		frameNo= frameNo%traceFile[filmNumber].Fps;
	}
	if(sec >59)
	{
		min=sec/60;
		sec=sec%60;
	}
	if(min >59)
	{
		hour=min/60;
		min=min%60;
	}
	std::stringstream buf;
	buf << "elapsed Time:" << hour << ":" << min << ":" << sec << ":" << frameNo ;
	std::string s = buf.str();
	getParentModule()->getDisplayString().setTagArg("t", 0, s.c_str());
	getDisplayString().setTagArg("t", 0, s.c_str());

}
void BaseVodStream::finishApp()
{

	// statistics
    globalStatistics->addStdDev("BaseVodStream: Total send bytes", stat_totalBytesend);
    globalStatistics->addStdDev("BaseVodStream: Total send frames", stat_totalFramesent);
    globalStatistics->addStdDev("BaseVodStream: Start Streaming Time",stat_startStreamingTime);
    globalStatistics->addStdDev("BaseVodStream: Total I frames sent", stat_totalIframesent);
    globalStatistics->addStdDev("BaseVodStream: Total P frames sent", stat_totalPframesent);
    globalStatistics->addStdDev("BaseVodStream: Total B frames sent", stat_totalBframesent);
}

