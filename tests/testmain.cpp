//-----------------------------------------------------------------------------
// MBR Image Tools
// 
// testmain.cpp
//
// Temporary main function for makeshift unit testing.
//
// Created	: 2 October 2024
// Author	: Michael Pingleton
//-----------------------------------------------------------------------------

#include "../include/mbrimage.hpp"

#include <iostream>
#include <string>

using namespace std;

int main(int argc, char** argv)
{
	cout << "Hello world!" << endl;

	SectorAddress s = SectorAddress();
	DriveGeometry g = DriveGeometry();
	MbrPartition p = MbrPartition();

	cout << s.toInfoString() << endl << g.toInfoString() << endl << p.toInfoString() << endl << endl;

	MbrDiskImage diskImage = MbrDiskImage();

	// Standard 3.5-inch diskette.
	diskImage.geometry = DriveGeometry();
	diskImage.geometry.bytes = 512;
	diskImage.geometry.sectors = 18;
	diskImage.geometry.heads = 2;
	diskImage.geometry.cylinders = 80;

	diskImage.initBuffer();

	// Get the first sector, make it bootable.	
	MbrSector sector = MbrSector();
	sector.address = SectorAddress(0);
	sector.initBuffer(diskImage.geometry);

	diskImage.r(&sector);
	sector.pBuffer[510] = 0x55;
	sector.pBuffer[511] = 0xAA;
	diskImage.w(&sector);

	cout << diskImage.toInfoString() << endl;

	return 0;
}
