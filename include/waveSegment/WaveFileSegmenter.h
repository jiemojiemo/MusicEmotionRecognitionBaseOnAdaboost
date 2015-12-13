#pragma once
#include "WaveSegmenter.h"
class WaveFileSegmenter
{
	DISALLOW_COPY_AND_ASSIGN(WaveFileSegmenter);
public:
	WaveFileSegmenter(const std::string& srcFilePath);
	~WaveFileSegmenter();
	void Setup();

	double GetSourceFileDuration();
	void doSegment( double timeBeg, double timeEnd,const std::string& newFileName="segment.wav" );

private:
	class Impl;
	Impl* m_pImpl;
};