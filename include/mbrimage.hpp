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

	uint32_t getLBA();
	uint32_t getCylinder(DriveGeometry g);
	uint32_t getHead(DriveGeometry g);
	uint32_t getSector(DriveGeometry g);
	std::string toInfoString();
};

class MbrPartition;

class MbrSector
{
public:
	uint8_t* pBuffer;
	int64_t bufferSize;
	bool isSelfAllocated;

	SectorAddress address;

	MbrSector();
	~MbrSector();

	void initBuffer(DriveGeometry g);
	bool isBootSector();
	bool getPartitionEntry(int n, MbrPartition* pPartition, DriveGeometry g);
	void setPartitionEntry(int n, MbrPartition* pPartition, DriveGeometry g);
	void readFile(std::string filename);
	void saveFile(std::string filename);
	std::string toInfoString();
};

class MbrPartition
{
public:
	uint8_t type;
	SectorAddress begin;
	uint32_t sectors;

	MbrPartition();
	~MbrPartition();

	void setActive();
	SectorAddress absoluteAddress(uint32_t i);
	std::string toInfoString();
};

class MbrDiskImage
{
public:
	uint8_t* pBuffer;
	int64_t bufferSize;
	bool isSelfAllocated;

	DriveGeometry geometry;
	std::vector<MbrPartition> partitions;

	MbrDiskImage();
	~MbrDiskImage();

	void initBuffer();
	void r(MbrSector* pSector);
	void w(MbrSector* pSector);
	bool isBootable();
	void readFile(std::string filename);
	void saveFile(std::string filename);
	std::string toInfoString();
};

#endif
