#include <iostream>

#include "json/json.h"
#include "ExtractDataset.h"

using namespace std;

void processData(vector<Business>& businessList)
{
	return;
}

int main()
{
	ExtractDataset extractMessage;
	string path = "/home/cheetah/Desktop/Sub_of_Ports_in_Las_Vegas_All.txt";
	extractMessage.readDataset(path);

	processData(extractMessage.businessList);
	return 0;
}
