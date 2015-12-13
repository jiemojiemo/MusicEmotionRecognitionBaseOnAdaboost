#pragma once
#include "common.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <string>

#define ERR_FILE_NOT_FOUND -1
#define ERR_READING_FILE -2
#define ERR_FILE_NOT_RIFF -3

#define STANDARD_HEADER_SIZE 44
#define FMT_HEADER_SIZE 24
#define RIFF_HEADER_SIZE 12
#define DATA_HEADER_SIZE 8

typedef unsigned int uint32_t;
typedef unsigned short uint16_t;
typedef unsigned char uint8_t;
typedef signed char int8_t;
typedef signed short int16_t;
typedef signed int int32_t;
/* HEADER STRUCTS
*****************/

typedef struct {
	uint8_t RIFF[4];
	uint32_t file_size;
	uint8_t WAVE[4];
}RIFF_h;

typedef struct {
	uint8_t fmt[4];
	uint32_t sub_chunk_1_size;
	uint16_t audio_format;
	uint16_t channels;
	uint32_t sample_rate;
	uint32_t bytes_per_sec;
	uint16_t block_align;
	uint16_t bits_per_sample;
}fmt_h;

typedef struct {
	uint8_t data[4];
	uint32_t sub_chunk_2_size;
}data_h;

typedef struct {
	RIFF_h riff;
	fmt_h fmt;
	data_h data;
}WAV_header;

/* END OF HEADER STRUCTS
************************/

class WaveReader {

private:
	WAV_header m_header;
	std::string m_filePath;
	std::vector<int16_t> m_sampleDataVec;
	static const char *riff_name;
	static const char *wave_name;
	static const char *fmt_name;
	static const char *data_name;
public:
	/// Constructor to build a new wav file from a file
	/// @param file_path to attach the object to a file
	/// @param verbose to show the file properties once the data is readen. True by default.
	WaveReader(std::string filePath);

	/// Destructor
	~WaveReader() = default;

	/**
	*	@bref read header and read sigal data
	*	@pre set m_filePath
	**/
	void Setup();
	
	/// print_header prints the header of this WAV object
	void PrintHeader();

	/// get_header gets a copy of the header of this WAV object
	WAV_header GetHeader() { return m_header; }

	double GetSourceFileDuration()
	{
		int samples = m_header.data.sub_chunk_2_size / ((m_header.fmt.bits_per_sample / 8)*m_header.fmt.channels);
		double seconds = (double)samples / (double)m_header.fmt.sample_rate;
		return seconds;
	}

	std::vector<int16_t> GetSigalData() { return m_sampleDataVec; }

	void SetFilePath(std::string filePath) { m_filePath = filePath; }
private:
	/// read_wav_file reads the contents of the file attached to the WAV object
	/// @param verbose to show the file properties once the data is readen.
	int ReadHeaderAndSigalData(bool verbose);
};

