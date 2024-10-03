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

bool MbrSector::getPartitionEntry(int n, MbrPartition* pPartition, DriveGeometry g)
{
	if (n < 0 || n > 3 || pPartition == NULL) return false;

	uint16_t offset = 446 + (n * 16);
	
	uint8_t status = pBuffer[offset];
	
	if (status == 0) return false;

	uint8_t hFirst = pBuffer[offset + 1];
	uint8_t sFirst = pBuffer[offset + 2] & 0x3F;
	uint16_t cFirst = ((pBuffer[offset + 2] & 0xC0) << 2) | pBuffer[offset + 3];
	uint8_t hLast = pBuffer[offset + 5];
	uint8_t sLast = pBuffer[offset + 6] & 0x3F;
	uint16_t cLast = ((pBuffer[offset + 6] & 0xC0) << 2) | pBuffer[offset + 7];

	SectorAddress chsFirstAddress = SectorAddress(g, cFirst, hFirst, sFirst);
	SectorAddress chsLastAddress = SectorAddress(g, cLast, hLast, sLast);

	pPartition->type = pBuffer[offset + 4];

	uint32_t lbaFirst = 0;
	for (int i = 0; i < 4; i++)
	{
		((uint8_t*)&lbaFirst)[i] = pBuffer[offset + 8 + i];
		((uint8_t*)&pPartition->sectors)[i] = pBuffer[offset + 12 + i];
	}

	SectorAddress lbaFirstAddress = SectorAddress(lbaFirst);
	SectorAddress lbaLastAddress = SectorAddress(lbaFirst + pPartition->sectors);

	cout << "First LBA: " << lbaFirstAddress.toInfoString() << "; Last LBA: " << lbaLastAddress.toInfoString() << endl;
	cout << "First CHS: " << chsFirstAddress.toInfoString() << "; Last CHS: " << chsLastAddress.toInfoString() << endl;

	if (chsFirstAddress.address != lbaFirstAddress.address) throw 2; // TODO: If problems happen here, it may be because the disk is larger than 1024 cylinders, which would cause integer overflow, then exception here.
	else if (chsLastAddress.address != lbaLastAddress.address) throw 3;

	pPartition->begin = lbaFirstAddress;

	return pPartition->type > 0 && status > 0;
}

void MbrSector::setPartitionEntry(int n, MbrPartition* pPartition, DriveGeometry g)
{
	if (n < 0 || n > 3) return;

	uint16_t offset = 446 + (n * 16);

	if (pPartition == NULL)
	{
		for (int i = 0; i < 16; i++) pBuffer[offset + i] = 0;
		return;
	}

	pBuffer[offset] = 0x80;

	SectorAddress end = SectorAddress(pPartition->begin.address + pPartition->sectors);
	uint8_t hFirst = pPartition->begin.getHead(g); // TODO: Coercing these into an 8-bit value may cause problems.
	uint8_t sFirst = pPartition->begin.getSector(g); // If crashes happen upon trying to write a partition table to a disk with more than 1024 cylinders, this may be the issue.
	uint16_t cFirst = pPartition->begin.getCylinder(g);
	uint8_t hLast = end.getHead(g);
	uint8_t sLast = end.getSector(g);
	uint16_t cLast = end.getCylinder(g);

	pBuffer[offset + 1] = hFirst;
	pBuffer[offset + 2] = (sFirst & 0x3F) | ((cFirst & 0xC0) >> 2);
	pBuffer[offset + 3] = cFirst & 0xFF;
	pBuffer[offset + 5] = hLast;
	pBuffer[offset + 6] = (sLast & 0x3F) | ((cLast & 0xC0) >> 2);
	pBuffer[offset + 7] = cLast & 0xFF;

	pBuffer[offset + 4] = pPartition->type;

	uint32_t lbaFirst = pPartition->begin.getLBA();
	for (int i = 0; i < 4; i++)
	{
		pBuffer[offset + 8 + i] = ((uint8_t*)&lbaFirst)[i];
		pBuffer[offset + 12 + i] = ((uint8_t*)&pPartition->sectors)[i];
	}
}

string MbrSector::toInfoString()
{
	string tmp = "Sector: (";

	tmp.append(address.toInfoString());
	tmp.append(")");

	return tmp;
}
