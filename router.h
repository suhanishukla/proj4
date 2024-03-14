//
//  router.h
//  cs32project4
//
//  Created by Suhani Shukla on 3/13/24.
//

#ifndef router_h
#define router_h

#include "base_classes.h"
#include "geodb.h"
#include <queue>
#include <unordered_set>

class Router: public RouterBase {
public:
    Router(const GeoDatabaseBase& geo_db) : m_geo_db(geo_db){}
    virtual ~Router();
    virtual std::vector<GeoPoint> route(const GeoPoint& pt1, const GeoPoint& pt2) const {
        std::vector<GeoPoint> result;
        std::queue<GeoPoint> q;
        std::unordered_set<GeoPoint> visited;
        //push starting point
        q.push(pt1);
        visited.insert(pt1);
        while (!q.empty()) {
            //get front item of queue
            GeoPoint curr = q.front();
            q.pop();
            double currlat = curr.latitude;//maybe need better way to access lat and lon of struct
            double currlon = curr.longitude;
            //check if ending
            if (currlat == pt2.latitude and currlon == pt2.longitude) {
                //add elements in locationofprevious to return vector
                
            }
            //check connecting points
            std::vector<GeoPoint> currConnectedPts = m_geo_db.get_connected_points(curr);
            for (int i = 0; i < currConnectedPts.size(); i++) {
                if (visited.find(currConnectedPts[i]) == visited.end()) {
                    //if this connected point is not already visited
                    q.push(currConnectedPts[i]);
                    //add it to visited
                    visited.insert(currConnectedPts[i]);
                    locationOfPreviousWayPoint.insert(curr.to_string(), currConnectedPts[i].to_string());
                }
            }
            
        }
        
    }
private:
    HashMap<std::string> locationOfPreviousWayPoint; //string reps of geopoints
    const GeoDatabaseBase& m_geo_db;
    
};

#endif /* router_h */
