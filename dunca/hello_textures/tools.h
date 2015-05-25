#ifndef SD_FW_TOOLS
#define SD_FW_TOOLS

#include <string>

class Tools
{
public:
    static bool readFile(std::string& code, const char* path);
};

#endif // #ifdef SD_FW_TOOLS