#include "SistemaDeEvacuacao.h"
#include "Graph.h"
#include "utils.h"
#include "String.h"

#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>

#define ALGORITHM_COMPLEXITY_REPORTING false
#define WEIGTH_WHEN_CONGESTED 1000000.0  //10000000 to ensure the congested edge by traffic is only chosen in last case
#define MAX_DEVIATION 5

SistemaDeEvacuacao::SistemaDeEvacuacao()
{

}

void SistemaDeEvacuacao::ParseNodesFile(ifstream & nodes_fs)
{
	unsigned int line_count = 0;
	string curr_node_str;
	istringstream curr_node_iss;
	int node_id;
	int x, y;
	Vertex<Coords> *curr_vertex;
	Vertex<Coords> *reverse_curr_vertex;


	while (getline(nodes_fs, curr_node_str))
	{
		line_count++;
		//Fill the input string stream curr_node_iss with the content of the current line
		curr_node_iss.str(curr_node_str);

		//Extract the id from the istringstream and check if it has been extracted successfully; if not throw exception
		curr_node_iss >> node_id;
		curr_node_iss.ignore(INT_MAX, ';');
		if (curr_node_iss.fail())
		{
			throw ErrorParsingFile(line_count);
		}

		//Extract the longitude (in radians) from the istringstream and check if it has been extracted successfully; if not throw exception
		curr_node_iss >> x;
		curr_node_iss.ignore(INT_MAX, ';');
		if (curr_node_iss.fail())
		{
			throw ErrorParsingFile(line_count);
		}

		//Extract the latitude (in radians) from the istringstream and check if it has been extracted successfully; if not throw exception
		curr_node_iss >> y;
		if (curr_node_iss.fail())
		{
			throw ErrorParsingFile(line_count);
		}


		Coords coords(x, y, node_id);

		if (coords.GetX() > map_max_x)
			map_max_x = coords.GetX();
		if (coords.GetY() > map_max_y)
			map_max_y = coords.GetY();
		if (coords.GetX() < map_min_x)
			map_min_x = coords.GetX();
		if (coords.GetY() < map_min_y)
			map_min_y = coords.GetY();

		curr_vertex = map_graph.addVertex(coords);
		reverse_curr_vertex = reversed_map_graph.addVertex(coords);

		//Associate the id of the vertex to its pointer using the unordered_map vertex_of_node_id
		vertex_of_node_id[node_id] = curr_vertex;
		reversed_vertex_of_node_id[node_id] = reverse_curr_vertex;

		curr_node_iss.clear();
	}
}

void SistemaDeEvacuacao::ParseRoadsFile(ifstream & roads_fs)
{
	unsigned int line_count = 0;
	string curr_road_str;
	istringstream curr_road_iss;
	int road_id;
	string road_name;
	char is_2_ways_ch;
	bool is_2_ways;


	while (getline(roads_fs, curr_road_str))
	{
		line_count++;

		//Fill the input string stream curr_taxi_iss with the content of the current line
		curr_road_iss.str(curr_road_str);

		//Extract the road_id from the istringstream
		curr_road_iss >> road_id;
		curr_road_iss.ignore(INT_MAX, ';');
		if (curr_road_iss.fail())
		{
			throw ErrorParsingFile(line_count);
		}

		//Extract the road_name from the istringstream
		getline(curr_road_iss, road_name, ';');
		if (curr_road_iss.fail())
		{
			throw ErrorParsingFile(line_count);
		}

		//Extract the is_two_ways value from the istringstream
		curr_road_iss >> is_2_ways_ch;
		if (curr_road_iss.fail())
		{
			throw ErrorParsingFile(line_count);
		}

		//Process the is_2_ways value
		if (is_2_ways_ch == 'F')
			is_2_ways = false;
		else
			is_2_ways = true;

		road_is_2_ways[road_id] = is_2_ways;
		this->road_name[road_id] = road_name;
	}
}

