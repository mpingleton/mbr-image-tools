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

	for (int i = 0; i < 2880; i++)
	{
		SectorAddress sa = SectorAddress(i);
		cout << "-> LBA: " << sa.getLBA();
		cout << "\tCyl: " << sa.getCylinder(diskImage.geometry);
		cout << "\tHed: " << sa.getHead(diskImage.geometry);
		cout << "\tSec: " << sa.getSector(diskImage.geometry);
		cout << endl;
	}

	MbrPartition partition = MbrPartition();
	partition.setActive();
	partition.begin = SectorAddress(5);
	partition.sectors = 2874;

	MbrSector mbrSectorA = MbrSector();
	MbrSector mbrSectorB = MbrSector();
	mbrSectorA.address = SectorAddress(0);
	mbrSectorA.initBuffer(diskImage.geometry);
	mbrSectorB.address = SectorAddress(0);
	mbrSectorB.initBuffer(diskImage.geometry);

	diskImage.r(&mbrSectorA);
	mbrSectorA.setPartitionEntry(0, &partition, diskImage.geometry);
	diskImage.w(&mbrSectorA);

	cout << "Sector written" << endl;

	diskImage.r(&mbrSectorB);
	MbrPartition p2 = MbrPartition();
	mbrSectorB.getPartitionEntry(0, &p2, diskImage.geometry);
	cout << p2.toInfoString() << endl;

	int i = 0;
	while (i < diskImage.geometry.bytes)
	{
		cout << " -> ";
		for (int l = 0; l < 50; l++)
		{
			if (i >= diskImage.geometry.bytes) break;
			cout << to_string(mbrSectorB.pBuffer[i]) << " ";
			i++;
		}
		cout << endl << endl;
	}

	return 0;
}
