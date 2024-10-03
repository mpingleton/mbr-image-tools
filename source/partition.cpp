//-----------------------------------------------------------------------------
// MBR Image Tools
//
// partition.cpp
//
// Methods for the MbrPartition class.
//
// Created	: 2 October 2024
// Author	: Michael Pingleton
//-----------------------------------------------------------------------------

#include "../include/mbrimage.hpp"

#include <iostream>
#include <string>

using namespace std;

MbrPartition::MbrPartition()
{
	type = 0;
	begin = SectorAddress();
	sectors = 0;
}

MbrPartition::~MbrPartition()
{
	type = 0;
	begin.~SectorAddress();
	sectors = 0;
}

void MbrPartition::setActive()
{
	type = 0x01;
};

SectorAddress MbrPartition::absoluteAddress(uint32_t i)
{
	SectorAddress a = SectorAddress(begin.address + i);
	return a;
}

string MbrPartition::toInfoString()
{
	string tmp = "Partition entry: (";

	tmp.append("Type: ");
	tmp.append(to_string(type));
	tmp.append("; ");

	tmp.append(begin.toInfoString());
	tmp.append("; ");

	tmp.append("Sector count: ");
	tmp.append(to_string(sectors));
	tmp.append(")");

	return tmp;
}