void SistemaDeEvacuacao::ParseNodesToRoadsFile(ifstream & nodes_to_roads_fs)
{
	unsigned int line_count = 0;
	string curr_edge_str;
	istringstream curr_edge_iss;
	unsigned int edge_id = 1;
	int road_id, node1_id, node2_id;
	unsigned int capacity;

	while (getline(nodes_to_roads_fs, curr_edge_str))
	{
		line_count++;
		//Fill the input string stream curr_edge_iss with the content of the current line
		curr_edge_iss.str(curr_edge_str);

		//Extract the road_id from the istringstream and check if it has been extracted successfully; if not throw exception
		curr_edge_iss >> road_id;
		curr_edge_iss.ignore(INT_MAX, ';');
		if (curr_edge_iss.fail())
		{
			throw ErrorParsingFile(line_count);
		}

		//Extract the node1_id from the istringstream and check if it has been extracted successfully; if not throw exception
		curr_edge_iss >> node1_id;
		curr_edge_iss.ignore(INT_MAX, ';');
		if (curr_edge_iss.fail())
		{
			throw ErrorParsingFile(line_count);
		}

		//Extract the node2_id from the istringstream and check if it has been extracted successfully; if not throw exception
		curr_edge_iss >> node2_id;
		curr_edge_iss.ignore(INT_MAX, ';');
		if (curr_edge_iss.fail())
		{
			throw ErrorParsingFile(line_count);
		}

		//Extract the capacity of the edge from the istringstream and check if it has been extracted successfully; if not throw exception
		curr_edge_iss >> capacity;
		if (curr_edge_iss.fail())
		{
			throw ErrorParsingFile(line_count);
		}

		//Add the edge that conects two nodes (node1_id to node2_id)
		//To get the vertex that respects to an osm node id, we need to access the unordered_map that maps the osm ids to internal ids, and then to the unordered_map that maps the internal id to the Vertex *
		Vertex<Coords> *vertex1 = vertex_of_node_id[node1_id];
		Vertex<Coords> *vertex2 = vertex_of_node_id[node2_id];
		Vertex<Coords> *reversed_vertex1 = reversed_vertex_of_node_id[node1_id];
		Vertex<Coords> *reversed_vertex2 = reversed_vertex_of_node_id[node2_id];

		double dist_btw_nodes = DistanceBetCartesianCoords(vertex1->getInfo(), vertex2->getInfo());

		vertex1->addEdge(edge_id++, vertex2, dist_btw_nodes, road_id, capacity);
		vertex2->IncrementIndegree();
		reversed_vertex2->addEdge(edge_id++, reversed_vertex1, dist_btw_nodes, road_id, capacity);
		reversed_vertex1->IncrementIndegree();

		pair<unordered_multimap<int, int>::iterator, unordered_multimap<int, int>::iterator> range = node_ids_of_road.equal_range(road_id);
		pair<const int, int> entry1(road_id, node1_id);
		pair<const int, int> entry2(road_id, node2_id);
		if (find(range.first, range.second, entry1) == range.second)
			node_ids_of_road.insert(entry1);
		if (find(range.first, range.second, entry2) == range.second)
			node_ids_of_road.insert(entry2);

		if (road_is_2_ways[road_id])
		{
			vertex2->addEdge(edge_id++, vertex1, dist_btw_nodes, road_id, capacity);
			vertex1->IncrementIndegree();
			reversed_vertex1->addEdge(edge_id++, reversed_vertex2, dist_btw_nodes, road_id, capacity);
			reversed_vertex2->IncrementIndegree();

		}

		curr_edge_iss.clear();
	}

	for (unordered_map<int, string>::iterator it = road_name.begin(); it != road_name.end(); it++)
	{
		if (node_ids_of_road.find(it->first) == node_ids_of_road.end())
		{
			cout << "Uma das ruas lida do ficheiro de ruas nao tem nos correspondentes. Por favor verifique a validade dos ficheiros. O programa ira terminar." << endl << endl;
			system("pause");
			exit(1);
		}
	}
}

void SistemaDeEvacuacao::ParseCars(ifstream & cars_fs)
{
	unsigned int line_count = 0;
	string curr_car_str;
	istringstream curr_car_iss;
	string src_road_name, dst_road_name;
	int source_node_id, dst_node_id;
	LicensePlate curr_plate;

	while (getline(cars_fs, curr_car_str))
	{
		line_count++;
		curr_car_iss.str(curr_car_str);
		try
		{
			curr_car_iss >> curr_plate;
		}
		catch (LicensePlate::ErrorReadingLicensePlate)
		{
			throw ErrorParsingFile(line_count);
		}
		catch (LicensePlate::InvalidLicensePlate)
		{
			throw InvalidInfoParsedFromFile(line_count);
		}
		curr_car_iss.ignore(INT_MAX, ';');

		//Extract the road_name from the istringstream
		getline(curr_car_iss, src_road_name, ';');
		if (curr_car_iss.fail())
		{
			throw ErrorParsingFile(line_count);
		}
		RemoveSpacesFromPhrase(src_road_name);

		//Extract the road_name from the istringstream
		getline(curr_car_iss, dst_road_name, ';');
		if (curr_car_iss.fail())
		{
			throw ErrorParsingFile(line_count);
		}
		RemoveSpacesFromPhrase(dst_road_name);

		Car car = GetCarFromSrcAndDstStrings(curr_plate, src_road_name, dst_road_name);
		if (!(car.GetSource() == 0 || car.GetDestination() == 0))
			cars.push_back(car);

		curr_car_iss.clear();
	}
}

