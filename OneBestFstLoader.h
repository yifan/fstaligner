/*
 * OneBestFstLoader.h
 *
 *  Created on: 2010-12-6
 *      Author: yifanzhang
 */

#ifndef ONEBESTFSTLOADER_H_
#define ONEBESTFSTLOADER_H_

#include "FstLoader.h"

class OneBestFstLoader: public FstLoader {
public:
	OneBestFstLoader(const char *filename);
	OneBestFstLoader(const std::string &content);
	virtual ~OneBestFstLoader();
};

#endif /* ONEBESTFSTLOADER_H_ */
