#
# OMNeT++/OMNEST Makefile for oversim
#
# This file was generated with the command:
#  opp_makemake -f --deep -I../../inet/src/networklayer/common -I../../inet/src/networklayer/rsvp_te -I../../inet/src/networklayer/icmpv6 -I../../inet/src/transport/tcp -I../../inet/src/networklayer/mpls -I../../inet/src/networklayer/ted -I../../inet/src/networklayer/contract -I../../inet/src/util -I../../inet/src/transport/contract -I../../inet/src/linklayer/mfcore -I../../inet/src/networklayer/ldp -I../../inet/src/applications/udpapp -I../../inet/src/networklayer/ipv4 -I../../inet/src/base -I../../inet/src/util/headerserializers -I../../inet/src/networklayer/ipv6 -I../../inet/src/transport/sctp -I../../inet/src/world -I../../inet/src/applications/pingapp -I../../inet/src/linklayer/contract -I../../inet/src/networklayer/arp -I../../inet/src/transport/udp -L../../inet/out/$(CONFIGNAME)/src -linet -KINET_PROJ=../../inet
#

# Name of target to be created (-o option)
TARGET = oversim$(EXE_SUFFIX)

# User interface (uncomment one) (-u option)
USERIF_LIBS = $(ALL_ENV_LIBS) # that is, $(TKENV_LIBS) $(CMDENV_LIBS)
#USERIF_LIBS = $(CMDENV_LIBS)
#USERIF_LIBS = $(TKENV_LIBS)

# C++ include paths (with -I)
INCLUDE_PATH = \
    -I../../inet/src/networklayer/common \
    -I../../inet/src/networklayer/rsvp_te \
    -I../../inet/src/networklayer/icmpv6 \
    -I../../inet/src/transport/tcp \
    -I../../inet/src/networklayer/mpls \
    -I../../inet/src/networklayer/ted \
    -I../../inet/src/networklayer/contract \
    -I../../inet/src/util \
    -I../../inet/src/transport/contract \
    -I../../inet/src/linklayer/mfcore \
    -I../../inet/src/networklayer/ldp \
    -I../../inet/src/applications/udpapp \
    -I../../inet/src/networklayer/ipv4 \
    -I../../inet/src/base \
    -I../../inet/src/util/headerserializers \
    -I../../inet/src/networklayer/ipv6 \
    -I../../inet/src/transport/sctp \
    -I../../inet/src/world \
    -I../../inet/src/applications/pingapp \
    -I../../inet/src/linklayer/contract \
    -I../../inet/src/networklayer/arp \
    -I../../inet/src/transport/udp \
    -I. \
    -Iapplications \
    -Iapplications/dht \
    -Iapplications/tierdummy \
    -Iapplications/vodapp \
    -Icommon \
    -Icommon/cbr \
    -Ioverlay \
    -Ioverlay/cdnp2ptracker \
    -Ioverlay/cdnserver \
    -Ioverlay/meshoverlay \
    -Ioverlay/peersmesh \
    -Itier2 \
    -Itier2/vodplayer \
    -Itier2/vodstream \
    -Iunderlay \
    -Iunderlay/inetunderlay \
    -Iunderlay/simpleunderlay \
    -Iunderlay/singlehostunderlay \
    -Iunderlay/singlehostunderlay/stun

# Additional object and library files to link with
EXTRA_OBJS =

# Additional libraries (-L, -l options)
LIBS = -L../../inet/out/$(CONFIGNAME)/src  -linet
LIBS += -Wl,-rpath,`abspath ../../inet/out/$(CONFIGNAME)/src`

# Output directory
PROJECT_OUTPUT_DIR = ../out
PROJECTRELATIVE_PATH = src
O = $(PROJECT_OUTPUT_DIR)/$(CONFIGNAME)/$(PROJECTRELATIVE_PATH)

# Object files for local .cc and .msg files
OBJS = \
    $O/applications/dht/DHT.o \
    $O/applications/dht/DHTDataStorage.o \
    $O/applications/tierdummy/TierDummy.o \
    $O/applications/vodapp/BufferMap.o \
    $O/applications/vodapp/Chunk.o \
    $O/applications/vodapp/LocalVariables.o \
    $O/applications/vodapp/Scheduling.o \
    $O/applications/vodapp/VideoBuffer.o \
    $O/applications/vodapp/VideoFrame.o \
    $O/applications/vodapp/VodApp.o \
    $O/common/BaseApp.o \
    $O/common/BaseOverlay.o \
    $O/common/BaseRpc.o \
    $O/common/BinaryValue.o \
    $O/common/BootstrapList.o \
    $O/common/ChurnGenerator.o \
    $O/common/cnetcommbuffer.o \
    $O/common/CoordinateSystem.o \
    $O/common/CryptoModule.o \
    $O/common/DenaCastNoChurn.o \
    $O/common/FinisherModule.o \
    $O/common/GenericPacketParser.o \
    $O/common/GlobalNodeList.o \
    $O/common/GlobalParameters.o \
    $O/common/GlobalStatistics.o \
    $O/common/GlobalTraceManager.o \
    $O/common/IterativeLookup.o \
    $O/common/LifetimeChurn.o \
    $O/common/NeighborCache.o \
    $O/common/NoChurn.o \
    $O/common/NodeHandle.o \
    $O/common/NodeVector.o \
    $O/common/Nps.o \
    $O/common/OverlayKey.o \
    $O/common/OverSimMessage.o \
    $O/common/ParetoChurn.o \
    $O/common/PeerInfo.o \
    $O/common/ProxNodeHandle.o \
    $O/common/RandomChurn.o \
    $O/common/RecursiveLookup.o \
    $O/common/RpcListener.o \
    $O/common/SHA1.o \
    $O/common/SVivaldi.o \
    $O/common/TopologyVis.o \
    $O/common/TraceChurn.o \
    $O/common/TransportAddress.o \
    $O/common/UnderlayConfigurator.o \
    $O/common/Vector2D.o \
    $O/common/Vivaldi.o \
    $O/common/cbr/CBR-DHT.o \
    $O/common/cbr/CoordBasedRouting.o \
    $O/common/cbr/Landmark.o \
    $O/common/cbr/simplex.o \
    $O/common/cbr/yang.o \
    $O/overlay/cdnp2ptracker/CDNP2PTracker.o \
    $O/overlay/cdnserver/CdnServer.o \
    $O/overlay/meshoverlay/ChunkPackets.o \
    $O/overlay/meshoverlay/MeshOverlay.o \
    $O/overlay/peersmesh/PeersMesh.o \
    $O/tier2/vodplayer/BasePlayer.o \
    $O/tier2/vodplayer/MPEG4Player.o \
    $O/tier2/vodstream/BaseVodStream.o \
    $O/tier2/vodstream/MPEG4VodStream.o \
    $O/underlay/inetunderlay/AccessNet.o \
    $O/underlay/inetunderlay/InetInfo.o \
    $O/underlay/inetunderlay/InetUnderlayConfigurator.o \
    $O/underlay/inetunderlay/InetUnderlayConfigurator2.o \
    $O/underlay/simpleunderlay/SimpleInfo.o \
    $O/underlay/simpleunderlay/SimpleNodeEntry.o \
    $O/underlay/simpleunderlay/SimpleUDP.o \
    $O/underlay/simpleunderlay/SimpleUnderlayConfigurator.o \
    $O/underlay/singlehostunderlay/apptunoutscheduler.o \
    $O/underlay/singlehostunderlay/realtimescheduler.o \
    $O/underlay/singlehostunderlay/RealworldApp.o \
    $O/underlay/singlehostunderlay/RealworldConnector.o \
    $O/underlay/singlehostunderlay/RealworldDevice.o \
    $O/underlay/singlehostunderlay/SingleHostUnderlayConfigurator.o \
    $O/underlay/singlehostunderlay/TunOutDevice.o \
    $O/underlay/singlehostunderlay/tunoutscheduler.o \
    $O/underlay/singlehostunderlay/UdpOutDevice.o \
    $O/underlay/singlehostunderlay/udpoutscheduler.o \
    $O/underlay/singlehostunderlay/ZeroconfConnector.o \
    $O/underlay/singlehostunderlay/stun/stun.o \
    $O/underlay/singlehostunderlay/stun/udp.o \
    $O/applications/dht/DHTMessage_m.o \
    $O/applications/vodapp/VideoMessage_m.o \
    $O/common/Churn_m.o \
    $O/common/CommonMessages_m.o \
    $O/common/ExtAPIMessages_m.o \
    $O/common/GlobalTraceManager_m.o \
    $O/common/Nps_m.o \
    $O/common/OverSimMessage_m.o \
    $O/common/cbr/CBR-DHTMessage_m.o \
    $O/common/cbr/CoordMessages_m.o \
    $O/overlay/meshoverlay/MeshOverlayMessage_m.o \
    $O/overlay/meshoverlay/TrackerMessage_m.o \
    $O/overlay/peersmesh/MeshMessage_m.o \
    $O/underlay/simpleunderlay/SimpleUDPPacket_m.o

# Message files
MSGFILES = \
    applications/dht/DHTMessage.msg \
    applications/vodapp/VideoMessage.msg \
    common/Churn.msg \
    common/CommonMessages.msg \
    common/ExtAPIMessages.msg \
    common/GlobalTraceManager.msg \
    common/Nps.msg \
    common/OverSimMessage.msg \
    common/cbr/CBR-DHTMessage.msg \
    common/cbr/CoordMessages.msg \
    overlay/meshoverlay/MeshOverlayMessage.msg \
    overlay/meshoverlay/TrackerMessage.msg \
    overlay/peersmesh/MeshMessage.msg \
    underlay/simpleunderlay/SimpleUDPPacket.msg

# Other makefile variables (-K)
INET_PROJ=../../inet

#------------------------------------------------------------------------------

# Pull in OMNeT++ configuration (Makefile.inc or configuser.vc)

ifneq ("$(OMNETPP_CONFIGFILE)","")
CONFIGFILE = $(OMNETPP_CONFIGFILE)
else
ifneq ("$(OMNETPP_ROOT)","")
CONFIGFILE = $(OMNETPP_ROOT)/Makefile.inc
else
CONFIGFILE = $(shell opp_configfilepath)
endif
endif

ifeq ("$(wildcard $(CONFIGFILE))","")
$(error Config file '$(CONFIGFILE)' does not exist -- add the OMNeT++ bin directory to the path so that opp_configfilepath can be found, or set the OMNETPP_CONFIGFILE variable to point to Makefile.inc)
endif

include $(CONFIGFILE)

# Simulation kernel and user interface libraries
OMNETPP_LIB_SUBDIR = $(OMNETPP_LIB_DIR)/$(TOOLCHAIN_NAME)
OMNETPP_LIBS = -L"$(OMNETPP_LIB_SUBDIR)" -L"$(OMNETPP_LIB_DIR)" $(USERIF_LIBS) $(KERNEL_LIBS) $(SYS_LIBS)

COPTS = $(CFLAGS)  $(INCLUDE_PATH) -I$(OMNETPP_INCL_DIR)
MSGCOPTS = $(INCLUDE_PATH)

#------------------------------------------------------------------------------
# User-supplied makefile fragment(s)
# >>>
# inserted from file 'makefrag':
INCLUDE_PATH += -I$(OMNETPP_ROOT)/src/sim/parsim -I/sw/include
LIBS += -L/sw/lib

# <<<
#------------------------------------------------------------------------------

# Main target
all: $(TARGET)

$(TARGET) : $O/$(TARGET)
	$(LN) $O/$(TARGET) .

$O/$(TARGET): $(OBJS)  $(wildcard $(EXTRA_OBJS)) Makefile
	@$(MKPATH) $O
	$(CXX) $(LDFLAGS) -o $O/$(TARGET)  $(OBJS) $(EXTRA_OBJS) $(WHOLE_ARCHIVE_ON) $(LIBS) $(WHOLE_ARCHIVE_OFF) $(OMNETPP_LIBS)

.PHONY:

.SUFFIXES: .cc

$O/%.o: %.cc
	@$(MKPATH) $(dir $@)
	$(CXX) -c $(COPTS) -o $@ $<

%_m.cc %_m.h: %.msg
	$(MSGC) -s _m.cc $(MSGCOPTS) $?

msgheaders: $(MSGFILES:.msg=_m.h)

