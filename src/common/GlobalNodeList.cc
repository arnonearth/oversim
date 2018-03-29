//
// Copyright (C) 2006 Institut fuer Telematik, Universitaet Karlsruhe (TH)
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
 * @file GlobalNodeList.cc
 * @author Markus Mauch, Robert Palmer
 */

#include <iostream>

#include <omnetpp.h>

#include <NotificationBoard.h>
#include <BinaryValue.h>
#include <OverlayKey.h>
#include <PeerInfo.h>
#include <BaseOverlay.h>
#include <GlobalStatisticsAccess.h>
#include <hashWatch.h>
#include <BootstrapList.h>

#include "GlobalNodeList.h"

Define_Module(GlobalNodeList);

std::ostream& operator<<(std::ostream& os, const bootstrapEntry entry)
{
    NodeHandle* nodeHandle = dynamic_cast<NodeHandle*>(entry.node);

    os << "Address: " << entry.node->getAddress()
       << " Port: " << entry.node->getPort();

    if (nodeHandle) {
        os << " NodeId: " << nodeHandle->getKey();
    }

    os << " ModuleID: "
       << entry.info->getModuleID() << " Bootstrapped: "
       << (entry.info->isBootstrapped() ? "true" : "false")
       << " NPS Layer: " << ((int) entry.info->getNpsLayer())
       << " TypeID: " << (entry.info->getTypeID());

    return os;
}

void GlobalNodeList::initialize()
{
    maxNumberOfKeys = par("maxNumberOfKeys");
    keyProbability = par("keyProbability");

    WATCH_UNORDERED_MAP(peerSet);
    WATCH_VECTOR(keyList);
    WATCH(bootstrappedPeerSize);
    WATCH(bootstrappedMaliciousNodes);
    WATCH(maliciousNodes);
    WATCH(landmarkPeerSize);

    createKeyList(maxNumberOfKeys);
    bootstrappedPeerSize = 0;
    landmarkPeerSize = 0;

    for (int i = 0; i < MAX_NODETYPES; i++) {
        bootstrappedPeerSizePerType[i] = 0;
        landmarkPeerSizePerType[i] = 0;
    }

    bootstrappedMaliciousNodes = 0;
    maliciousNodes = 0;
    preKilledNodes = 0;

    if (par("maliciousNodeChange")) {
        if ((double) par("maliciousNodeProbability") > 0)
            error("maliciousNodeProbability and maliciousNodeChange are not supported concurrently");

        cMessage* msg = new cMessage("maliciousNodeChange");
        scheduleAt(simTime() + (int) par("maliciousNodeChangeStartTime"), msg);
        maliciousNodesVector.setName("MaliciousNodeRate");
        maliciousNodesVector.record(0);
        maliciousNodeRatio = 0;
    }

    min_ip = 0xFFFFFFFF;
    max_ip = 0x00000000;

    for (int i=0; i<MAX_NODETYPES; i++) {
        for (int j=0; j<MAX_NODETYPES; j++) {
            connectionMatrix[i][j] = true;
        }
    }

    globalStatistics = GlobalStatisticsAccess().get();

    cMessage* timer = new cMessage("oracleTimer");

    scheduleAt(simTime(), timer);
}

void GlobalNodeList::handleMessage(cMessage* msg)
{
    if (msg->isName("maliciousNodeChange")) {
        double newRatio = maliciousNodeRatio + (double) par("maliciousNodeChangeRate"); // ratio to obtain
        if (maliciousNodeRatio < (double) par("maliciousNodeChangeStartValue"))
            newRatio = (double) par("maliciousNodeChangeStartValue");

        if (newRatio < (double) par("maliciousNodeChangeStopValue")) // schedule next event
            scheduleAt(simTime() + (int) par("maliciousNodeChangeInterval"), msg);

        int nodesNeeded = (int) (((double) par("maliciousNodeChangeRate")) * peerSet.size());

        EV << "[GlobalNodeList::handleMessage()]\n"
           << "    Changing " << nodesNeeded << " nodes to be malicious"
           << endl;

        for (int i = 0; i < nodesNeeded; i++) {
            // search a node that is not yet malicious
            NodeHandle node;
            do {
                node = getRandomNode(0, false);
            } while (isMalicious(node));

            setMalicious(node, true);
        }

        maliciousNodesVector.record(newRatio);
        maliciousNodeRatio = newRatio;

        return;
    }

    else if (msg->isName("oracleTimer")) {
        RECORD_STATS(globalStatistics->recordOutVector(
                     "GlobalNodeList: Number of nodes", peerSet.size()));
        RECORD_STATS(globalStatistics->recordOutVector(
                     "GlobalNodeList: Number of bootstrapped malicious nodes",
                     bootstrappedMaliciousNodes));
        scheduleAt(simTime() + 50, msg);
    } else {
        opp_error("GlobalNodeList::handleMessage: Unknown message type!");
    }
}

