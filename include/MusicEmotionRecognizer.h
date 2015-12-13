#pragma once
#include "common.h"
#include <string>
#include <vector>
#include <memory>
class MusicEmotionRecognizer
{
	DISALLOW_COPY_AND_ASSIGN(MusicEmotionRecognizer);
public:
	/*
		@param modelArray contain file names, models create by adaboost training
		@see Adaboost Training
	*/
	MusicEmotionRecognizer( const std::vector<std::string>& modelArray  );

	~MusicEmotionRecognizer();
	/*
		@param wavMusicPath the path of the wav file
		@return a string which is model file name
	*/
	std::string Predict(const std::string& wavMusicPath);
private:
	class Impl;
	Impl* m_pImpl;
	
};
