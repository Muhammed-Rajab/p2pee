#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <string>

#include <openssl/err.h>
#include <openssl/pem.h>
#include <openssl/rsa.h>

EVP_PKEY *load_private_key(const std::string &path) {
  FILE *fp = fopen(path.c_str(), "r");
  if (!fp) {
    std::cerr << "failed to open private key file: " << path << std::endl;
    return nullptr;
  }

  EVP_PKEY *pkey = PEM_read_PrivateKey(fp, nullptr, nullptr, nullptr);
  fclose(fp);

  if (!pkey) {
    std::cerr << "failed to load private key: path: '" << path << "'"
              << std::endl;
    ERR_print_errors_fp(stderr);
    return nullptr;
  }

  return pkey;
}

EVP_PKEY *load_public_key(const std::string &path) {
  FILE *fp = fopen(path.c_str(), "r");
  if (!fp) {
    std::cerr << "failed to open public key file: " << path << std::endl;
    return nullptr;
  }

  EVP_PKEY *pkey = PEM_read_PUBKEY(fp, nullptr, nullptr, nullptr);
  fclose(fp);

  if (!pkey) {
    std::cerr << "failed to load public key: path: '" << path << "'"
              << std::endl;

    ERR_print_errors_fp(stderr);
    return nullptr;
  }

  return pkey;
}

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
