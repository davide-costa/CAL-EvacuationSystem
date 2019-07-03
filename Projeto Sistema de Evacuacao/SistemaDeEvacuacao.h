#pragma once

#include "Graph.h"
#include "Coords.h"
#include "Car.h"
#include "graphviewer.h"

#include <unordered_map>


using namespace std;

class SistemaDeEvacuacao
{
private:
	Graph<Coords> map_graph;
	Graph<Coords> reversed_map_graph;
	GraphViewer *graph_viewer;
	unordered_map<int, Vertex<Coords> *> vertex_of_node_id;
	unordered_map<int, Vertex<Coords> *> reversed_vertex_of_node_id;
	unordered_map<int, bool> road_is_2_ways;
	unordered_map<int, string> road_name;
	unordered_multimap<int, int> node_ids_of_road;
	vector<Car> cars;
	vector<vector<int>> issue_vertexes_ids_groups;
	vector<vector<Coords>> issue_coords_groups;
	vector<vector<Vertex<Coords>*>> strongly_conected_trees;
	int map_max_x = INT_MIN;
	int map_max_y = INT_MIN;
	int map_min_x = INT_MAX;
	int map_min_y = INT_MAX;
public:
	class ParsingFileException
	{
	private:
		unsigned int line;
	public:
		ParsingFileException(unsigned int line)
		{
			this->line = line;
		}
		unsigned int GetLine()
		{
			return line;
		}
	};
	class ErrorParsingFile : public ParsingFileException 
	{
	public:
		ErrorParsingFile(unsigned int line) :ParsingFileException(line) {}
	};
	class InvalidInfoParsedFromFile : public ParsingFileException 
	{
	public:
		InvalidInfoParsedFromFile(unsigned int line) :ParsingFileException(line) {}
	};

	SistemaDeEvacuacao();
	void ParseNodesFile(ifstream &nodes_fs);
	void ParseRoadsFile(ifstream &roads_fs);
	void ParseNodesToRoadsFile(ifstream &nodes_to_roads_fs);
	void ParseCars(ifstream &cars_fs);
	void ReadIssueVertexesIds();
	bool ValidateIssueVertexesIds(const string &vertexes_str, const string &error_msg);
	void HandleCarsList();
	void RemoveIssueEdges(Graph<Coords> &map_graph, vector<vector<Edge<Coords>*>> &removed_edges_groups);
	void RemoveIssueEdgesReversed(Graph<Coords> &map_graph, vector<vector<Edge<Coords>*>> &removed_edges_groups);
	void RestoreIssueEdges(Graph<Coords> &map_graph, const vector<vector<Edge<Coords>*>> &removed_edges_groups);
	bool AreVertexesConected(int src_vertex_id, int dest_vertex_id);
	bool CarPathIsAffected(Car &car);
	void CreateGraphViewer(int window_x_size, int window_y_size, string vertex_color, string edge_color);
	void ClearGraphViewerEdges();
	void DisplayMap();
	void ShowCarRoute(unsigned int car_number);
	unsigned int GetCarsSize() const;


	//Strings functions
	Car GetCarFromSrcAndDstStrings(const LicensePlate &plate, string src_road_name, string dst_road_name);
	unsigned int GetDesiredRoadIdFromUser(string road_name);
	unsigned int ReadDesiredSrcRoadIdFromUser(const vector<unsigned int>& ids);
	unsigned int ReadDesiredDstRoadIdFromUser(const vector<unsigned int>& ids);
	unsigned int ReadDesiredNodeIdFromUser(const vector<unsigned int>& ids);
	void DisplayRoads(const vector<unsigned int>& ids, const vector<string>& names);
	void GetNodesOfARoad(unsigned int road_id, vector<unsigned int>& node_ids);
	void DisplayNodesOfARoad(const vector<unsigned int>& node_ids);
};

