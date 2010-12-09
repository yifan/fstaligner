/*
 * FstLoader.cpp
 *
 *  Created on: 2010-12-6
 *      Author: yifanzhang
 */

#include "FstLoader.h"

FstLoader::FstLoader(const char *filename) {
	// TODO Auto-generated constructor stub
}

FstLoader::FstLoader(const std::string &content) {
	// TODO Auto-generated constructor stub
}

FstLoader::~FstLoader() {
	// TODO Auto-generated destructor stub
}

void FstLoader::addToSymbolTable(fst::SymbolTable &symbol) const {
	for (TokenType::const_iterator i = mToken.begin();
		i != mToken.end(); ++ i) {
		if (symbol.Find(*i) == symbol.kNoSymbol)
			symbol.AddSymbol(*i);
	}
}

fst::StdVectorFst FstLoader::convertToFst(const fst::SymbolTable &symbol) const {
	fst::StdVectorFst transducer;

	transducer.AddState();
	transducer.SetStart(0);

	int state = 0;
	for (TokenType::const_iterator i = mToken.begin();
		i != mToken.end(); ++ i, ++ state) {
		transducer.AddState();
		transducer.AddArc(state, fst::StdArc(symbol.Find(*i), symbol.Find(*i), 0.0f, state+1));
	}

	transducer.SetFinal(state, 0.0f);

	return transducer;
}
