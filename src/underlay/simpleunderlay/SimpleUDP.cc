//
// Copyright (C) 2000 Institut fuer Telematik, Universitaet Karlsruhe
// Copyright (C) 2004,2005 Andras Varga
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
//

/**
 * @file SimpleUDP.cc
 * @author Jochen Reber
 */

//
// Author: Jochen Reber
// Rewrite: Andras Varga 2004,2005
// Modifications: Stephan Krause
//

#include <omnetpp.h>

#include <CommonMessages_m.h>
#include <GlobalNodeListAccess.h>
#include <GlobalStatisticsAccess.h>
#include <MeshOverlayMessage_m.h>

#include <SimpleInfo.h>
#include "SimpleUDPPacket.h"
#include "SimpleUDP.h"
#include "IPControlInfo.h"
#include "IPv6ControlInfo.h"
#include "ICMPAccess.h"
#include "ICMPv6Access.h"
#include "IPAddressResolver.h"

// the following is only for ICMP error processing
#include "ICMPMessage_m.h"
#include "ICMPv6Message_m.h"
#include "IPDatagram_m.h"
#include "IPv6Datagram_m.h"


#define EPHEMERAL_PORTRANGE_START 1024
#define EPHEMERAL_PORTRANGE_END   5000


Define_Module( SimpleUDP );

std::string SimpleUDP::delayFaultTypeString;
std::map<std::string, SimpleUDP::delayFaultTypeNum> SimpleUDP::delayFaultTypeMap;

static std::ostream & operator<<(std::ostream & os,
                                 const SimpleUDP::SockDesc& sd)
{
    os << "sockId=" << sd.sockId;
    os << " appGateIndex=" << sd.appGateIndex;
    os << " userId=" << sd.userId;
    os << " localPort=" << sd.localPort;
    if (sd.remotePort!=0)
        os << " remotePort=" << sd.remotePort;
    if (!sd.localAddr.isUnspecified())
        os << " localAddr=" << sd.localAddr;
    if (!sd.remoteAddr.isUnspecified())
        os << " remoteAddr=" << sd.remoteAddr;
    if (sd.interfaceId!=-1)
        os << " interfaceId=" << sd.interfaceId;

    return os;
}

static std::ostream & operator<<(std::ostream & os,
                                 const SimpleUDP::SockDescList& list)
{
    for (SimpleUDP::SockDescList::const_iterator i=list.begin();
	 i!=list.end(); ++i)
        os << "sockId=" << (*i)->sockId << " ";
    return os;
}

//--------

SimpleUDP::SimpleUDP()
{
    globalStatistics = NULL;
}

SimpleUDP::~SimpleUDP()
{
    for (SocketsByIdMap::iterator i=socketsByIdMap.begin();
    i!=socketsByIdMap.end(); ++i)
        delete i->second;
}

void SimpleUDP::initialize(int stage)
{
    if(stage == MIN_STAGE_UNDERLAY) {
        WATCH_PTRMAP(socketsByIdMap);
        WATCH_MAP(socketsByPortMap);

        lastEphemeralPort = EPHEMERAL_PORTRANGE_START;
        icmp = NULL;
        icmpv6 = NULL;

        numSent = 0;
        numPassedUp = 0;
        numDroppedWrongPort = 0;
        numDroppedBadChecksum = 0;
        numQueueLost = 0;
        numPartitionLost = 0;
        numDestUnavailableLost = 0;
        WATCH(numSent);
        WATCH(numPassedUp);
        WATCH(numDroppedWrongPort);
        WATCH(numDroppedBadChecksum);
        WATCH(numQueueLost);
        WATCH(numPartitionLost);
        WATCH(numDestUnavailableLost);

        globalNodeList = GlobalNodeListAccess().get();
        globalStatistics = GlobalStatisticsAccess().get();
        constantDelay = par("constantDelay");
        useCoordinateBasedDelay = par("useCoordinateBasedDelay");

        delayFaultTypeString = par("delayFaultType").stdstringValue();
        delayFaultTypeMap["live_all"] = delayFaultLiveAll;
        delayFaultTypeMap["live_planetlab"] = delayFaultLivePlanetlab;
        delayFaultTypeMap["simulation"] = delayFaultSimulation;

        switch (delayFaultTypeMap[delayFaultTypeString]) {
        case SimpleUDP::delayFaultLiveAll:
        case SimpleUDP::delayFaultLivePlanetlab:
        case SimpleUDP::delayFaultSimulation:
            faultyDelay = true;
            break;
        default:
            faultyDelay = false;
        }

        jitter = par("jitter");
        //// Add by DenaCast
        dropErrorPackets = par("dropErrorPackets");
        nodeEntry = NULL;
        WATCH_PTR(nodeEntry);
    }
}

