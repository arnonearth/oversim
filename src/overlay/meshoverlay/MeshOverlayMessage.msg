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

//
// @author Abdollah Ghaffari Sheshjavani (AGH),Behnam Ahmadifar, Yasser Seyyedi
//
cplusplus {{
#include <TransportAddress.h>
#include <CommonMessages_m.h>

// constants for message length in bit
static const int SEQNO_L = 8;
static const int LENGTH_L = 16;
static const int COMMAND_L = 1;


#define ENCAPVIDEOMESSAGE_PACKET_L(msg) (BASEOVERLAY_L(msg) + SEQNO_L + LENGTH_L)
#define ENCAPVIDEOMESSAGE_L(msg) (BASEOVERLAY_L(msg))
#define ENCAPBUFFERMAP_L(msg) (BASEOVERLAY_L(msg)) 
#define ERRORRECOVERYMESSAGE_L(msg) (SEQNO_L + BASEOVERLAY_L(msg) + CHUNKNUMBER_L + TRANSPORTADDRESS_L + LENGTH_L + COMMAND_L)

}}           
class BaseOverlayMessage;
class noncobject TransportAddress;


//
// Frame Request/Response DenaCast-Message definition
//  @author Behnam Ahmadifar, Yasser Seyyedi
//

message EncapVideoMessage extends BaseOverlayMessage
{
 	int seqNo;	//Sequence number of a packetized frame
 	int length; //length of a frame
 	int redundant; // number of redundant packets (for FEC)
	double creationTime; //creationTime of the first packet
	int segmentSize; //Used to indicate the order of the packet
}
message ErrorRecoveryMessage extends BaseOverlayMessage
{
	int seqNo;	//Sequence number of a packetized frame
	int chunkNo; // chunk number
	int filmId; // film id for vod
	int length; // chunk length
	int packetLength; // packet length
	TransportAddress srcNode; // source address
}
//
// BufferMap Exchange definition
//  @author Behnam Ahmadifar, Yasser Seyyedi
//
message EncapBufferMap extends BaseOverlayMessage
{
}
message EncapSelectFilm extends BaseOverlayMessage
{
}

