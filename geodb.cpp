//
//  geodb.cpp
//  cs32project4
//
//  Created by Suhani Shukla on 3/11/24.
//
//#include "geodb.h"
//#include "base_classes.h"
//#include "geotools.h"
//#include "geopoint.h"
//#include <iostream>
//#include <fstream>
//
//
//bool GeoDatabase::load(const std::string& map_data_file) {
//    std::ifstream infile(map_data_file);
//    if (!infile) {
//        //unable to open file
//        return false;
//    }
//    std::string line;
//    std::string currStreetName;
//    std::string streetName;
//    double lat1, lon1, lat2, lon2;
//    int numPoi;
//    while (std::getline(infile, line)) {
//        streetName = line;
//        std::getline(infile, line);
//        std::istringstream coordinatesStream(line);
//        coordinatesStream >> lat1 >> lon1 >> lat2 >> lon2;
//        
//        GeoPoint startPoint(lat1, lon1);
//        GeoPoint endPoint(lat2, lon2);
//        
//        processStreetSegment(startPoint, endPoint, streetName);
//        std::getline(infile, line);
//        std::istringstream numPointsStream(line);
//        numPointsStream >> numPoi;
//        
//        //processStreetSegment(currStreetName, lat1, lon1, lat2, lon2, numPoi);
//        infile.close();
//        return true;
//        
//    }
//}
//void GeoDatabase::processStreetSegment(const GeoPoint& startPoint, const GeoPoint& endPoint, const std::string& streetName) {
//
//};
//
//
//