void SimpleUDP::finish()
{
    globalStatistics->addStdDev("SimpleUDP: Packets sent",
                                numSent);
    globalStatistics->addStdDev("SimpleUDP: Packets dropped with bad checksum",
                                numDroppedBadChecksum);
    globalStatistics->addStdDev("SimpleUDP: Packets dropped due to queue overflows",
                                numQueueLost);
    globalStatistics->addStdDev("SimpleUDP: Packets dropped due to network partitions",
                                numPartitionLost);
    globalStatistics->addStdDev("SimpleUDP: Packets dropped due to unavailable destination",
                                numDestUnavailableLost);
}

void SimpleUDP::bind(int gateIndex, UDPControlInfo *ctrl)
{
    // XXX checks could be added, of when the bind should be allowed to proceed

    if (ctrl->getSrcPort() <= 0 || ctrl->getSrcPort() > 65535)
        opp_error("bind: invalid src port number %d", ctrl->getSrcPort());

    // create and fill in SockDesc
    SockDesc *sd = new SockDesc();
    sd->sockId = ctrl->getSockId();
    sd->userId = ctrl->getUserId();
    sd->appGateIndex = gateIndex;
    sd->localAddr = ctrl->getSrcAddr();
    sd->remoteAddr = ctrl->getDestAddr();
    sd->localPort = ctrl->getSrcPort();
    sd->remotePort = ctrl->getDestPort();
    sd->interfaceId = ctrl->getInterfaceId();

    if (sd->sockId==-1)
        error("sockId in BIND message not filled in");
    if (sd->localPort==0)
        sd->localPort = getEphemeralPort();

    sd->onlyLocalPortIsSet = sd->localAddr.isUnspecified() &&
                             sd->remoteAddr.isUnspecified() &&
                             sd->remotePort==0 &&
                             sd->interfaceId==-1;

    cModule *node = getParentModule();
    IPvXAddress ip = IPAddressResolver().addressOf(node);
    EV << "[SimpleUDP::bind() @ " << ip << "]\n"
       << "    Binding socket: " << *sd
       << endl;

    // add to socketsByIdMap
    ASSERT(socketsByIdMap.find(sd->sockId)==socketsByIdMap.end());
    socketsByIdMap[sd->sockId] = sd;

    // add to socketsByPortMap
    // create if doesn't exist
    SockDescList& list = socketsByPortMap[sd->localPort];
    list.push_back(sd);
}

void SimpleUDP::connect(int sockId, IPvXAddress addr, int port)
{
    SocketsByIdMap::iterator it = socketsByIdMap.find(sockId);
    if (it==socketsByIdMap.end())
        error("socket id=%d doesn't exist (already closed?)", sockId);
    if (addr.isUnspecified())
        opp_error("connect: unspecified remote address");
    if (port<=0 || port>65535)
        opp_error("connect: invalid remote port number %d", port);

    SockDesc *sd = it->second;
    sd->remoteAddr = addr;
    sd->remotePort = port;

    sd->onlyLocalPortIsSet = false;

    EV << "[SimpleUDP::connect() @ " << sd->localAddr << "]\n"
       << "    Connecting socket: " << *sd
       << endl;
}

