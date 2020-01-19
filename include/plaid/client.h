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

  // Accounts
  StatusWrapped<GetBalancesResponse>
  GetBalancesWithOptions(const std::string &access_token,
                         const GetBalancesOptions &options);
  StatusWrapped<GetBalancesResponse>
  GetBalances(const std::string &access_token);
  StatusWrapped<GetAccountsResponse>
  GetAccountsWithOptions(const std::string &access_token,
                         const GetAccountsOptions &options);
  StatusWrapped<GetAccountsResponse>
  GetAccounts(const std::string &access_token);

  // Categories
  StatusWrapped<GetCategoriesResponse> GetCategories();

private:
  Client(const Credentials &creds);

  std::string AppendUrl(const std::string &extra) const;

  Credentials creds_;
};

} // namespace plaid

#endif // PLAID_CLIENT_H_