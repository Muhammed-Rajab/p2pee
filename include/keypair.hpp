#ifndef KEYPAIR_4_P2PEE_HPP
#define KEYPAIR_4_P2PEE_HPP

#include <iomanip>
#include <stdexcept>
#include <string>
#include <vector>

#include <openssl/err.h>
#include <openssl/evp.h>
#include <openssl/pem.h>

class KeyPair {
public:
  KeyPair(const std::string &private_path, const std::string &public_path) {
    private_key_ = load_private_key(private_path);
    public_key_ = load_public_key(public_path);
  }

  ~KeyPair() {
    if (private_key_)
      EVP_PKEY_free(private_key_);
    if (public_key_)
      EVP_PKEY_free(public_key_);
  }

  bool is_valid() const { return private_key_ && public_key_; }

  bool verify_pair() const {
    if (!is_valid())
      return false;

    const std::string msg = "test";

    // Sign with private key
    EVP_MD_CTX *sign_ctx = EVP_MD_CTX_new();
    size_t sig_len = 0;

    if (!sign_ctx ||
        !EVP_DigestSignInit(sign_ctx, nullptr, EVP_sha256(), nullptr,
                            private_key_) ||
        !EVP_DigestSignUpdate(sign_ctx, msg.data(), msg.size()) ||
        !EVP_DigestSignFinal(sign_ctx, nullptr, &sig_len)) {
      EVP_MD_CTX_free(sign_ctx);
      ERR_print_errors_fp(stderr);
      return false;
    }

    std::vector<unsigned char> sig(sig_len);
    if (!EVP_DigestSignFinal(sign_ctx, sig.data(), &sig_len)) {
      EVP_MD_CTX_free(sign_ctx);
      ERR_print_errors_fp(stderr);
      return false;
    }
    EVP_MD_CTX_free(sign_ctx);

    // Verify with public key
    EVP_MD_CTX *verify_ctx = EVP_MD_CTX_new();
    if (!verify_ctx ||
        !EVP_DigestVerifyInit(verify_ctx, nullptr, EVP_sha256(), nullptr,
                              public_key_) ||
        !EVP_DigestVerifyUpdate(verify_ctx, msg.data(), msg.size()) ||
        EVP_DigestVerifyFinal(verify_ctx, sig.data(), sig_len) != 1) {
      EVP_MD_CTX_free(verify_ctx);
      ERR_print_errors_fp(stderr);
      return false;
    }
    EVP_MD_CTX_free(verify_ctx);

    return true;
  }

  std::string generate_id() const {
    if (!public_key_) {
      throw std::runtime_error("Public key is not loaded.");
    }

    BIO *mem = BIO_new(BIO_s_mem());
    if (!PEM_write_bio_PUBKEY(mem, public_key_)) {
      BIO_free(mem);
      throw std::runtime_error("Failed to write public key to memory.");
    }

    char *data = nullptr;
    long len = BIO_get_mem_data(mem, &data);
    if (len <= 0 || !data) {
      BIO_free(mem);
      throw std::runtime_error("Failed to extract public key data.");
    }

    std::string pubkey_str(data, len);
    BIO_free(mem);

    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256(reinterpret_cast<const unsigned char *>(pubkey_str.c_str()),
           pubkey_str.size(), hash);

    std::ostringstream oss;
    for (int i = 0; i < SHA256_DIGEST_LENGTH; ++i) {
      oss << std::hex << std::setw(2) << std::setfill('0')
          << static_cast<int>(hash[i]);
    }

    return oss.str();
  }

  EVP_PKEY *get_private() const { return private_key_; }

  EVP_PKEY *get_public() const { return public_key_; }

private:
  EVP_PKEY *private_key_ = nullptr;
  EVP_PKEY *public_key_ = nullptr;

  EVP_PKEY *load_private_key(const std::string &path) {
    FILE *fp = fopen(path.c_str(), "r");
    if (!fp)
      return nullptr;
    EVP_PKEY *pkey = PEM_read_PrivateKey(fp, nullptr, nullptr, nullptr);
    fclose(fp);
    return pkey;
  }

  EVP_PKEY *load_public_key(const std::string &path) {
    FILE *fp = fopen(path.c_str(), "r");
    if (!fp)
      return nullptr;
    EVP_PKEY *pkey = PEM_read_PUBKEY(fp, nullptr, nullptr, nullptr);
    fclose(fp);
    return pkey;
  }
};

#endif // !KEYPAIR_4_P2PEE_HPP
