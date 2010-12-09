/*
 * FstAligner.h
 *
 *  Created on: 2010-12-6
 *      Author: yifanzhang
 */

#ifndef FSTALIGNER_H_
#define FSTALIGNER_H_

#include <iostream>
#include <sstream>
#include <iomanip>
#include <string>
#include "FstLoader.h"

struct FstAlignOption {
	bool bForceEnterAndExit;

	float corCost;
	float insCost;
	float delCost;
	float subCost;

	std::string symEps;
	std::string symOov;
	std::string symIns;
	std::string symDel;
	std::string symSub;

	std::string saveCompositionDot;
	std::string saveShortestPathDot;

	FstAlignOption()
		: bForceEnterAndExit(false), corCost(0.0f), insCost(3.0f), delCost(3.0f), subCost(4.0f),
		  symEps("<del>"), symOov("<oov>"), symIns("<ins>"), symDel("<del>"), symSub("<sub>"),
		  saveCompositionDot(""), saveShortestPathDot("")
	{}
};


struct FstAlignResult {
	float mScore;
	int	mNum, mIns, mDel, mSub;
	typedef std::list< std::pair<std::string,std::string> > AlignmentType;
	AlignmentType alignment;
	string mHyp;
	string mRef;

	FstAlignResult()
		: mScore(0.0), mNum(0), mIns(0), mDel(0), mSub(0)
	{}
	void update(const FstAlignOption &option) {
		std::vector<int> width;
		{
			std::ostringstream stream;
			for (AlignmentType::const_iterator i=alignment.begin(); i!=alignment.end(); ++i) {
				width.push_back(std::max(i->first.length(), i->second.length()));
				stream << std::setw(1) << " " << std::setw(width.back()) << i->first;

				if (i->first == option.symDel) {
					mNum ++;
					mDel ++;
				}
				else if (i->second == option.symIns) {
					mIns ++;
				}
				else {
					mNum ++;
					if (i->first != i->second)
						mSub ++;
				}
			}
			mHyp = stream.str();
		}
		{
			std::ostringstream stream;
			std::vector<int>::const_iterator w = width.begin();
			for (AlignmentType::const_iterator i=alignment.begin(); i!=alignment.end(); ++i, ++w) {
				stream << std::setw(1) << " " << std::setw(*w) << i->second;
			}
			mRef = stream.str();
		}
		mScore = static_cast<float>(mIns+mDel+mSub)/static_cast<float>(mNum);
	}
	void writeScore(std::ostream &stream) {
		stream << " Score: " << mScore << "\n";
	}
	void writeAlignment(std::ostream &stream) {
		stream << "REF:" << mRef << "\n";
		stream << "HYP:" << mHyp << "\n";
	}

};


class FstAligner {
public:
	FstAligner();
	virtual ~FstAligner();

	void align(const FstAlignOption &option, const FstLoader &refLoader, const FstLoader &hypLoader, FstAlignResult &result);
};

#endif /* FSTALIGNER_H_ */
