/*******************************************************************************
 * Name    : mst.cpp
 * Author  : Daniel Azami
 * Version : 1.0
 * Date    : December 12, 2020
 * Description : Implements Kruskal's algorithm to create a minimum spanning tree
 ******************************************************************************/

#include <string>
#include <sstream>
#include <vector>
#include <iostream>
#include <fstream>
#include <limits>
#include <iomanip>
#include <cmath>
#include <tuple>
#include <cstdlib>
#include <utility>
#include <math.h>
#include <limits.h>
#include <stdio.h>
#include <algorithm>

using namespace std;

bool sort_helper(const tuple<int, int, int, string>& first,
              const tuple<int, int, int, string>& second)
{
    return (get<2>(first) < get<2>(second));
}

int find(int x, vector<int> vertices) {
	int y = 0;
	while (x != 0) {
		y = x;
		x = vertices[x];
	}
	return y;
}

int main(int argc, const char *argv[]) {

	int num_vertices;
	vector< tuple<int, int, int, string> > tokens;

    if (argc != 2) {
        cerr << "Usage: " << argv[0] << " <input file>" << endl;
        return 1;
    }

    ifstream input_file(argv[1]);
    if (!input_file) {
        cerr << "Error: Cannot open file '" << argv[1] << "'." << endl;
        return 1;
    }

    input_file.exceptions(ifstream::badbit);
    string line;
    try {
        unsigned int line_number = 1;
        char split_char = ',';

        while (getline(input_file, line)) {
        	istringstream split(line);
        	istringstream iss(line);
        	if (line_number == 1) {
        		if (!(iss>>num_vertices)) {
        			cerr << "Error: Invalid number of vertices '" << line << flush << "' on line 1." << endl;
        			return 1;
        		}
        		if (num_vertices < 1 || num_vertices > 1000) {
        			cerr << "Error: Invalid number of vertices '" << num_vertices << "' on line 1." << endl;
        			return 1;
        		}
        		++line_number;
        		continue;
        	}
        	vector <string> temp;
            for (string each; getline(split, each, split_char);) {
            	temp.push_back(each);
            }

            if (temp.size() != 4) {
            	cerr << "Error: Invalid edge data '" << line << "' on line " << line_number << "." << endl;
            	return 1;
            }
            try {
                if (stoi(temp[0]) > num_vertices || stoi(temp[0]) < 1) {
                	cerr << "Error: Starting vertex '" << temp[0] << "' on line " << line_number << " is not among valid values 1-" << num_vertices << "." << endl;
                	return 1;
                }
            } catch(...) {
            	cerr << "Error: Starting vertex '" << temp[0] << "' on line " << line_number << " is not among valid values 1-" << num_vertices << "." << endl;
            	return 1;

            }

            try {
                if (stoi(temp[1]) > num_vertices || stoi(temp[1]) < 1) {
                	cerr << "Error: Ending vertex '" << temp[1] << "' on line " << line_number << " is not among valid values 1-" << num_vertices << "." << endl;
                	return 1;
                }
            } catch(...) {
            	cerr << "Error: Ending vertex '" << temp[1] << "' on line " << line_number << " is not among valid values 1-" << num_vertices << "." << endl;
            	return 1;
            }

            try {
                if (stoi(temp[2]) <= 0) {
                	cerr << "Error: Invalid edge weight '" << to_string(stoi(temp[2])) << "' on line " << line_number << "." << endl;
                	return 1;
                }
            } catch(...) {
            	cerr << "Error: Invalid edge weight '" << temp[2] << "' on line " << line_number << "." << endl;
            	return 1;
            }


            tokens.push_back(make_tuple(stoi(temp[0]), stoi(temp[1]), stoi(temp[2]), temp[3]));
        	++line_number;
        }
        //Minimum number of edges = n-1.
        if (tokens.size() < (size_t) num_vertices-1 ||  num_vertices == 1) {
        	cout << "No solution." << endl;
        	return 1;
        }

        input_file.close();

    } catch (const ifstream::failure &f) {
        cerr << "Error: An I/O error occurred reading '" << argv[1] << "'.";
        return 1;
    }

    //Sort edges by weight.
    sort(tokens.begin(), tokens.end(), sort_helper);


    //Makeset
    vector<int> vertices(num_vertices+1, 0);

    vector< tuple<string, int>> mst_edges;
    int mst_weight = 0;;

    for (int i = 0; i < (int) tokens.size(); i++) {
    	if (count(vertices.begin(), vertices.end(), 0) == 1) {
			break;
		}
    	int vertex_from = get<0>(tokens[i]);
    	int vertex_to = get<1>(tokens[i]);
    	if (find(vertex_from, vertices) != find(vertex_to, vertices)) {
    		vertices[find(vertex_from, vertices)] = find(vertex_to, vertices);
    		string edge = get<3>(tokens[i]);
    		int edge_weight = get<2>(tokens[i]);
    		mst_edges.push_back(make_tuple(edge, edge_weight));
    		mst_weight += get<2>(tokens[i]);
    	}
    }

    cout << "Total wire length (meters): " << mst_weight << endl;

    //Sort mst edges alphabetically.
    sort(mst_edges.begin(), mst_edges.end());

    for (auto e : mst_edges) {
    	string street_name = get<0>(e);
    	string edge_weight = "[" + to_string(get<1>(e)) + "]";
    	string edge = street_name + " " + edge_weight;
    	cout << edge << endl;
    }
    return 0;
}
