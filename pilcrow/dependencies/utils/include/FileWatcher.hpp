#pragma once
#include <filesystem>
#include <unordered_map>
#include "EventArena.hpp"
class FolderObserver : public EventArena {
  public:
    struct FileModifiedEvent {
      std::filesystem::directory_entry file;
    };
    struct FileCreatedEvent {
      std::filesystem::directory_entry file;
    };
    // Detecting deleted items is currently not supported.
    //struct FileDeletedEvent {
    //  std::filesystem::directory_entry file;
    //};
    FolderObserver(std::string path) : m_watched_folder(path) {
      scan(false);
    }
    FolderObserver(FolderObserver &&other)
      : m_watched_folder(std::move(other.m_watched_folder))
      , m_modified_times(std::move(other.m_modified_times)) {}
    void scan(bool notify = true) {
      for(const auto &path :
          std::filesystem::recursive_directory_iterator{m_watched_folder}) {
        if(!path.is_regular_file()) continue; // Only watch actual files
        auto path_str{path.path().lexically_normal()};
        if (auto iter{ m_modified_times.find(path_str) };
           iter != m_modified_times.end()) {
          if(Modified(iter, path)) {
            iter->second = path.last_write_time();
            if(notify) Emit(FileModifiedEvent{path});
          }
        } else { // created
          m_modified_times[path_str] = path.last_write_time();
          if(notify) Emit(FileCreatedEvent{path});
        }
      }
    }
  private:
    bool Modified(const std::unordered_map<typename std::filesystem::path::string_type,
                                           std::filesystem::file_time_type>::iterator& item, 
                  const std::filesystem::directory_entry &file){
      return item->second != file.last_write_time();
    }
    std::filesystem::path m_watched_folder;
    std::unordered_map<typename std::filesystem::path::string_type, std::filesystem::file_time_type> m_modified_times;
};