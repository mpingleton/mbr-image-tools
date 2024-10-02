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

#include <stdint.h>
#include <string>
#include <vector>

class DriveGeometry
{
public:
	uint32_t bytes;
	uint32_t sectors;
	uint32_t heads;
	uint32_t cylinders;

	DriveGeometry();
	~DriveGeometry();

	uint32_t totalBytes();
	uint32_t totalSectors();
	uint32_t totalTracks();
	std::string toInfoString();
};

class SectorAddress
{
public:
	uint32_t address;

	SectorAddress();
	SectorAddress(uint32_t lba);
	SectorAddress(DriveGeometry g, uint32_t c, uint32_t h, uint32_t s);
	~SectorAddress();

	uint32_t getCylinder(DriveGeometry g);
	uint32_t getHead(DriveGeometry g);
	uint32_t getSector(DriveGeometry g);
	std::string toInfoString();
};

class MbrSector
{
	SectorAddress address;
	uint8_t* pBuffer;
	int64_t bufferSize;

	MbrSector();
	~MbrSector();

	bool isBootSector();

	std::string toInfoString();
};

class MbrPartition
{
public:
	int type;
	SectorAddress begin;
	uint32_t sectors;

	MbrPartition();
	~MbrPartition();

	std::string toInfoString();
};

class MbrDiskImage
{
public:
	uint8_t* pBuffer;
	int64_t bufferSize;
	int64_t cursorPosition;
	DriveGeometry geometry;
	std::vector<MbrPartition> partitions;

	MbrDiskImage();
	~MbrDiskImage();

	std::string toInfoString();
};

#endif
