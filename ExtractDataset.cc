/*
 * ExtractDataset.cpp
 *
 *  Created on: 23 Dec 2015
 *      Author: admin
 */
#include <fstream>
#include <iostream>
#include <random>
#include <functional>
#include <string>

#include "ExtractDataset.h"
#include "json/json.h"


int ExtractDataset::readDataset(std::string path) {
    Json::Value root;
    Json::Reader reader;
    //Json::StyledStreamWriter writer;
    Json::FastWriter writer;
    // Read json file
    std::ifstream readJsonFile(path, std::ifstream::binary);
    std::ofstream writeJsonFile(std::string("/home/cheetah/Desktop/new"), std::ofstream::binary);
    
    std::string cur_line;
    bool success;
	double min_longitude = 0;
	double max_longitude = 0;
	double min_latitude = 0;
	double max_latitude = 0;


    // Set random
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dis_la(0, 28000);
    std::uniform_real_distribution<double> dis_lo(0, 32000);
    int node_id = 0;
    int cnt = 1;
    do {
        std::getline(readJsonFile, cur_line);
        success = reader.parse(cur_line, root, false);

        if(success) {


            root["longitude"] = dis_lo(gen);
            root["latitude"] = dis_la(gen);

            root["user_id"] = node_id;
            if((cnt % 5) != 0)
            {
                root["user_id"] = node_id;
                cnt++;
            }
            else
            {
                node_id++;
                cnt = 1;
            }
            std::string hash_string = std::to_string(root["user_id"].asInt()) + root["text_review"].asString() + root["business_id"].asString();
	    std::hash<std::string> str_hash;	
            root["hash_value"] = (unsigned int)str_hash(hash_string);

            //writer.write(writeJsonFile, root);
            writeJsonFile<<writer.write(root);
			if(min_longitude == 0)
			{
				min_longitude = root["longitude"].asDouble();
				max_longitude = root["longitude"].asDouble();
				min_latitude = root["latitude"].asDouble();
				max_latitude = root["latitude"].asDouble();
			}
			else
			{
				if(min_longitude > root["longitude"].asDouble())
				{
					min_longitude = root["longitude"].asDouble();
				}
				else if(max_longitude < root["longitude"].asDouble())
				{
					max_longitude = root["longitude"].asDouble();
				}
				else if(min_latitude > root["latitude"].asDouble())
				{
					min_latitude = root["latitude"].asDouble();
				}
				else if(max_latitude < root["latitude"].asDouble())
				{
					max_latitude = root["latitude"].asDouble();
				}
			}
	}
    } while (success);
	std::cout<<"min_longitude: "<<min_longitude<<" max_longitude: "<<max_longitude<<std::endl;
	std::cout<<"min_latitude: "<<min_latitude<<" max_latitude"<<max_latitude<<std::endl;	
    readJsonFile.close();
    writeJsonFile.close();
    return 0;
}