void SimpleUDP::unbind(int sockId)
{
    // remove from socketsByIdMap
    SocketsByIdMap::iterator it = socketsByIdMap.find(sockId);
    if (it==socketsByIdMap.end())
        error("socket id=%d doesn't exist (already closed?)", sockId);
    SockDesc *sd = it->second;
    socketsByIdMap.erase(it);

    EV << "[SimpleUDP::unbind() @ " << sd->localAddr << "]\n"
       << "    Unbinding socket: " << *sd
       << endl;

    // remove from socketsByPortMap
    SockDescList& list = socketsByPortMap[sd->localPort];
    for (SockDescList::iterator it=list.begin(); it!=list.end(); ++it)
        if (*it == sd) {
            list.erase(it);
            break;
        }
    if (list.empty())
        socketsByPortMap.erase(sd->localPort);
    delete sd;
}

short SimpleUDP::getEphemeralPort()
{
    // start at the last allocated port number + 1, and search for an unused one
    short searchUntil = lastEphemeralPort++;
    if (lastEphemeralPort == EPHEMERAL_PORTRANGE_END) // wrap
        lastEphemeralPort = EPHEMERAL_PORTRANGE_START;

    while (socketsByPortMap.find(lastEphemeralPort)!=socketsByPortMap.end()) {
        if (lastEphemeralPort == searchUntil) // got back to starting point?
            error("Ephemeral port range %d..%d exhausted, all ports occupied", EPHEMERAL_PORTRANGE_START, EPHEMERAL_PORTRANGE_END);
        lastEphemeralPort++;
        if (lastEphemeralPort == EPHEMERAL_PORTRANGE_END) // wrap
            lastEphemeralPort = EPHEMERAL_PORTRANGE_START;
    }

    // found a free one, return it
    return lastEphemeralPort;
}

void SimpleUDP::handleMessage(cMessage *msg)
{
    // received from the network layer
    if (msg->arrivedOn("network_in")) {
        if (dynamic_cast<ICMPMessage *>(msg) || dynamic_cast<ICMPv6Message *>(msg))
            processICMPError(msg);
        else
            processUDPPacket(check_and_cast<SimpleUDPPacket *>(msg));
    } else // received from application layer
    {
        if (msg->getKind()==UDP_C_DATA)
            processMsgFromApp(check_and_cast<cPacket*>(msg));
        else
            processCommandFromApp(msg);
    }

    if (ev.isGUI())
        updateDisplayString();
}

void SimpleUDP::updateDisplayString()
{
    char buf[80];
    sprintf(buf, "passed up: %d pks\nsent: %d pks", numPassedUp, numSent);
    if (numDroppedWrongPort>0) {
        sprintf(buf+strlen(buf), "\ndropped (no app): %d pks", numDroppedWrongPort);
        getDisplayString().setTagArg("i",1,"red");
    }
    if (numQueueLost>0) {
        sprintf(buf+strlen(buf), "\nlost (queue overflow): %d pks", numQueueLost);
        getDisplayString().setTagArg("i",1,"red");
    }
    getDisplayString().setTagArg("t",0,buf);
}

bool SimpleUDP::matchesSocket(SockDesc *sd, SimpleUDPPacket *udp, IPControlInfo *ipCtrl)
{
    // IPv4 version
    if (sd->remotePort!=0 && sd->remotePort!=udp->getSourcePort())
        return false;
    if (!sd->localAddr.isUnspecified() && sd->localAddr.get4()!=ipCtrl->getDestAddr())
        return false;
    if (!sd->remoteAddr.isUnspecified() && sd->remoteAddr.get4()!=ipCtrl->getSrcAddr())
        return false;
    if (sd->interfaceId!=-1 && sd->interfaceId!=ipCtrl->getInterfaceId())
        return false;
    return true;
}

