#include "WaveReader.h"

using namespace std;

const char *WaveReader::riff_name = "RIFF";
const char *WaveReader::wave_name = "WAVE";
const char *WaveReader::fmt_name = "fmt ";
const char *WaveReader::data_name = "data";



/* CONSTRUCTOR FROM FILE
************************/
WaveReader::WaveReader(std::string filePath) :m_filePath(filePath) {
	//read the wave file and store the contents in the signal vector and the header struct
}

/**
	@pre set m_filePath
**/
int WaveReader::ReadHeaderAndSigalData(bool verbose) {
	if (m_filePath.empty()) {
		cerr << "Error: this object is not attached to any file. Set a pathname first please."
			<< endl;
		return -1;
	}
	bool data_header_found = false;
	unsigned int found_header_size = 0;
	//open up a new input stream
	ifstream wav_file(m_filePath.c_str(), ios::in | ios::binary);
	if (!wav_file) {
		cerr << "Error: the file can't be opened" << endl;
		exit(-1);
	}
	//start reading the RIFF header
	wav_file.read((char *)&m_header.riff, RIFF_HEADER_SIZE);
	if (wav_file.gcount() != RIFF_HEADER_SIZE || memcmp(m_header.riff.RIFF, "RIFF", 4)) {
		cerr << "Error: not a valid RIFF file" << endl;
		return ERR_FILE_NOT_RIFF;
	}
	found_header_size += wav_file.tellg();
	wav_file.read((char *)&m_header.fmt, FMT_HEADER_SIZE);
	if (wav_file.gcount() != FMT_HEADER_SIZE) {
		cerr << "Error: couldn't read the fmt header" << endl;
		return ERR_READING_FILE;
	}
	if (m_header.fmt.audio_format != 1) {
		cerr << "Error: this is not in PCM format" << endl;
		return ERR_READING_FILE;
	}
	found_header_size += wav_file.tellg();
	cout << wav_file.tellg() << endl;
	//look for the data header, leaving the other possible headers aside
	wav_file.seekg(0, ios::beg);
	while (!data_header_found) {
		wav_file.read((char *)&m_header.data, DATA_HEADER_SIZE);
		wav_file.seekg(-DATA_HEADER_SIZE, ios::cur);
		if (memcmp(m_header.data.data, "data", 4)) {
			//we don't have a data ID
			/*cerr << "found a non-data header, so skipping " << 1 <<
				" bytes" << endl;*/
			wav_file.seekg(1, ios::cur);
		}
		else {
			data_header_found = true;
		}
		found_header_size += wav_file.tellg();
	}
	//and finally, read the signal data
	unsigned int sampleCount = m_header.data.sub_chunk_2_size / sizeof(int16_t);
	m_sampleDataVec.clear();
	m_sampleDataVec.resize(sampleCount);
	wav_file.read((char*)&m_sampleDataVec[0], m_header.data.sub_chunk_2_size);
	if ((uint32_t)wav_file.gcount() != m_header.data.sub_chunk_2_size) {
		cerr << "Error: Something went wrong reading the signal data" << endl;
		return ERR_READING_FILE;
	}
	if (verbose) PrintHeader();
	return 0;
}


void WaveReader::Setup()
{
	ReadHeaderAndSigalData(false);
}

/* Print the existing header
****************************/
void WaveReader::PrintHeader() {
	if (memcmp(m_header.riff.RIFF, "RIFF", 4)) {
		cerr << "The header contains nothing" << endl;
		return;
	}
	cout << "=====================================" << endl;
	cout << "RIFF ID: " << m_header.riff.RIFF << endl;
	cout << "File size: " << m_header.riff.file_size + 8 << " Bytes" << endl;
	cout << "WAVE ID: " << m_header.riff.WAVE << endl;
	cout << "-------------------------------------" << endl;
	cout << "FMT ID: " << m_header.fmt.fmt << endl;
	cout << "fmt header size: " << m_header.fmt.sub_chunk_1_size << " Bytes" << endl;
	switch (m_header.fmt.audio_format) {
	case 1:
		cout << "Audio format: PCM" << endl;
		break;
	}
	cout << "Channels: " << m_header.fmt.channels << endl;
	cout << "Sample rate: " << m_header.fmt.sample_rate << " Hz" << endl;
	cout << "Byte rate: " << m_header.fmt.bytes_per_sec << " Bytes/s" << endl;
	cout << "Block align: " << m_header.fmt.block_align << " Bytes" << endl;
	cout << "Bits per sample: " << m_header.fmt.bits_per_sample << " bits" << endl;
	cout << "-------------------------------------" << endl;
	cout << "DATA ID: " << m_header.data.data << endl;
	cout << "Data size (Audio): " << m_header.data.sub_chunk_2_size << " bytes" << endl;
	cout << "=====================================" << endl;
}


