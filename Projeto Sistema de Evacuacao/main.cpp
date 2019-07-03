#include <cstdio>
#include <fstream>
#include <iostream>
#include <sstream>
#include <conio.h>

#include "graphviewer.h"
#include "edgetype.h"
#include "SistemaDeEvacuacao.h"
#include "utils.h"

using namespace std;

#define NODES_FN "Nodes.txt"
#define ROADS_FN "Roads.txt"
#define NODES_TO_ROADS_FN "NodesToRoads.txt"

#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_LEFT 75
#define KEY_RIGHT 77
#define ESC 27

#define FILE_LOADING_REPORTING false

//---------------------------------------Reading From Files Functions---------------------------------------
//------------------------------------------------------------------------------
//Recieves a file stream that was not openned yet and reads the file name from user
//Returns the file name that it read from the user
//Does not return anything while it is not able to successfully read the file name introduced
//If it can read the filename but can't open it, exits with an error message
string ReadFile(ifstream &f_stream)
{
	string f_name;
	string error_msg = "Ocorreu um erro ao ler o nome de ficheiro que introduziu. Por favor, tente novamente, introduzindo um nome de ficheiro valido: ";
	f_name = ReadString(error_msg);
	f_stream.open(f_name);
	if (!(f_stream.is_open())) //Check if the file was openned successfully
	{
		cerr << endl << "Ocorreu um erro ao tentar abrir o ficheiro. O programa ira terminar." << endl;
		system("pause");
		exit(1);
	}
	return f_name;
}
//------------------------------------------------------------------------------
void OpenFile(const string &f_name, ifstream &f_stream)
{
	f_stream.open(f_name);
	if (!(f_stream.is_open())) //Check if the file was openned successfully
	{
		cerr << endl << "Ocorreu um erro ao tentar abrir um dos ficheiros que contem a informacao sobre o mapa. Por favor, garanta que todos os ficheiros necessarios estao disponiveis. O programa ira terminar." << endl;
		system("pause");
		exit(1);
	}
}
//------------------------------------------------------------------------------
//Reads all the filenames from user and opens the file streams (checking if the files exist)
void ReadFiles(ifstream &cars_fs, string &cars_fn)
{
	cout << endl << "Introduza o nome do ficheiro que contem as informacoes dos carros para desviar: ";
	cars_fn = ReadFile(cars_fs);

	cout << endl;
}
//------------------------------------------------------------------------------
//Parses the contents of all the files to the vectors in the class VendeMaisMais
void ParseFileContents(SistemaDeEvacuacao &EvacSystem, ifstream &cars_fs)
{
	//Load information from map files
	const string nodes_fn = NODES_FN;
	const string rods_fn = ROADS_FN;
	const string nodes_to_roads_fn = NODES_TO_ROADS_FN;

	ifstream nodes_fs;
	ifstream rods_fs;
	ifstream nodes_to_roads_fs;

	OpenFile(nodes_fn, nodes_fs);
	OpenFile(rods_fn, rods_fs);
	OpenFile(nodes_to_roads_fn, nodes_to_roads_fs);

#if FILE_LOADING_REPORTING
	cout << "Started parsing files." << endl << endl;
	unsigned int start_time = GetTickCount64();
	unsigned int elapsed_time;
#endif

	try
	{
		EvacSystem.ParseNodesFile(nodes_fs);
		nodes_fs.close();
#if FILE_LOADING_REPORTING
		elapsed_time = GetTickCount64() - start_time;
		cout << "Nodes file parsed in " << elapsed_time << " ms" << endl;
		start_time = GetTickCount64();
#endif
		EvacSystem.ParseRoadsFile(rods_fs);
		rods_fs.close();
#if FILE_LOADING_REPORTING
		elapsed_time = GetTickCount64() - start_time;
		cout << "Roads file parsed in " << elapsed_time << " ms" << endl;
		start_time = GetTickCount64();
#endif
		EvacSystem.ParseNodesToRoadsFile(nodes_to_roads_fs);
		nodes_to_roads_fs.close();
#if FILE_LOADING_REPORTING
		elapsed_time = GetTickCount64() - start_time;
		cout << "Nodes to Roads file parsed in " << elapsed_time << " ms" << endl;
		start_time = GetTickCount64();
#endif
	}
	catch (SistemaDeEvacuacao::ErrorParsingFile &error)
	{
		cout << "Ocorreu um erro ao ler a informacao de um dos ficheiros que contem a informacao do mapa. O programa ira terminar." << endl << endl;
		system("pause");
		exit(1);
	}
	catch (SistemaDeEvacuacao::InvalidInfoParsedFromFile &error)
	{
		cout << "A informacao lida de um dos ficheiros que contem a informacao do mapa e invalida. Por favor verifique a validade dos ficheiros. O programa ira terminar." << endl << endl;
		system("pause");
		exit(1);
	}

	//Load information from cars file
	string cars_fn;
	ReadFiles(cars_fs, cars_fn);

#if FILE_LOADING_REPORTING
	start_time = GetTickCount64();
#endif

	try
	{
		EvacSystem.ParseCars(cars_fs);
		cars_fs.close();
#if FILE_LOADING_REPORTING
		elapsed_time = GetTickCount64() - start_time;
		cout << "Cars file parsed in " << elapsed_time << " ms" << endl << endl;
		start_time = GetTickCount64();
#endif
	}
	catch (SistemaDeEvacuacao::ErrorParsingFile &error)
	{
		cout << "Ocorreu um erro ao ler a informacao do ficheiro de carros na linha " << error.GetLine() << ". Por favor verifique a validade do ficheiro. O programa ira terminar" << endl << endl;
		system("pause");
		exit(1);
	}
	catch (SistemaDeEvacuacao::InvalidInfoParsedFromFile &error)
	{
		cout << "A informacao lida do ficheiro do ficheiro de carros na linha " << error.GetLine() << " e invalida. Por favor verifique a validade do ficheiro. O programa ira terminar." << endl << endl;
		system("pause");
		exit(1);
	}

	cout << "A informacao contida em todos os ficheiros foi lida com sucesso" << endl << endl;
	system("pause");
}