void SistemaDeEvacuacao::ReadIssueVertexesIds()
{
	cout << "O acidente acontece em arestas (ligacoes entre nos) e deve introduzir pelo menos dois nos, de forma a que exista pelo menos uma aresta interrompida (afetada por um acidente)." << endl;
	cout << "O programa assume que as arestas que ligam os nos introduzidos estao incacessiveis devido ao acidente." << endl;
	cout << "Pode introduzir varios grupos de arestas. Introduza cada grupo de arestas numa linha diferente (ENTER) para separar os varios grupos de arestas" << endl << endl;
	cout << "Introduza os ids dos nos (do ficheiro de nos) onde ocorreu o acidente, separados por espaco:" << endl;
	string error_msg = "Ocorreu um erro ao ler um dos ids dos nos que introduziu. Por favor, tente novamente, introduzindo um id valido: ";
	string vertexes_str;
	bool at_least_one_valid_edge = false;
	getline(cin, vertexes_str);
	RemoveSpacesFromPhrase(vertexes_str);
	while (true)
	{
		if (cin.fail() || vertexes_str.empty())
		{
			if (cin.eof())
				if (at_least_one_valid_edge)
					break;
				else
					cin.clear();
			else if (!vertexes_str.empty())
			{
				cin.clear();
				cin.ignore(INT_MAX, '\n');
			}
			cout << endl << error_msg;
			getline(cin, vertexes_str);
			RemoveSpacesFromPhrase(vertexes_str);
			continue;
		}
		if (ValidateIssueVertexesIds(vertexes_str, error_msg))
			at_least_one_valid_edge = true;

		getline(cin, vertexes_str);
		RemoveSpacesFromPhrase(vertexes_str);
	}
}

bool SistemaDeEvacuacao::ValidateIssueVertexesIds(const string &vertexes_str, const string &error_msg)
{
	vector<int> temp_issue_vertexes_ids;
	vector<Coords> temp_issue_coords;
	istringstream vertexes_iss;
	vertexes_iss.str(vertexes_str);
	int vertex_id;
	Coords vertex_coords;
	vector<int> wrong_ids;
	while (true)
	{
		vertexes_iss >> vertex_id;
		if (vertexes_iss.fail())
		{
			if (vertexes_iss.eof())
				break;
			else
			{
				cout << error_msg;
				return false;
			}
		}
		else
		{
			//Check if the node exists; if not, add to the vector of nodes that don't exist, to show to the user the ones that are invalid
			if (vertex_of_node_id.find(vertex_id) == vertex_of_node_id.end())
			{
				wrong_ids.push_back(vertex_id);
			}
			else
			{
				vertex_coords = vertex_of_node_id[vertex_id]->getInfo();
				//Check if the node has already been introduced by the user (it cannot be repeated)
				if (find(temp_issue_vertexes_ids.begin(), temp_issue_vertexes_ids.end(), vertex_id) != temp_issue_vertexes_ids.end())
				{
					cout << endl << endl;
					cout << "O seguinte id introduzido encontra-se repetido pelo menos duas vezes na lista de nos que introduziu: ";
					cout << vertex_id;
					cout << endl << endl;
					cout << "\nPor favor, tente novamente, introduzindo ids de nos validos: ";
					return false;
				}
				temp_issue_vertexes_ids.push_back(vertex_id);
				temp_issue_coords.push_back(vertex_coords);

			}
		}
	}

	unsigned int issue_vertexes_size = temp_issue_vertexes_ids.size();

	if (!wrong_ids.empty())
	{
		if (wrong_ids.size() == 1)
		{
			cout << endl << endl;
			cout << "O seguinte id introduzido nao corresponde a nenhum no do mapa carregado dos ficheiros:";
		}
		else if (wrong_ids.size() > 1)
		{
			cout << endl << endl;
			cout << "Os seguintes ids introduzidos nao correspondem a nenhum no do mapa carregado dos ficheiros:";
		}
		cout << endl;
		for (unsigned int i = 0; i < wrong_ids.size(); i++)
		{
			cout << wrong_ids[i] << endl;
		}

		cout << "\nPor favor, tente novamente, introduzindo ids de nos validos: ";
		return false;
	}

	if (issue_vertexes_size < 2)
	{
		cout << "Tem de introduzir pelo menos 2 ids de nos" << endl << endl;
		cout << "\nPor favor, tente novamente, introduzindo ids de nos validos: ";
		return false;
	}

	//Check if the issue_vertexes matches a part of a road
	for (unsigned int i = 0; i < issue_vertexes_size - 1; i++)
	{
		Vertex<Coords> *src_vertex = vertex_of_node_id[temp_issue_vertexes_ids[i]];
		Vertex<Coords> *dest_vertex = vertex_of_node_id[temp_issue_vertexes_ids[i + 1]];
		vector<Edge<Coords>> adjacent_edges;
		src_vertex->GetAdjacentEdges(adjacent_edges);

		bool adjacent_found = false;
		unsigned int adjacent_edges_size = adjacent_edges.size();
		for (unsigned int j = 0; j < adjacent_edges_size; j++)
		{
			if (adjacent_edges[j].GetDest() == *dest_vertex)
			{
				adjacent_found = true;
				break;
			}
		}

		if (!adjacent_found)
		{
			cout << endl << endl;
			cout << "Nao existe ligacao entre os ids fornecidos. Introduza novamente um conjunto de ids:";
			return false;
		}
	}

	issue_vertexes_ids_groups.push_back(temp_issue_vertexes_ids);
	issue_coords_groups.push_back(temp_issue_coords);
	return true;
}

