#include <cstdlib>
#include <iostream>
#include <string>

int main(int argc, char **argv) {

  if (argc < 3) {
    std::cerr << "Usage: " << argv[0]
              << " <private_key.pem> <public_key.pem>\n";
    return EXIT_FAILURE;
  }

  std::string private_key_path = argv[1];
  std::string public_key_path = argv[2];

  std::cout << "private key is at \"" << private_key_path << "\"" << std::endl;
  std::cout << "public key is at \"" << public_key_path << "\"" << std::endl;

  std::cout << "Hello, P2PEE" << std::endl;

  // TODO: load and use them keys;
  return EXIT_SUCCESS;
}
