#pragma once
#include<iostream>
#include<mutex>
#include<thread>
#include<chrono>
#include<atomic>
#include<vector>
#include<set>

constexpr int NUM_TEST = 10'000'000;
constexpr int MAX_THREADS = 8;
constexpr int NUM_RANGE = 1000;