void SistemaDeEvacuacao::HandleCarsList()
{
#if ALGORITHM_COMPLEXITY_REPORTING
	unsigned long long int total_time = 0;
#endif
	//Calculate the strongly connected trees for verfication if the source and destination vertexes are conected
	strongly_conected_trees = map_graph.StronglyConnectedComponents(reversed_map_graph);

	//Separate between affected cars by the accident and not affected
	for (unsigned int i = 0; i < cars.size(); i++)
	{
		if (!AreVertexesConected(cars[i].GetSource(), cars[i].GetDestination()))
		{
			cout << "O carro com matricula " << cars[i] << " segue um caminho nao conexo (o ponto de destino nao e alcancavel a partir do ponto de origem)." << endl;
			cout << "Este carro ira ser ignorado e nao ira ser calculada nenhuma rota de contorno ao local do acidente." << endl << endl;
			cars.erase(cars.begin() + i);
			i--;
			continue;
		}

		//calculate best route (assuming the car always takes the best route (lowest distance))
		Coords src_coords = vertex_of_node_id[cars[i].GetSource()]->getInfo();
		Coords dest_coords = vertex_of_node_id[cars[i].GetDestination()]->getInfo();
#if ALGORITHM_COMPLEXITY_REPORTING
		unsigned long long int start_time = GetTickCount();
#endif
		map_graph.dijkstraShortestPath(src_coords, dest_coords);
#if ALGORITHM_COMPLEXITY_REPORTING
		unsigned long long int elapsed_time = GetTickCount() - start_time;
		total_time += elapsed_time;
#endif
		vector<Coords> path = map_graph.getPath(src_coords, dest_coords);
		cars[i].SetOriginalPath(path);
		if (CarPathIsAffected(cars[i]))
			cars[i].SetPathAffected();
		else
			cars[i].SetPathNotAffected();
	}

	//remove edges that are interrupted by the accident
	vector< vector<Edge<Coords>* >> removed_edges_groups;
	vector< vector<Edge<Coords>* >> reversed_removed_edges_groups;
	RemoveIssueEdges(map_graph, removed_edges_groups);
	RemoveIssueEdgesReversed(reversed_map_graph, reversed_removed_edges_groups);

	//handle affected cars
	//Calculate the strongly connected trees (after removing the edges affected by the accident, which can alter the conectivity) for verfication if the source and destination vertexes are conected
	if (cars.size() != 0)
		strongly_conected_trees = map_graph.StronglyConnectedComponents(reversed_map_graph);
	for (unsigned int i = 0; i < cars.size(); i++)
	{
		if (!cars[i].IsPathAffected())
			continue;

		//see if the graph stays conected between source and destination after removing the accident edge
		if (!AreVertexesConected(cars[i].GetSource(), cars[i].GetDestination()))
		{
			cout << "O carro com matricula " << cars[i] << " nao consegue alcancar o destino com a estrada cortada no local do acidente, nao existindo por isso um outro caminho conexo (o ponto de destino nao e alcancavel a partir do ponto de origem)." << endl;
			cout << "Este carro ira ser ignorado e nao ira ser calculada nenhuma rota de contorno ao local do acidente." << endl << endl;
			cars.erase(cars.begin() + i);
			i--;
			continue;
		}

		//calculate the shortest path not passing by the accident
		Coords src_coords = vertex_of_node_id[cars[i].GetSource()]->getInfo();
		Coords dest_coords = vertex_of_node_id[cars[i].GetDestination()]->getInfo();
		map_graph.dijkstraShortestPath(src_coords, dest_coords);
		vector<Coords> path = map_graph.getPath(src_coords, dest_coords);
		cars[i].SetAlternativePath(path);

		//if the path is not affected, that means that the car will follow the original path
		//Set the fill of the edges accordingly
		unsigned int path_size = path.size();
		for (unsigned int i = 0; i < path_size - 1; i++)
		{
			vector<Edge<Coords>> &adjacent_edges = vertex_of_node_id[path[i].GetId()]->GetAdjacentEdgesReference();
			unsigned int ajacent_edges_size = adjacent_edges.size();
			for (unsigned int j = 0; j < ajacent_edges_size; j++)
			{
				if (adjacent_edges[j].GetDestPointer() == vertex_of_node_id[path[i + 1].GetId()])
				{
					adjacent_edges[j].IncrementFill();
					if (adjacent_edges[j].IsFull())
						adjacent_edges[j].IncrementWeight(WEIGTH_WHEN_CONGESTED);
				}
			}
		}

	}

	//after all alternative routes have been calculated, restore the original graph (i.e. add the edges blocked by the accident in the graph)
	RestoreIssueEdges(map_graph, removed_edges_groups);
	RestoreIssueEdges(reversed_map_graph, reversed_removed_edges_groups);

	cout << "Todos os carros foram processados com sucesso." << endl << endl << "O programa ira agora mostrar os percursos de carros (originais se nao afetados pelo problema; orginais e alternativos se afetados pelo problema." << endl << endl;
#if ALGORITHM_COMPLEXITY_REPORTING
	cout << endl << endl;
	cout << "Detalhes de complexidade do algoritmo dijkstra:" << endl << endl;
	cout << "Numero de carros processados: " << cars.size() << endl;
	cout << "Numero de nos do mapa: " << vertex_of_node_id.size() << endl;
	cout << "Numero de ruas (constituidas por uma ou mais arestas): " << road_name.size() << endl;
	cout << "Tempo demorado: " << total_time << " ms";
	cout << endl << endl;
#endif
	system("pause");
}

