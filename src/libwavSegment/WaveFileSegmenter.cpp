#include "WaveFileSegmenter.h"
#include "WaveSegmenter.h"
#include "WaveReader.h"
#include <iostream>
#include <fstream>
using std::cerr;
using std::cout;
using std::endl;
using std::fstream;
// WaveFileSegmenter::Impl
class WaveFileSegmenter::Impl
{
public:
	Impl( const std::string& filePath);
	~Impl() { if (m_segmenter != nullptr) delete m_segmenter; }

	WAV_header GetHeader( const std::vector<int16_t>& data, uint32_t sample_rate, uint16_t bits_per_sample, uint16_t channels);

	bool Save( const WAV_header& header, const std::vector<int16_t>& data,const std::string& newFilePath );
	WaveSegmenter* m_segmenter;
};

WaveFileSegmenter::Impl::Impl(const std::string& filePath) :
	m_segmenter(new WaveSegmenter(filePath))
{

}

WAV_header WaveFileSegmenter::Impl::GetHeader(const std::vector<int16_t>& data, uint32_t sample_rate, uint16_t bits_per_sample, uint16_t channels)
{
	//set up the RIFF header
	WAV_header header;
	memcpy(header.riff.RIFF, "RIFF", 4);
	header.riff.file_size = (uint32_t)data.size()*sizeof(data[0]) + STANDARD_HEADER_SIZE - 8;
	memcpy(header.riff.WAVE, "WAVE", 4);
	//set up the fmt header
	memcpy(header.fmt.fmt, "fmt ", 4);
	header.fmt.sub_chunk_1_size = FMT_HEADER_SIZE - 8;
	header.fmt.audio_format = 1; //PCM
	header.fmt.channels = channels;
	header.fmt.sample_rate = sample_rate;
	header.fmt.bytes_per_sec = sample_rate*channels*bits_per_sample / 8;
	header.fmt.block_align = channels * bits_per_sample / 8;
	header.fmt.bits_per_sample = bits_per_sample;
	//set up the data header
	memcpy(header.data.data, "data", 4);
	header.data.sub_chunk_2_size = (uint32_t)data.size()*sizeof(data[0]);

	return header;
}

bool WaveFileSegmenter::Impl::Save(const WAV_header & header, const std::vector<int16_t>& data, const std::string & newFilePath)
{
	if (newFilePath.empty()) {
		cerr << "Error: this object is not attached to any file. Set the pathname first" << endl;
		return false;
	}
	fstream output(newFilePath.c_str(), std::ios::out | std::ios::binary);
	if (output.is_open()) {
		output.write((char*)&header, sizeof(header));
		output.write((char*)&data[0], header.data.sub_chunk_2_size);
	}
	else {
		cerr << "Error: can't open " << newFilePath << " to write" << endl;
		return false;
	}
	return true;
}

// WaveFileSegmenter
WaveFileSegmenter::WaveFileSegmenter(const std::string& srcFilePath) :
	m_pImpl(new Impl(srcFilePath))
{

}

WaveFileSegmenter::~WaveFileSegmenter()
{
		if (m_pImpl != nullptr)
			delete m_pImpl;
}

/**
*	Setup
*	@brief 
*	@param
*	@post WaveSegmenter will be Setup
*	@pre 
*	@return 
*	@par Method 
*	@note
*	@remarks
*	@warning
*	@todo
**/
void WaveFileSegmenter::Setup()
{
	m_pImpl->m_segmenter->Setup();
}

double WaveFileSegmenter::GetSourceFileDuration()
{
	return m_pImpl->m_segmenter->GetDuration();
}

/**
*	Setup
*	@brief segment wav file, get timeBeg--timeEnd data then add header. 
*	@param 
*	@post WaveSegmenter will be Setup, a new wav file created
*	@pre
*	@return
*	@par Method 
*	@note just segment 16-bits mono 44100 wav file
*	@remarks
*	@warning
*	@todo
**/
void WaveFileSegmenter::doSegment(double timeBeg, double timeEnd, const std::string& newFileName)
{
	auto sigalData( m_pImpl->m_segmenter->GetMonoSegment(timeBeg, timeEnd) );
	auto header(m_pImpl->GetHeader(sigalData, 44100, 16, 1));
	bool isSuccessSave = m_pImpl->Save(header, sigalData, newFileName);

	if (!isSuccessSave)
	{
		cerr << __FUNCTION__ << __LINE__ << "Failed.\n";
	}
	else
	{
		cout << __FUNCTION__ << __LINE__ << "Successed. new file:" << newFileName << endl;
	}
}
