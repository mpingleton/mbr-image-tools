//-----------------------------------------------------------------------------
// MBR Image Tools
//
// image.cpp
//
// Methods for the MbrDiskImage class.
//
// Created	: 2 October 2024
// Author	: Michael Pingleton
//-----------------------------------------------------------------------------

#include "../include/mbrimage.hpp"

#include <iostream>
#include <string>

using namespace std;

MbrDiskImage::MbrDiskImage()
{
	pBuffer = NULL;
	bufferSize = 0;
	geometry = DriveGeometry();
}

MbrDiskImage::~MbrDiskImage()
{
	if (isSelfAllocated)
	{
		delete[] pBuffer;
		bufferSize = 0;
		isSelfAllocated = false;
	}
	
	geometry.~DriveGeometry();
	partitions.clear();
}

void MbrDiskImage::initBuffer()
{
	if (isSelfAllocated) delete[] pBuffer;

	bufferSize = geometry.totalBytes();
	isSelfAllocated = true;
	pBuffer = new uint8_t[bufferSize]();
}

void MbrDiskImage::r(MbrSector* pSector)
{
	if (pSector->bufferSize != geometry.bytes) throw 1;

	for (int i = 0; i < geometry.bytes; i++) pSector->pBuffer[i] = pBuffer[pSector->address.address + i];
}

void MbrDiskImage::w(MbrSector* pSector)
{
	if (pSector->bufferSize != geometry.bytes) throw 1;

	for (int i = 0; i < geometry.bytes; i++) pBuffer[pSector->address.address + i] = pSector->pBuffer[i];
}

bool MbrDiskImage::isBootable()
{
	MbrSector s = MbrSector();
	s.initBuffer(geometry);
	s.address.address = 0;

	r(&s);

	return s.isBootSector();
}

string MbrDiskImage::toInfoString()
{
	string tmp = "Disk image: (";

	tmp.append("Bootable: ");
	if (isBootable()) tmp.append("Yes");
	else tmp.append("No");
	tmp.append("; ");

	tmp.append("Drive size (bytes): ");
	tmp.append(to_string(geometry.totalBytes()));
	tmp.append("; ");

	tmp.append(geometry.toInfoString());
	tmp.append("; ");

	tmp.append("Partitions: ");
	tmp.append(to_string(partitions.size()));
	tmp.append(")");

	for (int i = 0; i < partitions.size(); i++)
	{
		tmp.append("\t->");
		tmp.append(partitions[i].toInfoString());
		tmp.append("\n");
	}

	return tmp;
}