const NodeHandle& GlobalNodeList::getBootstrapNode(const NodeHandle &node)
{
    uint32_t nodeType;
    PeerHashMap::iterator it;

    // always prefer boot node from the same partition
    // if there is no such node, go through all
    // connected partitions until a bootstrap node is found
    if (!node.isUnspecified()) {
        it = peerSet.find(node.getAddress());

        // this should never happen
        if (it == peerSet.end()) {
           return getRandomNode(0, true);
        }

        nodeType = it->second.info->getTypeID();
        const NodeHandle &tempNode1 = getRandomNode(nodeType, true);

        if (tempNode1.isUnspecified()) {
            for (uint32_t i = 1; i < MAX_NODETYPES; i++) {
                if (i == nodeType)
                    continue;

                if (connectionMatrix[nodeType][i]) {
                    const NodeHandle &tempNode2 = getRandomNode(i, true);
                    if (!tempNode2.isUnspecified())
                        return tempNode2;
                }
            }
            return NodeHandle::UNSPECIFIED_NODE;
        } else {
            return tempNode1;
        }
    } else {
        return getRandomNode(0, true);
    }
}
const NodeHandle& GlobalNodeList::getRandomNode(uint32_t nodeType,
                                                bool bootstrappedNeeded,
                                                bool inoffensiveNeeded)
{
    if (inoffensiveNeeded &&
            ((nodeType != 0) || (bootstrappedNeeded == false))) {
        throw cRuntimeError("GlobalNodeList::getRandomNode(): "
                            "inoffensiveNeeded must only be used "
                            "with nodeType = 0 and bootstrappedNeeded = true!");
    }

    if (peerSet.size() == 0)
        return NodeHandle::UNSPECIFIED_NODE;
    if (bootstrappedNeeded && bootstrappedPeerSize == 0)
        return NodeHandle::UNSPECIFIED_NODE;
    if (nodeType && bootstrappedPeerSizePerType[nodeType] == 0)
        return NodeHandle::UNSPECIFIED_NODE;
    if (inoffensiveNeeded &&
            (bootstrappedPeerSize - bootstrappedMaliciousNodes <= 0))
        return NodeHandle::UNSPECIFIED_NODE;
    else {
        // return random TransportAddress in O(log n)
        PeerHashMap::iterator it = peerSet.end();
        bootstrapEntry tempEntry = {NULL, NULL};

        while (it == peerSet.end() ||(nodeType && (it->second.info->getTypeID() != nodeType))
                || (bootstrappedNeeded && !it->second.info->isBootstrapped())
                || (inoffensiveNeeded && it->second.info->isMalicious())) {

            IPvXAddress randomAddr(intuniform(min_ip, max_ip));

            it = peerSet.find(randomAddr);

            if (it == peerSet.end()) {
                it = peerSet.insert(std::make_pair(randomAddr,tempEntry)).first;
                peerSet.erase(it++);
            }

            if (it == peerSet.end())
                it = peerSet.begin();
        }

        if (dynamic_cast<NodeHandle*>(it->second.node)) {
            return *dynamic_cast<NodeHandle*>(it->second.node);
        } else {
            return NodeHandle::UNSPECIFIED_NODE;
        }
    }
}

