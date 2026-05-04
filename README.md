## Project Structure

```
.
├── include
│   ├── authentication.hpp
│   ├── channels
│   │   └── channel.hpp
│   ├── client.hpp
│   ├── server.hpp
│   └── utls.hpp
├── main
│   └── main.cpp
├── Makefile
└── src
    ├── authentication
    │   └── authentication.cpp
    ├── channels
    │   ├── channel_join_part.cpp
    │   ├── channel_manager.cpp
    │   ├── channel_message.cpp
    │   ├── channel_modes.cpp
    │   └── channel_operators.cpp
    ├── server
    │   ├── accept.cpp
    │   ├── bye.cpp
    │   ├── clean.cpp
    │   ├── desconnect.cpp
    │   ├── extract.cpp
    │   ├── HandleEvent.cpp
    │   ├── HandleSignals.cpp
    │   ├── receve.cpp
    │   └── server.cpp
    └── utils
        └── utls.cpp
```