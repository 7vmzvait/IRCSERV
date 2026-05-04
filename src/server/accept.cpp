#include "../../include/server.hpp"
#include "../../include/client.hpp"
#include "../../include/utls.hpp"

void server::accept_connection()
{
    client  __client;
    pollfd  _poll;
    struct sockaddr_in client_addr;
    socklen_t len = sizeof(client_addr);

    int client_fd = accept(_fd_server, (struct sockaddr*)&client_addr, &len); 
    // int client_fd = accept(_fd_server,NULL,NULL);
    _poll.fd = client_fd;
    _poll.events = POLLIN;
    _pfds.push_back(_poll);
    ////////////////////////////////////////////////////////////////
    std::cout << "[" << Utils::getTime() << "] Accepted Connection from: 127.0.0.1:" << _port << " (connection " << client_fd << ")" << std::endl;
    ////////////////////////////////////////////////////////////////
    __client.fd = client_fd;
    __client.port = _port;
    __client.ip = inet_ntoa(client_addr.sin_addr);
    // __client.ip = "127.0.0.1";
    __client.pass_ok = false;
    __client.user_ok = false;
    __client.nick_ok = false;
    __client.regestred = false;
    __client.status = false;
    __client.first = true;
    __client.hostname = Utils::getMachineName();
    /////////////////////////////////////////////////////////////////
    // Utils::sendWelcome(client_fd);
    _clients[client_fd] = __client;
}