void SistemaDeEvacuacao::RemoveIssueEdges(Graph<Coords> &map_graph, vector<vector<Edge<Coords>*>> &removed_edges_groups)
{
	unsigned int issue_coords_groups_size = issue_coords_groups.size();
	for (unsigned int i = 0; i < issue_coords_groups_size; i++)
	{
		vector<Edge<Coords>*> removed_edges;
		unsigned int issue_coords_size = issue_coords_groups[i].size();
		for (unsigned int j = 0; j < issue_coords_size - 1; j++)
		{
			Edge<Coords> *removed_edge = map_graph.removeEdge(issue_coords_groups[i][j], issue_coords_groups[i][j + 1]);
			if (removed_edge != NULL)
				removed_edges.push_back(removed_edge);
		}
		removed_edges_groups.push_back(removed_edges);
	}
}

void SistemaDeEvacuacao::RemoveIssueEdgesReversed(Graph<Coords>& map_graph, vector<vector<Edge<Coords>*>>& removed_edges_groups)
{
	unsigned int issue_coords_groups_size = issue_coords_groups.size();
	for (unsigned int i = 0; i < issue_coords_groups_size; i++)
	{
		vector<Edge<Coords>*> removed_edges;
		unsigned int issue_coords_size = issue_coords_groups[i].size();
		for (unsigned int j = 0; j < issue_coords_size - 1; j++)
		{
			Edge<Coords> *removed_edge = map_graph.removeEdge(issue_coords_groups[i][j + 1], issue_coords_groups[i][j]);
			if (removed_edge != NULL)
				removed_edges.push_back(removed_edge);
		}
		removed_edges_groups.push_back(removed_edges);
	}
}

void SistemaDeEvacuacao::RestoreIssueEdges(Graph<Coords> &map_graph, const vector<vector<Edge<Coords>*>>& removed_edges_groups)
{
	unsigned int removed_edges_groups_size = removed_edges_groups.size();
	for (unsigned int i = 0; i < removed_edges_groups_size; i++)
	{
		unsigned int remove_edges_size = removed_edges_groups[i].size();
		for (unsigned int j = 0; j < remove_edges_size; j++)
		{
			map_graph.addEdge(removed_edges_groups[i][j]->GetID(), issue_coords_groups[i][j], issue_coords_groups[i][j + 1], removed_edges_groups[i][j]->GetWeight(), removed_edges_groups[i][j]->GetRoadId(), removed_edges_groups[i][j]->GetCapacity());
			delete removed_edges_groups[i][j];
		}
	}
}

bool SistemaDeEvacuacao::AreVertexesConected(int src_vertex_id, int dest_vertex_id)
{
	unsigned int strongly_conected_trees_size = strongly_conected_trees.size();
	for (unsigned int i = 0; i < strongly_conected_trees_size; i++)
	{
		unsigned int found = 0;
		unsigned int strongly_conected_tree_size = strongly_conected_trees[i].size();
		for (unsigned int j = 0; j < strongly_conected_tree_size && found != 2; j++)
		{
			if (strongly_conected_trees[i][j] == vertex_of_node_id[src_vertex_id])
				found++;
			else if (strongly_conected_trees[i][j] == vertex_of_node_id[dest_vertex_id])
				found++;
		}

		if (found == 2)
			return true;
	}

	return false;
}

void SistemaDeEvacuacao::CreateGraphViewer(int window_x_size, int window_y_size, string vertex_color, string edge_color)
{
	int size_x = (int)round(map_max_x - map_min_x);
	int size_y = (int)round(map_max_y - map_min_y);

	graph_viewer = new GraphViewer(size_x, size_y, false);
	graph_viewer->createWindow(window_x_size, window_y_size);
	graph_viewer->defineVertexColor(vertex_color);
	graph_viewer->defineEdgeColor(edge_color);
}

