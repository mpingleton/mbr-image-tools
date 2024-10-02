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
	cursorPosition = 0;
	geometry = DriveGeometry();
}

MbrDiskImage::~MbrDiskImage()
{
	if (bufferSize > 0)
	{
		delete[] pBuffer;
		bufferSize = 0;
	}
	
	cursorPosition = 0;
	geometry.~DriveGeometry();
	partitions.clear();
}

string MbrDiskImage::toInfoString()
{
	string tmp = "Disk image: (";

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
