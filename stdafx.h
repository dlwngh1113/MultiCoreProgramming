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
#include<unordered_map>
#include<tbb/parallel_for.h>
#include<tbb/concurrent_unordered_map.h>
#include<tbb/concurrent_hash_map.h>
#include<shared_mutex>
#include<tbb/task.h>

constexpr int NUM_TEST = 5'000'000;
constexpr int MAX_THREADS = 8;
constexpr int NUM_RANGE = 1000;