void GlobalNodeList::sendNotificationToAllPeers(int category)
{
    PeerHashMap::iterator it;
    for (it = peerSet.begin(); it != peerSet.end(); it++) {
        NotificationBoard* nb = check_and_cast<NotificationBoard*>(
                simulation.getModule(it->second.info->getModuleID())
                ->getSubmodule("notificationBoard"));

        nb->fireChangeNotification(category);
    }
}

void GlobalNodeList::addPeer(const IPvXAddress& ip, PeerInfo* info)
{
    bootstrapEntry temp;
    temp.node = new TransportAddress(ip);
    temp.info = info;
    temp.info->setPreKilled(false);

    peerSet.insert(std::make_pair(temp.node->getAddress(), temp));
    // set bounds for random node retrieval
    // TODO: doesn't work with IPv6
    if (ip.get4().getInt() < min_ip) min_ip = ip.get4().getInt();
    if (ip.get4().getInt() > max_ip) max_ip = ip.get4().getInt();

    if (uniform(0, 1) < (double) par("maliciousNodeProbability") ||
            (par("maliciousNodeChange") && uniform(0, 1) < maliciousNodeRatio)) {
        setMalicious(*temp.node, true);
    }

    if (peerSet.size() == 1) {
        // we need at least one inoffensive bootstrap node
        setMalicious(*temp.node, false);
    }
}

void GlobalNodeList::registerPeer(const TransportAddress& peer)
{
    PeerHashMap::iterator it = peerSet.find(peer.getAddress());
    if (it == peerSet.end())
        error("unable to bootstrap peer, peer is not in peer set");
    else {
        PeerInfo* info = it->second.info;

        if (!info->isBootstrapped()) {
            bootstrappedPeerSize++;
            bootstrappedPeerSizePerType[info->getTypeID()]++;
            info->setBootstrapped();

            if (info->isMalicious())
                bootstrappedMaliciousNodes++;
        }

        delete it->second.node;
        peerSet.erase(it);

        bootstrapEntry temp;
        temp.node = new TransportAddress(peer);
        temp.info = info;
        peerSet.insert(std::make_pair(temp.node->getAddress(), temp));
    }
}

void GlobalNodeList::registerPeer(const NodeHandle& peer)
{
    PeerHashMap::iterator it = peerSet.find(peer.getAddress());
    if (it == peerSet.end())
        error("unable to bootstrap peer, peer is not in peer set");
    else {
        PeerInfo* info = it->second.info;

        if (!info->isBootstrapped()) {
            bootstrappedPeerSize++;
            bootstrappedPeerSizePerType[info->getTypeID()]++;
            info->setBootstrapped();

            if (info->isMalicious())
                bootstrappedMaliciousNodes++;
        }

        delete it->second.node;
        peerSet.erase(it);

        bootstrapEntry temp;
        temp.node = new NodeHandle(peer);
        temp.info = info;
        peerSet.insert(std::make_pair(temp.node->getAddress(), temp));
    }
}

void GlobalNodeList::refreshEntry(const TransportAddress& peer)
{
    PeerHashMap::iterator it = peerSet.find(peer.getAddress());
    if (it == peerSet.end()) {
        error("unable to refresh entry, peer is not in peer set");
    } else {
        PeerInfo* info = it->second.info;

        delete it->second.node;
        peerSet.erase(it);

        bootstrapEntry temp;
        temp.node = new TransportAddress(peer);
        temp.info = info;
        peerSet.insert(std::make_pair(temp.node->getAddress(), temp));
    }
}

void GlobalNodeList::removePeer(const TransportAddress& peer)
{
    PeerHashMap::iterator it = peerSet.find(peer.getAddress());
    if(it != peerSet.end() && it->second.info->isBootstrapped()) {
        bootstrappedPeerSize--;
        bootstrappedPeerSizePerType[it->second.info->getTypeID()]--;
        it->second.info->setBootstrapped(false);

        if(it->second.info->isMalicious())
            bootstrappedMaliciousNodes--;

        it->second.info->setBootstrapped(false);
    }
}

