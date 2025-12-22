#include "server.hpp"
#include <argparse/argparse.hpp>
#include <exception>
#include <spdlog/spdlog.h>

int main(int argc, char **argv) {
  asio::io_context ctx;
  argparse::ArgumentParser program("redis");

  program.add_argument("--port").help("port to bind to").default_value(6379);

  try {
    program.parse_args(argc, argv);
  } catch (const std::exception &err) {
    std::cerr << err.what() << std::endl;
    std::cerr << program;
    std::exit(1);
  }

  int port_val;
  try {
    port_val = program.get<int>("--port");
  } catch (...) {
    port_val = std::stoi(program.get<std::string>("--port"));
  }

  Server server{ctx, port_val};
  server.init_server();
}
