#include "archie/archive.hpp"

#include <algorithm>
#include <atomic>
#include <filesystem>
#include <iterator>
#include <ranges>
#include <span>
#include <thread>
#include <vector>

#include "core/buffer.hpp"

void archie::archive::archive_root() {
  try {
    if (std::filesystem::is_directory(m_root)) {
      iterate_root();
    } else {
      // internal::read_file(m_root);
    }
  } catch(...) {
    throw;
  }
}

void archie::archive::iterate_root() {
  auto is_file = [](auto &path) { return path.is_regular_file(); };

  auto paths{
    std::ranges::to<std::vector>(
      std::filesystem::recursive_directory_iterator{ m_root })
  };

  auto files{ std::ranges::partition(paths, is_file) };

  delegate_work({ std::ranges::begin(paths), std::ranges::begin(files) });
}

void archie::archive::delegate_work(const std::span<std::filesystem::directory_entry>& files) const {
  std::atomic_size_t idx{ 0 };

  auto work = [&]() {
    for (auto i = idx.fetch_add(1); i < files.size(); i = idx.fetch_add(1)) {
     // auto bytes = file{ files[i] };
    } };

  std::vector<std::jthread> threads;
  for (size_t i{ 0 }; i < m_usable_threads; i++) {
    threads.push_back(std::jthread(work));
  }

  threads.clear();
}
