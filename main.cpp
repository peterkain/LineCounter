#include <iostream>
#include <fstream>
#include <algorithm>
#include <iterator>
#include <chrono>
#include <vector>
#include <experimental/filesystem>

namespace fs = std::experimental::filesystem::v1;

using namespace     std;
using namespace     std::chrono;
using hr_clock =    std::chrono::high_resolution_clock;

void print_help()
{
    cout << "\
LineCounter v1.0\nCopyright 2018-: Peter Kain\n\
USAGE: <Filename> | ALL:<Directory> | FILE:<FilenameFile>\n\
  <Filename>: Path to file\n\
  <Directory>: Path to directory, will take all of the files in the directory\n\
  <FilenameFile>: Path to file, which contains relative paths to files\n";
}

void get_files(vector<string>& file_names)
{
    string file;
	cout << "Enter :h for help\n";
    cout << "Enter files, one by one, :q to quit:\n";
    while(true)
    {
        getline(cin, file);
        if(file == ":h")
            print_help();
        else if(file == ":q")
            return;
        else if(file.substr(0, 5) == "FILE:")
        {
            ifstream inputfile(file.substr(5));
            copy(istream_iterator<string>(inputfile),
                 istream_iterator<string>(),
                 back_inserter(file_names));
        } 
        else if(file.substr(0, 4) == "ALL:")
        {
            string prefix = file.substr(4);
            for(auto& p : fs::directory_iterator(prefix))
                file_names.push_back(p.path().string());
        } 
        else if(!file.empty())
        {
            file_names.push_back(file);
        }
    }
}

int main()
{
    string input {};
    while(input != "q")
    {
        vector<string> file_names {};
        int result = 0;

        get_files(file_names);

        if(!file_names.empty())
        {
            cout << "Counting...\n";
            auto start = hr_clock::now();

            for(unsigned int i = 0; i < file_names.size(); i++)
            {
                string filename = file_names[i];
                ifstream file(filename, ios::in | ios::binary);
                if(file.fail())
                {
                    cout << "Directory or not found: " << filename << "\n";
                    continue;
                }

                cout << "Status: " << filename << '\n';

                result += count(istreambuf_iterator<char>(file),
                                istreambuf_iterator<char>(), '\n');
            }

            auto end = hr_clock::now();
            auto time = duration_cast<milliseconds>(end - start).count();

            cout << "Total number of lines: " << result << endl;
            cout << "Total time needed: " << time << " milliseconds\n" << endl;
        }
        else
        {
            cout << "No files specified\n";
        }

        cout << "Input enter to continue, 'q' to quit: ";
        getline(cin, input);
        cout << endl;
    }
    return 0;
}