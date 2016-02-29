#ifndef DOCUMENT_H
#define DOCUMENT_H

#include <string>

class Document
{
public:
	/* Add for assign 2 start */
	long documentLength;
	long docOffset;	//	The offset position of each document in a file
	/* Add for assign 2 end */
	
	unsigned int docNo;
	
	Document()
	{}

	Document(unsigned int docNo) {
		this->docNo = docNo;
	}

	Document(unsigned int docNo, long docOffset)
	{
		this->docNo = docNo;
		this->docOffset = docOffset;
	}
	
	Document(const Document& doc)
	{
		this->documentLength = doc.documentLength;
		this->docOffset = doc.docOffset;
		this->docNo = doc.docNo;
	}

	void setDocNo(unsigned int docNo) {
		this->docNo = docNo;
	}
	
	void setDocumentLength(long documentLength) {
		this->documentLength = documentLength;
	}
	
	void setDocOffset(long docOffset) {
		this->docOffset = docOffset;
	}
};

#endif
