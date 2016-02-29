#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <algorithm>
#include <regex>

#include "json/json.h"
#include "Index.h"
#include "Document.h"
#include "CollectionFrequency.h"

using namespace std;

vector<string> attr_key;
map<string, string> stopList;

void parseText(string& contents, unsigned int key, Index* index)
{
	// trim leading spaces
	size_t startpos = contents.find_first_not_of(" \t");
	if( string::npos != startpos )
	{
	    contents = contents.substr( startpos );
	}

	// trim trailing spaces
	size_t endpos = contents.find_last_not_of(" \t");
	if( string::npos != endpos )
	{
	    contents = contents.substr( 0, endpos+1 );
	}

	// to lower case
	transform(contents.begin(), contents.end(), contents.begin(), ::tolower);

	size_t pos = 0;
	while ((pos = contents.find(string("-"), pos)) != std::string::npos) {
		contents.replace(pos, string("-").length(), string(" "));
		pos += contents.length();
	}

        smatch m;
        regex e("[a-zA-Z]+(?![<\\w>])");
        while(regex_search(contents, m, e))
        {
                for(auto word:m)
		{
			if(stopList.find(string(word)) == stopList.end())	// The word is not in the stop list
			{
				if(index->lexicon.find(word) != index->lexicon.end()) {
                                        //Existed word in hashmap
                                        Lexicon& term = index->lexicon[word];
					if(term.fdt.find(key) != term.fdt.end())
					{
						term.fdt[key].setFrequency();
					}
					else
					{
						CollectionFrequency newFdt(key, 1);
						term.docFrequency++;
						term.fdt.insert(std::pair<unsigned int, CollectionFrequency>(newFdt.docId, newFdt));
					}
                                } else {
                                        //New word in hashmap
                                        Lexicon newTerm(word, 0);
					newTerm.fdt.insert(std::pair<unsigned int, CollectionFrequency>(key, CollectionFrequency(key ,1)));
                                        newTerm.docFrequency++;
                                        index->lexicon.insert(std::pair<std::string, Lexicon>(word, newTerm));
                                }			
			}	
		}
                contents = m.suffix().str();
        }
	return;
}


void appendAttributes(string& str, Json::Value& attributes)
{
        for(vector<string>::iterator it = attr_key.begin(); it != attr_key.end(); it++)
        {
                if(attributes.isMember(*it))
                {
                        if(attributes[(*it)].asString().compare(string("true")) == 0)
			{
				string word = attributes[(*it)].asString(); 
				transform(word.begin(), word.end(), word.begin(), ::tolower);
				str.append(" " + word);
			}
                }
        }
        return;

}

void appendCategories(string& str, Json::Value& categories)
{
	for(const Json::Value& word : categories)
	{
		string wd = word.asString();
		transform(wd.begin(), wd.end(), wd.begin(), ::tolower);
		str.append(" " + wd);
	}
	return;
}

void createTermIndex(string path, Index* index, map<unsigned int, Document>& docMap){
	fstream file(path.c_str());


	Json::Value root;
	Json::Reader reader;


	//Read content of a new document
	string review;
	int line_number = 0;	// Record read line number
	while(getline(file, review))
	{
		reader.parse(review, root, false);
		unsigned int docId = root["hash_value"].asUInt();
	
		Document doc(docId);
		//Set document length
		doc.setDocumentLength(review.length());
		doc.setDocOffset(line_number);
		docMap.insert(pair<unsigned int, Document>(docId, doc));
		line_number++;
		string r = root["text_review"].asString();
		if(root.isMember("attributes"))
		{
			appendAttributes(r, root["attributes"]);
		}
		if(root.isMember("categories"))
		{
			appendCategories(r, root["categories"]);
		}
		parseText(r, docId, index);
		review.clear();
	}
	file.close();
}

void readStopList(string stopListPath) {
	string word;
	ifstream in_stream(stopListPath.c_str());
	in_stream.seekg(in_stream.beg);
	while(getline(in_stream, word)) {
		stopList.insert(pair<string, string>(word, word));
	}
	in_stream.close();
	return;
}

void writeFile(map<string, Lexicon>& lexicon, map<unsigned int, Document>& docMap) {
	string lexiconFilePath("/home/cheetah/Desktop/irIndex/lexicon");
	string invlistsFilePath("/home/cheetah/Desktop/irIndex/ivlist");
	string docMapFilePath("/home/cheetah/Desktop/irIndex/map");
	
	ofstream lexiconFile(lexiconFilePath);
	ofstream invlistsFile(invlistsFilePath);
	ofstream docmapFile(docMapFilePath);

	cout<<"Write map start"<<endl;
	for (map<unsigned int, Document>::iterator it = docMap.begin(); it != docMap.end(); it++) {
			string content = to_string(it->second.docNo) + " " + to_string(it->second.docOffset) + "\n";
			docmapFile<<content;
	}
	cout<<"Write map end"<<endl<<endl;
	
	cout<<"Write lexicon and invlists start"<<endl;
	int offset = 0;	//Calculate the term position in invert list file
	for(map<string, Lexicon>::iterator it = lexicon.begin(); it != lexicon.end(); it++) {
		//Write document frequency
		invlistsFile.write((char*)&it->second.docFrequency, sizeof(int));
		int num = 0;	//count the numbers of document for each term
		for(map<unsigned int, CollectionFrequency>::iterator fdtIt = it->second.fdt.begin(); fdtIt != it->second.fdt.end(); fdtIt++) {
			unsigned int* withinFrequency = new unsigned int[2];
			withinFrequency[0] = fdtIt->second.docId;
			withinFrequency[1] = fdtIt->second.fdt;
			//Write within document frequency
			invlistsFile.write((char*)withinFrequency, sizeof(int));
			invlistsFile.write((char*)(withinFrequency + 1), sizeof(int));
			delete[] withinFrequency;
			num++;
		}
		
		//Write lexicon file
		string content = it->second.word + " " + to_string(it->second.docFrequency) + " " + to_string(offset);
		lexiconFile<<content<<endl;
		offset = offset + (num * 2 + 1) * sizeof(unsigned int);
	}
	cout<<"Write lexicon and invlists end"<<endl;
	
	lexiconFile.close();
	invlistsFile.close();
}

int main()
{
	string stopListPath = "/home/cheetah/Desktop/stop-words_english_6_en.txt";

	readStopList(stopListPath);
	cout<<stopList.size()<<endl;

	map<unsigned int, Document> docMap;

	string path("/home/cheetah/Desktop/new_random_docId");
	Index* index = new Index();
	createTermIndex(path, index, docMap);
	writeFile(index->lexicon, docMap);
	docMap.clear();
	delete index;	
}
