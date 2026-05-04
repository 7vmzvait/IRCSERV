#include "../../include/server.hpp"
#include "../../include/client.hpp"
#include "../../include/utls.hpp"

void server::recv_data(size_t &index)
{
    int fd = _pfds[index].fd;

    if (fd == _fd_server)
        return;

    std::map<int, client>::iterator it = _clients.find(fd);
    if (it == _clients.end())
        return;

    char tmp[1024];
    int n = recv(fd, tmp, sizeof(tmp) - 1, 0);
    if (n <= 0)
    {
        this->disconnect_client(index);
        this->status = true;
        return;
    }
    
    tmp[n] = '\0';
    it->second.buffer.append(tmp, n);
    std::string line;
    while (!(line = Extract_data(it->second)).empty())
    {
        channel->handle_input(line, it->second,auth);
        if (_clients.find(fd) == _clients.end())
        {
            this->status = true;
            break;
        }
    }
}