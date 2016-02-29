#include <iostream>
#include <string>
#include <fstream>
#include <limits>

#include "Lexicon.h"
#include "Document.h"
#include "json/json.h"

using namespace std;

int main()
{
	map<string, Lexicon> lexiconMap;
	map<unsigned int, Document> docMap;
	ifstream ivsFile(string("/home/cheetah/Desktop/ivlist").c_str());
	ifstream lexiconfile(string("/home/cheetah/Desktop/lexicon").c_str());
	ifstream mapFile(string("/home/cheetah/Desktop/map").c_str());

	unsigned int docNo;
	while(mapFile>>docNo)
	{
		long docOffset;
		mapFile>>docOffset;
		docMap.insert(pair<unsigned int, Document>(docNo, Document(docNo, docOffset)));
	}

	string word;
	while (lexiconfile>>word) {
		int docFrequency;
		lexiconfile>>docFrequency;
		long invlistPos;
		lexiconfile>>invlistPos;
		lexiconMap.insert(pair<string, Lexicon>(word, Lexicon(word, invlistPos, docFrequency)));
	}

	cout<<lexiconMap.size()<<endl;
		
	string w = "adds";
	Lexicon term = lexiconMap[w];
	long pos = term.location;
	ivsFile.seekg(pos, ivsFile.beg); // Jump to the relavant start position
							// in invert list
	int ft = term.docFrequency; // Read document frequency
	cout<<"doc freq: "<<ft<<endl;
	ivsFile.read((char*)&ft, sizeof(int));
	cout<<"doc freq in invs: "<<ft<<endl<<endl;
	for (int j = 0; j < ft; j++) {
		unsigned int docId;
		ivsFile.read((char*)&docId, sizeof(int));
		cout<<"doc Id: "<<docId<<endl;
		int fdt;
		ivsFile.read((char*)&fdt, sizeof(int));
		cout<<"within doc freq: "<<fdt<<endl;

		cout<<"Doc line: "<<docMap[docId].docOffset<<endl;
		long line_number = docMap[docId].docOffset;
		cout<<"jump line: "<<line_number<<endl;

		Json::Value root;
		Json::Reader reader;

		ifstream jsonFile(std::string("/home/cheetah/Desktop/new").c_str());
		// Jump to specific line
		jsonFile.seekg(jsonFile.beg);
		for(int i = 0; i < line_number; i++){
			jsonFile.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
		}

		string line;
		cout<<"^^^^^"<<line<<endl;
		getline(jsonFile, line);
		cout<<"*****"<<line<<endl;
                reader.parse(line, root, false);
               	//cout<<root["text_review"].asString()<<endl;
		line.clear();
		jsonFile.close();

		
//		CollectionFrequency collection(docId, fdt);
//		term.getFdt().put(docId, collection);
//		if (!documents.containsKey(docId)) {
			// Initiate documents list
//			documents.put(docMap.get(docId), 0f);
//		}
	}
	ivsFile.close();
}
