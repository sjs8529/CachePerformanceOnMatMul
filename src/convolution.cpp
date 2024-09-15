#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdlib>
#include <ctime>

using namespace std;

void parsec_roi_begin() 
{
}

void parsec_roi_end() 
{
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

// Function to generate a random NxN kernel without a seed
vector<vector<int>> generateRandomKernel(int kernelSize) {
    vector<vector<int>> kernel(kernelSize, vector<int>(kernelSize));
    
    // Initialize the random number generator with the current time
    srand(time(0)); 

    for (int i = 0; i < kernelSize; ++i) {
        for (int j = 0; j < kernelSize; ++j) {
            kernel[i][j] = rand() % 10 - 5; // Random values between -5 and 4
        }
    }
    return kernel;
}

// Function to perform 2D convolution
void convolve(const vector<vector<int>>& matrix, const vector<vector<int>>& kernel, vector<vector<int>>& output) {
    int numRows = matrix.size();
    int numCols = matrix[0].size();
    int kRows = kernel.size();
    int kCols = kernel[0].size();

    int outRows = numRows - kRows + 1;
    int outCols = numCols - kCols + 1;

    output.resize(outRows, vector<int>(outCols, 0));

    // Convolution with stride of 1
    for (int i = 0; i < outRows; ++i) {
        for (int j = 0; j < outCols; ++j) {
            int sum = 0;
            for (int ki = 0; ki < kRows; ++ki) {
                for (int kj = 0; kj < kCols; ++kj) {
                    sum += matrix[i + ki][j + kj] * kernel[ki][kj];
                }
            }
            output[i][j] = sum;
        }
    }
}

int main(int argc, char* argv[]) {
    if (argc < 5 || string(argv[1]) != "--input") {
        cerr << "Usage: " << argv[0] << " --input <input_file> <numRows> <numCols> <kernel_size>" << endl;
        return 1;
    }

    string filename = argv[2];
    int numRows = atoi(argv[3]);
    int numCols = atoi(argv[4]);
    int kernelSize = atoi(argv[5]); // Size of the kernel (e.g., 3 for 3x3, 4 for 4x4)

    if (kernelSize < 3 || kernelSize > min(numRows, numCols)) {
        cerr << "Invalid kernel size. It should be >= 3 and <= the matrix size." << endl;
        return 1;
    }

    // Read matrix from file
    vector<vector<int>> matrix = parseMatrixFile(filename, numRows, numCols);

    // Generate a random NxN kernel based on the provided size
    vector<vector<int>> kernel = generateRandomKernel(kernelSize);

    // Output matrix for storing the result of the convolution
    vector<vector<int>> output;

    parsec_roi_begin();
    // Perform convolution
    convolve(matrix, kernel, output);
    parsec_roi_end();


    return 0;
}
