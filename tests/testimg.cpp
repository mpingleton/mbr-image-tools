//-----------------------------------------------------------------------------
// MBR Image Tools
//
// testimg.cpp
//
// Temporary main function for makeshift unit testing.
//
// Created	: 2 October 2024
// Author	: Michael Pingleton
//-----------------------------------------------------------------------------

#include "../include/mbrimage.hpp"

#include <iostream>

using namespace std;

int main(int argc, char** argv)
{
	MbrDiskImage diskImage = MbrDiskImage();
	diskImage.geometry = DriveGeometry();
	diskImage.geometry.bytes = 512;
	diskImage.geometry.sectors = 18;
	diskImage.geometry.heads = 2;
	diskImage.geometry.cylinders = 80;
	diskImage.initBuffer();

	MbrSector bootSector = MbrSector();
	bootSector.address = SectorAddress(0);
	bootSector.initBuffer(diskImage.geometry);
	bootSector.readFile("testbootloader.img");

	cout << "Bootloader found: " << bootSector.toInfoString() << endl;

	MbrPartition p1 = MbrPartition();
	p1.setActive();
	p1.begin = SectorAddress(10);
	p1.sectors = 200;

	MbrPartition p2 = MbrPartition();
	p2.setActive();
	p2.begin = SectorAddress(210);
	p2.sectors = 2669;

	bootSector.setPartitionEntry(0, &p1, diskImage.geometry);
	bootSector.setPartitionEntry(1, &p2, diskImage.geometry);
	
	diskImage.w(&bootSector);

	diskImage.saveFile("floppy.img");
	return 0;
}
