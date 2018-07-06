#include <algorithm>
#include <codecvt>  // conver to std::string
#include <fstream>
#include <iostream>
#include <locale>  // convert to std::string
#include <random>
#include <sstream>
#include <sstream>  // string parsing
#include <string>
#include <vector>

#include <cpprest/json.h>
#include <cpprest/uri.h>

#include "../include/RESTAPI.h"
#include <Simulation.hpp>

REST_VM::REST_VM(Simulation &sim, const utility::string_t &url)
  : m_listener(url), m_simulation(sim) {
  m_listener.support(
    std::bind(&REST_VM::handle_request, this, std::placeholders::_1));
}
REST_VM::~REST_VM() {
  // dtor
}

void REST_VM::handle_error(pplx::task<void> &t) {
  try {
    t.get();
  } catch(...) {
    // Ignore the error, Log it if a logger is available
    std::cout << "An error occurred.\n";
  }
}

void REST_VM::handle_request(const web::http::http_request &message) {
  using web::http::uri;
  ucout << message.to_string() << std::endl;

  auto paths = uri::split_path(uri::decode(message.relative_uri().path()));

  if(paths.empty()) {
    message.reply(web::http::status_codes::BadRequest, "Empty Request.");
  } else if(paths[0] == U("Worlds")) {
    std::string       world_name;
    utility::string_t str;
    if(paths.size() > 1) {
      // @@TODO: this conversion isn't safe and should be updated in the future.
      world_name.assign(paths[1].begin(), paths[1].end());
      World &world{m_simulation.GetWorld(world_name)};
      paths.erase(paths.begin(), paths.begin() + 2);
      if(paths.empty()) {
        // @@TODO: return contents of the world.
        message.reply(web::http::status_codes::OK,
                      "The world \"" + world_name
                        + "\" was found successfully.");
      } else if(paths[0] == U("Entities")) {
        if(paths.size() == 1) {
          //@@TODO: return list of all entities

          message.reply(web::http::status_codes::OK,
                        "Retrieving all entities of of the world \""
                          + world_name + "\".");
        } else if(paths.size() > 1) {
          utility::istringstream_t iss(paths[1]);
          EntityID                 ID;
          iss >> ID.first;
          iss.get();  // pull out the comma
          iss >> ID.second;
          Entity *entity{world.GetEntity(ID)};
          if(entity != nullptr) {
            message.reply(web::http::status_codes::OK,
                          U("The Entity with ID {") + paths[1]
                            + U("} is named \"")
                            + utility::string_t(entity->Name().begin(),
                                                entity->Name().end())
                            + U("\""));
          } else {
            message.reply(web::http::status_codes::NotFound,
                          "Entity requested does not exist.");
          }
        }
      }
    }
  } else {
    message.reply(web::http::status_codes::InternalError,
                  "That operation is not yet supported by the REST API.");
  }
}