void GlobalNodeList::killPeer(const IPvXAddress& ip)
{
    PeerHashMap::iterator it = peerSet.find(ip);
    if(it != peerSet.end()) {
        if(it->second.info->isBootstrapped()) {
            bootstrappedPeerSize--;
            bootstrappedPeerSizePerType[it->second.info->getTypeID()]--;

            if(it->second.info->isMalicious())
                bootstrappedMaliciousNodes--;

            it->second.info->setBootstrapped(false);
        }

        if (it->second.info->isPreKilled()) {
            it->second.info->setPreKilled(false);
            preKilledNodes--;
        }

        // if valid NPS landmark: decrease landmarkPeerSize
        PeerInfo* peerInfo = getPeerInfo(ip);
        if (peerInfo->getNpsLayer() > -1) {
            landmarkPeerSize--;
            landmarkPeerSizePerType[it->second.info->getTypeID()]--;
        }

        delete it->second.node;
        delete it->second.info;
        peerSet.erase(it);
    }
}

void GlobalNodeList::createKeyList(uint32_t size)
{
    for (uint32_t i = 0; i < size; i++)
        keyList.push_back(OverlayKey::random());
}

GlobalNodeList::KeyList* GlobalNodeList::getKeyList(uint32_t maximumKeys)
{
    if (maximumKeys > keyList.size()) {
        maximumKeys = keyList.size();
    }
    // copy keylist to temporary keylist
    KeyList tmpKeyList;
    tmpKeyList.clear();

    for (uint32_t i=0; i < keyList.size(); i++) {
        tmpKeyList.push_back(keyList[i]);
    }

    KeyList* returnList = new KeyList;

    for (uint32_t i=0; i < ((float)maximumKeys * keyProbability); i++) {
        uint32_t index = intuniform(0, tmpKeyList.size()-1);

        returnList->push_back(tmpKeyList[index]);
        tmpKeyList.erase(tmpKeyList.begin()+index);
    }

    return returnList;
}

const OverlayKey& GlobalNodeList::getRandomKeyListItem() const
{
    return keyList[intuniform(0,keyList.size()-1)];
}

PeerInfo* GlobalNodeList::getPeerInfo(const TransportAddress& peer)
{
    PeerHashMap::iterator it = peerSet.find(peer.getAddress());

    if (it == peerSet.end())
        return NULL;
    else
        return it->second.info;
}

PeerInfo* GlobalNodeList::getPeerInfo(const IPvXAddress& ip)
{
    PeerHashMap::iterator it = peerSet.find(ip);

    if (it == peerSet.end())
        return NULL;
    else
        return it->second.info;
}

PeerInfo* GlobalNodeList::getRandomPeerInfo(uint32_t nodeType,
                                             bool bootstrappedNeeded) {
    // return random TransportAddress in O(log n)
    PeerHashMap::iterator it;
    bootstrapEntry tempEntry = {NULL, NULL};

    IPvXAddress randomAddr(intuniform(min_ip, max_ip));

    it = peerSet.find(randomAddr);
    if (it == peerSet.end()) {
        it = peerSet.insert(std::make_pair(randomAddr,tempEntry)).first;
        peerSet.erase(it++);
    }

    if (it == peerSet.end())
        it = peerSet.begin();

    // if nodeType != 0, search for next node with the given type
    if (nodeType) {
        while((nodeType && (it->second.info->getTypeID() != nodeType))
              || (bootstrappedNeeded && !it->second.info->isBootstrapped())) {
            ++it;
            if (it == peerSet.end()) it = peerSet.begin();
        }
    }

    return it->second.info;
}

void GlobalNodeList::setPreKilled(const TransportAddress& address)
{
    PeerInfo* peer = getPeerInfo(address);

    if ((peer != NULL) && !(peer->isPreKilled())) {
        preKilledNodes++;
        peer->setPreKilled(true);
    }
}

