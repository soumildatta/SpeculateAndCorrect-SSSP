#ifndef TGRAPH_H
#define TGRAPH_H

#include <sstream>
using std::istringstream;

#include <string>
using std::string;
using std::getline;
using std::literals::string_literals::operator""s;

#include <fstream>
using std::ifstream;

#include <unordered_map>
using std::unordered_map;

struct tGraph
{

    // TODO: Figure out what this does
    inline uint32_t getNodeID(unordered_map<string, uint32_t> &nodeIdToIndex, const string &nodeID)
    {
        // If the nodeID does not exist in the map, insert
        if(nodeIdToIndex.count(nodeID) == 0u) return nodeIdToIndex[nodeID] = nodeIdToIndex.size();
        else return nodeIdToIndex[nodeID];
    }

    inline void parseDimacs(const string &filename)
    {
        ifstream file(filename);
        if(file.peek() == ifstream::traits_type::eof())
        {
            // TODO: throw error here
            throw('File empty');
        }

        unordered_map<string, uint32_t> nodeIdToIndex;
        uint32_t lineNum { 0u };
        string oldLine;

        for(string line; getline(file, line);)
        {
            ++lineNum;

            string command;
            string src;
            string dest;
            string weight;

            istringstream iss(line);
            iss >> command;
            // Only lines starting with the 'a' command need to be processed
            if(command != "a"s) continue;

            // We are on the correct line now, process graph
            iss >> src;
            iss >> dest;
            iss >> weight;

            // TODO: Add parsed items to edge list struct
        }

        file.close();
    }
};

#endif