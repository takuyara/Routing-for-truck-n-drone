//Pionniers du TJ, benissiez-moi par votre Esprits Saints!
#pragma once
#include "utils.h"
#include <string>
namespace gconfig{
	const int V = 10000, E = 200000, Z = 1;
	const int M = 10, N = 200, L = 12;
	const double S = 0.06, R = 0.0022, P = 0.5;
	const int maxint = 100000000;
	const double city_rate = 0.8;
	const double cbonder = 5000, bonder = 10000;
	const double rateinf = 0.85, ratesup = 1.15;
	const double highway_prob = 0.01;
	const double streetw = 0.007, streett = 0.03;
	const double highw = 0.012, hight = 0.012, highwb = 5.0;
	const int street_rate = 1, highway_rate = 500, crossprev = 1000;
	const double city_cust_rate = 0.88;
	const double max_street_len = 450, max_highway_len = 7500;
	const double max_time = 28000, suburb_interval = 12000, min_city_interval = 3000;
	const double regular_time_rate = 0.4;
	const double rst1 = 2000, rfn1 = 18000, rst2 = 18000, rfn2 = 24000;
	const std::string file_name("input.txt");
}

