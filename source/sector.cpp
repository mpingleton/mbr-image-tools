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
	pBuffer = NULL;
	bufferSize = 0;
	isSelfAllocated = false;

	address = SectorAddress();	
}

MbrSector::~MbrSector()
{
	if (isSelfAllocated)
	{
		delete[] pBuffer;
		bufferSize = 0;
		isSelfAllocated = false;
	}

	address.~SectorAddress();
}

void MbrSector::initBuffer(DriveGeometry g)
{
	if (isSelfAllocated) delete[] pBuffer;
	
	bufferSize = g.bytes;
	isSelfAllocated = true;
	pBuffer = new uint8_t[bufferSize]();
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
