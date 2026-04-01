#include "server.hpp"
#include <argparse/argparse.hpp>
#include <exception>
#include <optional>
#include <spdlog/spdlog.h>
#include <string>

int main(int argc, char **argv) {
  asio::io_context ctx;
  argparse::ArgumentParser program("redis");

  program.add_argument("--port").help("port to bind to").default_value(6379);
  program.add_argument("--replicaof")
      .help("address and port to be replica of")
      .default_value("master");

  try {
    program.parse_args(argc, argv);
  } catch (const std::exception &err) {
    std::cerr << err.what() << std::endl;
    std::cerr << program;
    std::exit(1);
  }

  int port_val;
  std::string master_address;
  std::string master_port = "-1";
  try {
    port_val = program.get<int>("--port");
  } catch (...) {
    port_val = std::stoi(program.get<std::string>("--port"));
  }

  master_address = program.get<std::string>("--replicaof");
  spdlog::info(master_address);
  if (master_address != std::string("master")) {
    std::istringstream iss(master_address);
    std::vector<std::string> master((std::istream_iterator<std::string>(iss)),
                                    std::istream_iterator<std::string>());
    master_address = master[0];
    master_port = master[1];
  }

  spdlog::info(master_address);

  ServerConfig config(master_address, port_val, master_port);

  // Server server{ctx, port_val, std::move(config)};
  std::unique_ptr<Server> server{make_server(ctx, std::move(config))};
  server->init_server();
}
