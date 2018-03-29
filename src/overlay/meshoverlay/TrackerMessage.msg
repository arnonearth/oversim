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
// @author Abdollah Ghaffari Sheshjavani(AGH),Behnam Ahmadifar, Yasser Seyyedi
//

cplusplus {{
#include <TransportAddress.h>
#include <CommonMessages_m.h>
#include <IPvXAddress.h>
// constants for message length in bit
static const int TRACKERCOMMAND_L = 4;
#define TRACKERMESSAGE_L(msg) ( TRANSPORTADDRESS_L + TRACKERCOMMAND_L + BASEOVERLAY_L(msg))

}} 
class BaseOverlayMessage;
class noncobject TransportAddress;
class noncobject IPvXAddress;

enum TRACKERCommand
{
    NEIGHBOR_REQUEST = 0;    // JOIN_REQUEST-Message
    NEIGHBOR_RESPONSE = 1;    // JOIN_RESPONSE-Message
    SELF_REGISTER = 3;		//Self Register 
    SELF_UNREGISTER = 4;
    REMAIN_NEIGHBOR = 5;
    Request_ListOfFilm=6;
    ChangeFilm=7;
    CascadeNumRequest=8;// when a peer seek forward used-AGH
    NEIGHBOR_Exit = 9;
    You_UnRegistered=10;
}
packet TrackerMessage extends BaseOverlayMessage 
{
    int command @enum(TRACKERCommand);
	TransportAddress srcNode;
	TransportAddress neighbors[];
	int neighborSize;
	bool isServer;//AGH
	int remainNeighbor;
	int cascadenumber;//AGH
	int hopCount;//AGH
	bool isHeadnode;
	bool connectDirectToServer;//AGH
	int selectedFilmNum;//AGH
	int selectedFilmStartTime;// for seek fwd in the film (AGH)
	int previousFilm;// for change film (AGH)
	int previousFilmStartTime;// for change film (AGH)
	int previousFilmEndTime;// for previous Film 
	bool specialInteractiveNeghborReq;// in structiure 3 &4 use for intractive peers
	IPvXAddress connectedAccess;
}
