//
//Copyright (C) 2012 Tarbiat Modares University All Rights Reserved.
// http://ece.modares.ac.ir/mnl/
// This system was designed and developed at the DML(Digital Media Lab http://dml.ir/)
// under supervision of Dr. Behzad Akbari (http://www.modares.ac.ir/ece/b.akbari)
///
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


// @file BasePlayer.ned
// @author Abdollah Ghaffari Sheshjavani(AGH),Behnam Ahmadifar, Yasser Seyyedi
//

package oversim.tier2.vodplayer;
import oversim.common.BaseApp;
//
// The main module of the BasePlayer implementation
//
simple BasePlayer extends BaseApp
{
    parameters:
        @class(BasePlayer);
        int Fps;				//Frame per second for playing
        int numOfBFrame;		// Number of "B" frame type between two "P" frames or "I" and "P" frames 
        int gopSize;			// Number of frames in a group of picture
        int videoAverageRate1 @unit(Kbps);
       	int videoAverageRate2 @unit(Kbps);
       	int videoAverageRate3 @unit(Kbps);
       	int videoAverageRate4 @unit(Kbps);
       	int videoAverageRate5 @unit(Kbps);
       	int videoAverageRate6 @unit(Kbps);
       	int videoAverageRate7 @unit(Kbps);
       	int videoAverageRate8 @unit(Kbps);
       	int videoAverageRate9 @unit(Kbps);
       	int videoAverageRate10 @unit(Kbps);	
        @display("t=elapsed Time: hour:min:sec:frame;tt=Additional tooltip information;i=device/player");
        double measuringTime @unit(s);
}
