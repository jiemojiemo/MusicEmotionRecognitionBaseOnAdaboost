#include "MusicEmotionRecognizer.h"
#include "sampleMaker.h"
#include <iostream>
#include <algorithm>
#include <fstream>
#include <math.h>
bool IsExistFile(const std::string& filePath)
{
	std::ifstream fin;
	fin.open(filePath);

	if (!fin.is_open())
		return false;
	fin.close();

	return true;
}



// Impl
class MusicEmotionRecognizer::Impl
{
public:
	Impl(const std::vector<std::string>& modelArray);

	
public:
	std::vector<std::string> m_modelArray;
};

/*
	@bref construction
*/
MusicEmotionRecognizer::Impl::Impl(const std::vector<std::string>& modelArray) :
	m_modelArray(modelArray)
{
}




// MusicEmotionRecognizer
MusicEmotionRecognizer::MusicEmotionRecognizer(const std::vector<std::string>& modelArray):
	m_pImpl(new MusicEmotionRecognizer::Impl(modelArray))
{
}

MusicEmotionRecognizer::~MusicEmotionRecognizer()
{
	if (m_pImpl != nullptr)
		delete m_pImpl;
}

std::string MusicEmotionRecognizer::Predict(const std::string & wavMusicPath)
{
	int numSegments = 5;
	if (!IsExistFile(wavMusicPath))
	{
		std::cerr << "Can not open " << wavMusicPath << std::endl;
		return "null";
	}
	SampleMaker sampleMaker(wavMusicPath);
	sampleMaker.Setup();
	sampleMaker.Run(numSegments);
	auto result = sampleMaker.GetResult();

	vector<vector<double>> sample;
	auto featureIndexArray( sampleMaker.GetFeatureIndexArray() );
	auto numFeature = *std::max_element(featureIndexArray.begin(), featureIndexArray.end());
	cout << numFeature << endl;
	sample.resize(numSegments);
	for (int i = 0; i < numSegments; ++i)
	{
		sample[i].resize(numFeature);
		for (int j = 0; j < featureIndexArray.size(); ++j)
		{
			sample[i][featureIndexArray[j]-1] = result[j][i];
		}
	}

	return "null";
}