bool SimpleUDP::matchesSocket(SockDesc *sd, SimpleUDPPacket *udp, IPv6ControlInfo *ipCtrl)
{
    // IPv6 version
    if (sd->remotePort!=0 && sd->remotePort!=udp->getSourcePort())
        return false;
    if (!sd->localAddr.isUnspecified() && sd->localAddr.get6()!=ipCtrl->getDestAddr())
        return false;
    if (!sd->remoteAddr.isUnspecified() && sd->remoteAddr.get6()!=ipCtrl->getSrcAddr())
        return false;
    if (sd->interfaceId!=-1 && sd->interfaceId!=ipCtrl->getInterfaceId())
        return false;
    return true;
}

bool SimpleUDP::matchesSocket(SockDesc *sd, const IPvXAddress& localAddr, const IPvXAddress& remoteAddr, short remotePort)
{
    return (sd->remotePort==0 || sd->remotePort!=remotePort) &&
           (sd->localAddr.isUnspecified() || sd->localAddr==localAddr) &&
           (sd->remoteAddr.isUnspecified() || sd->remoteAddr==remoteAddr);
}

void SimpleUDP::sendUp(cMessage *payload, SimpleUDPPacket *udpHeader, IPControlInfo *ipCtrl, SockDesc *sd)
{
    // send payload with UDPControlInfo up to the application -- IPv4 version
    UDPControlInfo *udpCtrl = new UDPControlInfo();
    udpCtrl->setSockId(sd->sockId);
    udpCtrl->setUserId(sd->userId);
    udpCtrl->setSrcAddr(ipCtrl->getSrcAddr());
    udpCtrl->setDestAddr(ipCtrl->getDestAddr());
    udpCtrl->setSrcPort(udpHeader->getSourcePort());
    udpCtrl->setDestPort(udpHeader->getDestinationPort());
    udpCtrl->setInterfaceId(ipCtrl->getInterfaceId());
    payload->setControlInfo(udpCtrl);

    send(payload, "appOut", sd->appGateIndex);
    numPassedUp++;
}

void SimpleUDP::sendUp(cMessage *payload, SimpleUDPPacket *udpHeader, IPv6ControlInfo *ipCtrl, SockDesc *sd)
{
    // send payload with UDPControlInfo up to the application -- IPv6 version
    UDPControlInfo *udpCtrl = new UDPControlInfo();
    udpCtrl->setSockId(sd->sockId);
    udpCtrl->setUserId(sd->userId);
    udpCtrl->setSrcAddr(ipCtrl->getSrcAddr());
    udpCtrl->setDestAddr(ipCtrl->getDestAddr());
    udpCtrl->setSrcPort(udpHeader->getSourcePort());
    udpCtrl->setDestPort(udpHeader->getDestinationPort());
    udpCtrl->setInterfaceId(ipCtrl->getInterfaceId());
    payload->setControlInfo(udpCtrl);

    send(payload, "appOut", sd->appGateIndex);
    numPassedUp++;
}

void SimpleUDP::processUndeliverablePacket(SimpleUDPPacket *udpPacket, cPolymorphic *ctrl)
{
    numDroppedWrongPort++;

    // send back ICMP PORT_UNREACHABLE
    if (dynamic_cast<IPControlInfo *>(ctrl)!=NULL) {
/*        if (!icmp)
            icmp = ICMPAccess().get();
        IPControlInfo *ctrl4 = (IPControlInfo *)ctrl;
        if (!ctrl4->getDestAddr().isMulticast())
            icmp->sendErrorMessage(udpPacket, ctrl4, ICMP_DESTINATION_UNREACHABLE, ICMP_DU_PORT_UNREACHABLE);*/
        /* TODO: implement icmp module */
        EV << "[SimpleUDP::processUndeliverablePacket()]\n"
                << "    Dropped packet bound to unreserved port, ignoring ICMP error"
                << endl;
    } else if (dynamic_cast<IPv6ControlInfo *>(udpPacket->getControlInfo())
               !=NULL) {
/*        if (!icmpv6)
            icmpv6 = ICMPv6Access().get();
        IPv6ControlInfo *ctrl6 = (IPv6ControlInfo *)ctrl;
        if (!ctrl6->getDestAddr().isMulticast())
            icmpv6->sendErrorMessage(udpPacket, ctrl6, ICMPv6_DESTINATION_UNREACHABLE, PORT_UNREACHABLE);*/
        /* TODO: implement icmp module */
        EV << "[SimpleUDP::processUndeliverablePacket()]\n"
           << "    Dropped packet bound to unreserved port, ignoring ICMP error"
           << endl;
    } else {
        error("(%s)%s arrived from lower layer without control info", udpPacket->getClassName(), udpPacket->getName());
    }
}

