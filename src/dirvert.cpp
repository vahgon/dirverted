#include "dvrt/dirvert.hpp"

#include <algorithm>
#include <atomic>
#include <filesystem>
#include <ranges>
#include <thread>
#include <utility>
#include <vector>

#include "dvrt/fbuff.hpp"

using directory_entry = std::filesystem::directory_entry;
using file_buff       = dvrt::__buff::fbuff;

dvrt::dirvert::dirvert(const char* path)
  : root_{ std::move(path) } {}

dvrt::dirvert::dirvert(const char* path, size_t bsize)
  : root_{ std::move(path) }
  , thread_cnt_{ std::move(bsize) } {}

void dvrt::dirvert::recursively_iterate_root() {
  auto paths_in_root = std::ranges::to<std::vector>(
                       std::filesystem::recursive_directory_iterator{ root_ });

  auto is_subdir = [](const std::filesystem::directory_entry& path) {
    return path.is_directory();
  };

  auto fnd_files{ std::ranges::partition(paths_in_root, is_subdir) };

  delegate_work(paths_in_root);
}

void dvrt::dirvert::delegate_work(
    std::vector<std::filesystem::directory_entry>& files) {
  std::atomic_size_t idx{ 0 };
  fdata_.reserve(files.size());
  auto work = [&]() {
    for (size_t i = idx.fetch_add(1); i < files.size(); i = idx.fetch_add(1)) {
      if (files[i].is_regular_file()) {
        auto bytes = file_buff{ files[i] }.get_bytes();
      }
    }
  };

  std::vector<std::jthread> threads;
  threads.reserve(thread_cnt_);

  for (size_t i = 0; i < thread_cnt_; i++)
    threads.emplace_back(work);
}
