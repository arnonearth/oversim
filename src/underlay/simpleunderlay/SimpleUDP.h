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
 * @file SimpleUDP.h
 * @author Jochen Reber
 */

//
// Author: Jochen Reber
// Rewrite: Andras Varga 2004,2005
// Modifikations: Stephan Krause
//

#ifndef __SIMPLEUDP_H__
#define __SIMPLEUDP_H__

#include <map>
#include <list>

#include <InitStages.h>
//#include <SimpleNodeEntry.h>
//#include <GlobalNodeList.h>
//#include <SimpleInfo.h>

#include "UDPControlInfo_m.h"

class SimpleUDPPacket;
class GlobalNodeList;
class SimpleNodeEntry;
class GlobalStatistics;

class IPControlInfo;
class IPv6ControlInfo;
class ICMP;
class ICMPv6;

//const char *ERROR_IP_ADDRESS = "10.0.0.255";
const int UDP_HEADER_BYTES = 8; /**< length of an udp header in bytes */


/**
 * Implements the UDP protocol: encapsulates/decapsulates user data into/from UDP.
 *
 * More info in the NED file.
 */
class INET_API SimpleUDP : public cSimpleModule
{
public:

    /**
     * defines a socket
     */
    struct SockDesc
    {
        int sockId; /**< socket deskriptor, supposed to be unique across apps */
        int userId; /**< we just send it back, but don't do anything with it */
        int appGateIndex; /**< indicates the application the socket is connected to */
        bool onlyLocalPortIsSet; /**< remoteAddr, localPort and remotePort are not set */
        IPvXAddress localAddr; /**< IP Adress to which replies have to be sent */
        IPvXAddress remoteAddr; /**< IP Adress to which queries are sent */
        ushort localPort; /**< port to which replies have to be sent */
        ushort remotePort; /**< port to which queries are sent */
        // FIXME do real sockets allow filtering by input interface?? - no they dont :)
        int interfaceId; /**< Interface to which the socket is connected */
    };

    typedef std::list<SockDesc *> SockDescList; /**< list of socket pointers */
    typedef std::map<int,SockDesc *> SocketsByIdMap; /**< list of socket ordered by sockId */
    typedef std::map<int,SockDescList> SocketsByPortMap; /**< list of sockets ordered by localPort */

    // delay fault type string and corresponding map for switch..case
    static std::string delayFaultTypeString;
    enum delayFaultTypeNum {
        delayFaultUndefined,
        delayFaultLiveAll,
        delayFaultLivePlanetlab,
        delayFaultSimulation
    };
    static std::map<std::string, delayFaultTypeNum> delayFaultTypeMap;

protected:
    // sockets
    SocketsByIdMap socketsByIdMap; /**< list of socket ordered by sockId */
    SocketsByPortMap socketsByPortMap; /**< list of sockets ordered by localPort */

    // other state vars
    short lastEphemeralPort; /**< last used port (all ports above should be free) */
    ICMP *icmp; /**< pointer to a received icmp (IPv4) message */
    ICMPv6 *icmpv6; /**< pointer to a received icmp (IPv6) message */

    // statistics
    int numSent; /**< number of packets sent to the network */
    int numPassedUp; /**< number of packet passed up to applications */
    int numDroppedWrongPort; /**< number of dropped packets due to wrong destination port */
    int numDroppedBadChecksum; /**< number of dropped packets due to bad checksum */
    int numQueueLost; /**< number of lost packets due to queue full */
    int numPartitionLost; /**< number of lost packets due to network partitions */
    int numDestUnavailableLost; /**< number of lost packets due to unavailable destination */
    simtime_t delay; /**< simulated delay between sending and receiving udp module */
    bool dropErrorPackets;

    simtime_t constantDelay; /**< constant delay between two peers */
    bool useCoordinateBasedDelay; /**< delay should be calculated from euklidean distance between two peers */
    double jitter; /**< amount of jitter in % of total delay */
    bool faultyDelay; /** violate the triangle inequality?*/
    GlobalNodeList* globalNodeList; /**< pointer to GlobalNodeList */
    GlobalStatistics* globalStatistics; /**< pointer to GlobalStatistics */
    SimpleNodeEntry* nodeEntry; /**< nodeEntry of the overlay node this module belongs to */

public:
    /**
     * set or change the nodeEntry of this module
     *
     * @param entry the new nodeEntry
     */
    void setNodeEntry(SimpleNodeEntry* entry);

protected:
    /**
     * utility: show current statistics above the icon
     */
    void updateDisplayString();

