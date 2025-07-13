#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <string>

#include <openssl/err.h>
#include <openssl/pem.h>
#include <openssl/rsa.h>

#include "include/keypair.hpp"

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

  KeyPair pair(private_key_path, public_key_path);

  if (!pair.verify_pair()) {
    std::cerr << "got invalid keys" << std::endl;
    return EXIT_FAILURE;
  } else {
    std::cout << "shit is valid bruv!" << std::endl;
  }

  return EXIT_SUCCESS;
}
