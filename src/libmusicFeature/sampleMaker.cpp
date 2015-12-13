#include "sampleMaker.h"
#include <numeric>
#include <algorithm>
#include <assert.h>

template <typename T>
T Calcvariance(const vector<T>& vec)
{
	auto sum = std::accumulate(begin(vec), end(vec), T(0));
	auto mean = sum / vec.size();

	T accum = 0;
	std::for_each(begin(vec), end(vec), [&](const T t)
	{
		accum += (t - mean) * (t - mean);
	});

	return sqrt(accum / (vec.size() - 1));
}

vector<int> GetFeatures()
{
	vector<int> featureIndexVec;
	for (int i = 1; i <= 7; ++i)
		featureIndexVec.push_back(i);
	for (int i = 9; i <= 18; ++i)
		featureIndexVec.push_back(i);
	featureIndexVec.push_back(40);
	featureIndexVec.push_back(41);

	return featureIndexVec;

}

SampleMaker::SampleMaker(const std::string & wavInputPath) :
	m_segmenter(wavInputPath),
	m_featureIndexArray(GetFeatures())
{
}

void SampleMaker::Setup()
{
	m_segmenter.setup();
}

void SampleMaker::Run(int numSegments)
{
	assert(numSegments > 0);
	m_result.clear();
	m_result.resize(m_featureIndexArray.size());
	for (int i = 0; i < m_featureIndexArray.size(); ++i)
	{
		m_segmenter.run(m_featureIndexArray[i], 2, numSegments);
		auto featureResults(m_segmenter.getFeatureResults());

		for (auto result : featureResults)
		{
			if (result.size() > 1)
				m_result[i].push_back(Calcvariance(result));
			else
				m_result[i].push_back(result[0]);
		}
	}
}

std::vector<int> SampleMaker::GetFeatureIndexArray() const
{
	return m_featureIndexArray;
}

void SampleMaker::SetFeatureIndexArray(const std::vector<int>& featureIndexArray)
{
	m_featureIndexArray = featureIndexArray;
}

std::vector<std::vector<double>> SampleMaker::GetResult() const
{
	return m_result;
}

void SampleMaker::WriteToFile(const std::string & label, const std::string & outputFileName)
{
	ofstream fout(label + outputFileName, ios::out | ios::app);
	if (!fout.is_open())
	{
		cerr << "Can not open output file.\n";
		exit(-1);
	}
	ON_SCOPE_EXIT([&fout]() { fout.close(); });
	int numSegments = m_result[0].size();
	for (int i = 0; i < numSegments; ++i)
	{
		fout << label << " ";
		for (int j = 0; j < m_result.size(); ++j)
		{
			fout << m_featureIndexArray[j] << ":" << m_result[j][i];
			fout << " ";
		}
		fout << endl;
	}
}



