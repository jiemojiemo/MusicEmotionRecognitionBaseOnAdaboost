#pragma once
#include "fileVector.h"
#include "common.h"

template<typename T>
class WavFileVector : public FileVector<T>
{
public:
	using FileVector::FileVector;
private:
	virtual void readDataFromFile();
};

template<typename T>
inline void WavFileVector<T>::readDataFromFile()
{
	FILE* fd = fopen(fileName_.c_str(), "rb");
	ON_SCOPE_EXIT([&fd]() {fclose(fd); });
	if (fd == nullptr)
	{
		fprintf(stderr, "Cannot open the file!\n");
		exit(-1);
	}
	fseek(fd, 0, SEEK_END);
	unsigned long fileSize = ftell(fd) - 0x2c;
	fseek(fd, 0x2c, SEEK_SET);
	short* data = (short*)malloc(fileSize);
	ON_SCOPE_EXIT([&data]() {free (data); });
	fread(data, 1, fileSize, fd);
	int sampleCount = fileSize / sizeof(short);

	//copy data to vector
	for (int i = 0; i < sampleCount; ++i)
	{
		dataVector_.push_back(data[i]);
	}
}
