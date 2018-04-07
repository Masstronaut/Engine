#pragma once

#include <cpprest/http_listener.h>
#include <cpprest/asyncrt_utils.h>

class Simulation;

class REST_VM
{
public:
  REST_VM(Simulation &sim, utility::string_t url);
  ~REST_VM();

  pplx::task<void> open() { return m_listener.open(); }
  pplx::task<void> close() { return m_listener.close(); }

private:
  void handle_request(web::http::http_request message);
  //void parse_world(web::http::http_request);
  void handle_error(pplx::task<void>& t);
  web::http::experimental::listener::http_listener m_listener;
  Simulation &m_simulation;
};
