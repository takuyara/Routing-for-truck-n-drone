//Pair
#include <utility>
std::pair<int, int> p = std::make_pair(10, 20);
p.first, p.second
//Tuple
#include <tuple>
std::tuple<int, int, int, double> p = std:make_tuple(10, 20, 40, 0.5);
std::get<0>(p) == 10
std::get<1>(p) == 20
std::get<3>(p) == 0.5
//Vector
std::vector<int> p; // int p[variable_length];
p.push_back(100); // list append
p[0] // as in array, p[0]
p.size() // get length
//Map
std::map<int, int> p; // int -> int
p[key] = value; // Insert
p[key] // Check
p.find(key) == p.end() //Not Found

