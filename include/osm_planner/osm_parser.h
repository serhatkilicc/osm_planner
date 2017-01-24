//
// Created by michal on 31.12.2016.
//

#ifndef PROJECT_OSM_PARSER_H
#define PROJECT_OSM_PARSER_H


#include <tinyxml.h>
#include <ros/ros.h>
#include <visualization_msgs/Marker.h>
#include <nav_msgs/Path.h>

class OsmParser {
public:

    typedef struct osm_node {
        int id;
        double latitude;
        double longitude;
    } OSM_NODE;

    typedef struct osm_way {
        int id;
        std::vector<int> nodesId;
    } OSM_WAY;

    typedef struct translate_table {
        int oldID;
        int newID;
    } TRANSLATE_TABLE;

    OsmParser(std::string xml);

    //publishing functions
    void publishPoint(int pointID, visualization_msgs::Marker::_color_type color);
    void publishPoint(double latitude, double longitude, visualization_msgs::Marker::_color_type color);
    void publishPath();
    void publishPath(std::vector<int> nodesInPath);
    void publishPath(std::vector<int> nodesInPath, double target_lat, double target_lon);

    //GETTERS
    std::vector< std::vector<double> > getGraphOfVertex(); //for dijkstra algorithm
    int getNearestPoint(double lat, double lon); //return OSM node ID

    //SETTERS
    void setStartPoint(double latitude, double longitude); //set the zero cartezian point

protected:

    double getDistance(OSM_NODE node1, OSM_NODE node2);
    double getBearing(OSM_NODE node1, OSM_NODE node2);

private:

    //publishers
    ros::Publisher marker_pub;
    ros::Publisher path_pub;
    ros::Publisher shortest_path_pub;

    //msgs for shortest path
    nav_msgs::Path sh_path;

    //vector arrays of OSM nodes and ways
    std::vector <OSM_WAY> ways;
    std::vector <OSM_NODE> nodes;
    std::vector <TRANSLATE_TABLE> table;
    std::vector <std::vector <double> > networkArray;

    //start point - must be set and than you can publishing paths
    OSM_NODE startPoint;

    void createWays(TiXmlHandle* hRootWay);
    void createNodes(TiXmlHandle *hRootNode);
    void createNetwork();
    void getNodesInWay(TiXmlElement* wayElement, OSM_WAY *way);
    bool translateID(int id, int *ret_value);
};
#endif //PROJECT_OSM_PARSER_H