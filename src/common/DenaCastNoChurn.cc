/*
 * DenaCastNoChurn.cc
 *
 *  Created on: Dec 31, 2010
 *      Author: root
 */

#include <TransportAddress.h>
#include <UnderlayConfigurator.h>
#include <algorithm>

#include "DenaCastNoChurn.h"

Define_Module(DenaCastNoChurn);

void DenaCastNoChurn::initializeChurn()
{
    Enter_Method_Silent();
    simtimelimit = atof(ev.getConfig()->getConfigValue("sim-time-limit"));
    initialMean = par("initPhaseCreationInterval");
    targetOverlayTerminalNum = par("targetOverlayTerminalNum");
    fillArrivalVector();
    initAddMoreTerminals = true;
    mobilityTimer = new cMessage("mobilityTimer");
    scheduleAt(simTime()+ (*ArrivalVector.begin() - simTime().dbl()), mobilityTimer);
    ArrivalVector.erase(ArrivalVector.begin(),ArrivalVector.begin()+1);
}

void DenaCastNoChurn::handleMessage(cMessage* msg)
{
    if (!msg->isSelfMessage()) {
        throw cRuntimeError("DenaCastNoChurn::handleMessage(): "
                                "Unknown message received!");
        delete msg;
        return;
    }

    if (msg == mobilityTimer)
    {
        if (terminalCount < targetOverlayTerminalNum)
        {
            TransportAddress* ta = underlayConfigurator->createNode(type);
            delete ta; // Address not needed in this churn model
        }

        if (terminalCount >= targetOverlayTerminalNum)
        {
            initAddMoreTerminals = false;
            underlayConfigurator->initFinished();
        }
        else
        {
            scheduleAt(simTime()+ (*ArrivalVector.begin() - simTime().dbl()), msg);
            ArrivalVector.erase(ArrivalVector.begin(),ArrivalVector.begin()+1);
        }
    }
}

void DenaCastNoChurn::updateDisplayString()
{
    char buf[80];
    sprintf(buf, "DenaCast No churn");
    getDisplayString().setTagArg("t", 0, buf);
}

DenaCastNoChurn::~DenaCastNoChurn() {
    // destroy self timer messages
    cancelAndDelete(mobilityTimer);
}
void DenaCastNoChurn::fillArrivalVector()
{
	int firstRound = targetOverlayTerminalNum* 0.7;
	int secondRound = targetOverlayTerminalNum - firstRound;
	for(int i = 0 ; i < firstRound ; i++)
		ArrivalVector.push_back(uniform(0,simtimelimit/2));
	for(int i = 0 ; i < secondRound ; i++)
		ArrivalVector.push_back(uniform(simtimelimit/2,simtimelimit-20));
	std::sort(ArrivalVector.begin(),ArrivalVector.end());
}
