/*
 * FstLoader.h
 *
 *  Created on: 2010-12-6
 *      Author: yifanzhang
 */

#ifndef FSTLOADER_H_
#define FSTLOADER_H_

#include <string>
#include <list>

#include <fst/fstlib.h>

class FstLoader {
protected:
	typedef std::vector<std::string> TokenType;
	TokenType mToken;
public:
	FstLoader(const char *filename);
	FstLoader(const std::string &content);
	virtual ~FstLoader();

	virtual void addToSymbolTable(fst::SymbolTable &symbol) const;
	fst::StdVectorFst convertToFst(const fst::SymbolTable &symbol) const;
	const std::string &getToken(int index) const {
		return mToken.at(index);
	}
};

#endif /* FSTLOADER_H_ */
