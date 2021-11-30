#pragma once
#include<iostream>
#include<mutex>
#include<thread>
#include<chrono>
#include<atomic>
#include<vector>
#include<set>
#include<string>
#include<omp.h>
#include<tbb/parallel_for.h>
#include<unordered_map>
#include<tbb/concurrent_unordered_map.h>

constexpr int NUM_TEST = 50'000'000;
constexpr int MAX_THREADS = 4;
constexpr int NUM_RANGE = 1000;