void SistemaDeEvacuacao::ClearGraphViewerEdges()
{
	vector<Vertex<Coords>*> vertex_set = map_graph.getVertexSet();
	unsigned int num_vertex = vertex_set.size();
	for (unsigned int i = 0; i < num_vertex; i++)
	{
		vector<Edge<Coords>> &adjacent_edges = vertex_set[i]->GetAdjacentEdgesReference();
		unsigned int num_adjacent_edges = adjacent_edges.size();
		for (unsigned int j = 0; j < num_adjacent_edges; j++)
		{
			graph_viewer->setEdgeDashed(adjacent_edges[j].GetID(), false);
			graph_viewer->removeEdge(adjacent_edges[j].GetID());
		}
	}
}

void SistemaDeEvacuacao::DisplayMap()
{
	static bool first_display = true;
	if (!first_display) //at the first display there are no edges to remove
		ClearGraphViewerEdges();


	vector<Edge<Coords>> adj;
	Coords coords;
	for (unordered_map<int, Vertex<Coords> *>::iterator it = vertex_of_node_id.begin(); it != vertex_of_node_id.end(); it++)
	{
		Vertex<Coords> *vertex1 = it->second;
		vertex1->GetAdjacentEdges(adj);
		coords = vertex1->getInfo();
		int x = (int)round(coords.GetX() - map_min_x);
		int y = (int)round(coords.GetY() - map_min_y);

		if (first_display) //Create and draw the nodes only once
		{
			unsigned int node_id = coords.GetId();
			graph_viewer->addNode(node_id, x, y);
			graph_viewer->setVertexLabel(node_id, to_string(node_id));
		}
		for (unsigned int i = 0; i < adj.size(); i++)
		{
			Vertex<Coords> vertex2 = adj[i].GetDest();
			graph_viewer->addEdge(adj[i].GetID(), vertex1->getInfo().GetId(), vertex2.getInfo().GetId(), EdgeType::DIRECTED);
			graph_viewer->setEdgeLabel(adj[i].GetID(), road_name[adj[i].GetRoadId()]);
		}
	}
	graph_viewer->rearrange();
	first_display = false;
}

bool SistemaDeEvacuacao::CarPathIsAffected(Car & car)
{
	vector<Coords> path = car.GetOriginalPath();
	for (unsigned int i = 0; i < path.size() - 1; i++)
	{
		unsigned int issue_coords_groups_size = issue_coords_groups.size();
		for (unsigned int j = 0; j < issue_coords_groups_size; j++)
		{
			unsigned int issue_coords_size = issue_coords_groups[j].size();
			for (unsigned int k = 0; k < issue_coords_size - 1; k++)
			{
				if (path[i] == issue_coords_groups[j][k] && path[i + 1] == issue_coords_groups[j][k + 1])
					return true;
			}
		}
	}

	//if the path is not affected, that means that the car will follow the original path
	//Set the fill of the edges accordingly
	for (unsigned int i = 0; i < path.size() - 1; i++)
	{
		vector<Edge<Coords>> &adjacent_edges = vertex_of_node_id[path[i].GetId()]->GetAdjacentEdgesReference();
		unsigned int ajacent_edges_size = adjacent_edges.size();
		for (unsigned int j = 0; j < ajacent_edges_size; j++)
		{
			if (adjacent_edges[j].GetDestPointer() == vertex_of_node_id[path[i + 1].GetId()])
			{
				adjacent_edges[j].IncrementFill();
				if (adjacent_edges[j].IsFull())
					adjacent_edges[j].IncrementWeight(WEIGTH_WHEN_CONGESTED);
			}
		}
	}

	return false;
}

