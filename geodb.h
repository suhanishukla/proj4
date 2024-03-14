//
//  geodb.hpp
//  cs32project4
//
//  Created by Suhani Shukla on 3/11/24.
//

#ifndef geodb_h
#define geodb_h

#include <stdio.h>
#include "geotools.h"
#include "base_classes.h"
#include "HashMap.h"
#include <vector>
#include <string>
#include "geopoint.h"
#include "geotools.h"

struct StreetNames {
    std::vector<std::string> names;
};

class GeoDatabase : public GeoDatabaseBase
{
public:
    GeoDatabase();
    virtual ~GeoDatabase();
    virtual bool load(const std::string& map_data_file) {

            std::ifstream infile(map_data_file);
            if (!infile) {
                //unable to open file
                return false;
            }
            std::string line;
            std::string streetName;
            std::string lat1, lon1, lat2, lon2;
            int nPoi;
            std::string poiName;
            while (std::getline(infile, line)) {
                //read in streetname
                std::getline(infile, line);
                streetName = line;
                //read in geopoints of start and end of segment
                std::istringstream coordinatesStream(line);
                coordinatesStream >> lat1 >> lon1 >> lat2 >> lon2;
                
                
                GeoPoint s = GeoPoint(lat1, lon1);
                GeoPoint e = GeoPoint(lat2, lon2);
                GeoPoint MP;
                std::vector<GeoPoint> mptemp;
                //read in number of poi
                infile >> nPoi;
                infile.ignore(10000, '\n');
                
                if (nPoi > 0) {
                    MP = midpoint(s, e);
                    //insert the streetname
                    std::vector<std::string> temp;
                    temp.push_back(streetName);
                    StreetNames tempstruct;
                    tempstruct.names = temp;
                    streetNamesMap.insert(MP.to_string(), tempstruct);
                }
                //read in names and geopoints of poi
                for (int i = 0; i < nPoi; i++) {
                    std::getline(infile, poiName, '|');
                    std::istringstream coordinatesStream(line);
                    coordinatesStream >> lat1 >> lon1;
                    GeoPoint currPoi = GeoPoint(lat1, lon1);
                    poiLocations.insert(poiName, currPoi);
                    
                    std::vector<GeoPoint> poitemp;
                    mptemp.push_back(currPoi);
                    poitemp.push_back(MP);
                    connectedPoints.insert(currPoi.to_string(), poitemp);
                }
                //add start and end of street segment to connectedPoints
                std::vector<GeoPoint> stemp, etemp;
                stemp.push_back(e);
                etemp.push_back(s);
                if (nPoi > 0) {
                    stemp.push_back(MP);
                    etemp.push_back(MP);
                    mptemp.push_back(e);
                    mptemp.push_back(s);
                }
                connectedPoints.insert(s.to_string(), stemp);
                connectedPoints.insert(e.to_string(), etemp);
                connectedPoints.insert(MP.to_string(), mptemp);
                
                //insert streetnames for start and end
                
                
                
                if (streetNamesMap.find(s.to_string()) != nullptr) {
                    streetNamesMap.find(s.to_string())->names.push_back(streetName);
                } else {
                    std::vector<std::string> temp;
                    temp.push_back(streetName);
                    StreetNames tempstruct;
                    tempstruct.names = temp;
                    streetNamesMap.insert(s.to_string(), tempstruct);
                }
                if (streetNamesMap.find(e.to_string()) != nullptr) {
                    streetNamesMap.find(e.to_string())->names.push_back(streetName);
                } else {
                    std::vector<std::string> temp;
                    temp.push_back(streetName);
                    StreetNames tempstruct;
                    tempstruct.names = temp;
                    streetNamesMap.insert(e.to_string(), tempstruct);
                }
                
                
            }
        return true;
    }
    virtual bool get_poi_location(const std::string& poi, GeoPoint& point) const {
        if (poiLocations.find(poi) == nullptr) {return false;}
    
        point.latitude = poiLocations.find(poi)->latitude;
        point.longitude = poiLocations.find(poi)->longitude;
        point.sLatitude = poiLocations.find(poi)->sLatitude;
        point.sLongitude = poiLocations.find(poi)->sLongitude;
        return true;
    }
    virtual std::vector<GeoPoint> get_connected_points(const GeoPoint& pt) const {
        std::vector <GeoPoint> result;
         connectedPoints.find(pt.to_string());
        for (int i = 0; i < connectedPoints.find(pt.to_string())->size(); i++) {
            result[i] = connectedPoints.find(pt.to_string())->at(i);
       }
        return result;
    }
    virtual std::string get_street_name(const GeoPoint& pt1, const GeoPoint& pt2) const {
        std::string result = "";
        
        StreetNames temp1;
        StreetNames temp2;
        std::vector<std::string> pt1streets;
        std::vector<std::string> pt2streets;
        if (streetNamesMap.find(pt1.to_string()) != nullptr) {
            //pt1 is start end or midpoint
            temp1.names = streetNamesMap.find(pt1.to_string())->names;
            if (streetNamesMap.find(pt2.to_string()) != nullptr) {
                //pt2 is start end or midpoint
                temp2.names = streetNamesMap.find(pt2.to_string())->names;
                //find common names between temp1 and temp2
                pt1streets = temp1.names;
                pt2streets = temp2.names;
                for (int i = 0; i < pt1streets.size(); i++) {
                    for (int j = 0; j < pt2streets.size(); j++) {
                        if (pt1streets[i] == pt2streets[j]) {
                            result = pt1streets[i];
                        }
                    }
                }
            } else if (connectedPoints.find(pt2.to_string()) != nullptr) {
                //pt2 is a poi
                if (connectedPoints.find(pt2.to_string())->at(0).longitude == pt1.longitude and connectedPoints.find(pt2.to_string())->at(0).latitude == pt1.latitude) {
                    result = "a path";
                }
            }
        } else if (connectedPoints.find(pt1.to_string()) != nullptr) {
            //p1 is a poi
            if (connectedPoints.find(pt1.to_string())->at(0).longitude == pt2.longitude and connectedPoints.find(pt1.to_string())->at(0).latitude == pt2.latitude) {
                result = "a path";
            }
            
        }
            //if pt1 is poi
        
        return result;
    }
        
    
private:
    HashMap<GeoPoint> poiLocations; //maps from poi name to poi location
    HashMap<std::vector<GeoPoint>> connectedPoints; //maps from string rep of geopoints to vector of connected geopoints
    HashMap<StreetNames> streetNamesMap;


};

//get connected geopoints
//if given midpoint return points of intereset on the street and begining and end of street
//if given point of interest return midpoint
//get street name

//store geopoints and streetname 

//hashmap key is a string with is poi name
//value is struct streetsegment struct with geopoint start end name of street and geopoint midpoint


#endif /* geodb_hpp */
