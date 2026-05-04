#include "../../include/server.hpp"
#include "../../include/client.hpp"
#include "../../include/utls.hpp"

std::string server::Extract_data(client &c)
{
    std::string &buf = c.buffer;

    size_t pos = buf.find('\n');
    if (pos == std::string::npos)
        return "";

    std::string line = buf.substr(0, pos);
    buf.erase(0, pos + 1);
    // Remove trailing '\r' if present
    if (!line.empty() && line[line.size() - 1] == '\r')
        line.erase(line.size() - 1, 1);

    return line;
}