TransportAddress* GlobalNodeList::getRandomAliveNode(uint32_t nodeType)
{
    if (peerSet.size() <= preKilledNodes) {
        // all nodes are already marked for deletion;
        return NULL;
    } else {
        // return random address in O(log n)
        PeerHashMap::iterator it;
        bootstrapEntry tempEntry = {NULL, NULL};

        IPvXAddress randomAddr(intuniform(min_ip, max_ip));

        it = peerSet.find(randomAddr);

        if (it == peerSet.end()) {
            it = peerSet.insert(std::make_pair(randomAddr,tempEntry)).first;
            peerSet.erase(it++);
        }

        if (it == peerSet.end()) {
            it = peerSet.begin();
        }

        while ((nodeType && (it->second.info->getTypeID() != nodeType))
               || it->second.info->isPreKilled()) {
            it++;
            if (it == peerSet.end()) {
                it = peerSet.begin();
            }
        }

        return it->second.node;
    }

    return NULL;
}

void GlobalNodeList::setMalicious(const TransportAddress& address, bool malicious)
{
    PeerInfo* peer = getPeerInfo(address);

    if (peer != NULL) {
        if(malicious && !peer->isMalicious()) {
            maliciousNodes++;
            if (peer->isBootstrapped()) {
                bootstrappedMaliciousNodes++;
            }
        }

        if (!malicious && peer->isMalicious()) {
            maliciousNodes--;
            if (peer->isBootstrapped()) {
                bootstrappedMaliciousNodes--;
            }
        }
        peer->setMalicious(malicious);
    }
}

bool GlobalNodeList::isMalicious(const TransportAddress& address)
{
    PeerInfo* peer = getPeerInfo(address);

    if(peer != NULL)
        return peer->isMalicious();

    return false;
}

cObject** GlobalNodeList::getContext(const TransportAddress& address)
{
    PeerInfo* peer = getPeerInfo(address);

    if(peer != NULL)
        return peer->getContext();

    return NULL;
}

void GlobalNodeList::setOverlayReadyIcon(const TransportAddress& address,
        bool ready)
{
    if (ev.isGUI()) {
        const char* color;

        if (ready) {
            // change color if node is malicious
            color = isMalicious(address) ? "green" : "";
        } else {
            color = isMalicious(address) ? "yellow" : "red";
        }

        PeerInfo* info = getPeerInfo(address);

        if(info != NULL) {
            simulation.getModule(info->getModuleID())
            ->getDisplayString().setTagArg("i2", 1, color);
        }
    }
}

bool GlobalNodeList::areNodeTypesConnected(uint32_t a, uint32_t b)
{
    if ((a > MAX_NODETYPES) || (b > MAX_NODETYPES)) {
        throw cRuntimeError("GlobalNodeList::areNodeTypesConnected(): nodeType "
              "bigger then MAX_NODETYPES");
    }

    return connectionMatrix[a][b];
}

void GlobalNodeList::connectNodeTypes(uint32_t a, uint32_t b)
{
    if ((a > MAX_NODETYPES) || (b > MAX_NODETYPES)) {
        throw cRuntimeError("GlobalNodeList::connectNodeTypes(): nodeType "
              "bigger then MAX_NODETYPES");
    }

    connectionMatrix[a][b]=true;

    EV << "[GlobalNodeList::connectNodeTypes()]\n"
       << "    Connecting " << a << "->" << b
       << endl;

}

void GlobalNodeList::disconnectNodeTypes(uint32_t a, uint32_t b)
{
    if ((a > MAX_NODETYPES) || (b > MAX_NODETYPES)) {
        throw cRuntimeError("GlobalNodeList::disconnectNodeTypes(): nodeType "
              "bigger then MAX_NODETYPES");
    }

    connectionMatrix[a][b]=false;

    EV << "[GlobalNodeList::disconnectNodeTypes()]\n"
       << "    Disconnecting " << a << "->" << b
       << endl;

}

void GlobalNodeList::mergeBootstrapNodes(int toPartition, int fromPartition,
                                          int numNodes)
{
    BootstrapList* bootstrapList =
        check_and_cast<BootstrapList*>(simulation.getModule(
            getRandomPeerInfo(toPartition, false)->getModuleID())->
            getSubmodule("bootstrapList"));

    bootstrapList->insertBootstrapCandidate(getRandomNode(fromPartition, true),
                                       DNSSD);
}

GlobalNodeList::~GlobalNodeList()
{
    PeerHashMap::iterator it;
    for(it = peerSet.begin(); it != peerSet.end(); ++it) {
        delete it->second.node;
        delete it->second.info;
    }
}

