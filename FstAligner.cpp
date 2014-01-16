/*
 * FstAligner.cpp
 *
 * align hypothesis fst to reference fst, produce WER
 *
 *  Created on: 2010-12-6
 *      Author: freqyifan@gmail.com
 */

#include "FstAligner.h"

#include <iostream>
#include <fstream>
#include <fst/fstlib.h>
#include <fst/script/draw.h>

FstAligner::FstAligner() {
	// TODO Auto-generated constructor stub

}

FstAligner::~FstAligner() {
	// TODO Auto-generated destructor stub
}

void drawFstToFile(fst::StdVectorFst &fstToDraw, const fst::SymbolTable &symbol, const std::string &name) {
	std::ofstream stream(name.c_str());
	if (stream.is_open()) {
		fst::script::FstClass object(fstToDraw);
		fst::script::DrawFst(object, &symbol, &symbol, 0, false,
							name, 8.5, 11, false, false, 0.40, 0.25, 14, 5,
							false, &stream, name);
	}
	stream.close();
}

void FstAligner::align(const FstAlignOption & option,
		const FstLoader &refLoader, const FstLoader &hypLoader,
		FstAlignResult &result) {
	fst::SymbolTable symbol("symbol");

	symbol.AddSymbol(option.symEps);
	symbol.AddSymbol(option.symOov);
	symbol.AddSymbol(option.symIns);
	symbol.AddSymbol(option.symDel);
	symbol.AddSymbol(option.symSub);

	refLoader.addToSymbolTable(symbol);
	hypLoader.addToSymbolTable(symbol);
	fst::StdVectorFst refFst = refLoader.convertToFst(symbol);
	fst::StdVectorFst hypFst = hypLoader.convertToFst(symbol);

	symbol.WriteText("symbol");

	assert(refFst.NumStates() > 1);
	assert(hypFst.NumStates() > 1);

	// add possible insertion/deletion/substitution to reference
	int nstate = refFst.NumStates();

	if (option.bForceEnterAndExit == false) {
		refFst.AddArc(0, fst::StdArc(symbol.Find(option.symOov), symbol.Find(option.symIns),
										option.insCost, 0));
		refFst.AddArc(nstate-1, fst::StdArc(symbol.Find(option.symOov), symbol.Find(option.symIns),
										option.insCost, nstate-1));
	}
	else {
		nstate --;
	}

	for (int i=1; i<nstate; i++) {
		refFst.AddArc(i-1, fst::StdArc(symbol.Find(option.symEps), symbol.Find(refLoader.getToken(i-1)),
										option.delCost, i));
		refFst.AddArc(i-1, fst::StdArc(symbol.Find(option.symOov), symbol.Find(refLoader.getToken(i-1)),
										option.subCost, i));
		refFst.AddArc(i, fst::StdArc(symbol.Find(option.symOov), symbol.Find(option.symIns),
										option.insCost, i));
	}

	fst::ArcSort(&hypFst, fst::StdOLabelCompare());
	fst::ArcSort(&refFst, fst::StdILabelCompare());

	typedef fst::SigmaMatcher< fst::SortedMatcher<fst::StdFst> >RM;
	fst::ComposeFstOptions<fst::StdArc, RM> opts;
	opts.gc_limit = 0;
	opts.matcher1 = new RM(hypFst, fst::MATCH_NONE, fst::kNoLabel);
	opts.matcher2 = new RM(refFst, fst::MATCH_INPUT, symbol.Find(option.symOov));

	fst::StdComposeFst cFst(hypFst, refFst, opts);
	fst::StdVectorFst composedFst(cFst);

	if (option.saveCompositionDot != "")
		drawFstToFile(composedFst, symbol, option.saveCompositionDot);

	fst::StdVectorFst pathFst;
	fst::ShortestPath(composedFst, &pathFst);
	if (option.saveShortestPathDot != "")
		drawFstToFile(pathFst, symbol, option.saveShortestPathDot);

	fst::StdVectorFst::StateId state = pathFst.Start();
	while (state > 0) {
		fst::ArcIterator<fst::StdVectorFst> aiter(pathFst, state);
		const fst::StdArc &arc = aiter.Value();
		result.alignment.push_back(std::make_pair(symbol.Find(arc.ilabel), symbol.Find(arc.olabel)));
		state = arc.nextstate;
	}

	result.update(option);
}