clean:
	-rm -rf $O
	-rm -f oversim oversim.exe liboversim.so liboversim.a liboversim.dll liboversim.dylib
	-rm -f ./*_m.cc ./*_m.h
	-rm -f applications/*_m.cc applications/*_m.h
	-rm -f applications/dht/*_m.cc applications/dht/*_m.h
	-rm -f applications/tierdummy/*_m.cc applications/tierdummy/*_m.h
	-rm -f applications/vodapp/*_m.cc applications/vodapp/*_m.h
	-rm -f common/*_m.cc common/*_m.h
	-rm -f common/cbr/*_m.cc common/cbr/*_m.h
	-rm -f overlay/*_m.cc overlay/*_m.h
	-rm -f overlay/cdnp2ptracker/*_m.cc overlay/cdnp2ptracker/*_m.h
	-rm -f overlay/cdnserver/*_m.cc overlay/cdnserver/*_m.h
	-rm -f overlay/meshoverlay/*_m.cc overlay/meshoverlay/*_m.h
	-rm -f overlay/peersmesh/*_m.cc overlay/peersmesh/*_m.h
	-rm -f tier2/*_m.cc tier2/*_m.h
	-rm -f tier2/vodplayer/*_m.cc tier2/vodplayer/*_m.h
	-rm -f tier2/vodstream/*_m.cc tier2/vodstream/*_m.h
	-rm -f underlay/*_m.cc underlay/*_m.h
	-rm -f underlay/inetunderlay/*_m.cc underlay/inetunderlay/*_m.h
	-rm -f underlay/simpleunderlay/*_m.cc underlay/simpleunderlay/*_m.h
	-rm -f underlay/singlehostunderlay/*_m.cc underlay/singlehostunderlay/*_m.h
	-rm -f underlay/singlehostunderlay/stun/*_m.cc underlay/singlehostunderlay/stun/*_m.h

cleanall: clean
	-rm -rf $(PROJECT_OUTPUT_DIR)

depend:
	$(MAKEDEPEND) $(INCLUDE_PATH) -f Makefile -P\$$O/ -- $(MSG_CC_FILES)  ./*.cc applications/*.cc applications/dht/*.cc applications/tierdummy/*.cc applications/vodapp/*.cc common/*.cc common/cbr/*.cc overlay/*.cc overlay/cdnp2ptracker/*.cc overlay/cdnserver/*.cc overlay/meshoverlay/*.cc overlay/peersmesh/*.cc tier2/*.cc tier2/vodplayer/*.cc tier2/vodstream/*.cc underlay/*.cc underlay/inetunderlay/*.cc underlay/simpleunderlay/*.cc underlay/singlehostunderlay/*.cc underlay/singlehostunderlay/stun/*.cc

# DO NOT DELETE THIS LINE -- make depend depends on it.
$O/applications/dht/DHT.o: applications/dht/DHT.cc \
	common/SHA1.h \
	$(INET_PROJ)/src/base/INETDefs.h \
	$(INET_PROJ)/src/base/INotifiable.h \
	applications/dht/DHTDataStorage.h \
	common/BinaryValue.h \
	common/TopologyVis.h \
	common/ProxNodeHandle.h \
	common/RpcMacros.h \
	$(INET_PROJ)/src/networklayer/contract/IPvXAddress.h \
	common/RpcState.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddress.h \
	common/NodeHandle.h \
	common/AbstractLookup.h \
	common/BaseApp.h \
	common/IterativeLookupConfiguration.h \
	common/TransportAddress.h \
	common/NodeVector.h \
	common/BaseOverlay.h \
	common/RecursiveLookup.h \
	applications/dht/DHTMessage_m.h \
	common/BaseRpc.h \
	common/OverSimMessage.h \
	common/GlobalStatistics.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddressResolver.h \
	applications/dht/DHT.h \
	common/Comparator.h \
	$(INET_PROJ)/src/networklayer/contract/IPv6Address.h \
	common/CommonMessages_m.h \
	common/oversim_mapset.h \
	common/OverSimMessage_m.h \
	common/RpcListener.h \
	common/Vector2D.h \
	common/OverlayKey.h \
	common/InitStages.h
$O/applications/dht/DHTDataStorage.o: applications/dht/DHTDataStorage.cc \
	common/hashWatch.h \
	common/TransportAddress.h \
	common/NodeVector.h \
	$(INET_PROJ)/src/base/INETDefs.h \
	common/OverSimMessage.h \
	applications/dht/DHTDataStorage.h \
	common/BinaryValue.h \
	common/ProxNodeHandle.h \
	$(INET_PROJ)/src/networklayer/contract/IPvXAddress.h \
	common/Comparator.h \
	common/CommonMessages_m.h \
	$(INET_PROJ)/src/networklayer/contract/IPv6Address.h \
	common/oversim_mapset.h \
	common/NodeHandle.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddress.h \
	common/OverSimMessage_m.h \
	common/Vector2D.h \
	common/OverlayKey.h \
	common/InitStages.h
$O/applications/dht/DHTMessage_m.o: applications/dht/DHTMessage_m.cc \
	common/TransportAddress.h \
	$(INET_PROJ)/src/base/INETDefs.h \
	applications/dht/DHTMessage_m.h \
	common/OverSimMessage.h \
	common/BinaryValue.h \
	$(INET_PROJ)/src/networklayer/contract/IPvXAddress.h \
	common/CommonMessages_m.h \
	$(INET_PROJ)/src/networklayer/contract/IPv6Address.h \
	common/NodeHandle.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddress.h \
	common/oversim_mapset.h \
	common/OverSimMessage_m.h \
	common/Vector2D.h \
	common/OverlayKey.h
$O/applications/tierdummy/TierDummy.o: applications/tierdummy/TierDummy.cc \
	$(INET_PROJ)/src/base/INotifiable.h \
	$(INET_PROJ)/src/base/INETDefs.h \
	common/BinaryValue.h \
	common/TopologyVis.h \
	common/ProxNodeHandle.h \
	common/RpcMacros.h \
	$(INET_PROJ)/src/networklayer/contract/IPvXAddress.h \
	common/RpcState.h \
	common/NodeHandle.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddress.h \
	common/AbstractLookup.h \
	common/BaseApp.h \
	common/IterativeLookupConfiguration.h \
	common/TransportAddress.h \
	common/NodeVector.h \
	common/BaseOverlay.h \
	common/RecursiveLookup.h \
	common/BaseRpc.h \
	applications/tierdummy/TierDummy.h \
	common/OverSimMessage.h \
	common/Comparator.h \
	common/CommonMessages_m.h \
	$(INET_PROJ)/src/networklayer/contract/IPv6Address.h \
	common/oversim_mapset.h \
	common/OverSimMessage_m.h \
	common/RpcListener.h \
	common/Vector2D.h \
	common/InitStages.h \
	common/OverlayKey.h
$O/applications/vodapp/VideoFrame.o: applications/vodapp/VideoFrame.cc \
	applications/vodapp/VideoFrame.h
$O/applications/vodapp/LocalVariables.o: applications/vodapp/LocalVariables.cc \
	common/TransportAddress.h \
	applications/vodapp/VideoFrame.h \
	$(INET_PROJ)/src/networklayer/contract/IPvXAddress.h \
	$(INET_PROJ)/src/base/INETDefs.h \
	applications/vodapp/BufferMap.h \
	$(INET_PROJ)/src/networklayer/contract/IPv6Address.h \
	applications/vodapp/LocalVariables.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddress.h \
	common/oversim_mapset.h \
	applications/vodapp/VideoBuffer.h \
	applications/vodapp/Chunk.h
$O/applications/vodapp/VodApp.o: applications/vodapp/VodApp.cc \
	$(INET_PROJ)/src/base/INETDefs.h \
	$(INET_PROJ)/src/base/INotifiable.h \
	applications/vodapp/VodApp.h \
	applications/vodapp/Chunk.h \
	common/HashFunc.h \
	common/PeerInfo.h \
	common/ChurnGenerator.h \
	common/BinaryValue.h \
	common/TopologyVis.h \
	common/ProxNodeHandle.h \
	common/RpcMacros.h \
	applications/vodapp/VideoFrame.h \
	$(INET_PROJ)/src/networklayer/contract/IPvXAddress.h \
	common/GlobalNodeList.h \
	tier2/vodstream/BaseVodStream.h \
	applications/vodapp/LocalVariables.h \
	common/RpcState.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddress.h \
	common/NodeHandle.h \
	applications/vodapp/VideoBuffer.h \
	overlay/peersmesh/MeshMessage_m.h \
	common/AbstractLookup.h \
	common/BaseApp.h \
	common/IterativeLookupConfiguration.h \
	common/TransportAddress.h \
	common/NodeVector.h \
	common/BaseOverlay.h \
	common/RecursiveLookup.h \
	common/BaseRpc.h \
	common/oversim_byteswap.h \
	common/OverSimMessage.h \
	common/GlobalStatistics.h \
	applications/vodapp/VideoMessage_m.h \
	common/Comparator.h \
	applications/vodapp/BufferMap.h \
	$(INET_PROJ)/src/networklayer/contract/IPv6Address.h \
	common/CommonMessages_m.h \
	common/oversim_mapset.h \
	common/OverSimMessage_m.h \
	common/RpcListener.h \
	common/Vector2D.h \
	common/OverlayKey.h \
	common/InitStages.h
$O/applications/vodapp/Scheduling.o: applications/vodapp/Scheduling.cc \
	$(INET_PROJ)/src/base/INotifiable.h \
	$(INET_PROJ)/src/base/INETDefs.h \
	applications/vodapp/VodApp.h \
	applications/vodapp/Chunk.h \
	common/HashFunc.h \
	common/BinaryValue.h \
	common/ChurnGenerator.h \
	common/TopologyVis.h \
	common/ProxNodeHandle.h \
	common/RpcMacros.h \
	applications/vodapp/VideoFrame.h \
	common/GlobalNodeList.h \
	$(INET_PROJ)/src/networklayer/contract/IPvXAddress.h \
	tier2/vodstream/BaseVodStream.h \
	applications/vodapp/LocalVariables.h \
	common/RpcState.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddress.h \
	common/NodeHandle.h \
	applications/vodapp/VideoBuffer.h \
	overlay/peersmesh/MeshMessage_m.h \
	common/AbstractLookup.h \
	common/BaseApp.h \
	common/IterativeLookupConfiguration.h \
	common/TransportAddress.h \
	common/NodeVector.h \
	common/BaseOverlay.h \
	common/RecursiveLookup.h \
	common/BaseRpc.h \
	common/oversim_byteswap.h \
	common/OverSimMessage.h \
	common/GlobalStatistics.h \
	applications/vodapp/VideoMessage_m.h \
	common/Comparator.h \
	applications/vodapp/BufferMap.h \
	common/CommonMessages_m.h \
	$(INET_PROJ)/src/networklayer/contract/IPv6Address.h \
	common/oversim_mapset.h \
	common/RpcListener.h \
	common/OverSimMessage_m.h \
	common/Vector2D.h \
	common/InitStages.h \
	common/OverlayKey.h
$O/applications/vodapp/VideoBuffer.o: applications/vodapp/VideoBuffer.cc \
	common/TransportAddress.h \
	applications/vodapp/VideoFrame.h \
	$(INET_PROJ)/src/networklayer/contract/IPvXAddress.h \
	$(INET_PROJ)/src/base/INETDefs.h \
	applications/vodapp/BufferMap.h \
	$(INET_PROJ)/src/networklayer/contract/IPv6Address.h \
	common/oversim_mapset.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddress.h \
	applications/vodapp/Chunk.h \
	applications/vodapp/VideoBuffer.h
$O/applications/vodapp/VideoMessage_m.o: applications/vodapp/VideoMessage_m.cc \
	common/TransportAddress.h \
	applications/vodapp/VideoMessage_m.h \
	applications/vodapp/VideoFrame.h \
	$(INET_PROJ)/src/networklayer/contract/IPvXAddress.h \
	$(INET_PROJ)/src/base/INETDefs.h \
	applications/vodapp/BufferMap.h \
	$(INET_PROJ)/src/networklayer/contract/IPv6Address.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddress.h \
	common/oversim_mapset.h \
	applications/vodapp/Chunk.h
$O/applications/vodapp/BufferMap.o: applications/vodapp/BufferMap.cc \
	common/TransportAddress.h \
	$(INET_PROJ)/src/networklayer/contract/IPvXAddress.h \
	$(INET_PROJ)/src/base/INETDefs.h \
	applications/vodapp/BufferMap.h \
	$(INET_PROJ)/src/networklayer/contract/IPv6Address.h \
	common/oversim_mapset.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddress.h
$O/applications/vodapp/Chunk.o: applications/vodapp/Chunk.cc \
	applications/vodapp/VideoFrame.h \
	applications/vodapp/Chunk.h
$O/common/DenaCastNoChurn.o: common/DenaCastNoChurn.cc \
	common/ChurnGenerator.h \
	common/UnderlayConfigurator.h \
	common/TransportAddress.h \
	common/DenaCastNoChurn.h \
	$(INET_PROJ)/src/networklayer/contract/IPvXAddress.h \
	$(INET_PROJ)/src/base/INETDefs.h \
	$(INET_PROJ)/src/networklayer/contract/IPv6Address.h \
	common/oversim_mapset.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddress.h \
	common/InitStages.h
$O/common/CoordinateSystem.o: common/CoordinateSystem.cc \
	common/ProxNodeHandle.h \
	common/TransportAddress.h \
	$(INET_PROJ)/src/networklayer/contract/IPvXAddress.h \
	$(INET_PROJ)/src/base/INETDefs.h \
	$(INET_PROJ)/src/networklayer/contract/IPv6Address.h \
	common/CoordinateSystem.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddress.h \
	common/oversim_mapset.h \
	common/NodeHandle.h \
	common/OverlayKey.h
$O/common/TopologyVis.o: common/TopologyVis.cc \
	common/TransportAddress.h \
	$(INET_PROJ)/src/base/INETDefs.h \
	common/GlobalNodeListAccess.h \
	common/oversim_byteswap.h \
	common/HashFunc.h \
	common/PeerInfo.h \
	common/BinaryValue.h \
	common/ChurnGenerator.h \
	common/TopologyVis.h \
	common/GlobalNodeList.h \
	$(INET_PROJ)/src/networklayer/contract/IPvXAddress.h \
	$(INET_PROJ)/src/networklayer/contract/IPv6Address.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddress.h \
	common/NodeHandle.h \
	common/oversim_mapset.h \
	common/OverlayKey.h \
	common/InitStages.h
$O/common/NodeVector.o: common/NodeVector.cc \
	common/ProxNodeHandle.h \
	common/TransportAddress.h \
	common/NodeVector.h \
	$(INET_PROJ)/src/networklayer/contract/IPvXAddress.h \
	$(INET_PROJ)/src/base/INETDefs.h \
	common/Comparator.h \
	$(INET_PROJ)/src/networklayer/contract/IPv6Address.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddress.h \
	common/oversim_mapset.h \
	common/NodeHandle.h \
	common/OverlayKey.h
$O/common/ChurnGenerator.o: common/ChurnGenerator.cc \
	common/ChurnGenerator.h \
	common/UnderlayConfigurator.h \
	common/UnderlayConfiguratorAccess.h \
	common/InitStages.h
$O/common/Churn_m.o: common/Churn_m.cc \
	common/TransportAddress.h \
	$(INET_PROJ)/src/networklayer/contract/IPvXAddress.h \
	common/Churn_m.h \
	$(INET_PROJ)/src/base/INETDefs.h \
	$(INET_PROJ)/src/networklayer/contract/IPv6Address.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddress.h \
	common/oversim_mapset.h \
	common/NodeHandle.h \
	common/OverlayKey.h
$O/common/CommonMessages_m.o: common/CommonMessages_m.cc \
	common/TransportAddress.h \
	$(INET_PROJ)/src/base/INETDefs.h \
	common/OverSimMessage.h \
	common/BinaryValue.h \
	$(INET_PROJ)/src/networklayer/contract/IPvXAddress.h \
	common/CommonMessages_m.h \
	$(INET_PROJ)/src/networklayer/contract/IPv6Address.h \
	common/NodeHandle.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddress.h \
	common/oversim_mapset.h \
	common/OverSimMessage_m.h \
	common/Vector2D.h \
	common/OverlayKey.h
$O/common/Vivaldi.o: common/Vivaldi.cc \
	$(INET_PROJ)/src/base/INETDefs.h \
	$(INET_PROJ)/src/base/INotifiable.h \
	common/GlobalStatisticsAccess.h \
	common/HashFunc.h \
	common/BinaryValue.h \
	common/TopologyVis.h \
	common/ProxNodeHandle.h \
	common/RpcMacros.h \
	$(INET_PROJ)/src/networklayer/contract/IPvXAddress.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddress.h \
	common/RpcState.h \
	common/NodeHandle.h \
	common/cbr/simplex.h \
	common/AbstractLookup.h \
	common/BaseApp.h \
	common/NeighborCache.h \
	common/IterativeLookupConfiguration.h \
	common/TransportAddress.h \
	common/NodeVector.h \
	common/BaseOverlay.h \
	common/RecursiveLookup.h \
	common/BaseRpc.h \
	common/oversim_byteswap.h \
	common/OverSimMessage.h \
	common/Nps.h \
	common/cbr/yang.h \
	common/Vivaldi.h \
	common/GlobalStatistics.h \
	common/SVivaldi.h \
	common/Comparator.h \
	$(INET_PROJ)/src/networklayer/contract/IPv6Address.h \
	common/CoordinateSystem.h \
	common/CommonMessages_m.h \
	common/Nps_m.h \
	common/oversim_mapset.h \
	common/OverSimMessage_m.h \
	common/RpcListener.h \
	common/Vector2D.h \
	common/InitStages.h \
	common/OverlayKey.h
$O/common/Nps_m.o: common/Nps_m.cc \
	common/TransportAddress.h \
	$(INET_PROJ)/src/base/INETDefs.h \
	common/OverSimMessage.h \
	common/BinaryValue.h \
	$(INET_PROJ)/src/networklayer/contract/IPvXAddress.h \
	common/CommonMessages_m.h \
	$(INET_PROJ)/src/networklayer/contract/IPv6Address.h \
	common/Nps_m.h \
	common/NodeHandle.h \
	common/oversim_mapset.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddress.h \
	common/OverSimMessage_m.h \
	common/Vector2D.h \
	common/OverlayKey.h
$O/common/GlobalTraceManager.o: common/GlobalTraceManager.cc \
	common/GlobalTraceManager_m.h \
	common/TransportAddress.h \
	$(INET_PROJ)/src/base/INETDefs.h \
	common/GlobalTraceManager.h \
	common/GlobalNodeListAccess.h \
	common/oversim_byteswap.h \
	common/HashFunc.h \
	common/BinaryValue.h \
	common/TraceChurn.h \
	common/ChurnGenerator.h \
	common/UnderlayConfigurator.h \
	$(INET_PROJ)/src/networklayer/contract/IPvXAddress.h \
	common/GlobalNodeList.h \
	common/UnderlayConfiguratorAccess.h \
	$(INET_PROJ)/src/networklayer/contract/IPv6Address.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddress.h \
	common/NodeHandle.h \
	common/oversim_mapset.h \
	common/OverlayKey.h \
	common/InitStages.h
$O/common/RpcListener.o: common/RpcListener.cc \
	common/TransportAddress.h \
	$(INET_PROJ)/src/base/INETDefs.h \
	common/OverSimMessage.h \
	common/BinaryValue.h \
	$(INET_PROJ)/src/networklayer/contract/IPvXAddress.h \
	common/CommonMessages_m.h \
	$(INET_PROJ)/src/networklayer/contract/IPv6Address.h \
	common/oversim_mapset.h \
	common/RpcState.h \
	common/NodeHandle.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddress.h \
	common/OverSimMessage_m.h \
	common/RpcListener.h \
	common/Vector2D.h \
	common/OverlayKey.h
$O/common/NodeHandle.o: common/NodeHandle.cc \
	common/TransportAddress.h \
	$(INET_PROJ)/src/networklayer/contract/IPvXAddress.h \
	$(INET_PROJ)/src/base/INETDefs.h \
	$(INET_PROJ)/src/networklayer/contract/IPv6Address.h \
	common/oversim_mapset.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddress.h \
	common/NodeHandle.h \
	common/OverlayKey.h
$O/common/GlobalNodeList.o: common/GlobalNodeList.cc \
	$(INET_PROJ)/src/base/INETDefs.h \
	$(INET_PROJ)/src/base/INotifiable.h \
	common/GlobalStatisticsAccess.h \
	common/HashFunc.h \
	common/PeerInfo.h \
	common/ChurnGenerator.h \
	common/BinaryValue.h \
	common/ProxNodeHandle.h \
	common/TopologyVis.h \
	common/RpcMacros.h \
	$(INET_PROJ)/src/networklayer/contract/IPvXAddress.h \
	common/GlobalNodeList.h \
	common/BootstrapNodeHandle.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddress.h \
	$(INET_PROJ)/src/base/ModuleAccess.h \
	common/RpcState.h \
	$(INET_PROJ)/src/base/NotificationBoard.h \
	common/NodeHandle.h \
	common/AbstractLookup.h \
	common/BaseApp.h \
	common/IterativeLookupConfiguration.h \
	common/TransportAddress.h \
	common/hashWatch.h \
	common/NodeVector.h \
	common/BaseOverlay.h \
	common/BootstrapList.h \
	common/RecursiveLookup.h \
	$(INET_PROJ)/src/base/NotifierConsts.h \
	common/BaseRpc.h \
	common/oversim_byteswap.h \
	common/OverSimMessage.h \
	common/GlobalStatistics.h \
	common/Comparator.h \
	$(INET_PROJ)/src/networklayer/contract/IPv6Address.h \
	common/CommonMessages_m.h \
	common/oversim_mapset.h \
	common/OverSimMessage_m.h \
	common/RpcListener.h \
	common/Vector2D.h \
	common/OverlayKey.h \
	common/InitStages.h
$O/common/LifetimeChurn.o: common/LifetimeChurn.cc \
	common/TransportAddress.h \
	$(INET_PROJ)/src/base/INETDefs.h \
	common/GlobalStatisticsAccess.h \
	common/ChurnGenerator.h \
	common/GlobalStatistics.h \
	common/UnderlayConfigurator.h \
	common/Churn_m.h \
	$(INET_PROJ)/src/networklayer/contract/IPvXAddress.h \
	common/LifetimeChurn.h \
	$(INET_PROJ)/src/networklayer/contract/IPv6Address.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddress.h \
	common/oversim_mapset.h \
	common/NodeHandle.h \
	common/OverlayKey.h \
	common/InitStages.h
$O/common/OverlayKey.o: common/OverlayKey.cc \
	common/BinaryValue.h \
	common/ProxNodeHandle.h \
	common/TransportAddress.h \
	common/SHA1.h \
	$(INET_PROJ)/src/networklayer/contract/IPvXAddress.h \
	$(INET_PROJ)/src/base/INETDefs.h \
	common/Comparator.h \
	$(INET_PROJ)/src/networklayer/contract/IPv6Address.h \
	common/NodeHandle.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddress.h \
	common/oversim_mapset.h \
	common/OverlayKey.h
$O/common/IterativeLookup.o: common/IterativeLookup.cc \
	$(INET_PROJ)/src/base/INotifiable.h \
	$(INET_PROJ)/src/base/INETDefs.h \
	common/BinaryValue.h \
	common/TopologyVis.h \
	common/ProxNodeHandle.h \
	common/RpcMacros.h \
	$(INET_PROJ)/src/networklayer/contract/IPvXAddress.h \
	common/RpcState.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddress.h \
	common/NodeHandle.h \
	common/AbstractLookup.h \
	common/IterativeLookupConfiguration.h \
	common/TransportAddress.h \
	common/NodeVector.h \
	common/IterativeLookup.h \
	common/BaseOverlay.h \
	common/RecursiveLookup.h \
	common/BaseRpc.h \
	common/OverSimMessage.h \
	common/GlobalStatistics.h \
	common/UnderlayConfigurator.h \
	common/Comparator.h \
	common/CommonMessages_m.h \
	$(INET_PROJ)/src/networklayer/contract/IPv6Address.h \
	common/oversim_mapset.h \
	common/OverSimMessage_m.h \
	common/RpcListener.h \
	common/LookupListener.h \
	common/Vector2D.h \
	common/InitStages.h \
	common/OverlayKey.h
$O/common/TraceChurn.o: common/TraceChurn.cc \
	common/TransportAddress.h \
	$(INET_PROJ)/src/base/INETDefs.h \
	common/GlobalNodeListAccess.h \
	common/oversim_byteswap.h \
	common/HashFunc.h \
	common/PeerInfo.h \
	common/TraceChurn.h \
	common/ChurnGenerator.h \
	common/BinaryValue.h \
	common/UnderlayConfigurator.h \
	common/GlobalNodeList.h \
	$(INET_PROJ)/src/networklayer/contract/IPvXAddress.h \
	$(INET_PROJ)/src/networklayer/contract/IPv6Address.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddress.h \
	common/oversim_mapset.h \
	common/NodeHandle.h \
	common/OverlayKey.h \
	common/InitStages.h
$O/common/UnderlayConfigurator.o: common/UnderlayConfigurator.cc \
	common/TransportAddress.h \
	$(INET_PROJ)/src/base/INETDefs.h \
	common/ChurnGeneratorAccess.h \
	common/GlobalStatisticsAccess.h \
	common/GlobalNodeListAccess.h \
	common/oversim_byteswap.h \
	common/HashFunc.h \
	common/BinaryValue.h \
	common/ChurnGenerator.h \
	common/GlobalStatistics.h \
	common/UnderlayConfigurator.h \
	$(INET_PROJ)/src/networklayer/contract/IPvXAddress.h \
	common/GlobalNodeList.h \
	$(INET_PROJ)/src/networklayer/contract/IPv6Address.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddress.h \
	common/NodeHandle.h \
	common/oversim_mapset.h \
	$(INET_PROJ)/src/base/ModuleAccess.h \
	common/OverlayKey.h \
	common/InitStages.h
$O/common/GlobalStatistics.o: common/GlobalStatistics.cc \
	common/GlobalStatistics.h
$O/common/Nps.o: common/Nps.cc \
	common/GlobalNodeListAccess.h \
	common/PeerInfo.h \
	common/cbr/Landmark.h \
	common/cbr/CoordBasedRoutingAccess.h \
	common/RpcMacros.h \
	common/GlobalNodeList.h \
	underlay/simpleunderlay/SimpleUDPPacket_m.h \
	common/RpcState.h \
	common/cbr/simplex.h \
	common/BaseApp.h \
	common/IterativeLookupConfiguration.h \
	common/oversim_byteswap.h \
	common/OverSimMessage.h \
	common/Nps.h \
	common/GlobalStatistics.h \
	common/UnderlayConfigurator.h \
	common/cbr/CoordBasedRouting.h \
	underlay/simpleunderlay/SimpleNodeEntry.h \
	common/CommonMessages_m.h \
	common/OverSimMessage_m.h \
	common/RpcListener.h \
	common/Vector2D.h \
	common/OverlayKey.h \
	common/InitStages.h \
	$(INET_PROJ)/src/base/INotifiable.h \
	$(INET_PROJ)/src/base/INETDefs.h \
	common/GlobalStatisticsAccess.h \
	common/HashFunc.h \
	common/BinaryValue.h \
	common/ChurnGenerator.h \
	common/ProxNodeHandle.h \
	common/TopologyVis.h \
	$(INET_PROJ)/src/networklayer/contract/IPvXAddress.h \
	common/NodeHandle.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddress.h \
	common/AbstractLookup.h \
	common/NeighborCache.h \
	common/TransportAddress.h \
	common/NodeVector.h \
	common/BaseOverlay.h \
	underlay/simpleunderlay/SimpleInfo.h \
	common/RecursiveLookup.h \
	common/BaseRpc.h \
	common/cbr/yang.h \
	common/Vivaldi.h \
	common/SVivaldi.h \
	common/Comparator.h \
	common/CoordinateSystem.h \
	$(INET_PROJ)/src/networklayer/contract/IPv6Address.h \
	common/Nps_m.h \
	common/oversim_mapset.h
$O/common/OverSimMessage_m.o: common/OverSimMessage_m.cc \
	common/OverSimMessage_m.h
$O/common/SVivaldi.o: common/SVivaldi.cc \
	$(INET_PROJ)/src/base/INotifiable.h \
	$(INET_PROJ)/src/base/INETDefs.h \
	common/GlobalStatisticsAccess.h \
	common/HashFunc.h \
	common/BinaryValue.h \
	common/TopologyVis.h \
	common/ProxNodeHandle.h \
	common/RpcMacros.h \
	$(INET_PROJ)/src/networklayer/contract/IPvXAddress.h \
	common/RpcState.h \
	common/NodeHandle.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddress.h \
	common/cbr/simplex.h \
	common/AbstractLookup.h \
	common/BaseApp.h \
	common/NeighborCache.h \
	common/IterativeLookupConfiguration.h \
	common/TransportAddress.h \
	common/NodeVector.h \
	common/BaseOverlay.h \
	common/RecursiveLookup.h \
	common/BaseRpc.h \
	common/oversim_byteswap.h \
	common/OverSimMessage.h \
	common/Nps.h \
	common/cbr/yang.h \
	common/Vivaldi.h \
	common/GlobalStatistics.h \
	common/SVivaldi.h \
	common/Comparator.h \
	common/CoordinateSystem.h \
	$(INET_PROJ)/src/networklayer/contract/IPv6Address.h \
	common/CommonMessages_m.h \
	common/Nps_m.h \
	common/oversim_mapset.h \
	common/OverSimMessage_m.h \
	common/RpcListener.h \
	common/Vector2D.h \
	common/InitStages.h \
	common/OverlayKey.h
$O/common/BaseRpc.o: common/BaseRpc.cc \
	$(INET_PROJ)/src/base/INETDefs.h \
	$(INET_PROJ)/src/base/INotifiable.h \
	common/GlobalStatisticsAccess.h \
	common/cnetcommbuffer.h \
	common/HashFunc.h \
	common/BinaryValue.h \
	common/ProxNodeHandle.h \
	common/TopologyVis.h \
	common/RpcMacros.h \
	$(INET_PROJ)/src/networklayer/contract/IPvXAddress.h \
	common/UnderlayConfiguratorAccess.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddress.h \
	common/RpcState.h \
	$(INET_PROJ)/src/base/NotificationBoard.h \
	common/NodeHandle.h \
	$(INET_PROJ)/src/base/ModuleAccess.h \
	common/cbr/simplex.h \
	common/AbstractLookup.h \
	common/BaseApp.h \
	common/NeighborCache.h \
	common/OverlayAccess.h \
	common/IterativeLookupConfiguration.h \
	common/TransportAddress.h \
	common/NodeVector.h \
	common/BaseOverlay.h \
	common/RecursiveLookup.h \
	common/BaseRpc.h \
	$(INET_PROJ)/src/base/NotifierConsts.h \
	common/oversim_byteswap.h \
	common/OverSimMessage.h \
	common/Nps.h \
	common/cbr/yang.h \
	common/Vivaldi.h \
	common/GlobalStatistics.h \
	common/UnderlayConfigurator.h \
	common/SVivaldi.h \
	common/CryptoModule.h \
	common/Comparator.h \
	$(INET_PROJ)/src/networklayer/contract/IPv6Address.h \
	common/CommonMessages_m.h \
	common/CoordinateSystem.h \
	common/Nps_m.h \
	common/oversim_mapset.h \
	common/OverSimMessage_m.h \
	common/RpcListener.h \
	common/Vector2D.h \
	common/OverlayKey.h \
	common/InitStages.h
$O/common/FinisherModule.o: common/FinisherModule.cc \
	common/FinisherModule.h \
	common/GlobalStatistics.h \
	common/GlobalStatisticsAccess.h
$O/common/OverSimMessage.o: common/OverSimMessage.cc \
	common/cnetcommbuffer.h \
	common/OverSimMessage_m.h \
	common/OverSimMessage.h
$O/common/TransportAddress.o: common/TransportAddress.cc \
	common/TransportAddress.h \
	$(INET_PROJ)/src/networklayer/contract/IPvXAddress.h \
	$(INET_PROJ)/src/base/INETDefs.h \
	$(INET_PROJ)/src/networklayer/contract/IPv6Address.h \
	common/oversim_mapset.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddress.h
$O/common/Vector2D.o: common/Vector2D.cc \
	common/Vector2D.h
$O/common/BaseApp.o: common/BaseApp.cc \
	$(INET_PROJ)/src/transport/contract/UDPControlInfo_m.h \
	$(INET_PROJ)/src/base/INotifiable.h \
	$(INET_PROJ)/src/base/INETDefs.h \
	common/GlobalStatisticsAccess.h \
	common/GlobalNodeListAccess.h \
	common/HashFunc.h \
	common/ChurnGenerator.h \
	common/BinaryValue.h \
	common/ProxNodeHandle.h \
	common/TopologyVis.h \
	common/RpcMacros.h \
	common/GlobalNodeList.h \
	$(INET_PROJ)/src/networklayer/contract/IPvXAddress.h \
	common/UnderlayConfiguratorAccess.h \
	common/RpcState.h \
	common/NodeHandle.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddress.h \
	$(INET_PROJ)/src/base/NotificationBoard.h \
	$(INET_PROJ)/src/base/ModuleAccess.h \
	common/AbstractLookup.h \
	common/BaseApp.h \
	common/OverlayAccess.h \
	common/IterativeLookupConfiguration.h \
	common/TransportAddress.h \
	common/NodeVector.h \
	common/BaseOverlay.h \
	common/RecursiveLookup.h \
	common/BaseRpc.h \
	$(INET_PROJ)/src/base/NotifierConsts.h \
	common/oversim_byteswap.h \
	common/OverSimMessage.h \
	$(INET_PROJ)/src/transport/contract/UDPSocket.h \
	common/GlobalStatistics.h \
	common/UnderlayConfigurator.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddressResolver.h \
	common/Comparator.h \
	common/CommonMessages_m.h \
	$(INET_PROJ)/src/networklayer/contract/IPv6Address.h \
	common/oversim_mapset.h \
	common/OverSimMessage_m.h \
	common/RpcListener.h \
	$(INET_PROJ)/src/applications/udpapp/UDPAppBase.h \
	common/Vector2D.h \
	common/OverlayKey.h \
	common/InitStages.h
$O/common/SHA1.o: common/SHA1.cc \
	common/SHA1.h
$O/common/ParetoChurn.o: common/ParetoChurn.cc \
	common/TransportAddress.h \
	$(INET_PROJ)/src/base/INETDefs.h \
	common/GlobalStatisticsAccess.h \
	common/ChurnGenerator.h \
	common/GlobalStatistics.h \
	common/UnderlayConfigurator.h \
	common/Churn_m.h \
	$(INET_PROJ)/src/networklayer/contract/IPvXAddress.h \
	$(INET_PROJ)/src/networklayer/contract/IPv6Address.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddress.h \
	common/NodeHandle.h \
	common/oversim_mapset.h \
	common/ParetoChurn.h \
	common/InitStages.h \
	common/OverlayKey.h
$O/common/GenericPacketParser.o: common/GenericPacketParser.cc \
	common/cnetcommbuffer.h \
	common/GenericPacketParser.h \
	common/PacketParser.h
$O/common/CryptoModule.o: common/CryptoModule.cc \
	$(INET_PROJ)/src/base/INETDefs.h \
	$(INET_PROJ)/src/base/INotifiable.h \
	common/GlobalStatisticsAccess.h \
	common/cnetcommbuffer.h \
	common/BinaryValue.h \
	common/ProxNodeHandle.h \
	common/TopologyVis.h \
	common/RpcMacros.h \
	$(INET_PROJ)/src/networklayer/contract/IPvXAddress.h \
	common/RpcState.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddress.h \
	common/NodeHandle.h \
	$(INET_PROJ)/src/base/NotificationBoard.h \
	$(INET_PROJ)/src/base/ModuleAccess.h \
	common/AbstractLookup.h \
	common/OverlayAccess.h \
	common/IterativeLookupConfiguration.h \
	common/TransportAddress.h \
	common/NodeVector.h \
	common/BaseOverlay.h \
	common/RecursiveLookup.h \
	$(INET_PROJ)/src/base/NotifierConsts.h \
	common/BaseRpc.h \
	common/OverSimMessage.h \
	common/GlobalStatistics.h \
	common/CryptoModule.h \
	common/Comparator.h \
	$(INET_PROJ)/src/networklayer/contract/IPv6Address.h \
	common/CommonMessages_m.h \
	common/oversim_mapset.h \
	common/OverSimMessage_m.h \
	common/RpcListener.h \
	common/Vector2D.h \
	common/OverlayKey.h \
	common/InitStages.h
$O/common/GlobalParameters.o: common/GlobalParameters.cc \
	common/GlobalParameters.h \
	common/OverlayKey.h
$O/common/BootstrapList.o: common/BootstrapList.cc \
	underlay/singlehostunderlay/ZeroconfConnector.h \
	$(INET_PROJ)/src/base/INETDefs.h \
	$(INET_PROJ)/src/base/INotifiable.h \
	common/HashFunc.h \
	common/BinaryValue.h \
	common/ChurnGenerator.h \
	common/ProxNodeHandle.h \
	common/TopologyVis.h \
	common/RpcMacros.h \
	common/GlobalNodeList.h \
	$(INET_PROJ)/src/networklayer/contract/IPvXAddress.h \
	common/UnderlayConfiguratorAccess.h \
	common/BootstrapNodeHandle.h \
	common/RpcState.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddress.h \
	common/NodeHandle.h \
	common/AbstractLookup.h \
	common/BaseApp.h \
	common/IterativeLookupConfiguration.h \
	common/hashWatch.h \
	common/TransportAddress.h \
	common/NodeVector.h \
	common/BaseOverlay.h \
	common/BootstrapList.h \
	common/RecursiveLookup.h \
	common/BaseRpc.h \
	common/oversim_byteswap.h \
	common/OverSimMessage.h \
	common/UnderlayConfigurator.h \
	common/Comparator.h \
	$(INET_PROJ)/src/networklayer/contract/IPv6Address.h \
	common/CommonMessages_m.h \
	common/oversim_mapset.h \
	common/OverSimMessage_m.h \
	common/RpcListener.h \
	common/Vector2D.h \
	common/OverlayKey.h \
	common/InitStages.h
$O/common/BinaryValue.o: common/BinaryValue.cc \
	common/BinaryValue.h
$O/common/NoChurn.o: common/NoChurn.cc \
	common/ChurnGenerator.h \
	common/UnderlayConfigurator.h \
	common/TransportAddress.h \
	$(INET_PROJ)/src/networklayer/contract/IPvXAddress.h \
	$(INET_PROJ)/src/base/INETDefs.h \
	common/NoChurn.h \
	$(INET_PROJ)/src/networklayer/contract/IPv6Address.h \
	common/oversim_mapset.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddress.h \
	common/InitStages.h
$O/common/PeerInfo.o: common/PeerInfo.cc \
	common/PeerInfo.h
$O/common/NeighborCache.o: common/NeighborCache.cc \
	$(INET_PROJ)/src/base/INETDefs.h \
	$(INET_PROJ)/src/base/INotifiable.h \
	common/GlobalStatisticsAccess.h \
	common/GlobalNodeListAccess.h \
	common/HashFunc.h \
	common/PeerInfo.h \
	common/ChurnGenerator.h \
	common/BinaryValue.h \
	common/ProxNodeHandle.h \
	common/TopologyVis.h \
	common/RpcMacros.h \
	$(INET_PROJ)/src/networklayer/contract/IPvXAddress.h \
	common/GlobalNodeList.h \
	common/RpcState.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddress.h \
	common/cbr/CoordMessages_m.h \
	common/NodeHandle.h \
	common/cbr/simplex.h \
	common/AbstractLookup.h \
	common/BaseApp.h \
	common/NeighborCache.h \
	common/IterativeLookupConfiguration.h \
	common/hashWatch.h \
	common/TransportAddress.h \
	common/NodeVector.h \
	common/BaseOverlay.h \
	common/RecursiveLookup.h \
	common/BaseRpc.h \
	common/oversim_byteswap.h \
	common/OverSimMessage.h \
	common/Nps.h \
	common/cbr/yang.h \
	common/Vivaldi.h \
	common/GlobalStatistics.h \
	common/SVivaldi.h \
	common/Comparator.h \
	$(INET_PROJ)/src/networklayer/contract/IPv6Address.h \
	common/CoordinateSystem.h \
	common/CommonMessages_m.h \
	common/Nps_m.h \
	common/oversim_mapset.h \
	common/OverSimMessage_m.h \
	common/RpcListener.h \
	common/Vector2D.h \
	common/InitStages.h \
	common/OverlayKey.h
$O/common/ExtAPIMessages_m.o: common/ExtAPIMessages_m.cc \
	common/ExtAPIMessages_m.h \
	common/TransportAddress.h \
	$(INET_PROJ)/src/base/INETDefs.h \
	common/OverSimMessage.h \
	common/BinaryValue.h \
	$(INET_PROJ)/src/networklayer/contract/IPvXAddress.h \
	common/CommonMessages_m.h \
	$(INET_PROJ)/src/networklayer/contract/IPv6Address.h \
	common/NodeHandle.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddress.h \
	common/oversim_mapset.h \
	common/OverSimMessage_m.h \
	common/Vector2D.h \
	common/OverlayKey.h
$O/common/cnetcommbuffer.o: common/cnetcommbuffer.cc \
	common/cnetcommbuffer.h
$O/common/RandomChurn.o: common/RandomChurn.cc \
	common/ChurnGenerator.h \
	common/RandomChurn.h \
	common/GlobalStatistics.h \
	common/UnderlayConfigurator.h \
	common/TransportAddress.h \
	$(INET_PROJ)/src/networklayer/contract/IPvXAddress.h \
	$(INET_PROJ)/src/base/INETDefs.h \
	common/GlobalStatisticsAccess.h \
	$(INET_PROJ)/src/networklayer/contract/IPv6Address.h \
	common/oversim_mapset.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddress.h \
	common/InitStages.h
$O/common/GlobalTraceManager_m.o: common/GlobalTraceManager_m.cc \
	common/GlobalTraceManager_m.h
$O/common/RecursiveLookup.o: common/RecursiveLookup.cc \
	$(INET_PROJ)/src/base/INotifiable.h \
	$(INET_PROJ)/src/base/INETDefs.h \
	common/BinaryValue.h \
	common/TopologyVis.h \
	common/ProxNodeHandle.h \
	common/RpcMacros.h \
	$(INET_PROJ)/src/networklayer/contract/IPvXAddress.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddress.h \
	common/RpcState.h \
	common/NodeHandle.h \
	common/AbstractLookup.h \
	common/IterativeLookupConfiguration.h \
	common/TransportAddress.h \
	common/NodeVector.h \
	common/BaseOverlay.h \
	common/RecursiveLookup.h \
	common/BaseRpc.h \
	common/OverSimMessage.h \
	common/Comparator.h \
	$(INET_PROJ)/src/networklayer/contract/IPv6Address.h \
	common/CommonMessages_m.h \
	common/oversim_mapset.h \
	common/OverSimMessage_m.h \
	common/LookupListener.h \
	common/RpcListener.h \
	common/Vector2D.h \
	common/InitStages.h \
	common/OverlayKey.h
$O/common/BaseOverlay.o: common/BaseOverlay.cc \
	common/GlobalNodeListAccess.h \
	common/RpcMacros.h \
	common/GlobalNodeList.h \
	common/BootstrapNodeHandle.h \
	common/RpcState.h \
	$(INET_PROJ)/src/base/ModuleAccess.h \
	common/BaseApp.h \
	common/IterativeLookupConfiguration.h \
	common/IterativeLookup.h \
	$(INET_PROJ)/src/base/NotifierConsts.h \
	common/oversim_byteswap.h \
	common/OverSimMessage.h \
	$(INET_PROJ)/src/transport/contract/UDPSocket.h \
	common/GlobalStatistics.h \
	common/UnderlayConfigurator.h \
	common/CommonMessages_m.h \
	common/OverSimMessage_m.h \
	common/RpcListener.h \
	$(INET_PROJ)/src/applications/udpapp/UDPAppBase.h \
	common/Vector2D.h \
	common/OverlayKey.h \
	common/InitStages.h \
	$(INET_PROJ)/src/transport/contract/UDPControlInfo_m.h \
	$(INET_PROJ)/src/base/INotifiable.h \
	$(INET_PROJ)/src/base/INETDefs.h \
	common/GlobalStatisticsAccess.h \
	common/HashFunc.h \
	common/BinaryValue.h \
	common/ChurnGenerator.h \
	common/ProxNodeHandle.h \
	common/GlobalParametersAccess.h \
	common/TopologyVis.h \
	$(INET_PROJ)/src/networklayer/contract/IPvXAddress.h \
	common/UnderlayConfiguratorAccess.h \
	common/NodeHandle.h \
	$(INET_PROJ)/src/base/NotificationBoard.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddress.h \
	common/AbstractLookup.h \
	common/TransportAddress.h \
	common/NodeVector.h \
	common/BaseOverlay.h \
	common/BootstrapList.h \
	common/RecursiveLookup.h \
	common/BaseRpc.h \
	common/GlobalParameters.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddressResolver.h \
	common/Comparator.h \
	$(INET_PROJ)/src/networklayer/contract/IPv6Address.h \
	common/oversim_mapset.h \
	common/LookupListener.h
$O/common/ProxNodeHandle.o: common/ProxNodeHandle.cc \
	common/ProxNodeHandle.h \
	common/TransportAddress.h \
	$(INET_PROJ)/src/networklayer/contract/IPvXAddress.h \
	$(INET_PROJ)/src/base/INETDefs.h \
	$(INET_PROJ)/src/networklayer/contract/IPv6Address.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddress.h \
	common/oversim_mapset.h \
	common/NodeHandle.h \
	common/OverlayKey.h
$O/common/cbr/yang.o: common/cbr/yang.cc \
	common/cbr/yang.h
$O/common/cbr/CoordMessages_m.o: common/cbr/CoordMessages_m.cc \
	common/TransportAddress.h \
	$(INET_PROJ)/src/base/INETDefs.h \
	common/OverSimMessage.h \
	common/BinaryValue.h \
	$(INET_PROJ)/src/networklayer/contract/IPvXAddress.h \
	common/CommonMessages_m.h \
	$(INET_PROJ)/src/networklayer/contract/IPv6Address.h \
	common/cbr/CoordMessages_m.h \
	common/NodeHandle.h \
	common/oversim_mapset.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddress.h \
	common/OverSimMessage_m.h \
	common/Vector2D.h \
	common/OverlayKey.h
$O/common/cbr/CoordBasedRouting.o: common/cbr/CoordBasedRouting.cc \
	common/TransportAddress.h \
	$(INET_PROJ)/src/base/INETDefs.h \
	common/GlobalNodeListAccess.h \
	common/oversim_byteswap.h \
	common/HashFunc.h \
	common/PeerInfo.h \
	common/BinaryValue.h \
	common/ChurnGenerator.h \
	common/GlobalNodeList.h \
	$(INET_PROJ)/src/networklayer/contract/IPvXAddress.h \
	common/cbr/CoordBasedRouting.h \
	$(INET_PROJ)/src/networklayer/contract/IPv6Address.h \
	common/NodeHandle.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddress.h \
	common/oversim_mapset.h \
	common/InitStages.h \
	common/OverlayKey.h
$O/common/cbr/CBR-DHTMessage_m.o: common/cbr/CBR-DHTMessage_m.cc \
	common/cbr/CBR-DHTMessage_m.h \
	common/TransportAddress.h \
	$(INET_PROJ)/src/base/INETDefs.h \
	applications/dht/DHTMessage_m.h \
	common/OverSimMessage.h \
	common/BinaryValue.h \
	$(INET_PROJ)/src/networklayer/contract/IPvXAddress.h \
	common/CommonMessages_m.h \
	$(INET_PROJ)/src/networklayer/contract/IPv6Address.h \
	common/NodeHandle.h \
	common/oversim_mapset.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddress.h \
	common/OverSimMessage_m.h \
	common/Vector2D.h \
	common/OverlayKey.h
$O/common/cbr/Landmark.o: common/cbr/Landmark.cc \
	common/PeerInfo.h \
	common/cbr/Landmark.h \
	common/RpcMacros.h \
	common/GlobalNodeList.h \
	underlay/simpleunderlay/SimpleUDPPacket_m.h \
	common/BootstrapNodeHandle.h \
	common/RpcState.h \
	$(INET_PROJ)/src/base/ModuleAccess.h \
	common/cbr/simplex.h \
	common/BaseApp.h \
	common/IterativeLookupConfiguration.h \
	$(INET_PROJ)/src/base/NotifierConsts.h \
	common/oversim_byteswap.h \
	common/OverSimMessage.h \
	common/Nps.h \
	underlay/simpleunderlay/SimpleUnderlayConfigurator.h \
	common/GlobalStatistics.h \
	common/UnderlayConfigurator.h \
	underlay/simpleunderlay/SimpleNodeEntry.h \
	common/CommonMessages_m.h \
	common/OverSimMessage_m.h \
	common/RpcListener.h \
	$(INET_PROJ)/src/base/BasicModule.h \
	common/Vector2D.h \
	common/InitStages.h \
	common/OverlayKey.h \
	$(INET_PROJ)/src/base/INotifiable.h \
	$(INET_PROJ)/src/base/INETDefs.h \
	common/GlobalStatisticsAccess.h \
	common/HashFunc.h \
	common/BinaryValue.h \
	common/ChurnGenerator.h \
	common/ProxNodeHandle.h \
	common/TopologyVis.h \
	$(INET_PROJ)/src/networklayer/contract/IPvXAddress.h \
	$(INET_PROJ)/src/base/NotificationBoard.h \
	common/NodeHandle.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddress.h \
	common/AbstractLookup.h \
	common/NeighborCache.h \
	common/TransportAddress.h \
	common/NodeVector.h \
	common/BaseOverlay.h \
	common/BootstrapList.h \
	underlay/simpleunderlay/SimpleInfo.h \
	common/RecursiveLookup.h \
	common/BaseRpc.h \
	common/cbr/yang.h \
	common/Vivaldi.h \
	common/SVivaldi.h \
	common/Comparator.h \
	common/CoordinateSystem.h \
	$(INET_PROJ)/src/networklayer/contract/IPv6Address.h \
	common/Nps_m.h \
	common/oversim_mapset.h
$O/common/cbr/simplex.o: common/cbr/simplex.cc \
	$(INET_PROJ)/src/base/INETDefs.h \
	$(INET_PROJ)/src/base/INotifiable.h \
	common/GlobalStatisticsAccess.h \
	common/HashFunc.h \
	common/BinaryValue.h \
	common/TopologyVis.h \
	common/ProxNodeHandle.h \
	common/RpcMacros.h \
	$(INET_PROJ)/src/networklayer/contract/IPvXAddress.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddress.h \
	common/RpcState.h \
	common/NodeHandle.h \
	common/cbr/simplex.h \
	common/AbstractLookup.h \
	common/BaseApp.h \
	common/NeighborCache.h \
	common/IterativeLookupConfiguration.h \
	common/TransportAddress.h \
	common/NodeVector.h \
	common/BaseOverlay.h \
	common/RecursiveLookup.h \
	common/BaseRpc.h \
	common/oversim_byteswap.h \
	common/OverSimMessage.h \
	common/Nps.h \
	common/cbr/yang.h \
	common/Vivaldi.h \
	common/GlobalStatistics.h \
	common/SVivaldi.h \
	common/Comparator.h \
	$(INET_PROJ)/src/networklayer/contract/IPv6Address.h \
	common/CoordinateSystem.h \
	common/CommonMessages_m.h \
	common/Nps_m.h \
	common/oversim_mapset.h \
	common/OverSimMessage_m.h \
	common/RpcListener.h \
	common/Vector2D.h \
	common/InitStages.h \
	common/OverlayKey.h
$O/common/cbr/CBR-DHT.o: common/cbr/CBR-DHT.cc \
	common/cbr/CBR-DHTMessage_m.h \
	common/SHA1.h \
	$(INET_PROJ)/src/base/INETDefs.h \
	$(INET_PROJ)/src/base/INotifiable.h \
	common/GlobalStatisticsAccess.h \
	common/HashFunc.h \
	common/BinaryValue.h \
	applications/dht/DHTDataStorage.h \
	common/cbr/CoordBasedRoutingAccess.h \
	common/TopologyVis.h \
	common/ProxNodeHandle.h \
	common/RpcMacros.h \
	common/cbr/CBR-DHT.h \
	$(INET_PROJ)/src/networklayer/contract/IPvXAddress.h \
	common/RpcState.h \
	common/NodeHandle.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddress.h \
	common/cbr/simplex.h \
	common/AbstractLookup.h \
	common/BaseApp.h \
	common/NeighborCache.h \
	common/IterativeLookupConfiguration.h \
	common/TransportAddress.h \
	common/NodeVector.h \
	common/BaseOverlay.h \
	common/RecursiveLookup.h \
	applications/dht/DHTMessage_m.h \
	common/BaseRpc.h \
	common/oversim_byteswap.h \
	common/OverSimMessage.h \
	common/Nps.h \
	common/cbr/yang.h \
	common/Vivaldi.h \
	common/GlobalStatistics.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddressResolver.h \
	common/SVivaldi.h \
	common/Comparator.h \
	common/cbr/CoordBasedRouting.h \
	common/CommonMessages_m.h \
	common/CoordinateSystem.h \
	$(INET_PROJ)/src/networklayer/contract/IPv6Address.h \
	common/Nps_m.h \
	common/oversim_mapset.h \
	common/OverSimMessage_m.h \
	common/RpcListener.h \
	common/Vector2D.h \
	common/InitStages.h \
	common/OverlayKey.h
$O/overlay/cdnp2ptracker/CDNP2PTracker.o: overlay/cdnp2ptracker/CDNP2PTracker.cc \
	$(INET_PROJ)/src/base/INETDefs.h \
	$(INET_PROJ)/src/base/INotifiable.h \
	overlay/meshoverlay/MeshOverlayMessage_m.h \
	applications/vodapp/Chunk.h \
	overlay/cdnp2ptracker/CDNP2PTracker.h \
	common/BinaryValue.h \
	common/ProxNodeHandle.h \
	common/TopologyVis.h \
	common/RpcMacros.h \
	applications/vodapp/VideoFrame.h \
	$(INET_PROJ)/src/networklayer/contract/IPvXAddress.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddress.h \
	common/RpcState.h \
	common/NodeHandle.h \
	common/AbstractLookup.h \
	common/IterativeLookupConfiguration.h \
	common/TransportAddress.h \
	common/NodeVector.h \
	common/BaseOverlay.h \
	common/RecursiveLookup.h \
	common/BaseRpc.h \
	common/OverSimMessage.h \
	overlay/meshoverlay/TrackerMessage_m.h \
	applications/vodapp/VideoMessage_m.h \
	common/Comparator.h \
	applications/vodapp/BufferMap.h \
	$(INET_PROJ)/src/networklayer/contract/IPv6Address.h \
	common/CommonMessages_m.h \
	common/oversim_mapset.h \
	common/OverSimMessage_m.h \
	common/RpcListener.h \
	common/Vector2D.h \
	common/InitStages.h \
	common/OverlayKey.h
$O/overlay/cdnserver/CdnServer.o: overlay/cdnserver/CdnServer.cc \
	applications/vodapp/VodApp.h \
	overlay/meshoverlay/MeshOverlayMessage_m.h \
	applications/vodapp/Chunk.h \
	common/PeerInfo.h \
	common/RpcMacros.h \
	common/GlobalNodeList.h \
	underlay/simpleunderlay/SimpleUDPPacket_m.h \
	common/RpcState.h \
	common/BaseApp.h \
	common/IterativeLookupConfiguration.h \
	common/oversim_byteswap.h \
	common/OverSimMessage.h \
	overlay/meshoverlay/TrackerMessage_m.h \
	common/GlobalStatistics.h \
	applications/vodapp/VideoMessage_m.h \
	applications/vodapp/BufferMap.h \
	underlay/simpleunderlay/SimpleNodeEntry.h \
	common/CommonMessages_m.h \
	common/RpcListener.h \
	common/OverSimMessage_m.h \
	common/Vector2D.h \
	common/OverlayKey.h \
	common/InitStages.h \
	$(INET_PROJ)/src/base/INotifiable.h \
	$(INET_PROJ)/src/base/INETDefs.h \
	overlay/meshoverlay/ChunkPackets.h \
	common/HashFunc.h \
	common/ChurnGenerator.h \
	common/BinaryValue.h \
	common/TopologyVis.h \
	common/ProxNodeHandle.h \
	overlay/meshoverlay/MeshOverlay.h \
	applications/vodapp/VideoFrame.h \
	$(INET_PROJ)/src/networklayer/contract/IPvXAddress.h \
	tier2/vodstream/BaseVodStream.h \
	applications/vodapp/LocalVariables.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddress.h \
	common/NodeHandle.h \
	applications/vodapp/VideoBuffer.h \
	overlay/peersmesh/MeshMessage_m.h \
	common/AbstractLookup.h \
	common/TransportAddress.h \
	common/NodeVector.h \
	overlay/cdnserver/CdnServer.h \
	common/BaseOverlay.h \
	underlay/simpleunderlay/SimpleInfo.h \
	common/RecursiveLookup.h \
	common/BaseRpc.h \
	common/Comparator.h \
	$(INET_PROJ)/src/networklayer/contract/IPv6Address.h \
	common/oversim_mapset.h
$O/overlay/meshoverlay/MeshOverlayMessage_m.o: overlay/meshoverlay/MeshOverlayMessage_m.cc \
	common/TransportAddress.h \
	$(INET_PROJ)/src/base/INETDefs.h \
	overlay/meshoverlay/MeshOverlayMessage_m.h \
	common/OverSimMessage.h \
	common/BinaryValue.h \
	$(INET_PROJ)/src/networklayer/contract/IPvXAddress.h \
	common/CommonMessages_m.h \
	$(INET_PROJ)/src/networklayer/contract/IPv6Address.h \
	common/NodeHandle.h \
	common/oversim_mapset.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddress.h \
	common/OverSimMessage_m.h \
	common/Vector2D.h \
	common/OverlayKey.h
$O/overlay/meshoverlay/ChunkPackets.o: overlay/meshoverlay/ChunkPackets.cc \
	applications/vodapp/VideoFrame.h \
	overlay/meshoverlay/ChunkPackets.h \
	applications/vodapp/Chunk.h
$O/overlay/meshoverlay/TrackerMessage_m.o: overlay/meshoverlay/TrackerMessage_m.cc \
	common/TransportAddress.h \
	$(INET_PROJ)/src/base/INETDefs.h \
	common/OverSimMessage.h \
	common/BinaryValue.h \
	overlay/meshoverlay/TrackerMessage_m.h \
	$(INET_PROJ)/src/networklayer/contract/IPvXAddress.h \
	common/CommonMessages_m.h \
	$(INET_PROJ)/src/networklayer/contract/IPv6Address.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddress.h \
	common/oversim_mapset.h \
	common/NodeHandle.h \
	common/OverSimMessage_m.h \
	common/Vector2D.h \
	common/OverlayKey.h
$O/overlay/meshoverlay/MeshOverlay.o: overlay/meshoverlay/MeshOverlay.cc \
	applications/vodapp/VodApp.h \
	overlay/meshoverlay/MeshOverlayMessage_m.h \
	applications/vodapp/Chunk.h \
	common/PeerInfo.h \
	common/RpcMacros.h \
	common/GlobalNodeList.h \
	underlay/simpleunderlay/SimpleUDPPacket_m.h \
	common/RpcState.h \
	common/BaseApp.h \
	common/IterativeLookupConfiguration.h \
	common/oversim_byteswap.h \
	common/OverSimMessage.h \
	common/GlobalStatistics.h \
	applications/vodapp/VideoMessage_m.h \
	applications/vodapp/BufferMap.h \
	common/CommonMessages_m.h \
	underlay/simpleunderlay/SimpleNodeEntry.h \
	common/RpcListener.h \
	common/OverSimMessage_m.h \
	common/Vector2D.h \
	common/OverlayKey.h \
	common/InitStages.h \
	$(INET_PROJ)/src/base/INotifiable.h \
	$(INET_PROJ)/src/base/INETDefs.h \
	overlay/meshoverlay/ChunkPackets.h \
	common/HashFunc.h \
	common/BinaryValue.h \
	common/ChurnGenerator.h \
	common/TopologyVis.h \
	common/ProxNodeHandle.h \
	overlay/meshoverlay/MeshOverlay.h \
	applications/vodapp/VideoFrame.h \
	$(INET_PROJ)/src/networklayer/contract/IPvXAddress.h \
	tier2/vodstream/BaseVodStream.h \
	applications/vodapp/LocalVariables.h \
	common/NodeHandle.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddress.h \
	applications/vodapp/VideoBuffer.h \
	overlay/peersmesh/MeshMessage_m.h \
	common/AbstractLookup.h \
	common/TransportAddress.h \
	common/NodeVector.h \
	common/BaseOverlay.h \
	underlay/simpleunderlay/SimpleInfo.h \
	common/RecursiveLookup.h \
	common/BaseRpc.h \
	common/Comparator.h \
	$(INET_PROJ)/src/networklayer/contract/IPv6Address.h \
	common/oversim_mapset.h
$O/overlay/peersmesh/PeersMesh.o: overlay/peersmesh/PeersMesh.cc \
	applications/vodapp/VodApp.h \
	overlay/meshoverlay/MeshOverlayMessage_m.h \
	applications/vodapp/Chunk.h \
	common/PeerInfo.h \
	common/RpcMacros.h \
	common/GlobalNodeList.h \
	underlay/simpleunderlay/SimpleUDPPacket_m.h \
	common/RpcState.h \
	common/BaseApp.h \
	common/IterativeLookupConfiguration.h \
	common/oversim_byteswap.h \
	common/OverSimMessage.h \
	overlay/meshoverlay/TrackerMessage_m.h \
	common/GlobalStatistics.h \
	overlay/peersmesh/PeersMesh.h \
	applications/vodapp/VideoMessage_m.h \
	applications/vodapp/BufferMap.h \
	common/CommonMessages_m.h \
	underlay/simpleunderlay/SimpleNodeEntry.h \
	common/RpcListener.h \
	common/OverSimMessage_m.h \
	common/Vector2D.h \
	common/InitStages.h \
	common/OverlayKey.h \
	$(INET_PROJ)/src/base/INETDefs.h \
	$(INET_PROJ)/src/base/INotifiable.h \
	overlay/meshoverlay/ChunkPackets.h \
	common/HashFunc.h \
	common/BinaryValue.h \
	common/ChurnGenerator.h \
	common/ProxNodeHandle.h \
	common/TopologyVis.h \
	overlay/meshoverlay/MeshOverlay.h \
	applications/vodapp/VideoFrame.h \
	$(INET_PROJ)/src/networklayer/contract/IPvXAddress.h \
	tier2/vodstream/BaseVodStream.h \
	applications/vodapp/LocalVariables.h \
	common/NodeHandle.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddress.h \
	applications/vodapp/VideoBuffer.h \
	overlay/peersmesh/MeshMessage_m.h \
	common/AbstractLookup.h \
	common/TransportAddress.h \
	common/NodeVector.h \
	common/BaseOverlay.h \
	underlay/simpleunderlay/SimpleInfo.h \
	common/RecursiveLookup.h \
	common/BaseRpc.h \
	common/Comparator.h \
	$(INET_PROJ)/src/networklayer/contract/IPv6Address.h \
	common/oversim_mapset.h
$O/overlay/peersmesh/MeshMessage_m.o: overlay/peersmesh/MeshMessage_m.cc \
	common/TransportAddress.h \
	$(INET_PROJ)/src/base/INETDefs.h \
	common/OverSimMessage.h \
	common/BinaryValue.h \
	$(INET_PROJ)/src/networklayer/contract/IPvXAddress.h \
	common/CommonMessages_m.h \
	$(INET_PROJ)/src/networklayer/contract/IPv6Address.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddress.h \
	common/oversim_mapset.h \
	common/NodeHandle.h \
	common/OverSimMessage_m.h \
	common/Vector2D.h \
	common/OverlayKey.h \
	overlay/peersmesh/MeshMessage_m.h
$O/tier2/vodplayer/MPEG4Player.o: tier2/vodplayer/MPEG4Player.cc \
	tier2/vodplayer/MPEG4Player.h \
	$(INET_PROJ)/src/base/INotifiable.h \
	$(INET_PROJ)/src/base/INETDefs.h \
	applications/vodapp/Chunk.h \
	common/HashFunc.h \
	common/ChurnGenerator.h \
	common/BinaryValue.h \
	common/TopologyVis.h \
	common/ProxNodeHandle.h \
	common/RpcMacros.h \
	tier2/vodplayer/BasePlayer.h \
	applications/vodapp/VideoFrame.h \
	common/GlobalNodeList.h \
	$(INET_PROJ)/src/networklayer/contract/IPvXAddress.h \
	tier2/vodstream/BaseVodStream.h \
	applications/vodapp/LocalVariables.h \
	common/NodeHandle.h \
	common/RpcState.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddress.h \
	applications/vodapp/VideoBuffer.h \
	common/AbstractLookup.h \
	common/BaseApp.h \
	common/IterativeLookupConfiguration.h \
	common/TransportAddress.h \
	common/NodeVector.h \
	common/BaseOverlay.h \
	common/RecursiveLookup.h \
	common/BaseRpc.h \
	common/oversim_byteswap.h \
	common/OverSimMessage.h \
	common/GlobalStatistics.h \
	applications/vodapp/VideoMessage_m.h \
	common/Comparator.h \
	applications/vodapp/BufferMap.h \
	common/CommonMessages_m.h \
	$(INET_PROJ)/src/networklayer/contract/IPv6Address.h \
	common/oversim_mapset.h \
	common/RpcListener.h \
	common/OverSimMessage_m.h \
	common/Vector2D.h \
	common/InitStages.h \
	common/OverlayKey.h
$O/tier2/vodplayer/BasePlayer.o: tier2/vodplayer/BasePlayer.cc \
	$(INET_PROJ)/src/base/INETDefs.h \
	$(INET_PROJ)/src/base/INotifiable.h \
	applications/vodapp/Chunk.h \
	common/HashFunc.h \
	common/BinaryValue.h \
	common/ChurnGenerator.h \
	common/ProxNodeHandle.h \
	common/TopologyVis.h \
	common/RpcMacros.h \
	applications/vodapp/VideoFrame.h \
	tier2/vodplayer/BasePlayer.h \
	$(INET_PROJ)/src/networklayer/contract/IPvXAddress.h \
	common/GlobalNodeList.h \
	tier2/vodstream/BaseVodStream.h \
	applications/vodapp/LocalVariables.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddress.h \
	common/RpcState.h \
	common/NodeHandle.h \
	applications/vodapp/VideoBuffer.h \
	common/AbstractLookup.h \
	common/BaseApp.h \
	common/IterativeLookupConfiguration.h \
	common/TransportAddress.h \
	common/NodeVector.h \
	common/BaseOverlay.h \
	common/RecursiveLookup.h \
	common/BaseRpc.h \
	common/oversim_byteswap.h \
	common/OverSimMessage.h \
	common/GlobalStatistics.h \
	applications/vodapp/VideoMessage_m.h \
	common/Comparator.h \
	applications/vodapp/BufferMap.h \
	common/CommonMessages_m.h \
	$(INET_PROJ)/src/networklayer/contract/IPv6Address.h \
	common/oversim_mapset.h \
	common/OverSimMessage_m.h \
	common/RpcListener.h \
	common/Vector2D.h \
	common/OverlayKey.h \
	common/InitStages.h
$O/tier2/vodstream/MPEG4VodStream.o: tier2/vodstream/MPEG4VodStream.cc \
	$(INET_PROJ)/src/base/INETDefs.h \
	$(INET_PROJ)/src/base/INotifiable.h \
	applications/vodapp/Chunk.h \
	common/HashFunc.h \
	common/ChurnGenerator.h \
	common/BinaryValue.h \
	common/TopologyVis.h \
	common/ProxNodeHandle.h \
	common/RpcMacros.h \
	applications/vodapp/VideoFrame.h \
	common/GlobalNodeList.h \
	$(INET_PROJ)/src/networklayer/contract/IPvXAddress.h \
	tier2/vodstream/BaseVodStream.h \
	applications/vodapp/LocalVariables.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddress.h \
	common/RpcState.h \
	common/NodeHandle.h \
	applications/vodapp/VideoBuffer.h \
	tier2/vodstream/MPEG4VodStream.h \
	common/AbstractLookup.h \
	common/BaseApp.h \
	common/IterativeLookupConfiguration.h \
	common/TransportAddress.h \
	common/NodeVector.h \
	common/BaseOverlay.h \
	common/RecursiveLookup.h \
	common/BaseRpc.h \
	common/oversim_byteswap.h \
	common/OverSimMessage.h \
	common/GlobalStatistics.h \
	applications/vodapp/VideoMessage_m.h \
	common/Comparator.h \
	applications/vodapp/BufferMap.h \
	common/CommonMessages_m.h \
	$(INET_PROJ)/src/networklayer/contract/IPv6Address.h \
	common/oversim_mapset.h \
	common/OverSimMessage_m.h \
	common/RpcListener.h \
	common/Vector2D.h \
	common/OverlayKey.h \
	common/InitStages.h
$O/tier2/vodstream/BaseVodStream.o: tier2/vodstream/BaseVodStream.cc \
	$(INET_PROJ)/src/base/INotifiable.h \
	$(INET_PROJ)/src/base/INETDefs.h \
	applications/vodapp/Chunk.h \
	common/HashFunc.h \
	common/BinaryValue.h \
	common/ChurnGenerator.h \
	common/TopologyVis.h \
	common/ProxNodeHandle.h \
	common/RpcMacros.h \
	applications/vodapp/VideoFrame.h \
	common/GlobalNodeList.h \
	$(INET_PROJ)/src/networklayer/contract/IPvXAddress.h \
	tier2/vodstream/BaseVodStream.h \
	applications/vodapp/LocalVariables.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddress.h \
	common/RpcState.h \
	common/NodeHandle.h \
	applications/vodapp/VideoBuffer.h \
	common/AbstractLookup.h \
	common/BaseApp.h \
	common/IterativeLookupConfiguration.h \
	common/TransportAddress.h \
	common/NodeVector.h \
	common/BaseOverlay.h \
	common/RecursiveLookup.h \
	common/BaseRpc.h \
	common/oversim_byteswap.h \
	common/OverSimMessage.h \
	common/GlobalStatistics.h \
	applications/vodapp/VideoMessage_m.h \
	common/Comparator.h \
	applications/vodapp/BufferMap.h \
	common/CommonMessages_m.h \
	$(INET_PROJ)/src/networklayer/contract/IPv6Address.h \
	common/oversim_mapset.h \
	common/RpcListener.h \
	common/OverSimMessage_m.h \
	common/Vector2D.h \
	common/InitStages.h \
	common/OverlayKey.h
$O/underlay/inetunderlay/InetInfo.o: underlay/inetunderlay/InetInfo.cc \
	common/PeerInfo.h \
	underlay/inetunderlay/InetInfo.h
$O/underlay/inetunderlay/InetUnderlayConfigurator.o: underlay/inetunderlay/InetUnderlayConfigurator.cc \
	$(INET_PROJ)/src/networklayer/ipv4/IPv4InterfaceData.h \
	$(INET_PROJ)/src/base/INotifiable.h \
	$(INET_PROJ)/src/base/INETDefs.h \
	$(INET_PROJ)/src/networklayer/ipv4/IPRoute.h \
	$(INET_PROJ)/src/linklayer/contract/MACAddress.h \
	common/HashFunc.h \
	common/PeerInfo.h \
	common/BinaryValue.h \
	common/ChurnGenerator.h \
	$(INET_PROJ)/src/networklayer/common/InterfaceToken.h \
	common/GlobalNodeList.h \
	$(INET_PROJ)/src/networklayer/contract/IPvXAddress.h \
	$(INET_PROJ)/src/networklayer/common/IInterfaceTable.h \
	$(INET_PROJ)/src/base/ModuleAccess.h \
	$(INET_PROJ)/src/base/NotificationBoard.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddress.h \
	common/NodeHandle.h \
	$(INET_PROJ)/src/networklayer/common/InterfaceEntry.h \
	common/TransportAddress.h \
	underlay/inetunderlay/InetInfo.h \
	$(INET_PROJ)/src/base/NotifierConsts.h \
	underlay/inetunderlay/AccessNet.h \
	common/oversim_byteswap.h \
	common/UnderlayConfigurator.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddressResolver.h \
	common/StringConvert.h \
	underlay/inetunderlay/InetUnderlayConfigurator.h \
	$(INET_PROJ)/src/networklayer/contract/IPv6Address.h \
	common/oversim_mapset.h \
	common/InitStages.h \
	common/OverlayKey.h \
	$(INET_PROJ)/src/networklayer/ipv4/IRoutingTable.h
$O/underlay/inetunderlay/InetUnderlayConfigurator2.o: underlay/inetunderlay/InetUnderlayConfigurator2.cc \
	$(INET_PROJ)/src/networklayer/ipv4/IPv4InterfaceData.h \
	$(INET_PROJ)/src/base/INotifiable.h \
	$(INET_PROJ)/src/base/INETDefs.h \
	$(INET_PROJ)/src/networklayer/ipv4/IPRoute.h \
	$(INET_PROJ)/src/linklayer/contract/MACAddress.h \
	common/HashFunc.h \
	common/PeerInfo.h \
	common/BinaryValue.h \
	common/ChurnGenerator.h \
	$(INET_PROJ)/src/networklayer/common/InterfaceToken.h \
	common/GlobalNodeList.h \
	$(INET_PROJ)/src/networklayer/contract/IPvXAddress.h \
	$(INET_PROJ)/src/networklayer/common/IInterfaceTable.h \
	$(INET_PROJ)/src/base/ModuleAccess.h \
	$(INET_PROJ)/src/base/NotificationBoard.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddress.h \
	common/NodeHandle.h \
	$(INET_PROJ)/src/networklayer/common/InterfaceEntry.h \
	common/TransportAddress.h \
	underlay/inetunderlay/InetInfo.h \
	$(INET_PROJ)/src/base/NotifierConsts.h \
	common/oversim_byteswap.h \
	underlay/inetunderlay/AccessNet.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddressResolver.h \
	underlay/inetunderlay/InetUnderlayConfigurator2.h \
	common/UnderlayConfigurator.h \
	common/StringConvert.h \
	$(INET_PROJ)/src/networklayer/contract/IPv6Address.h \
	common/oversim_mapset.h \
	common/InitStages.h \
	common/OverlayKey.h \
	$(INET_PROJ)/src/networklayer/ipv4/IRoutingTable.h
$O/underlay/inetunderlay/AccessNet.o: underlay/inetunderlay/AccessNet.cc \
	$(INET_PROJ)/src/networklayer/ipv4/IPv4InterfaceData.h \
	$(INET_PROJ)/src/base/INETDefs.h \
	$(INET_PROJ)/src/networklayer/ipv4/IPRoute.h \
	$(INET_PROJ)/src/base/NotifierConsts.h \
	$(INET_PROJ)/src/linklayer/contract/MACAddress.h \
	underlay/inetunderlay/AccessNet.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddressResolver.h \
	$(INET_PROJ)/src/networklayer/common/InterfaceToken.h \
	$(INET_PROJ)/src/networklayer/contract/IPvXAddress.h \
	$(INET_PROJ)/src/networklayer/common/IInterfaceTable.h \
	$(INET_PROJ)/src/networklayer/contract/IPv6Address.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddress.h \
	$(INET_PROJ)/src/networklayer/ipv4/IRoutingTable.h \
	$(INET_PROJ)/src/networklayer/common/InterfaceEntry.h \
	common/InitStages.h
$O/underlay/simpleunderlay/SimpleUDPPacket_m.o: underlay/simpleunderlay/SimpleUDPPacket_m.cc \
	$(INET_PROJ)/src/base/INETDefs.h \
	underlay/simpleunderlay/SimpleUDPPacket_m.h
$O/underlay/simpleunderlay/SimpleNodeEntry.o: underlay/simpleunderlay/SimpleNodeEntry.cc \
	$(INET_PROJ)/src/transport/contract/UDPControlInfo_m.h \
	common/SHA1.h \
	$(INET_PROJ)/src/base/INETDefs.h \
	underlay/simpleunderlay/SimpleUDP.h \
	common/BinaryValue.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddressResolver.h \
	$(INET_PROJ)/src/networklayer/contract/IPvXAddress.h \
	underlay/simpleunderlay/SimpleUDPPacket_m.h \
	underlay/simpleunderlay/SimpleNodeEntry.h \
	$(INET_PROJ)/src/networklayer/contract/IPv6Address.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddress.h \
	common/OverlayKey.h \
	common/InitStages.h
$O/underlay/simpleunderlay/SimpleInfo.o: underlay/simpleunderlay/SimpleInfo.cc \
	common/PeerInfo.h \
	$(INET_PROJ)/src/networklayer/contract/IPvXAddress.h \
	$(INET_PROJ)/src/base/INETDefs.h \
	underlay/simpleunderlay/SimpleUDPPacket_m.h \
	underlay/simpleunderlay/SimpleNodeEntry.h \
	$(INET_PROJ)/src/networklayer/contract/IPv6Address.h \
	underlay/simpleunderlay/SimpleInfo.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddress.h
$O/underlay/simpleunderlay/SimpleUDP.o: underlay/simpleunderlay/SimpleUDP.cc \
	$(INET_PROJ)/src/networklayer/icmpv6/ICMPv6Access.h \
	$(INET_PROJ)/src/networklayer/ipv4/IPRoute.h \
	overlay/meshoverlay/MeshOverlayMessage_m.h \
	common/GlobalNodeListAccess.h \
	$(INET_PROJ)/src/networklayer/contract/IPControlInfo.h \
	common/PeerInfo.h \
	$(INET_PROJ)/src/networklayer/contract/IPv6ControlInfo_m.h \
	$(INET_PROJ)/src/networklayer/common/InterfaceToken.h \
	common/GlobalNodeList.h \
	$(INET_PROJ)/src/networklayer/common/IInterfaceTable.h \
	underlay/simpleunderlay/SimpleUDPPacket.h \
	underlay/simpleunderlay/SimpleUDPPacket_m.h \
	$(INET_PROJ)/src/base/ModuleAccess.h \
	$(INET_PROJ)/src/networklayer/contract/IPProtocolId_m.h \
	$(INET_PROJ)/src/networklayer/common/InterfaceEntry.h \
	$(INET_PROJ)/src/networklayer/ipv4/IPDatagram_m.h \
	$(INET_PROJ)/src/base/NotifierConsts.h \
	common/oversim_byteswap.h \
	common/OverSimMessage.h \
	common/GlobalStatistics.h \
	$(INET_PROJ)/src/networklayer/ipv6/IPv6Datagram.h \
	$(INET_PROJ)/src/networklayer/icmpv6/ICMPv6Message_m.h \
	common/CommonMessages_m.h \
	underlay/simpleunderlay/SimpleNodeEntry.h \
	$(INET_PROJ)/src/networklayer/ipv4/ICMPAccess.h \
	$(INET_PROJ)/src/networklayer/ipv4/ICMPMessage_m.h \
	common/OverSimMessage_m.h \
	common/Vector2D.h \
	common/InitStages.h \
	common/OverlayKey.h \
	$(INET_PROJ)/src/transport/contract/UDPControlInfo_m.h \
	$(INET_PROJ)/src/base/INotifiable.h \
	$(INET_PROJ)/src/base/INETDefs.h \
	underlay/simpleunderlay/SimpleUDP.h \
	common/GlobalStatisticsAccess.h \
	$(INET_PROJ)/src/networklayer/contract/IPv6ControlInfo.h \
	$(INET_PROJ)/src/linklayer/contract/MACAddress.h \
	common/HashFunc.h \
	common/ChurnGenerator.h \
	common/BinaryValue.h \
	$(INET_PROJ)/src/networklayer/ipv4/ICMPMessage.h \
	$(INET_PROJ)/src/networklayer/contract/IPvXAddress.h \
	$(INET_PROJ)/src/networklayer/icmpv6/ICMPv6.h \
	$(INET_PROJ)/src/base/NotificationBoard.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddress.h \
	common/NodeHandle.h \
	common/TransportAddress.h \
	$(INET_PROJ)/src/networklayer/ipv4/RoutingTableAccess.h \
	underlay/simpleunderlay/SimpleInfo.h \
	$(INET_PROJ)/src/networklayer/ipv6/RoutingTable6.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddressResolver.h \
	$(INET_PROJ)/src/networklayer/contract/IPControlInfo_m.h \
	$(INET_PROJ)/src/networklayer/ipv4/ICMP.h \
	$(INET_PROJ)/src/networklayer/contract/IPv6Address.h \
	$(INET_PROJ)/src/networklayer/ipv6/RoutingTable6Access.h \
	$(INET_PROJ)/src/networklayer/ipv6/IPv6Datagram_m.h \
	common/oversim_mapset.h \
	$(INET_PROJ)/src/networklayer/ipv4/IRoutingTable.h
$O/underlay/simpleunderlay/SimpleUnderlayConfigurator.o: underlay/simpleunderlay/SimpleUnderlayConfigurator.cc \
	$(INET_PROJ)/src/networklayer/ipv4/IPv4InterfaceData.h \
	$(INET_PROJ)/src/transport/contract/UDPControlInfo_m.h \
	$(INET_PROJ)/src/base/INotifiable.h \
	$(INET_PROJ)/src/base/INETDefs.h \
	underlay/simpleunderlay/SimpleUDP.h \
	$(INET_PROJ)/src/linklayer/contract/MACAddress.h \
	common/HashFunc.h \
	common/PeerInfo.h \
	common/ChurnGenerator.h \
	common/BinaryValue.h \
	$(INET_PROJ)/src/networklayer/common/InterfaceToken.h \
	common/GlobalNodeList.h \
	$(INET_PROJ)/src/networklayer/contract/IPvXAddress.h \
	underlay/simpleunderlay/SimpleUDPPacket_m.h \
	$(INET_PROJ)/src/networklayer/common/IInterfaceTable.h \
	$(INET_PROJ)/src/base/ModuleAccess.h \
	$(INET_PROJ)/src/base/NotificationBoard.h \
	common/NodeHandle.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddress.h \
	$(INET_PROJ)/src/networklayer/common/InterfaceEntry.h \
	common/TransportAddress.h \
	underlay/simpleunderlay/SimpleInfo.h \
	$(INET_PROJ)/src/base/NotifierConsts.h \
	common/oversim_byteswap.h \
	underlay/simpleunderlay/SimpleUnderlayConfigurator.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddressResolver.h \
	common/UnderlayConfigurator.h \
	common/StringConvert.h \
	underlay/simpleunderlay/SimpleNodeEntry.h \
	$(INET_PROJ)/src/networklayer/contract/IPv6Address.h \
	common/oversim_mapset.h \
	$(INET_PROJ)/src/base/BasicModule.h \
	common/InitStages.h \
	common/OverlayKey.h
$O/underlay/singlehostunderlay/RealworldApp.o: underlay/singlehostunderlay/RealworldApp.cc \
	underlay/singlehostunderlay/realtimescheduler.h \
	$(INET_PROJ)/src/base/INETDefs.h \
	underlay/singlehostunderlay/tunoutscheduler.h \
	underlay/singlehostunderlay/RealworldConnector.h \
	underlay/singlehostunderlay/RealworldApp.h \
	common/PacketParser.h
$O/underlay/singlehostunderlay/SingleHostUnderlayConfigurator.o: underlay/singlehostunderlay/SingleHostUnderlayConfigurator.cc \
	$(INET_PROJ)/src/networklayer/ipv4/IPv4InterfaceData.h \
	$(INET_PROJ)/src/base/INotifiable.h \
	$(INET_PROJ)/src/base/INETDefs.h \
	$(INET_PROJ)/src/networklayer/ipv4/IPRoute.h \
	underlay/singlehostunderlay/stun/stun_udp.h \
	common/GlobalNodeListAccess.h \
	$(INET_PROJ)/src/linklayer/contract/MACAddress.h \
	common/HashFunc.h \
	common/PeerInfo.h \
	common/BinaryValue.h \
	common/ChurnGenerator.h \
	$(INET_PROJ)/src/networklayer/common/InterfaceToken.h \
	$(INET_PROJ)/src/networklayer/contract/IPvXAddress.h \
	common/GlobalNodeList.h \
	$(INET_PROJ)/src/networklayer/common/IInterfaceTable.h \
	$(INET_PROJ)/src/base/ModuleAccess.h \
	$(INET_PROJ)/src/base/NotificationBoard.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddress.h \
	common/NodeHandle.h \
	underlay/singlehostunderlay/SingleHostUnderlayConfigurator.h \
	$(INET_PROJ)/src/networklayer/common/InterfaceEntry.h \
	common/TransportAddress.h \
	$(INET_PROJ)/src/base/NotifierConsts.h \
	common/oversim_byteswap.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddressResolver.h \
	common/UnderlayConfigurator.h \
	$(INET_PROJ)/src/networklayer/contract/IPv6Address.h \
	common/oversim_mapset.h \
	$(INET_PROJ)/src/base/BasicModule.h \
	$(INET_PROJ)/src/networklayer/ipv4/IRoutingTable.h \
	underlay/singlehostunderlay/stun/stun.h \
	common/OverlayKey.h \
	common/InitStages.h
$O/underlay/singlehostunderlay/ZeroconfConnector.o: underlay/singlehostunderlay/ZeroconfConnector.cc \
	underlay/singlehostunderlay/ZeroconfConnector.h \
	$(INET_PROJ)/src/base/INETDefs.h \
	$(INET_PROJ)/src/base/INotifiable.h \
	common/GlobalNodeListAccess.h \
	common/HashFunc.h \
	common/ChurnGenerator.h \
	common/BinaryValue.h \
	common/TopologyVis.h \
	common/ProxNodeHandle.h \
	common/RpcMacros.h \
	common/GlobalNodeList.h \
	$(INET_PROJ)/src/networklayer/contract/IPvXAddress.h \
	common/BootstrapNodeHandle.h \
	common/RpcState.h \
	common/NodeHandle.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddress.h \
	common/AbstractLookup.h \
	common/BaseApp.h \
	common/IterativeLookupConfiguration.h \
	common/TransportAddress.h \
	common/NodeVector.h \
	common/BaseOverlay.h \
	common/BootstrapList.h \
	common/RecursiveLookup.h \
	common/BaseRpc.h \
	common/oversim_byteswap.h \
	common/OverSimMessage.h \
	common/Comparator.h \
	$(INET_PROJ)/src/networklayer/contract/IPv6Address.h \
	common/CommonMessages_m.h \
	common/oversim_mapset.h \
	common/OverSimMessage_m.h \
	common/RpcListener.h \
	common/Vector2D.h \
	common/InitStages.h \
	common/OverlayKey.h
$O/underlay/singlehostunderlay/realtimescheduler.o: underlay/singlehostunderlay/realtimescheduler.cc \
	underlay/singlehostunderlay/realtimescheduler.h
$O/underlay/singlehostunderlay/RealworldDevice.o: underlay/singlehostunderlay/RealworldDevice.cc \
	$(INET_PROJ)/src/networklayer/common/InterfaceToken.h \
	underlay/singlehostunderlay/realtimescheduler.h \
	$(INET_PROJ)/src/base/INETDefs.h \
	underlay/singlehostunderlay/RealworldDevice.h \
	underlay/singlehostunderlay/RealworldConnector.h \
	$(INET_PROJ)/src/networklayer/common/IInterfaceTable.h \
	$(INET_PROJ)/src/networklayer/common/InterfaceTableAccess.h \
	$(INET_PROJ)/src/base/NotifierConsts.h \
	$(INET_PROJ)/src/base/ModuleAccess.h \
	$(INET_PROJ)/src/linklayer/contract/MACAddress.h \
	common/PacketParser.h \
	$(INET_PROJ)/src/networklayer/common/InterfaceEntry.h
$O/underlay/singlehostunderlay/apptunoutscheduler.o: underlay/singlehostunderlay/apptunoutscheduler.cc \
	underlay/singlehostunderlay/apptunoutscheduler.h \
	underlay/singlehostunderlay/realtimescheduler.h \
	$(INET_PROJ)/src/networklayer/contract/IPvXAddress.h \
	$(INET_PROJ)/src/base/INETDefs.h \
	$(INET_PROJ)/src/networklayer/contract/IPv6Address.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddress.h
$O/underlay/singlehostunderlay/TunOutDevice.o: underlay/singlehostunderlay/TunOutDevice.cc \
	$(INET_PROJ)/src/networklayer/ipv4/IPDatagram_m.h \
	underlay/singlehostunderlay/realtimescheduler.h \
	underlay/singlehostunderlay/RealworldDevice.h \
	$(INET_PROJ)/src/base/INETDefs.h \
	underlay/singlehostunderlay/tunoutscheduler.h \
	underlay/singlehostunderlay/RealworldConnector.h \
	$(INET_PROJ)/src/networklayer/common/InterfaceTableAccess.h \
	underlay/singlehostunderlay/TunOutDevice.h \
	$(INET_PROJ)/src/base/NotifierConsts.h \
	$(INET_PROJ)/src/linklayer/contract/MACAddress.h \
	$(INET_PROJ)/src/transport/udp/UDPPacket_m.h \
	$(INET_PROJ)/src/networklayer/common/InterfaceToken.h \
	$(INET_PROJ)/src/transport/udp/UDPPacket.h \
	$(INET_PROJ)/src/networklayer/common/IInterfaceTable.h \
	$(INET_PROJ)/src/base/ModuleAccess.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddress.h \
	common/PacketParser.h \
	$(INET_PROJ)/src/networklayer/contract/IPProtocolId_m.h \
	$(INET_PROJ)/src/networklayer/common/InterfaceEntry.h
$O/underlay/singlehostunderlay/tunoutscheduler.o: underlay/singlehostunderlay/tunoutscheduler.cc \
	underlay/singlehostunderlay/realtimescheduler.h \
	underlay/singlehostunderlay/tunoutscheduler.h
$O/underlay/singlehostunderlay/udpoutscheduler.o: underlay/singlehostunderlay/udpoutscheduler.cc \
	underlay/singlehostunderlay/realtimescheduler.h \
	$(INET_PROJ)/src/networklayer/contract/IPvXAddress.h \
	$(INET_PROJ)/src/base/INETDefs.h \
	$(INET_PROJ)/src/networklayer/contract/IPv6Address.h \
	underlay/singlehostunderlay/udpoutscheduler.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddress.h
$O/underlay/singlehostunderlay/UdpOutDevice.o: underlay/singlehostunderlay/UdpOutDevice.cc \
	$(INET_PROJ)/src/networklayer/ipv4/IPDatagram_m.h \
	underlay/singlehostunderlay/realtimescheduler.h \
	underlay/singlehostunderlay/RealworldDevice.h \
	$(INET_PROJ)/src/base/INETDefs.h \
	underlay/singlehostunderlay/RealworldConnector.h \
	underlay/singlehostunderlay/tunoutscheduler.h \
	$(INET_PROJ)/src/networklayer/common/InterfaceTableAccess.h \
	$(INET_PROJ)/src/base/NotifierConsts.h \
	$(INET_PROJ)/src/linklayer/contract/MACAddress.h \
	$(INET_PROJ)/src/transport/udp/UDPPacket_m.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddressResolver.h \
	$(INET_PROJ)/src/networklayer/common/InterfaceToken.h \
	$(INET_PROJ)/src/networklayer/contract/IPvXAddress.h \
	$(INET_PROJ)/src/networklayer/common/IInterfaceTable.h \
	$(INET_PROJ)/src/transport/udp/UDPPacket.h \
	$(INET_PROJ)/src/networklayer/contract/IPv6Address.h \
	$(INET_PROJ)/src/base/ModuleAccess.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddress.h \
	common/PacketParser.h \
	$(INET_PROJ)/src/networklayer/contract/IPProtocolId_m.h \
	$(INET_PROJ)/src/networklayer/common/InterfaceEntry.h \
	underlay/singlehostunderlay/UdpOutDevice.h
$O/underlay/singlehostunderlay/RealworldConnector.o: underlay/singlehostunderlay/RealworldConnector.cc \
	underlay/singlehostunderlay/realtimescheduler.h \
	$(INET_PROJ)/src/base/INETDefs.h \
	underlay/singlehostunderlay/RealworldConnector.h \
	common/PacketParser.h
$O/underlay/singlehostunderlay/stun/udp.o: underlay/singlehostunderlay/stun/udp.cc \
	underlay/singlehostunderlay/stun/stun_udp.h
$O/underlay/singlehostunderlay/stun/stun.o: underlay/singlehostunderlay/stun/stun.cc \
	underlay/singlehostunderlay/stun/stun_udp.h \
	underlay/singlehostunderlay/stun/stun.h

