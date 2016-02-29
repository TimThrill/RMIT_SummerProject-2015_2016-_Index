#ifndef LEXICON_H
#define LEXICON_H

#include <string>
#include <map>

#include "CollectionFrequency.h"

/* Lexicon node struct */
class Lexicon {
public:
	std::string word;
	int docFrequency;
	std::map<unsigned int, CollectionFrequency> fdt;
	long location;
	
	Lexicon()
	{
	}
	
	Lexicon(std::string word, long location, int docFrequency = 0)
	{
		this->word = word;
		this->location = location;
		this->docFrequency = docFrequency;
	}
	

	Lexicon(const Lexicon& lexicon)
	{
		this->word = lexicon.word;
		this->docFrequency = lexicon.docFrequency;
		this->location = lexicon.location;
		this->fdt = lexicon.fdt;
	}

	void setWord(std::string word) {
		this->word = word;
	}
	
	void setLocation(int location) {
		this->location = location;
	}	
};

#endif
