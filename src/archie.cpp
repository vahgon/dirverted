#include "archie/archie.hpp"

#include <algorithm>
#include <atomic>
#include <filesystem>
#include <iterator>
#include <ranges>
#include <thread>
#include <vector>

#include "archie/filedata.hpp"

namespace fs  = std::filesystem;

using file    = archie::file;
using dirvert = archie::dirvert;

dirvert::dirvert(const char* path)
: root_{ path } {}

dirvert::dirvert(const char* path, size_t num_threads)
: root_{ path }
, thread_cnt_{ num_threads } {}

void dirvert::determine_input() {
  if (fs::is_directory(root_))
    recursively_iterate_root();
  else {
    file f{ root_ };
    f.get_byte_reps();
  }
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

  auto work = [&]() {
    for (size_t i = idx.fetch_add(1); i < files.size(); i = idx.fetch_add(1)) {
      auto bytes = file{ files[i] };
    }
  };

  std::vector<std::jthread> threads;
  for (size_t i = 0; i < thread_cnt_; i++)
    threads.push_back(std::jthread(work));

  threads.clear();
}
