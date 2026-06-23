#include <iostream>

using namespace std;

int main(int argc, char* argv[]) {
    
    if (argc != 4) {
        cerr << "Uso " << (argc > 0 ? argv[0] : "./dispatcher")
            << " processes.txt files.txt string.txt\n";
            return 1;
    }

    string processesFile = argv[1];
    string filesFile = argv[2];
    string stringFile = argv[3];

    cout << "ArcadiaOS\n"
        << " processes: " << processesFile << "\n"
        << " files: " << filesFile << "\n"
        << " strings: " << stringFile << "\n";

    return 0;
}