void SistemaDeEvacuacao::ShowCarRoute(unsigned int car_number)
{
	Car car = cars[car_number];

	//info on the console about the car being shown
	cout << "Esta a ser mostrada a rota do carro com matricula: " << car.GetLicensePlate() << ". Este e o carro numero " << car_number + 1 << endl;

	string color;
	if (car.IsPathAffected())
		color = ORANGE;
	else
		color = GREEN;

	// Draw original path
	vector<Coords> original_path = car.GetOriginalPath();
	unsigned int path_size = original_path.size();
	for (unsigned int i = 0; i < path_size - 1; i++)
	{
		Vertex<Coords> *vertex = vertex_of_node_id[original_path[i].GetId()];
		vector<Edge<Coords>> adjacent_edges;
		vertex->GetAdjacentEdges(adjacent_edges);
		unsigned int adjacent_edges_size = adjacent_edges.size();
		for (unsigned int j = 0; j < adjacent_edges_size; j++)
		{
			if (adjacent_edges[j].GetDest() == original_path[i + 1])
			{
				unsigned int edge_id = adjacent_edges[j].GetID();
				graph_viewer->setEdgeColor(edge_id, color);
				graph_viewer->setEdgeThickness(edge_id, 3);
				graph_viewer->setEdgeDashed(edge_id, true);
				break;
			}
		}
	}

	//Draw accident edges
	unsigned int issue_coords_groups_size = issue_coords_groups.size();
	for (unsigned int i = 0; i < issue_coords_groups_size; i++)
	{
		unsigned int issue_coords_size = issue_coords_groups[i].size();
		for (unsigned int j = 0; j < issue_coords_size - 1; j++)
		{
			Vertex<Coords> *vertex = vertex_of_node_id[issue_coords_groups[i][j].GetId()];
			vector<Edge<Coords>> adjacent_edges;
			vertex->GetAdjacentEdges(adjacent_edges);
			unsigned int adjacent_edges_size = adjacent_edges.size();
			for (unsigned int k = 0; k < adjacent_edges_size; k++)
			{
				if (adjacent_edges[k].GetDest() == issue_coords_groups[i][j + 1])
				{
					graph_viewer->setEdgeColor(adjacent_edges[k].GetID(), RED);
					graph_viewer->setEdgeThickness(adjacent_edges[k].GetID(), 3);
					break;
				}
			}
		}
	}

	// Draw alternative path
	if (!car.IsPathAffected())
	{
		graph_viewer->rearrange();
		return;
	}
	vector<Coords> alternative_path = car.GetAlternativePath();
	path_size = alternative_path.size();
	for (unsigned int i = 0; i < path_size - 1; i++)
	{
		Vertex<Coords> *vertex = vertex_of_node_id[alternative_path[i].GetId()];
		vector<Edge<Coords>> adjacent_edges;
		vertex->GetAdjacentEdges(adjacent_edges);
		unsigned int adjacent_edges_size = adjacent_edges.size();
		for (unsigned int j = 0; j < adjacent_edges_size; j++)
		{
			if (adjacent_edges[j].GetDest() == alternative_path[i + 1])
			{
				graph_viewer->setEdgeColor(adjacent_edges[j].GetID(), GREEN);
				graph_viewer->setEdgeThickness(adjacent_edges[j].GetID(), 3);
				break;
			}
		}
	}

	graph_viewer->rearrange();
}

unsigned int SistemaDeEvacuacao::GetCarsSize() const
{
	return cars.size();
}

Car SistemaDeEvacuacao::GetCarFromSrcAndDstStrings(const LicensePlate &plate, string src_road_name, string dst_road_name)
{
	ClearScreen();
	unsigned int src_node_id, dst_node_id;
	cout << "Escolha as ruas de origem e destino para o carro com matricula " << plate << endl << endl;
	cout << "Origem: " << endl << endl;
	src_node_id = GetDesiredRoadIdFromUser(src_road_name);
	cout << endl;
	cout << "Destino: " << endl << endl;
	dst_node_id = GetDesiredRoadIdFromUser(dst_road_name);

	return Car(plate, src_node_id, dst_node_id);
}

unsigned int SistemaDeEvacuacao::GetDesiredRoadIdFromUser(string road_name)
{
	vector<string> road_names;
	vector<unsigned int> road_ids;
	vector<unsigned int> node_ids;

	unordered_map<int, string>::iterator itb = this->road_name.begin();
	unordered_map<int, string>::iterator ite = this->road_name.end();
	for (; itb != ite; itb++)
	{
		if (StringMatcher(road_name, itb->second))
		{
			road_ids.push_back(itb->first);
			road_names.push_back(itb->second);
		}
	}

	unsigned int node_id;
	if (road_names.size() != 0)
	{
		cout << "Foram encontradas as seguintes ocorrencias totais de ruas com o nome introduzido:";
		DisplayRoads(road_ids, road_names);
		cout << "Introduza o id que corresponde a rua desejada. Introduza 0 se nenhum da lista corresponde e, portanto, deseja fazer uma pesquisa parcial por nome de rua: " << endl;
		unsigned int road_id = ReadDesiredSrcRoadIdFromUser(road_ids);
		if (road_id == 0)
			goto PARTIAL_SEARCH;

		cout << "Foram encontradas os seguintes nos que formam a rua selecionada:";
		GetNodesOfARoad(road_id, node_ids);
		DisplayNodesOfARoad(node_ids);
		cout << "Introduza o id que corresponde ao no desejado (ponto da rua onde se encontra o carro)." << endl;
		node_id = ReadDesiredNodeIdFromUser(node_ids);
		return node_id;
	}
	else
		cout << "Nao foram encontradas ocorrencias para pesquisa total com esse nome de rua. Serao apresentadas as ocorrencias parciais." << endl << endl;

PARTIAL_SEARCH:
	//Clear vectors before partial search
	road_ids.clear();
	road_names.clear();
	node_ids.clear();

	unordered_set<string> words_not_to_include = { "do", "de", "da", "dos", "das" };
	vector<string> pattern_words;
	GetWordsFromAString(road_name, pattern_words, words_not_to_include);
	for (itb = this->road_name.begin(); itb != ite; itb++)
	{
		bool found = false;
		vector<string> text_words;
		GetWordsFromAString(itb->second, text_words, words_not_to_include);

		for (size_t i = 0; i < pattern_words.size(); i++)
		{
			for (size_t j = 0; j < text_words.size(); j++)
			{
				if (PartialStringMatcher(pattern_words[i], text_words[j]) <= MAX_DEVIATION)
				{
					found = true;
					goto FLAG;
				}
			}
		}

	FLAG:
		if (found)
		{
			road_ids.push_back(itb->first);
			road_names.push_back(itb->second);
		}
	}

	if (road_ids.size() != 0)
	{
		cout << "Foram encontradas as seguintes ocorrencias parciais de ruas com o nome introduzido:";
		DisplayRoads(road_ids, road_names);
		cout << "Introduza o id que corresponde a rua desejada: " << endl;
		unsigned int road_id = ReadDesiredDstRoadIdFromUser(road_ids);

		cout << "Foram encontradas os seguintes nos que formam a rua selecionada:";
		GetNodesOfARoad(road_id, node_ids);
		DisplayNodesOfARoad(node_ids);
		cout << "Introduza o id que corresponde ao no desejado (ponto da rua onde se encontra o carro)." << endl;
		node_id = ReadDesiredNodeIdFromUser(node_ids);
	}
	else
	{
		cout << "A string introduzida nao corresponde a nenhum nome de rua parcial, portanto este carro sera ignorado.";
		system("pause");
		return 0;
	}

	return node_id;
}

