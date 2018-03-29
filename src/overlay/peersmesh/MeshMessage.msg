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
//@author Abdollah Ghaffari Sheshjavani(AGH), Yasser Seyyedi, Behnam Ahmadifar
//

cplusplus {{
#include <TransportAddress.h>
#include <CommonMessages_m.h>

// constants for message length in bit
static const int MESHCOMMAND_L = 4;
#define MESHMESSAGE_L(msg) ( TRANSPORTADDRESS_L + MESHCOMMAND_L + BASEOVERLAY_L(msg))
#define IMPROVEMENT_L(msg) ( TRANSPORTADDRESS_L + 5*MESHCOMMAND_L )
#define IMPROVEMENTCHANGEPLACE_L(msg) ( 2*TRANSPORTADDRESS_L + 5.5*MESHCOMMAND_L +3)
}}           // noncobject means it's not a message class.
class BaseOverlayMessage;
class noncobject TransportAddress;

enum MESHCommand
{
    JOIN_REQUEST = 0;    // JOIN_REQUEST-Message
    JOIN_RESPONSE = 1;    // JOIN_RESPONSE-Message
    JOIN_ACK = 2;   	 // JOIN_ACK-Message
    JOIN_DENY = 3;    // JOIN_DENY-Message
    DISCONNECT = 4;    // DISCONNECT-Message
    ALIVE = 5;
    EMERGENCY_jOIN_REQUEST=6;
    EMERGENCY_DISCONNECT=7;
    EMERGENCY_RESPONSE=8;
    EMERGENCY_ACK=9;
    EMERGENCY_DENY=10;
    SEEK_FWD=11;
}
enum improveCommand
{
    IMPROVE_REQUEST = 0;   // IMPROVE_REQUEST-Message
    IMPROVE_RESPONSE = 1; // IMPROVE_RESPONSE-Message
    IMPROVE_ACK = 2;   	 // IMPROVE_ACK-Message
    IMPROVE_DENY = 3;   // IMPROVE_DENY-Message
    
}
enum improvechangePlaceCommand
{
    CHANGE_PLACE_REQUEST = 0;   // IMPROVE_REQUEST-Message
    CHANGE_PLACE_RESPONSE = 1; // IMPROVE_RESPONSE-Message
    CHANGE_PLACE_ACK = 2;   	 // IMPROVE_ACK-Message
    CHANGE_PLACE_DENY = 3;   // IMPROVE_DENY-Message
    CHANGE_NEIGHBOR_TYPE = 4;
    
}
//	join message in SimpleMesh 
//  @author Abdollah Ghaffari Sheshjavani(AGH), Behnam Ahmadifar, Yasser Seyyedi
//
packet MeshMessage extends BaseOverlayMessage
{
	int command @enum(MESHCommand);
	TransportAddress srcNode; 
	TransportAddress OtherNode;
	int disconnecthopCount;
	int filmId;
	int filmStartTime;// !its vod (AGH)
	int filmEndTime;// for previos neighbor
	bool isServer;
	bool connectDirectToServer;// AGH
	bool isHeadNode;//use in my structure-AGH
	int cascadeNumber;//AGH
	int hopCount;//AGH
	bool specialInteractiveNeghborReq;// in structiure 3 &4 use for intractive peers
	 // address of the node that sent the message
}
//  @author Abdollah Ghaffari Sheshjavani(AGH)
packet improvementMessage extends BaseOverlayMessage
{
	int command @enum(improveCommand);
	TransportAddress srcNode; 
	TransportAddress neighbors[];
	int filmId;
	int cascadeNumber;//AGH
	int score;//AGH
}
//  @author Abdollah Ghaffari Sheshjavani(AGH)
packet improvementchangePlaceMessage extends BaseOverlayMessage
{
	int command @enum(improvechangePlaceCommand);
	TransportAddress srcNode; 
	TransportAddress previusNode;
	int filmId;
	int downBandwidth;
	int recieveTime;
	bool isServer;
	bool connectDirectToServer;// AGH
	bool isHeadNode;//use in my structure-AGH
	int cascadeNumber;//AGH
	short neighborType;// 0 or 1 is active or passive
}