void SimpleUDP::processICMPError(cMessage *msg)
{
    // extract details from the error message, then try to notify socket that sent bogus packet
    int type = 0;
    int code = 0;
    IPvXAddress localAddr, remoteAddr;
    int localPort = 0;
    int remotePort = 0;

    if (dynamic_cast<ICMPMessage *>(msg)) {
        ICMPMessage *icmpMsg = (ICMPMessage *)msg;
        type = icmpMsg->getType();
        code = icmpMsg->getCode();
        icmpMsg->setBitLength(icmpMsg->getEncapsulatedPacket()->getBitLength()); // trick because payload in ICMP is conceptually truncated
        IPDatagram *datagram = check_and_cast<IPDatagram *>(icmpMsg->decapsulate());
        localAddr = datagram->getSrcAddress();
        remoteAddr = datagram->getDestAddress();
        SimpleUDPPacket *packet = check_and_cast<SimpleUDPPacket *>(datagram->decapsulate());
        localPort = packet->getSourcePort();
        remotePort = packet->getDestinationPort();
        delete icmpMsg;
        delete datagram;
        delete packet;
    } else if (dynamic_cast<ICMPv6Message *>(msg)) {
        ICMPv6Message *icmpMsg = (ICMPv6Message *)msg;
        type = icmpMsg->getType();
        code = -1; // FIXME this is dependent on getType()...
        IPv6Datagram *datagram = check_and_cast<IPv6Datagram *>(icmpMsg->decapsulate());
        localAddr = datagram->getSrcAddress();
        remoteAddr = datagram->getDestAddress();
        SimpleUDPPacket *packet = check_and_cast<SimpleUDPPacket *>(datagram->decapsulate());
        localPort = packet->getSourcePort();
        remotePort = packet->getDestinationPort();
        delete icmpMsg;
        delete datagram;
        delete packet;
    }
    EV << "[SimpleUDP::processICMPError() @ " << localAddr << "]\n"
       << "    ICMP error received: type=" << type << " code=" << code
       << "\n    about packet " << localAddr << ":" << localPort << " --> "
       << "    " << remoteAddr << ":" << remotePort
       << endl;

    // identify socket and report error to it
    SocketsByPortMap::iterator it = socketsByPortMap.find(localPort);
    if (it==socketsByPortMap.end()) {
        EV << "[SimpleUDP::processICMPError() @ " << localAddr << "]\n"
           << "    No socket on that local port, ignoring ICMP error"
           << endl;
        return;
    }
    SockDescList& list = it->second;
    SockDesc *srcSocket = NULL;
    for (SockDescList::iterator it=list.begin(); it!=list.end(); ++it) {
        SockDesc *sd = *it;
        if (sd->onlyLocalPortIsSet || matchesSocket(sd, localAddr, remoteAddr, remotePort)) {
            srcSocket = sd; // FIXME what to do if there's more than one matching socket ???
        }
    }
    if (!srcSocket) {
        EV << "[SimpleUDP::processICMPError() @ " << localAddr << "]\n"
           << "    No matching socket, ignoring ICMP error"
           << endl;
        return;
    }

    // send UDP_I_ERROR to socket
    EV << "[SimpleUDP::processICMPError() @ " << localAddr << "]\n"
       << "    Source socket is sockId=" << srcSocket->sockId << ", notifying"
       << endl;
    sendUpErrorNotification(srcSocket, UDP_I_ERROR, localAddr, remoteAddr, remotePort);
}

