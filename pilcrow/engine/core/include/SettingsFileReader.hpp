#pragma once
#include <Utils/include/Resource.hpp>
#include "Utils/include/ResourceSettings.h"
#include "Settings/WindowSettings.h"
#include "Settings/GameSettings.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

class SettingsFile : public Resource {
public:
  SettingsFile(const std::string &name);

  ~SettingsFile();

  unsigned ID() const;

  virtual bool Reloadable() const override;

  virtual std::string Directory() const override;

	void ApplyParsedSettings(const std::vector<std::string>& strings,
    const std::vector<float>& f);

  bool LoadSettingsINI(void);

protected:
  virtual bool LoadImpl();
  virtual void UnloadImpl();
private:
	unsigned m_ID{ 0 };
};

