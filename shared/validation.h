#ifndef VALIDATION_H
#define VALIDATION_H

#include <filesystem>
using std::filesystem::path;

#include <vector>
using std::vector;

#include "tGraph.h"

bool readSolution(const path &filepath, vector<nodeCost> &nodeCosts);

#endif /* VALIDATION_H */
