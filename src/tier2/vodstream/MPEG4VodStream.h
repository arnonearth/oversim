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

/**
 * @file MPEG4VodStream.h
 * @author Abdollah Ghaffari Sheshjavani, Behnam Ahmadifar, Yasser Seyyedi
 */

#ifndef MPEG4VODSTREAM_H_
#define MPEG4VODSTREAM_H_

#include "BaseVodStream.h"
#include "VideoMessage_m.h"

class MPEG4VodStream :public BaseVodStream
{
	/**
	 * function for reading file specialize for MPEG4 tracefile
	 */
	int readFrames(std::ifstream * file, int filmno);

};
#endif /* MPEG4VODSTREAM_H_ */
