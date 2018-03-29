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
 * @file BasePlayer.cc
 * @author Behnam Ahmadifar, Yasser Seyyedi,Abdollah Ghaffari Sheshjavani(AGH)
 */

#include "MPEG4Player.h"
#include <GlobalStatistics.h>

Define_Module(MPEG4Player);

void MPEG4Player::play()
{
	std::map <int,int>::iterator frameIt = bs->frameInfo.begin();
	frameIt = bs->frameInfo.find(frameCounter);
	if(frameCounter/Fps > measuringTime)
	{
		totalSize += frameIt->second;
		camSize += frameIt->second;
	}
	if(!playerBuffer.begin()->status && frameCounter/Fps > measuringTime)
	{
		lossSize += frameIt->second;
		playerLossSize += frameIt->second;
		globalStatistics->addStdDev("BasePlayer: Frame Loss Ratio ", 100);
		if(playerBuffer.begin()->frame.getFrameLength() != -1 && playerBuffer.begin()->frame.getFrameType() != 'N')
		{
			stat_frameDependancyLoss += 1;
			dependencyLoss_Size += frameIt->second;
			instance_dependencyLoss_Size = frameIt->second;
		}
	}
	else
	{
		globalStatistics->addStdDev("BasePlayer: Frame Loss Ratio ", 0);
		updateTime(playerBuffer.begin()->frame.getFrameNumber());
	}
	if(frameCounter % (Fps) == 0 && frameCounter/Fps > measuringTime)
	{
		std::stringstream buf;
		//buf << "BasePlayer: Distortion in sec " << frameCounter/(Fps);
		std::string s = buf.str();
		if(camSize != 0)
		{
			globalStatistics->addStdDev(s.c_str(), (playerLossSize/camSize)*100);
			globalStatistics->addStdDev("BasePlayer: Total Distortion", (playerLossSize/camSize)*100);
			char temp[100];
			int up=round( LV->getUpBandwidth()/100000);
			sprintf(temp, "BasePlayer: Total Distortion per uploadBandwidth %d",up);
			globalStatistics->addStdDev(temp, (playerLossSize/camSize)*100);
			globalStatistics->recordOutVector("BasePlayer: Total Distortion_vec", (playerLossSize/camSize)*100);
			globalStatistics->addStdDev("BasePlayer: Dependency Loss Distortion", (instance_dependencyLoss_Size/camSize)*100);
			// categorize peers to 1-free riders 2-low bandwidth 3- medium 4-high
			up= LV->getUpBandwidth()/1024;
			if(up <=(videoAverageRate[LV->currentFilmId]))// it is low bandwidth
			{
				globalStatistics->addStdDev("BasePlayer: Total Distortion in low-free up band width peers",(playerLossSize/camSize)*100);
			}
			if(up <=100)// it is free rider
			{
				globalStatistics->addStdDev("BasePlayer: Total Distortion in free riders",(playerLossSize/camSize)*100);
			}
			else if(up <=(videoAverageRate[LV->currentFilmId]))// it is low bandwidth
			{
				globalStatistics->addStdDev("BasePlayer: Total Distortion in low up band width peers",(playerLossSize/camSize)*100);
			}
			else if(up >=(videoAverageRate[LV->currentFilmId])&&up <(2*videoAverageRate[LV->currentFilmId]) )// it is medium band width
			{
				globalStatistics->addStdDev("BasePlayer: Total Distortion in medium up band width peers",(playerLossSize/camSize)*100);
			}
			else if(up >=(2*videoAverageRate[LV->currentFilmId]))// it is high bandwidth
			{
				globalStatistics->addStdDev("BasePlayer: Total Distortion in high up band width peers",(playerLossSize/camSize)*100);
			}
		}
		instance_dependencyLoss_Size = 0;
		playerLossSize = 0;
		camSize = 0;
	}

	frameCounter++;
	playerBuffer.erase(playerBuffer.begin(),playerBuffer.begin()+1);

}
void MPEG4Player::checkFrames()
{
	int last = playerBuffer.size()-1;
	if(playerBuffer[last].frame.getFrameType() == 'I')
	{
		playerBuffer[last].status = true;
		if(playerBuffer.size() >=3)
			if(playerBuffer[last-3].status == true)
			{
				if(playerBuffer[last-1].frame.getFrameType() == 'B')
					playerBuffer[last-1].status = true;
				if(playerBuffer[last-2].frame.getFrameType() == 'B')
					playerBuffer[last-2].status = true;
			}
	}
	else if(playerBuffer[last].frame.getFrameType() == 'P')
	{
		if( playerBuffer.size() >= 3)
			if(playerBuffer[last-3].status == true)
			{
				playerBuffer[last].status = true;
				if(playerBuffer[last-1].frame.getFrameType() == 'B')
					playerBuffer[last-1].status = true;
				if(playerBuffer[last-2].frame.getFrameType() == 'B')
					playerBuffer[last-2].status = true;
			}
	}
}
