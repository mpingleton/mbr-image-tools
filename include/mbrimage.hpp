//-----------------------------------------------------------------------------
// MBR Image Tools
//
// mbrimage.hpp
//
// The primary header file for the library.
//
// Created	: 1 October 2024
// Author	: Michael Pingleton
//-----------------------------------------------------------------------------

#ifndef MBR_IMAGE_HPP
#define MBR_IMAGE_HPP

#include <string>

class DriveGeometry
{
public:
	int bytes;
	int sectors;
	int heads;
	int cylinders;

	std::string toInfoString();
};

class SectorAddress
{
public:
	int address;

	void setFromCHS(DriveGeometry g, int c, int h, int s);
	int getCylinder(DriveGeometry g);
	int getHead(DriveNumber g);
	int getSector(DriveNumber g);
	std::string toInfoString();
};

class MbrSector
{
	SectorAddress address;
	char* pBuffer;
	int bufferSize;

	std::string toInfoString();
};

class MbrPartition
{
public:
	int type;
	SectorAddress begin;
	int sectors;

	std::string toInfoString();
};

class MbrDiskImage
{
public:
	char* pBuffer;
	int bufferSize;
	int cursorPosition;

	std::string toInfoString();
};

#endif
