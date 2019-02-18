/****************************************************************************	
*	Operator Console - an extensible user interface for the Imatest IT 		*
*	library																	*
*	Copyright (C) 2013 Imatest LLC.											*
*																			*
*	This program is free software: you can redistribute it and/or modify	*
*	it under the terms of the GNU General Public License as published by	*
*	the Free Software Foundation, either version 3 of the License, or		*
*	(at your option) any later version.										*
*																			*
*	This program is distributed in the hope that it will be useful,			*
*	but WITHOUT ANY WARRANTY; without even the implied warranty of			*
*	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the			*
*	GNU General Public License for more details.							*
*																			*
*	You should have received a copy of the GNU General Public License		*
*	along with this program.  If not, see <http://www.gnu.org/licenses/>. 	*
****************************************************************************/

#ifndef BLEMISH_TEST_H
#define BLEMISH_TEST_H

#include "imagetest.h"

///
/// contains the details of an Blemish test to be performed including all relevant fields for determining pass or fail
///
class BlemishTest : public ImageTest
{
public:
	BlemishTest(void);
	virtual ~BlemishTest(void);

protected:
	void ParseResults(string &results);
	void GetResults(const JSONNode& data, const JSONNode &passfail);
};

#endif
