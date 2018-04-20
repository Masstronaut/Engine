#include <string>
#include <vector>
#include <algorithm>
#include <sstream>
#include <iostream>
#include <fstream>
#include <random>
#include <locale> // convert to std::string
#include <codecvt>// conver to std::string
#include <sstream> // string parsing

#include <cpprest/uri.h>
#include <cpprest/json.h>

#include "../include/RESTAPI.h"
#include <Simulation.hpp>




REST_VM::REST_VM(Simulation &sim, utility::string_t url) 
  : m_listener(url)
  , m_simulation(sim) {
  m_listener.support(std::bind(&REST_VM::handle_request, this, std::placeholders::_1));
  mRoutesMap[U("Components")] = std::bind(&REST_VM::GetComponentsList, this);
}

REST_VM::~REST_VM()
{
  //dtor
}


void REST_VM::handle_error(pplx::task<void>& t)
{
  try
  {
    t.get();
  }
  catch (...)
  {
    // Ignore the error, Log it if a logger is available
    std::cout << "An error occurred.\n";
  }
}

web::json::value REST_VM::GetComponentsList() {
  web::json::value componentsList = web::json::value::object(true);
  web::json::value components = web::json::value::array(1);
  web::json::value transformObject = web::json::value::object(true);

  transformObject[U("typeName")] = web::json::value::string(U("Transform"));
    // position, rotation, scale
  web::json::value transformMembers = web::json::value::array(3);

  // Position member
  web::json::value position = web::json::value::object(true);
  position[U("name")] = web::json::value::string(U("Position"));
  position[U("typeName")] = web::json::value::string(U("vec3"));

  web::json::value positionMembers = web::json::value::array(3);

  web::json::value positionMemberX = web::json::value::object(true);
  positionMemberX[U("typeName")] = web::json::value::string(U("float"));
  positionMemberX[U("name")] = web::json::value::string(U("X"));

  web::json::value positionMemberY = web::json::value::object(true);
  positionMemberY[U("typeName")] = web::json::value::string(U("float"));
  positionMemberY[U("name")] = web::json::value::string(U("Y"));

  web::json::value positionMemberZ = web::json::value::object(true);
  positionMemberZ[U("typeName")] = web::json::value::string(U("float"));
  positionMemberZ[U("name")] = web::json::value::string(U("Z"));

  positionMembers[0] = positionMemberX;
  positionMembers[1] = positionMemberY;
  positionMembers[2] = positionMemberZ;
  position[U("members")] = positionMembers;

  // Rotation member
  web::json::value rotation = web::json::value::object(true);
  rotation[U("name")] = web::json::value::string(U("Rotation"));
  rotation[U("typeName")] = web::json::value::string(U("vec3"));

  web::json::value rotationMembers = web::json::value::array(3);

  web::json::value rotationMemberX = web::json::value::object(true);
  rotationMemberX[U("typeName")] = web::json::value::string(U("float"));
  rotationMemberX[U("name")] = web::json::value::string(U("X"));

  web::json::value rotationMemberY = web::json::value::object(true);
  rotationMemberY[U("typeName")] = web::json::value::string(U("float"));
  rotationMemberY[U("name")] = web::json::value::string(U("Y"));

  web::json::value rotationMemberZ = web::json::value::object(true);
  rotationMemberZ[U("typeName")] = web::json::value::string(U("float"));
  rotationMemberZ[U("name")] = web::json::value::string(U("Z"));

  rotationMembers[0] = rotationMemberX;
  rotationMembers[1] = rotationMemberY;
  rotationMembers[2] = rotationMemberZ;
  rotation[U("members")] = rotationMembers;

  // Scale memver
  web::json::value scale = web::json::value::object(true);
  scale[U("name")] = web::json::value::string(U("Scale"));
  scale[U("typeName")] = web::json::value::string(U("vec3"));

  web::json::value scaleMembers = web::json::value::array(3);

  web::json::value scaleMemberX = web::json::value::object(true);
  scaleMemberX[U("typeName")] = web::json::value::string(U("float"));
  scaleMemberX[U("name")] = web::json::value::string(U("X"));

  web::json::value scaleMemberY = web::json::value::object(true);
  scaleMemberY[U("typeName")] = web::json::value::string(U("float"));
  scaleMemberY[U("name")] = web::json::value::string(U("Y"));

  web::json::value scaleMemberZ = web::json::value::object(true);
  scaleMemberZ[U("typeName")] = web::json::value::string(U("float"));
  scaleMemberZ[U("name")] = web::json::value::string(U("Z"));

  scaleMembers[0] = scaleMemberX;
  scaleMembers[1] = scaleMemberY;
  scaleMembers[2] = scaleMemberZ;
  scale[U("members")] = scaleMembers;


  transformMembers[0] = position;
  transformMembers[1] = rotation;
  transformMembers[2] = scale;

  transformObject[U("members")] = transformMembers;
  components[0] = transformObject;
  componentsList[U("Components")] = components;
  return componentsList;
}


void REST_VM::handle_request(web::http::http_request message)
{
  using web::http::uri;
  ucout << message.to_string() << std::endl;

  auto paths = uri::split_path(uri::decode(message.relative_uri().path()));
  auto path = message.relative_uri().path();
  if (this->mRoutesMap.find(message.relative_uri().path()) != this->mRoutesMap.end()) {
    web::json::value responseMessage = this->mRoutesMap[path]();
    web::http::http_response response(web::http::status_codes::OK);
    response.headers().add(U("Access-Control-Allow-Origin"), U("*"));
    response.set_body(responseMessage);
    message.reply(response);
  }
  else {
    message.reply(web::http::status_codes::InternalError, "That operation is not yet supported by the REST API.");
  }
  return;
}

