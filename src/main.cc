#include "server.hpp"

int main(int argc, char **argv) {
  asio::io_context ctx;
  Server server{ctx};

  server.init_server();
}
