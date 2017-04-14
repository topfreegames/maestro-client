#include <iostream>
#include "../include/client.h"

int main() {
  Maestro::Client *client = new Maestro::Client("http://192.168.64.26:30537");
  client->initialize();
  //std::cout << "zoia " << client->get_ping_interval() << "\n";
  return EXIT_SUCCESS;
}
