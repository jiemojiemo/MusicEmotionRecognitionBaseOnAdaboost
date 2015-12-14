#include "MusicEmotionRecognizer.h"
#include <string>
#include <vector>
#include <iostream>

using namespace std;
int main(int argc, char* argv[])
{
	//MusicEmotionRecognizer recog({"123"});
	vector<string> modelNames = {
		"exci.txt.model",
		"plea.txt.model",
		"quie.txt.model",
		"sad.txt.model"};
	auto pRecog( make_unique<MusicEmotionRecognizer>(modelNames) );
	auto result = pRecog->Predict(argv[1]);
	cout << result << endl;
	return 0;
}