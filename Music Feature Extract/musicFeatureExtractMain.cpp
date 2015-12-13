/**
*	@file main.cpp
*	@class main
*	
*	@author Chad R. Befus, Chris Sanden, Cody Rioux
*	@version 0.1.3:Bravo
*
*	License
*	-----------
*	Copyright (c) 2010 Chad R. Befus, Chris Sanden
*	
*	CAMEL (Content-based Audio and Music Extraction Library) is unrestricted, open 
*	source "software", with respect, but not limited, to modification, use, 
*	publishing, and distribution, subject to the following conditions:
*	
*	1. This copyright notice is maintained in its current form throughout CAMEL.
*	
*	2. Any publication or distribution credits the usage of CAMEL appropriately.
*	
*	This software is provided "as is" without warranty. The authors are in no way
*	liable for any misuse or damages arising from the use, modification or 
*	distribution of it or any part thereof.
*
*	Description
*	-----------
*	@brief the default main file -- extract segments/features from a set of passed in values
*
*	
*	Given PCM ASCII text file name, An integer corresponding to a feature type, An integer
*	correspoding to a segmentation type, An integer corresponding to the number of desired
*	segments, and a text file name for output, this code segments the PCM data, extracts
*	the features and outputs the results to the file. 
*
*	Required Dependencies
*	-----------
*	std::vector
*	std::string
*	std::cstdlib
*	std::iostream
*	std::sstream
*	std::fstream
*	segmenter.h
*
*	Example Usage
*	-----------
*	~$ featExtract samplePCM.txt 1 2 10 output.txt
*
*
*	More examples to be found in example folder
*
*	@todo
*	
*
**/


#include <cstdlib>
#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>
#include <string>
#include <numeric>
#include <algorithm>
#include "common.h"
#include "sampleMaker.h"
#include "segmenter.h"

using namespace std;

template <typename T>
T Calcvariance(const vector<T>& vec)
{
	auto sum = accumulate(begin(vec), end(vec), T(0));
	auto mean = sum / vec.size();

	T accum = 0;
	for_each(begin(vec), end(vec), [&](const T t)
	{
		accum += (t - mean) * (t - mean);
	});

	return sqrt(accum / (vec.size() - 1));
}

/**
*	get domain for a window data
*	up to windowSize data processing
**/
void DomainTest()
{
	Domain domain;
	domain.setFileName("samplePCM.txt");
	domain.setup();
	domain.setWindowSize(1024);
	domain.setStartPosition(0);
	domain.setEndPosition(10000);
	vector<double> domainVec = domain.getFrequencyDomainWindow(0);
	for (int i = 0; i < 512; ++i) {
		cout << 44100.0/1024 * i << ": "<<domainVec[i] << "\t";
	}
	
}

void MainTest(int argc, char *argv[])
{
	/*
	*/
	//get the string name of the PCM file
	string strFileName = argv[1];	//samplePCM.txt

									//get the string feature type and convert to an int
	string strFeatureType = argv[2];	//1
	int intFeatureType = 0;
	stringstream ssFeatureType(strFeatureType);
	ssFeatureType >> intFeatureType;

	//get the string segment type and convert to an int
	string strSegmentType = argv[3];	//2
	int intSegmentType = 0;
	stringstream ssSegmentType(strSegmentType);
	ssSegmentType >> intSegmentType;

	//get the string number of segments desired and convert to an int
	string strNumSegments = argv[4];	//10
	int intNumSegments = 0;
	stringstream ssNumSegments(strNumSegments);
	ssNumSegments >> intNumSegments;

	//get the string output file name
	string strOutFileName = argv[5];	//output.txt

										//do the segmentation / feature extraction... simple right!?!
	Segmenter segProcess(strFileName);
	segProcess.setup();
	segProcess.run(intFeatureType, intSegmentType, intNumSegments);
	vector< vector<double> > vvdFeatResults = segProcess.getFeatureResults();
	vector<int> vviPosResults = segProcess.getPositionResults();

	//create the output file and output the data in some format
	ofstream fout(strOutFileName.c_str());
	fout << strOutFileName << endl;
	for (int i = 0; i < vvdFeatResults.size(); ++i) {
		fout << i << ": " << vviPosResults[i] << " -> " << vviPosResults[i + 1];
		fout << " = " << vviPosResults[i + 1] - vviPosResults[i] << " >> ";
		for (int j = 0; j < vvdFeatResults[i].size(); ++j) {
			fout << vvdFeatResults[i][j] << " -- ";
		}
		fout << endl;
	}

	fout.close();
	
}

/**
*	just read data from file
**/
void FileVectorTest()
{
	int vecStart = 0;
	int vecEnd = 100;
	FileVector<int> p;
	p.setFileName("samplePCM.txt");
	p.setup();
	for (int i = vecStart; i < p.size() && i < vecEnd; ++i)
		cout << p[i] << endl;
}


void FeatureTest()
{
	Domain domain;
	domain.setFileName("samplePCM.txt");
	domain.setup();
	domain.setWindowSize(1024);
	domain.setStartPosition(0);
	domain.setEndPosition(10000);
	vector<double> frequencyDomain = domain.getFrequencyDomainWindow(0);
	vector<int> timeDomain = domain.getTimeDomainWindow(0);

	Feature f;
	f.setWindowSize(1024);
	double value = f.calcWindowMean(timeDomain);
	cout << value << endl;


}

