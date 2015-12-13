#include "WaveSegmenter.h"
#include "WaveFileSegmenter.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>  
#include <io.h> 
#include <windows.h>
using namespace std;

void LoopSegment( const string& filePath );
string int2string(int x)
{
	stringstream stream;
	string stringX;
	stream << x;
	stringX = stream.str();
	return stringX;
}
string GetNewSegmentName(const string& filePath, int count)
{
	string newSegmentName( filePath.begin(), filePath.end() - 4 );
	newSegmentName += int2string(count);
	newSegmentName += ".wav";
	return newSegmentName;
}
int main( int argc, char* argv[] )
{
	if (argc < 2)
	{
		cerr << "Please input source file path.\n";
		return 0;
	}
	int i = 1;
	while (i < argc)
	{
		cout << argv[i] << endl;
		string filePath( argv[i++] );
		LoopSegment(filePath);
	}
}

void LoopSegment(const string& filePath)
{
	WaveFileSegmenter fileSegment(filePath);
	fileSegment.Setup();
	double duration = fileSegment.GetSourceFileDuration();

	double ignoreBeg = 30.0;
	double ignoreEnd = 30.0;
	double segmentTime = 30.0;
	double timeBeg = ignoreBeg;
	double timeEnd = duration - ignoreEnd;
	
	if (timeBeg + segmentTime > timeEnd)
	{
		cout << "music is too short to segment" << endl;
		return;
	}
	int segmentCount = (timeEnd - timeBeg) / segmentTime;
	int count = 0;
	do 
	{
		string segmentName(GetNewSegmentName(filePath, count));
		fileSegment.doSegment(timeBeg, timeBeg + segmentTime, segmentName);
		timeBeg += segmentTime;
	} while (++count < segmentCount);
}

