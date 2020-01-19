#ifndef PLAID_CLIENT_H_
#define PLAID_CLIENT_H_

#include "plaid/plaid.pb.h"
#include "plaid/status.h"

#include <memory>
#include <string>

namespace plaid {

enum class Enviroment { Sandbox, Development, Production };

struct Credentials {
  std::string url;
  std::string client_id;
  std::string public_key;
  std::string secret;

  Credentials(Enviroment env, const std::string &client_id,
              const std::string &public_key, const std::string &secret);
  ~Credentials() = default;
};

class Client {
public:
  static std::unique_ptr<Client> Create(const Credentials &creds);
  ~Client() = default;

  // Categories
  StatusWrapped<GetCategoriesResponse> GetCategories();

private:
  Client(const Credentials &creds);
  Credentials creds_;
};

} // namespace plaid

#endif // PLAID_CLIENT_H_