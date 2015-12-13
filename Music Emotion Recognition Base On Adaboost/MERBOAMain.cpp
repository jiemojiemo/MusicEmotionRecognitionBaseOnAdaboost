#include "MusicEmotionRecognizer.h"
#include <string>
#include <vector>

using namespace std;
int main()
{
	//MusicEmotionRecognizer recog({"123"});
	vector<string> modelNames = {"123","456"};
	auto pRecog( make_unique<MusicEmotionRecognizer>(modelNames) );
	pRecog->Predict("E:\\C++\\Music Emotion Recognition Base On Adaboost\\res\\plea\\plea_Bridgit Mendler - Ready Or Not0.wav");
	return 0;
}