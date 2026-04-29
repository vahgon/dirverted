#include "dvrt/dirvert.hpp"

#include <algorithm>
#include <atomic>
#include <filesystem>
#include <iterator>
#include <ranges>
#include <thread>
#include <vector>

#include "dvrt/fbuff.hpp"

using file_buff = dvrt::__buff::fbuff;
using dirvert   = dvrt::dirvert;

namespace fs    = std::filesystem;

dirvert::dirvert(const char* path)
  : root_{ path } {}

dirvert::dirvert(const char* path, size_t bsize)
  : root_{ path }
  , t_count_{ bsize } {}

void dirvert::determine_input() {
  if (fs::is_directory(root_))
    recursively_iterate_root();
  else
    file_buff{ root_ }.crc32_lookup_table();
}

void dirvert::recursively_iterate_root() {
  auto paths = std::ranges::to<std::vector>(
               fs::recursive_directory_iterator{ root_ });

  auto is_file = [](const fs::directory_entry& path) {
    return path.is_regular_file();
  };

  auto files{ std::ranges::partition(paths, is_file) };

  delegate_work({ std::ranges::begin(paths), std::ranges::begin(files) });
}

void dirvert::delegate_work(const std::span<fs::directory_entry>& files) {
  std::atomic_size_t idx{ 0 };
  fdata_.reserve(files.size());

  auto work = [&]() {
    for (size_t i = idx.fetch_add(1); i < files.size(); i = idx.fetch_add(1)) {
      auto bytes = file_buff{ files[i] }.crc32_lookup_table();
    }
  };

  std::vector<std::jthread> threads;
  for (size_t i = 0; i < t_count_; i++)
    threads.push_back(std::jthread(work));

  threads.clear();
}
