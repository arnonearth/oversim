//
// Copyright (C) 2012 Tarbiat Modares University All Rights Reserved.
// http://ece.modares.ac.ir/mnl/
// This system was designed and developed at the DML(Digital Media Lab http://dml.ir/)
// under supervision of Dr. Behzad Akbari (http://www.modares.ac.ir/ece/b.akbari)
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// @file Mpeg4VodStream.ned
// @author Abdollah Ghaffari sheshjavani(AGH)
//

package oversim.tier2.vodstream;
import oversim.common.BaseApp;

simple BaseVodStream extends BaseApp
{
    parameters:
        @class(BaseVodStream);
        int nof; //numberOfFilms in this simulation it can between 1 and 10 (max 10, if you want change this should change trace files number)
        string traceFile1;  // Address of trace file in your system
        string traceFile2;  // Address of trace file in your system
        string traceFile3;  // Address of trace file in your system
        string traceFile4;  // Address of trace file in your system
        string traceFile5;  // Address of trace file in your system
        string traceFile6;  // Address of trace file in your system
        string traceFile7;  // Address of trace file in your system
        string traceFile8; // Address of trace file in your system
        string traceFile9;  // Address of trace file in your system
        string traceFile10;  // Address of trace file in your system
        int Fps;// Frame per second of a video
        int gopSize; // number of frame available in one GoP (Group of picture)*/
     	int chunkSize  @unit(Frames);
	   	int numOfBFrame;				
        @display("t=elapsed Time: hour:min:sec:frame;tt=Additional tooltip information;i=device/odStream");

}
