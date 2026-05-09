#include "archie/archie.hpp"

#include <algorithm>
#include <atomic>
#include <filesystem>
#include <iterator>
#include <ranges>
#include <thread>
#include <vector>

#include "archie/file/file.hpp"

using file    = archie::file;
using archive = archie::archive;

void archive::determine_root_type() const {
  if (std::filesystem::is_directory(m_root)) {
    recursively_iterate_root();
  } else {
    file f{ m_root };
  }
}

void archive::recursively_iterate_root() const {
  auto paths{
    std::ranges::to<std::vector>(
      std::filesystem::recursive_directory_iterator{ m_root })
  };

  auto is_file = [](const std::filesystem::directory_entry& path) {
    return path.is_regular_file();
  };

  auto files{ std::ranges::partition(paths, is_file) };

  delegate_work({ std::ranges::begin(paths), std::ranges::begin(files) });
}

void archive::delegate_work(const std::span<std::filesystem::directory_entry>& files) const {
  std::atomic_size_t idx{ 0 };
 
  auto work = [&]() {
    for (size_t i = idx.fetch_add(1); i < files.size(); i = idx.fetch_add(1)) {
      auto bytes = file{ files[i] };
    }
  };

  std::vector<std::jthread> threads;
  for (size_t i{ 0 }; i < m_usable_threads; i++) {
    threads.push_back(std::jthread(work));
  }

  threads.clear();
}
