#ifndef __MISC_GLOBAL__
#define __MISC_GLOBAL__

#include<iostream>
#include<vector>

// * * Types
typedef unsigned int uint;

typedef vector<int> v1d;
typedef vector<v1d> v2d;
typedef vector<v2d> v3d;
typedef vector<v3d> v4d;

// * * Constants
const std::string RED("\033[0;31m");
const std::string GREEN("\033[0;32m");
const std::string RESET("\033[0;0m");

#endif