void SimpleUDP::sendUpErrorNotification(SockDesc *sd, int msgkind, const IPvXAddress& localAddr, const IPvXAddress& remoteAddr, short remotePort)
{
    cMessage *notifyMsg = new cMessage("ERROR", msgkind);
    UDPControlInfo *udpCtrl = new UDPControlInfo();
    udpCtrl->setSockId(sd->sockId);
    udpCtrl->setUserId(sd->userId);
    udpCtrl->setSrcAddr(localAddr);
    udpCtrl->setDestAddr(remoteAddr);
    udpCtrl->setSrcPort(sd->localPort);
    udpCtrl->setDestPort(remotePort);
    notifyMsg->setControlInfo(udpCtrl);

    send(notifyMsg, "appOut", sd->appGateIndex);
}

void SimpleUDP::processUDPPacket(SimpleUDPPacket *udpPacket)
{
    cModule *node = getParentModule();
//    IPvXAddress ip = IPAddressResolver().addressOf(node);
//    Speedhack SK

    // simulate checksum: discard packet if it has bit error
    EV << "[SimpleUDP::processUDPPacket() @ " << IPAddressResolver().addressOf(node) << "]\n"
       << "    Packet " << udpPacket->getName() << " received from network, dest port " << udpPacket->getDestinationPort()
       << endl;

    if (udpPacket->hasBitError() && dropErrorPackets) {
        EV << "[SimpleUDP::processUDPPacket() @ " << IPAddressResolver().addressOf(node) << "]\n"
           << "    Packet has bit error, discarding"
           << endl;
        delete udpPacket;
        numDroppedBadChecksum++;
        return;
    }
    else   /// add by denacast
    {
    	BaseOverlayMessage* encap = check_and_cast<BaseOverlayMessage*> (udpPacket->decapsulate());
    	if(dynamic_cast<EncapVideoMessage*>(encap) !=NULL)
    		encap->setBitError(udpPacket->hasBitError());
    	udpPacket->encapsulate(encap);
    }


    int destPort = udpPacket->getDestinationPort();
    cPolymorphic *ctrl = udpPacket->removeControlInfo();

    // send back ICMP error if no socket is bound to that port
    SocketsByPortMap::iterator it = socketsByPortMap.find(destPort);
    if (it==socketsByPortMap.end()) {
        EV << "[SimpleUDP::processUDPPacket() @ " << IPAddressResolver().addressOf(node) << "]\n"
           << "    No socket registered on port " << destPort
           << endl;
        processUndeliverablePacket(udpPacket, ctrl);
        return;
    }
    SockDescList& list = it->second;

    int matches = 0;

    // deliver a copy of the packet to each matching socket
    //    cMessage *payload = udpPacket->getEncapsulatedPacket();
    cMessage *payload = udpPacket->decapsulate();
    if (dynamic_cast<IPControlInfo *>(ctrl)!=NULL) {
        IPControlInfo *ctrl4 = (IPControlInfo *)ctrl;
        for (SockDescList::iterator it=list.begin(); it!=list.end(); ++it) {
            SockDesc *sd = *it;
            if (sd->onlyLocalPortIsSet || matchesSocket(sd, udpPacket, ctrl4)) {
//              EV << "[SimpleUDP::processUDPPacket() @ " << IPAddressResolver().addressOf(node) << "]\n"
//                 << "    Socket sockId=" << sd->sockId << " matches, sending up a copy"
//                 << endl;
//              sendUp((cMessage*)payload->dup(), udpPacket, ctrl4, sd);
//              ib: speed hack

                if (matches == 0) {
                    sendUp(payload, udpPacket, ctrl4, sd);
                } else
                    opp_error("Edit SimpleUDP.cc to support multibinding.");
                matches++;
            }
        }
    } else if (dynamic_cast<IPv6ControlInfo *>(udpPacket->getControlInfo())
               !=NULL) {
        IPv6ControlInfo *ctrl6 = (IPv6ControlInfo *)ctrl;
        for (SockDescList::iterator it=list.begin(); it!=list.end(); ++it) {
            SockDesc *sd = *it;
            if (sd->onlyLocalPortIsSet || matchesSocket(sd, udpPacket, ctrl6)) {
                EV << "[SimpleUDP::processUDPPacket() @ " << IPAddressResolver().addressOf(node) << "]\n"
                   << "    Socket sockId=" << sd->sockId << " matches, sending up a copy"
                   << endl;
                sendUp((cMessage*)payload->dup(), udpPacket, ctrl6, sd);
                matches++;
            }
        }
    } else {
        error("(%s)%s arrived from lower layer without control info", udpPacket->getClassName(), udpPacket->getName());
    }

    // send back ICMP error if there is no matching socket
    if (matches==0) {
        EV << "[SimpleUDP::processUDPPacket() @ " << IPAddressResolver().addressOf(node) << "]\n"
           << "    None of the sockets on port " << destPort << " matches the packet"
           << endl;
        processUndeliverablePacket(udpPacket, ctrl)
        ;
        return;
    }

    delete udpPacket;
    delete ctrl;
}


