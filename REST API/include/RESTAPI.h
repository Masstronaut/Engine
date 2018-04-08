#pragma once

#include <cpprest/http_listener.h>
#include <cpprest/asyncrt_utils.h>

class Simulation;
class Entity;
class World;

class REST_VM
{
public:
  REST_VM(Simulation &sim, utility::string_t url);
  REST_VM(const REST_VM&) = delete;
  REST_VM(REST_VM&&) = delete;
  REST_VM& operator=(const REST_VM&) = delete;
  REST_VM& operator=(REST_VM&&) = delete;

  ~REST_VM();

  pplx::task<void> Open() { return m_listener.open(); }
  pplx::task<void> Close() { return m_listener.close(); }

  class RequestHandler {
  public:
    RequestHandler(Simulation &sim, web::http::http_request request);
    void Handle();
  private:
    Entity* GetEntity(const utility::string_t &name);
    World* GetWorld(const utility::string_t &name);
    
    std::vector<utility::string_t> m_RequestPath;
    web::http::http_request m_Request;
    Simulation& m_Simulation;
  };

private:
  void handle_request(web::http::http_request message);
  //void parse_world(web::http::http_request);
  void handle_error(pplx::task<void>& t);
  web::http::experimental::listener::http_listener m_listener;
  Simulation &m_simulation;
};


