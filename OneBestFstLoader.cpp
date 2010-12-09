/*
 * OneBestFstLoader.cpp
 *
 *  Created on: 2010-12-6
 *      Author: yifanzhang
 */
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <iterator>

#include "OneBestFstLoader.h"


OneBestFstLoader::OneBestFstLoader(const char *filename)
	: FstLoader(filename) {
	std::ifstream stream(filename);

	if (!stream.is_open())
		throw std::runtime_error("Cannot open input file");

	std::copy(std::istream_iterator<std::string>(stream), std::istream_iterator<std::string>(),
			std::back_inserter(mToken));

	stream.close();
}

OneBestFstLoader::OneBestFstLoader(const std::string& content)
	: FstLoader(content) {
	std::istringstream stream(content);
	std::copy(std::istream_iterator<std::string>(stream), std::istream_iterator<std::string>(),
			std::back_inserter(mToken));
}

OneBestFstLoader::~OneBestFstLoader() {
	// TODO Auto-generated destructor stub
}
