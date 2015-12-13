#include "WaveSegmenter.h"
#include "common.h"
#include "WaveReader.h"
#include <assert.h>

// WaveSegmenter::Impl
class WaveSegmenter::Impl
{
public:
	Impl(const std::string& filePath);
	~Impl() = default;
	void Setup();
	
	WaveReader m_reader;
	std::vector<int16_t> m_monoDataVec;
private:
};

WaveSegmenter::Impl::Impl(const std::string& filePath):
	 m_reader(filePath)
{

}

WaveSegmenter::WaveSegmenter(const std::string& filePath):
	m_pImpl(new Impl(filePath))
{
}
WaveSegmenter::~WaveSegmenter()
{
	if (m_pImpl != nullptr)
		delete m_pImpl;
}
void WaveSegmenter::Impl::Setup()
{
	m_reader.Setup();

	// get mono data to m_monoDataVec
	auto wavHeader(m_reader.GetHeader());

	m_monoDataVec.clear();
	if (1 == wavHeader.fmt.channels)
	{
		m_monoDataVec = m_reader.GetSigalData();
	}
	else if (2 == wavHeader.fmt.channels)
	{
		auto steroDataVec(m_reader.GetSigalData());
		int i = 0;
		int size = steroDataVec.size();
		for (int i = 0; i < size;i++)
		{
			// get mono data
			m_monoDataVec.push_back(steroDataVec[i++]);
			//m_monoDataVec.push_back(steroDataVec[i]);

			//// ignore stereo data
			//if (i < size)
			//	++i;
			//if (i < size)
			//	++i;
		}
	}
}
/**
*	GetMonoData
*	@brief get mono data
*	@param 
*	@post 
*	@pre Setup(),m_filePath
*	@return a vector of sigal data
*	@par Method will pick up the mono data if the wave file is stero, only 16-bits sigal data can work
*	@note 
*	@remarks
*	@warning
*	@todo 
**/

// WaveSegmenter

void WaveSegmenter::Setup()
{
	m_pImpl->Setup();
}
double WaveSegmenter::GetDuration()
{
	return m_pImpl->m_reader.GetSourceFileDuration();
}
/**
	@bref return a wave segment
	@note if wave file is stereo, it will just pick up the mono data
	@pre filePath set, Setup()
**/
std::vector<int16_t> WaveSegmenter::GetMonoSegment(unsigned int sampleBegIndex, unsigned int sampleEndIndex)
{
	assert( sampleBegIndex < sampleEndIndex );
	assert( sampleBegIndex >= 0 && sampleEndIndex > 0 );

	auto iterBegin( m_pImpl->m_monoDataVec.begin() );
	auto iterEnd( m_pImpl->m_monoDataVec.begin() );
	std::advance(iterBegin, sampleBegIndex);
	std::advance(iterEnd, sampleEndIndex);

	std::vector<int16_t> wavSegmenteVec( iterBegin, iterEnd );
	return wavSegmenteVec;
}

std::vector<int16_t> WaveSegmenter::GetMonoSegment(double timeBegin, double timeEnd)
{
	assert( timeBegin > DBL_EPSILON || fabs(timeBegin)-0.0 < DBL_EPSILON);
	assert( timeEnd > DBL_EPSILON || fabs(timeEnd)-0.0 < DBL_EPSILON);
	assert( fabs(timeBegin) - fabs(timeEnd) < 0.000001);

	unsigned int sampleBegIndex = round(44100.0 * timeBegin);
	unsigned int sampleEndIndex = round(44100.0 * timeEnd);

	return this->GetMonoSegment(sampleBegIndex, sampleEndIndex);

}

void WaveSegmenter::SetFilePath(const std::string& filePath)
{
	m_pImpl->m_reader.SetFilePath(filePath);
}

