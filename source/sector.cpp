//-----------------------------------------------------------------------------
// MBR Image Tools
//
// sector.cpp
//
// Methods for the MbrSector class.
//
// Created	: 2 October 2024
// Author	: Michael Pingleton
//-----------------------------------------------------------------------------

#include "../include/mbrimage.hpp"

#include <iostream>
#include <string>

using namespace std;

MbrSector::MbrSector()
{
	address = SectorAddress();
	pBuffer = NULL;
	bufferSize = 0;
}

MbrSector::~MbrSector()
{
	address.~SectorAddress();
	if (bufferSize > 0)
	{
		delete[] pBuffer;
		bufferSize = 0;
	}
}

bool MbrSector::isBootSector()
{
	if (bufferSize <= 0) throw 1;

	return (pBuffer[510] == 0x55 && pBuffer[511] == 0xAA);
}

string MbrSector::toInfoString()
{
	string tmp = "Sector: (";

	tmp.append(address.toInfoString());
	tmp.append(")");

	return tmp;
}
