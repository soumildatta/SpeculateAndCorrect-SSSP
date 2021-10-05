/*
 * tConfig.h
 *
 *  Created on: Oct 5, 2021
 *      Author: sdatta
 */

#ifndef TCONFIG_H
#define TCONFIG_H

#include <filesystem>
using std::filesystem::path;

#include <cstdint>

// TEMP
#include <iostream>
using std::cout;
using std::endl;

enum eFlags:int
{
	inputFileFlag = 0,
	statsFileFlag,
	sourceNodeFlag,
	verboseFlag,
	eFlag_N
};

struct tConfig
{
	path		inputFilename;
	path 		statsFilename;
	uint32_t	sourceNode;
	bool		verboseFlag;

	void parseCMD(int argc, char *argv[])
	{
		cout << "Work in progress" << endl;
	}
};

#endif /* TCONFIG_H */
