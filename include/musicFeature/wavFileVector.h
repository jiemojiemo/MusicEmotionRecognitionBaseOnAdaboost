#pragma once
#include "fileVector.h"
#include "common.h"



class WavFileVector : public FileVector<int16_t>
{
	DISALLOW_COPY_AND_ASSIGN(WavFileVector);
public:
	using FileVector<int16_t>::FileVector;
private:
	virtual void readDataFromFile();
};