unsigned int SistemaDeEvacuacao::ReadDesiredSrcRoadIdFromUser(const vector<unsigned int>& ids)
{
	string error_msg = "Ocorreu um erro ao ler o id da rua. Por favor tente novamente introduzindo um id de rua valido: ";
	unsigned int road_id = ReadUnsignedInt(error_msg);
	while (find(ids.begin(), ids.end(), road_id) == ids.end() && road_id != 0) //because 0 indicates a new search (partial research)
	{
		cout << "O id introduzido nao existe na lista mostrada acima. Por favor introduza um id valido.\n";
		road_id = ReadUnsignedInt(error_msg);
	}

	return road_id;
}

unsigned int SistemaDeEvacuacao::ReadDesiredDstRoadIdFromUser(const vector<unsigned int>& ids)
{
	string error_msg = "Ocorreu um erro ao ler o id da rua. Por favor tente novamente introduzindo um id de rua valido: ";
	unsigned int road_id = ReadUnsignedInt(error_msg);
	while (find(ids.begin(), ids.end(), road_id) == ids.end())
	{
		cout << "O id introduzido nao existe na lista mostrada acima. Por favor introduza um id valido.\n";
		road_id = ReadUnsignedInt(error_msg);
	}

	return road_id;
}

unsigned int SistemaDeEvacuacao::ReadDesiredNodeIdFromUser(const vector<unsigned int>& ids)
{
	string error_msg = "Ocorreu um erro ao ler o id do no. Por favor tente novamente introduzindo um id de no valido: ";
	unsigned int node_id = ReadUnsignedInt(error_msg);
	while (find(ids.begin(), ids.end(), node_id) == ids.end())
	{
		cout << "O id introduzido nao existe na lista mostrada acima. Por favor introduza um id valido.\n";
		node_id = ReadUnsignedInt(error_msg);
	}

	return node_id;
}

void SistemaDeEvacuacao::DisplayRoads(const vector<unsigned int>& ids, const vector<string>& names)
{
	size_t ids_size = ids.size();
	size_t names_size = names.size();
	if (ids_size == 0 || names_size == 0 || ids_size != names_size)
		return;

	cout << setw(10) << "ID" << setw(45) << "Nome" << endl << endl;
	for (size_t i = 0; i < ids_size; i++)
	{
		cout << setw(10) << ids[i] << setw(45) << names[i] << endl << endl;
	}

}

void SistemaDeEvacuacao::GetNodesOfARoad(unsigned int road_id, vector<unsigned int> &node_ids)
{
	pair<unordered_multimap<int, int>::iterator, unordered_multimap<int, int>::iterator> range = node_ids_of_road.equal_range(road_id);

	for (unordered_multimap<int, int>::iterator it = range.first; it != range.second; it++)
		node_ids.push_back(it->second);
}

void SistemaDeEvacuacao::DisplayNodesOfARoad(const vector<unsigned int> &node_ids)
{
	cout << endl << setw(10) << "Node Id" << setw(45) << "Coordenadas" << endl << endl;

	for (size_t i = 0; i < node_ids.size(); i++)
	{
		Coords coords = vertex_of_node_id[node_ids[i]]->getInfo();
		cout << setw(10) << node_ids[i] << setw(45) << coords << endl << endl;
	}
}

