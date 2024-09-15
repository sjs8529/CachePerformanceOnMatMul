#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdlib>

using namespace std;

void parsec_roi_begin() 
{

}

void parsec_roi_end() 
{

}

// Function to gather data from the matrix
void gatherData(const vector<int>& indices, const vector<vector<int>>& matrix,
                vector<int>& gatheredData) {
    int rows = matrix.size();
    int cols = matrix[0].size();
    gatheredData.resize(indices.size());
    
    for (size_t i = 0; i < indices.size(); ++i) {
        int index = indices[i];
        int row = index / cols;
        int col = index % cols;
        if (row < rows && col < cols) {
            gatheredData[i] = matrix[row][col];
        } else {
            cerr << "Index out of bounds at position " << i << "!" << endl;
        }
    }
}

// Function to parse input matrix file
vector<vector<int>> parseMatrixFile(const string& filename, int& numRows, int& numCols) {
    vector<vector<int>> matrix;
    ifstream infile(filename);
    if (!infile) {
        cerr << "Error opening file: " << filename << endl;
        exit(1);
    }

    string line;
    while (getline(infile, line)) {
        istringstream iss(line);
        vector<int> row;
        int value;
        while (iss >> value) {
            row.push_back(value);
        }
        if (matrix.empty()) {
            numCols = row.size();
        }
        matrix.push_back(row);
    }
    infile.close();
    numRows = matrix.size();
    return matrix;
}

int main(int argc, char* argv[]) {
    if (argc < 4 || string(argv[1]) != "--input") {
        cerr << "Usage: " << argv[0] << " --input <input_file> <numRows> <numCols>" << endl;
        return 1;
    }

    string filename = argv[2];
    int numRows = atoi(argv[3]);
    int numCols = atoi(argv[4]);

    // Read matrices from file
    vector<vector<int>> matrix = parseMatrixFile(filename, numRows, numCols);

    // Generate random indices to gather data from
    const size_t num_samples = 1000;
    vector<int> indices(num_samples);

    srand(time(0));
    for (size_t i = 0; i < num_samples; ++i) {
        indices[i] = rand() % (numRows * numCols);
    }

    // Vector to store gathered data
    vector<int> gatheredData;
    parsec_roi_begin();
    // Gather data from the matrix
    gatherData(indices, matrix, gatheredData);
    parsec_roi_end();

    // Optionally, save the gatheredData to file or perform other operations here

    // Clean up and exit
    return 0;
}
