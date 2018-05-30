#include "..\include\SettingsFileReader.hpp"

SettingsFile::SettingsFile(const std::string & name) 
  : Resource(name) { 
  this->Load(IOType::text); 
}

SettingsFile::~SettingsFile() { 
  this->Unload(); 
}

inline unsigned SettingsFile::ID() const { return m_ID; }

inline bool SettingsFile::Reloadable() const { return true; }

inline std::string SettingsFile::Directory() const { return g_ResourcePath; }

inline void SettingsFile::ApplyParsedSettings(const std::vector<std::string>& strings, const std::vector<float>& f)
{
  if (strings.size() == 0) return;
  std::string c = strings[0];

  if (c == "screen_size") {
    // syntax: screen_size <width> <height>
    g_InitialWindowWidth = f[1];
    g_InitialWindowHeight = f[2];
  }

  else if (c == "start_fullscreen") {
    // syntax: start_fullscreen <0 = no, 1 = yes>
    g_StartFullscreen = bool(f[1]);
  }
  else if (c == "spawn_nanos") {
    g_SpawnNanos = bool(f[1]);
  }


  //TODO:
  else if (c == "camera") {
    // scene file syntax: camera x y z   ry   <orientation spec>

    // Parameters: (Rx, Ry, eye, orient) where
    // frustum height ratio Ry = f[4]
    // frustum width ratio  Rx = Ry * W / H
    // Eye position = ([1],f[2],f[3])
    // View orientation =  Orientation(5,strings,f) 

    //scene_camera = new RT_Camera(
    //	(f[4] * width / height),
    //	f[4],
    //	Vector3f(f[1], f[2], f[3]),
    //	Orientation(5, strings, f));
  }


  else {
    fprintf(stderr, "\n*********************************************\n");
    fprintf(stderr, "* Unknown command in ini parser: %s\n", c.c_str());
    fprintf(stderr, "*********************************************\n\n");
  }
}

inline bool SettingsFile::LoadSettingsINI(void)
{
  std::stringstream input(Data());

  if (input.fail())
  {
    std::cout << "ERROR in LoadSettingsINI: Bad input data. " << std::endl;
    return false;
  }

  // For each line in file
  for (std::string line; getline(input, line); )
  {
    // Parse as parallel lists of strings and floats
    std::vector<std::string> strings;
    std::vector<float> floats;

    // Parses space-separated strings until EOL
    std::stringstream lineStream(line);
    for (std::string s; lineStream >> s; )
    {
      float f;
      double d;

      if (!(std::stringstream(s) >> d)) d = nan(""); //nans for bad input
      f = (float)d;

      floats.push_back(f);
      strings.push_back(s);
    }

    if (strings.size() == 0) continue; // Skip blank lines
    if (strings[0][0] == '#') continue; // Skip comment lines

                                        // Pass the line's data to the apply function
    ApplyParsedSettings(strings, floats);
  }

  return true;
}

inline bool SettingsFile::LoadImpl() { return LoadSettingsINI(); }

inline void SettingsFile::UnloadImpl() { return; }



