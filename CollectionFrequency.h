#ifndef COLLECTION_FREQUENCY_H
#define COLLECTION_FREQUENCY_H

/* A link list implementation of postings */
class CollectionFrequency {
public:
	unsigned int docId;	/* Identifier of document */
	int fdt;	/* A count of how often t occurs in document d */

	CollectionFrequency()
	{
	}	

	CollectionFrequency(unsigned int docId, int fdt) {
		this->docId = docId;
		this->fdt = fdt;
	}
	
	void setFrequency(){
			this->fdt++;
	}
};

#endif
