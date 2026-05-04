#include "../../include/server.hpp"
#include "../../include/client.hpp"
#include "../../include/utls.hpp"

bool g_status = true;

server::~server()
{
    clean();
}

server::server():
    auth(_clients,_password)
{
    this->_password.clear();
    this->_port = 0;
    this->status = false;
    this->_fd_server = -1;
}

void server::setup_address()
{
     memset(&_addr, 0, sizeof(_addr));
    _addr.sin_family = AF_INET;
    _addr.sin_addr.s_addr = INADDR_ANY;
    _addr.sin_port = htons(_port);
}

void server::create_socket()
{
    this->_fd_server = socket(AF_INET, SOCK_STREAM, 0);
    if (this->_fd_server < 0)
    {
        throw  std::runtime_error("Error: Failed to create socket");
    }
}

void server::start_listning()
{
    if (listen(this->_fd_server, 10) < 0)
    {
        throw  std::runtime_error("Error: Failed to listen on socket");
    }
    std::cout << "[" << Utils::getTime() << "] Server listening [::0]:" << _port << " (socket " << this->_fd_server << ")" << std::endl;
}

void server::bind_socket()
{
    int opt = 1;
    setsockopt(this->_fd_server, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    if (bind(this->_fd_server,(struct sockaddr*)&_addr,sizeof(_addr)) < 0)
    {
        throw std::runtime_error("Failed to bind on port "+ Utils::to_str(_port) +" already in use");
    }
}

void server::setup_poll()
{
    pollfd __poll;

    __poll.fd = this->_fd_server;
    __poll.events = POLLIN;
    _pfds.push_back(__poll);
}

void server::init()
{
    this->setup_address();
    this->create_socket();
    this->bind_socket();
    this->start_listning();
    this->setup_poll();
}

server::server(int port,std::string password):
_port(port),
_password(password),
auth(_clients,password)
{
    channel = new managerchannel(_clients,password); 
    this->init();
    this->handleEvent();
}