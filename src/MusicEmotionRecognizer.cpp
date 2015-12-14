#include "MusicEmotionRecognizer.h"
#include "sampleMaker.h"
#include "AdaBoost.h"
#include <iostream>
#include <algorithm>
#include <fstream>
#include <math.h>
#include <map>




// Impl
class MusicEmotionRecognizer::Impl
{
public:
	Impl(const std::vector<std::string>& modelArray);

	bool IsExistFile(const std::string& filePath);

	std::vector <std::vector<double>> GetSamples(const std::string& filePath,
		int numSegments = 1);

	template <typename T>
	std::pair<std::string, T> GetMaxValueResult(const std::map<std::string, T>& results);
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

bool MusicEmotionRecognizer::Impl::IsExistFile(const std::string& filePath)
{
	std::ifstream fin;
	fin.open(filePath);

	if (!fin.is_open())
		return false;
	fin.close();

	return true;
}

std::vector <std::vector<double>> MusicEmotionRecognizer::Impl::GetSamples(
	const std::string& filePath,int numSegments)
{
	SampleMaker sampleMaker(filePath);
	sampleMaker.Setup();
	sampleMaker.Run(numSegments);
	auto result = sampleMaker.GetResult();

	vector<vector<double>> samples;
	auto featureIndexArray(sampleMaker.GetFeatureIndexArray());
	auto numFeature = *std::max_element(featureIndexArray.begin(), featureIndexArray.end());
	samples.resize(numSegments);
	for (int i = 0; i < numSegments; ++i)
	{
		samples[i].resize(numFeature);
		for (int j = 0; j < featureIndexArray.size(); ++j)
		{
			samples[i][featureIndexArray[j] - 1] = result[j][i];
		}
	}

	return samples;
}

template <typename T>
std::pair<std::string, T> MusicEmotionRecognizer::Impl::GetMaxValueResult(
	const std::map<std::string, T>& results)
{
	std::pair<std::string, T> maxItem("null",-1);
	for (auto item : results)
	{
		if (item.second > maxItem.second)
		{
			maxItem = item;
		}
	}
	return maxItem;
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
	if (!m_pImpl->IsExistFile(wavMusicPath))
	{
		std::cerr << "Can not open " << wavMusicPath << std::endl;
		return "null";
	}

	auto samples = m_pImpl->GetSamples(wavMusicPath,1);
	int testSampleTotal = samples.size();
	// Predict
	std::map<std::string, double> predicResult;
	
	for (auto item : m_pImpl->m_modelArray)
	{
		predicResult.insert(std::make_pair(item, -1.0));
		AdaBoost adaBoost;
		adaBoost.readFile(item);

		for (int i = 0; i < testSampleTotal; ++i)
		{
			double score = adaBoost.predict(samples[i]);
			//if (score > 0)
			//	predicResult[item]++;
			if (score > predicResult[item])
				predicResult[item] = score;
		}
	}
	// print the result
	for (auto item : predicResult)
	{
		cout << item.first << ":" << item.second << endl;
	}
	// find the max value
	auto bestResult = m_pImpl->GetMaxValueResult(predicResult);

	return bestResult.first;
}


