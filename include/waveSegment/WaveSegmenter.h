#pragma once
#include "common.h"
#include <string>
#include <vector>
#include <memory>

class WaveSegmenter
{
	DISALLOW_COPY_AND_ASSIGN(WaveSegmenter);
public:
	WaveSegmenter(const std::string& filePath);
	~WaveSegmenter();
	void Setup();
	double GetDuration();
	/*
		@note segment 16-bit mono wav file.if 16-bit stereo wav file,
		will get a mono result
	*/
	std::vector<int16_t> GetMonoSegment( unsigned int sampleBegIndex, unsigned int sampleEndIndex);
//	std::vector<int8_t> GetMonoSegment(unsigned int msBegin, unsigned int msEnd);//1s = 1000ms
	std::vector<int16_t> GetMonoSegment(double timeBegin, double timeEnd);//
	void SetFilePath(const std::string& filePath);
private:
	class Impl;
	Impl* m_pImpl;
};