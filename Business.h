/*
 * Business.h
 *
 *  Created on: 19 Dec 2015
 *      Author: admin
 */

#ifndef BUSINESS_H_
#define BUSINESS_H_

#include <string>
#include <map>
#include <vector>

class Business {
public:
    std::string businessId;
    std::string businessName;
    std::map<std::string, std::string> attributes;  // Attributes of the business
    std::vector<std::string> categories;    // Categories of the business
    std::string userId;
    double longitude;
    double latitude;
    std::string address;
    std::string textReview;
    double rating;

    Business(){};

    // Constructor
    Business(std::string iBusinessId,
            std::string iBusinessName,
            std::map<std::string, std::string> iAttributes,
            std::vector<std::string> iCategories,
            std::string iUserId,
            double iLongitude = 0,
            double iLatitude = 0,
            std::string iAddress = "",
            std::string iTextReview = "",
            double iRating = -1) : businessId(iBusinessId),
                                        businessName(iBusinessName),
                                        attributes(iAttributes),
                                        categories(iCategories),
                                        userId(iUserId),
                                        longitude(iLongitude),
                                        latitude(iLatitude),
                                        address(iAddress),
                                        textReview(iTextReview),
                                        rating(iRating)
    {}
};



#endif /* BUSINESS_H_ */