int main()
{
	SistemaDeEvacuacao EvacSystem;
	ifstream cars_fs;
	ParseFileContents(EvacSystem, cars_fs);
	ClearScreen();
	EvacSystem.ReadIssueVertexesIds();
	ClearScreen();
	EvacSystem.HandleCarsList();
	ClearScreen();

	//alterate between cars
	unsigned int cars_size = EvacSystem.GetCarsSize();
	if (cars_size == 0)
	{
		cout << "Nao existem rotas de carros para mostrar" << endl;
		goto END;
	}

	unsigned char cin_1, cin_2;
	int curr_car = 0;
	cout << "Por favor aguarde..." << endl;
	EvacSystem.CreateGraphViewer(1900, 1000, BLUE, DARK_GRAY);
	EvacSystem.DisplayMap();
	ClearScreen();
	EvacSystem.ShowCarRoute(curr_car);
	cout << endl << endl;
	cout << "Pressione as setas para alternar entre os carros." << endl;
	cout << "Setas esquerda e cima mostram o carro anterior <-" << endl;
	cout << "Setas direita e baixo mostram o carro seguinte ->" << endl << endl;
	cout << "Pressione ESC para sair." << endl;
	while (true)
	{
		cin_1 = _getch();
		if (cin_1 == ESC)
			break;

		cin_2 = _getch();
		if (cin_2 == KEY_DOWN || cin_2 == KEY_RIGHT)
			curr_car++;

		if (cin_2 == KEY_UP || cin_2 == KEY_LEFT)
			curr_car--;

		if (curr_car < 0)
			curr_car = cars_size - 1;
		else if (curr_car == cars_size)
			curr_car = 0;

		EvacSystem.DisplayMap();
		ClearScreen();
		EvacSystem.ShowCarRoute(curr_car);
		cout << endl << endl;
		cout << "Pressione as setas para alternar entre os carros." << endl;
		cout << "Setas esquerda e cima mostram o carro anterior <-" << endl;
		cout << "Setas direita e baixo mostram o carro seguinte ->" << endl << endl;
		cout << "Pressione ESC para sair." << endl;
	}

END:
	system("pause");
	return 0;
}