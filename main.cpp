//
// fstalign - text alignment
//
// this is an example using openfst to align reference text
// with hypothesis. 
//
// by yifanzhang

#include "FstAligner.h"
#include "OneBestFstLoader.h"

void help(const char *program) {
    std::cout << "usage:\n";
    std::cout << "    " << program << " ref.txt hyp.txt\n";
}

int main(int argc, char **argv) {

	if (argc != 3) {
		help(argv[0]);
		return -1;
	}
		
	FstAlignOption option;
	FstAlignResult result;
	FstAligner aligner;
	OneBestFstLoader ref(argv[1]);
	OneBestFstLoader hyp(argv[2]);
	
	aligner.align(option, ref, hyp, result);
	
	result.writeAlignment(std::cout);
	
	return 0;
}