    /**
     * bind socket
     *
     * @param gateIndex application gate connected to the socket
     * @param ctrl control information for communication
     */
    void bind(int gateIndex, UDPControlInfo *ctrl);

    /**
     * connect socket
     *
     * @param sockId id of the socket to connect
     * @param addr IPvXAddress of the remote socket
     * @param port port of the remote socket
     */
    void connect(int sockId, IPvXAddress addr, int port);

    /**
     * unbind socket
     *
     * @param sockId id of the socket to unbind
     */
    void unbind(int sockId);

    /**
     * ephemeral port.
     *
     * @return a free ephemeral port, if there is no free port, error is called
     */
    short getEphemeralPort();

    /**
     * decides if a received packet (IPv4) belongs to the specified socket
     *
     * @param sd the socket
     * @param udp the received SimpleUDPPacket
     * @param ctrl the IPControlInfo of udp
     * @return true if destination of the SimpleUDPPacket is the specified socket
     */
    bool matchesSocket(SockDesc *sd, SimpleUDPPacket *udp, IPControlInfo *ctrl);

    /**
     * decides if a received packet (IPv6) belongs to the specified socket
     *
     * @param sd the socket
     * @param udp the received SimpleUDPPacket
     * @param ctrl the IPControlInfo of udp
     * @return true if destination of the SimpleUDPPacket is the specified socket
     */
    bool matchesSocket(SockDesc *sd, SimpleUDPPacket *udp, IPv6ControlInfo *ctrl);

    /**
     * decides if a socket matches the specified parameters
     *
     * @param sd the socket
     * @param localAddr the specified localAddr
     * @param remoteAddr the specified remoteAddr
     * @param remotePort the specified remotePort
     * @return true if specified parameters match the sockets parameters
     */
    bool matchesSocket(SockDesc *sd, const IPvXAddress& localAddr, const IPvXAddress& remoteAddr, short remotePort);

    /**
     * sends payload up to the application
     *
     * @param payload the payload
     * @param udpHeader the udpHeader of the IPv4 message
     * @param ctrl the IPControlInfo of the IPv4 message
     * @param sd the socket at which the IPv4 message arrived
     */
    void sendUp(cMessage *payload, SimpleUDPPacket *udpHeader, IPControlInfo *ctrl, SockDesc *sd);

    /**
     * sends payload up to the application
     *
     * @param payload the payload
     * @param udpHeader the udpHeader of the IPv6 message
     * @param ctrl the IPControlInfo of the IPv6 message
     * @param sd the socket at which the IPv6 message arrived
     */
    void sendUp(cMessage *payload, SimpleUDPPacket *udpHeader, IPv6ControlInfo *ctrl, SockDesc *sd);

    /**
     * handles received packet which destination port is not bound to any socket
     *
     * @param udpPacket the SimpleUDPPacket with the wrong port number
     * @param ctrl the IPControlInfo of udpPacket
     */
    void processUndeliverablePacket(SimpleUDPPacket *udpPacket, cPolymorphic *ctrl);

    /**
     * sends an error up to the application
     *
     * @param sd the socket that received the error
     * @param msgkind the type of the error notification
     * @param localAddr the address from which the faulty message was sent
     * @param remoteAddr the address to which the faulty message was sent
     * @param remotePort the port to which the faulty message was sent
     */
    void sendUpErrorNotification(SockDesc *sd, int msgkind, const IPvXAddress& localAddr, const IPvXAddress& remoteAddr, short remotePort);

    /**
     * process an ICMP error packet
     *
     * @param icmpErrorMsg the received icmp Message
     */
    virtual void processICMPError(cMessage *icmpErrorMsg); // TODO use ICMPMessage

    /**
     * process UDP packets coming from IP
     *
     * @param udpPacket the received packet
     */
    virtual void processUDPPacket(SimpleUDPPacket *udpPacket);

    /**
     * process packets from application
     *
     * @param appData the data that has to be sent
     */
    virtual void processMsgFromApp(cPacket *appData);

    /**
     * process commands from application
     *
     * @param msg the command message
     */
    virtual void processCommandFromApp(cMessage *msg);

public:
    SimpleUDP();

    /** destructor */
    virtual ~SimpleUDP();

protected:
    /**
     * initialise the SimpleUDP module
     *
     * @param stage stage of initialisation phase
     */
    virtual void initialize(int stage);

    /**
     * returns the number of init stages
     *
     * @return the number of init stages
     */
    virtual int numInitStages() const
    {
        return MAX_STAGE_UNDERLAY + 1;
    }
    /**
     * process received messages
     *
     * @param msg the received message
     */
    virtual void handleMessage(cMessage *msg);

    void finish();
};

#endif