void FeatureExtractTest()
{
	FeatureExtract fe;
	fe.setFileName("samplePCM.txt");
	fe.setWindowSize(1024);
	fe.setup();

	//将0-88200数据进行加窗，计算出每个窗的结果，然后取平均值之类的。。
	fe.getFeature(0, 22119, 11);
	auto result = fe.getValues();
	cout << result.size() << endl;
	cout << result[0] << endl;
}

void SegmenterTest(const string& label, const string& filePath)
{

	//Segmenter s("res/demo1.wav");
	//s.setup();

	//auto pcmSize = s.getPcmVecSize();
	//auto totalSeconds = pcmSize / sampleRate;

	//vector<vector<double>> totalResult;
	//totalResult.resize(featureVec.size());
	//for (int i = 0; i < featureVec.size(); ++i)
	//{
	//	cout << featureVec[i] << endl;
	//	s.run( featureVec[i], 2, totalSeconds );

	//	auto featResult = s.getFeatureResults();

	//	totalResult.push_back(featResult[0]);
	//}
	//ofstream tranningOutput(filePath, ios::out | ios::app);
	//for (int i = 0; i < totalSeconds; ++i)
	//{
	//	tranningOutput << label << " ";
	//	cout << label << " ";
	//	cout << totalResult.size() << endl;
	//	for (int j = 0; j < featureVec.size(); ++j)
	//	{
	//		tranningOutput << featureVec[j] << ":" << totalResult[j][i] << " ";
	//		cout << featureVec[j] << ":" << totalResult[j][i] << " ";

	//	}
	//	tranningOutput << endl;
	//	cout << endl;
	//}
	//tranningOutput.close();


	Segmenter s("res/plea/plea_梁静茹 - Bonjour0.wav");
	s.setup();
	auto pcmSize = s.getPcmVecSize();

	auto totalSeconds = pcmSize / 44100;

	// run(int intFeatureType, int intSegmentType, int intNumSegments)
	// intSegmentType 暂时设置为 2
	// intNumSegments 
	s.run(40, 2, totalSeconds);
	auto featResult = s.getFeatureResults();
	auto positionOfResult = s.getPositionResults();
	
	for (auto result : featResult)	// every second
	{
		if (result.size() > 1)
		{
			// get variance
			auto variance = Calcvariance(result);
			cout << variance << endl;
			continue;
		}
		cout << result[0] << endl;
	}

}

vector<int> GetFeatureYouWant()
{
	//const int featureMFCC = 40;
	//const int featureBark = 41;
	//const int featureSpectralRolloff = 26;
	//const int featureSpectralCentoid = 11;
	//const int featureZCR = 7;
	//const int featureSpectralHPS = 27;

	//vector<int> featureIndexVec;
	//featureIndexVec.push_back(featureMFCC);
	//featureIndexVec.push_back(featureBark);
	//featureIndexVec.push_back(featureSpectralCentoid);

	vector<int> featureIndexVec;
	for (int i = 1; i <= 7; ++i)
		featureIndexVec.push_back(i);
	for (int i = 9; i <= 18; ++i)
		featureIndexVec.push_back(i);
	for (int i = 21; i <= 29; ++i)
		featureIndexVec.push_back(i);
	featureIndexVec.push_back(40);
	featureIndexVec.push_back(41);


	return featureIndexVec;
}

void GetTrainningDataTest(const string& label, const string& inputFilePath)
{
	auto featureIndexVec( GetFeatureYouWant() );
	Segmenter s(inputFilePath);
	s.setup();
	vector < vector<double> >totalFeatureResults;
	totalFeatureResults.resize(featureIndexVec.size());
	int numSegments = 30;
	for (int i = 0; i < featureIndexVec.size(); ++i)
	{
		s.run(featureIndexVec[i], 2, numSegments);
		auto featureResults = s.getFeatureResults();

		for (auto result : featureResults)	// every second
		{
			if (result.size() > 1)
			{
				// get variance
				auto variance = Calcvariance(result);
				totalFeatureResults[i].push_back(variance);
				cout << variance << endl;
				continue;
			}
			cout << result[0] << endl;
			totalFeatureResults[i].push_back(result[0]);
		}
	}

	// write result to file
	ofstream fout(label+"trainning.txt", ios::out | ios::app);
	ON_SCOPE_EXIT([&fout]() { fout.close(); });

	for (int i = 0; i < numSegments; ++i)
	{
		fout << label << " ";
		for (int j = 0; j < totalFeatureResults.size(); ++j)
		{
			fout << featureIndexVec[j] << ":"<<totalFeatureResults[j][i];
			fout << " ";
		}
		fout << endl;
	}
}

void SampleMakerTest(const string& label, const string& inputFilePath)
{
	SampleMaker sampleMaker(inputFilePath);
	sampleMaker.Setup();
	sampleMaker.Run(1);
	sampleMaker.WriteToFile(label);

}
int main(int argc, char *argv[])
{
	//MainTest();
	//DomainTest();
	//FeatureTest();
	//FeatureExtractTest();
	//FileVectorTest();
	//SampleMakerTest();

	if (argc != 3)
	{
		cerr << "Usea ./main label outputFilePath" << endl;
		return 0;
	}
	string label(argv[1]);
	string inputFilePath(argv[2]);
	cout << label << " " << inputFilePath << endl;
	SampleMakerTest(label, inputFilePath);
	//GetTrainningDataTest(label, inputFilePath);
}