void SimpleUDP::processMsgFromApp(cPacket *appData)
{
    cModule *node = getParentModule();
//    IPvXAddress ip = IPAddressResolver().addressOf(node);
//    Speedhack SK

    IPvXAddress srcAddr, destAddr;
    //cGate* destGate;

    UDPControlInfo *udpCtrl = check_and_cast<UDPControlInfo *>(appData->removeControlInfo());

    SimpleUDPPacket *udpPacket = new SimpleUDPPacket(appData->getName());

    //
    udpPacket->setByteLength(UDP_HEADER_BYTES + IP_HEADER_BYTES);
    udpPacket->encapsulate(appData);

    // set source and destination port
    udpPacket->setSourcePort(udpCtrl->getSrcPort());
    udpPacket->setDestinationPort(udpCtrl->getDestPort());

    srcAddr = udpCtrl->getSrcAddr();
    destAddr = udpCtrl->getDestAddr();
    if (!udpCtrl->getDestAddr().isIPv6()) {
        // send to IPv4
        //EV << "[SimpleUDP::processMsgFromApp() @ " << IPAddressResolver().addressOf(node) << "]\n"
	//<< "    Sending app packet " << appData->getName() << " over IPv4"
	//<< endl;
        IPControlInfo *ipControlInfo = new IPControlInfo();
        ipControlInfo->setProtocol(IP_PROT_UDP);
        ipControlInfo->setSrcAddr(srcAddr.get4());
        ipControlInfo->setDestAddr(destAddr.get4());
        ipControlInfo->setInterfaceId(udpCtrl->getInterfaceId());
        udpPacket->setControlInfo(ipControlInfo);
        delete udpCtrl;
    } else {
        // send to IPv6
        //EV << "[SimpleUDP::processMsgFromApp() @ " << IPAddressResolver().addressOf(node) << "]\n"
	//<< "    Sending app packet " << appData->getName() << " over IPv6"
	//<< endl;
        IPv6ControlInfo *ipControlInfo = new IPv6ControlInfo();
        ipControlInfo->setProtocol(IP_PROT_UDP);
        ipControlInfo->setSrcAddr(srcAddr.get6());
        ipControlInfo->setDestAddr(destAddr.get6());
        // ipControlInfo->setInterfaceId(udpCtrl->InterfaceId()); FIXME extend IPv6 with this!!!
        udpPacket->setControlInfo(ipControlInfo);
        delete udpCtrl;
    }

    SimpleInfo* info = dynamic_cast<SimpleInfo*>(globalNodeList->getPeerInfo(destAddr));
    numSent++;

    if (info == NULL) {
        EV << "[SimpleUDP::processMsgFromApp() @ " << IPAddressResolver().addressOf(node) << "]\n"
           << "    No route to host " << destAddr
           << endl;

        delete udpPacket;
        numDestUnavailableLost++;
        return;
    }

    SimpleNodeEntry* destEntry = info->getEntry();

    // calculate delay
    simtime_t totalDelay = 0;
    if (srcAddr != destAddr) {
        SimpleNodeEntry::SimpleDelay temp;
        if (faultyDelay) {
            SimpleInfo* thisInfo = static_cast<SimpleInfo*>(globalNodeList->getPeerInfo(srcAddr));
            temp = nodeEntry->calcDelay(udpPacket, *destEntry,
                                        !(thisInfo->getNpsLayer() == 0 ||
                                          info->getNpsLayer() == 0)); //TODO
        } else {
            temp = nodeEntry->calcDelay(udpPacket, *destEntry);
        }
        if (useCoordinateBasedDelay == false) {
            totalDelay = constantDelay;
        } else if (temp.second == false) {
            EV << "[SimpleUDP::processMsgFromApp() @ " << IPAddressResolver().addressOf(node) << "]\n"
               << "    Send queue full: packet " << udpPacket << " dropped"
               << endl;
            delete udpPacket;
            numQueueLost++;
            return;
        } else {
            totalDelay = temp.first;
        }
    }

    SimpleInfo* thisInfo = dynamic_cast<SimpleInfo*>(globalNodeList->getPeerInfo(srcAddr));

    if (!globalNodeList->areNodeTypesConnected(thisInfo->getTypeID(), info->getTypeID())) {
        EV << "[SimpleUDP::processMsgFromApp() @ " << IPAddressResolver().addressOf(node) << "]\n"
                   << "    Partition " << thisInfo->getTypeID() << "->" << info->getTypeID()
                   << " is not connected"
                   << endl;
        delete udpPacket;
        numPartitionLost++;
        return;
    }

    if (jitter) {
        // jitter
        //totalDelay += truncnormal(0, SIMTIME_DBL(totalDelay) * jitter);

        //workaround (bug in truncnormal(): sometimes returns inf)
        double temp = truncnormal(0, SIMTIME_DBL(totalDelay) * jitter);
        while (temp == INFINITY || temp != temp) { // reroll if temp is INF or NaN
            std::cerr << "\n******* SimpleUDP: truncnormal() -> inf !!\n"
                      << std::endl;
            temp = truncnormal(0, SIMTIME_DBL(totalDelay) * jitter);
        }

        totalDelay += temp;
    }

    if (udpPacket != NULL) {
        BaseOverlayMessage* temp = NULL;

        if (ev.isGUI() && udpPacket->getEncapsulatedPacket()) {
            if ((temp = dynamic_cast<BaseOverlayMessage*>(udpPacket
                    ->getEncapsulatedPacket()))) {
                switch (temp->getStatType()) {
                case APP_DATA_STAT:
                    udpPacket->setKind(1);
                    break;
                case APP_LOOKUP_STAT:
                    udpPacket->setKind(2);
                    break;
                case MAINTENANCE_STAT:
                default:
                    udpPacket->setKind(3);
                }
            } else {
                udpPacket->setKind(1);
            }
        }

        EV << "[SimpleUDP::processMsgFromApp() @ " << IPAddressResolver().addressOf(node) << "]\n"
           << "    Packet " << udpPacket << " sent with delay = " << totalDelay
           << endl;

        //RECORD_STATS(globalStatistics->addStdDev("SimpleUDP: delay", totalDelay));

        sendDirect(udpPacket, totalDelay, 0, destEntry->getGate());
    }
}

void SimpleUDP::processCommandFromApp(cMessage *msg)
{
    UDPControlInfo *udpCtrl = check_and_cast<UDPControlInfo *>(msg->removeControlInfo());
    switch (msg->getKind()) {
    case UDP_C_BIND:
        bind(msg->getArrivalGate()->getIndex(), udpCtrl);
        break;
    case UDP_C_CONNECT:
        connect(udpCtrl->getSockId(), udpCtrl->getDestAddr(), udpCtrl->getDestPort());
        break;
    case UDP_C_UNBIND:
        unbind(udpCtrl->getSockId());
        break;
    default:
        error("unknown command code (message kind) %d received from app", msg->getKind());
    }

    delete udpCtrl;
    delete msg;
}


void SimpleUDP::setNodeEntry(SimpleNodeEntry* entry)
{
    nodeEntry = entry;
}
