//-----------------------------------------------------------------------------
// MBR Image Tools
//
// geometry.cpp
//
// Methods for the DriveGeometry class.
//
// Created	: 2 October 2024
// Author	: Michael Pingleton
//-----------------------------------------------------------------------------

#include "../include/mbrimage.hpp"

#include <iostream>
#include <string>

using namespace std;

DriveGeometry::DriveGeometry()
{
	bytes = 0;
	sectors = 0;
	heads = 0;
	cylinders = 0;
}

DriveGeometry::~DriveGeometry()
{
	bytes = 0;
	sectors = 0;
	heads = 0;
	cylinders = 0;
}

uint32_t DriveGeometry::totalBytes()
{
	return totalSectors() * bytes;
}

uint32_t DriveGeometry::totalSectors()
{
	return totalTracks() * sectors;
}

uint32_t DriveGeometry::totalTracks()
{
	return cylinders * heads;
}

string DriveGeometry::toInfoString()
{
	string tmp = "Drive geometry: (";

	tmp.append("Bytes per sector: ");
	tmp.append(to_string(bytes));
	tmp.append("; ");

	tmp.append("Sectors per track: ");
	tmp.append(to_string(sectors));
	tmp.append("; ");

	tmp.append("Heads: ");
	tmp.append(to_string(heads));
	tmp.append("; ");

	tmp.append("Cylinder: ");
	tmp.append(to_string(cylinders));
	tmp.append(")");

	return tmp;
}
