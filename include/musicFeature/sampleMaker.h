#pragma once
#include "segmenter.h"
#include "common.h"
#include <memory>
#include <string>
#include <vector>
class SampleMaker
{
	DISALLOW_COPY_AND_ASSIGN(SampleMaker);
public:
	SampleMaker(const std::string& wavInputPath);

	/*
		@note Setup must be call first
	*/
	void Setup();

	/*
		@param numSegments divice the wav file
	*/
	void Run( int numSegments );


	/*
		@bref get the feature array
	*/
	std::vector<int> GetFeatureIndexArray()const;

	/*
		@bref set the feature array, just extrach what you want
		@see the featuer index in FeatureExtract::getFeature
	*/
	void SetFeatureIndexArray(const std::vector<int>& featureIndexArray);

	/*
		@note must call Run first
		@bref get feature result
	*/
	std::vector<std::vector<double>> GetResult()const;

	/*
		@bref write a sample to file
	*/
	void WriteToFile(const std::string& label, const std::string& outputFileName="training.txt");
private:
	std::vector<int> m_featureIndexArray;
	std::vector<std::vector<double>> m_result;
	Segmenter m_segmenter;
	
};
