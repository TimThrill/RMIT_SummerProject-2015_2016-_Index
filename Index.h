#ifndef INDEX_H
#define INDEX_H

#include <string>
#include <map>

#include "Lexicon.h"

class Index
{
public:
	std::map<std::string, Lexicon> lexicon;
};

#endif
