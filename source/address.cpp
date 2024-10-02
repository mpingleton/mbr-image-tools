//-----------------------------------------------------------------------------
// MBR Image Tools
//
// address.cpp
//
// Methods for the SectorAddress class.
//
// Created	: 2 October 2024
// Author	: Michael Pingleton 
//-----------------------------------------------------------------------------

#include "../include/mbrimage.hpp"

#include <iostream>
#include <string>

using namespace std;

SectorAddress::SectorAddress()
{
	address = 0;
}

SectorAddress::SectorAddress(uint32_t lba)
{
	address = lba;
}

SectorAddress::SectorAddress(DriveGeometry g, uint32_t c, uint32_t h, uint32_t s)
{
	address = (c * g.heads + h) * g.sectors + (s - 1);
}

SectorAddress::~SectorAddress()
{
	address = 0;
}

uint32_t SectorAddress::getLBA()
{
	return address;
}

uint32_t SectorAddress::getCylinder(DriveGeometry g)
{
	return address / (g.heads * g.sectors);
}

uint32_t SectorAddress::getHead(DriveGeometry g)
{
	return (address / g.sectors) % g.heads;
}

uint32_t SectorAddress::getSector(DriveGeometry g)
{
	return (address % g.sectors) + 1;
}

string SectorAddress::toInfoString()
{
	string tmp = "Sector address: ";

	tmp.append(to_string(address));

	return tmp;
}
