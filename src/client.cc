#include "plaid/client.h"
#include "plaid/transport.h"

#include <memory>
#include <stdexcept>

namespace plaid {

static std::string get_url_for_env(Enviroment env) {
  switch (env) {
  case Enviroment::Sandbox:
    return "https://sandbox.plaid.com/";
  case Enviroment::Development:
    return "https://development.plaid.com/";
  case Enviroment::Production:
    return "https://production.plaid.com/";
  default:
    throw std::runtime_error("invalid enviroment setting");
  }
}

Credentials::Credentials(Enviroment env, const std::string &client_id,
                         const std::string &public_key,
                         const std::string &secret) {
  this->url = get_url_for_env(env);
  this->client_id = client_id;
  this->public_key = public_key;
  this->secret = secret;
}

Client::Client(const Credentials &creds) : creds_(creds) {}

std::unique_ptr<Client> Client::Create(const Credentials &creds) {
  // cannot use std::make_unique in C++11
  return std::unique_ptr<Client>(new Client(creds));
}

std::string Client::AppendUrl(const std::string &extra) const {
  return creds_.url + extra;
}

template <class T>
static StatusWrapped<T> make_plaid_request(std::function<Request()> req) {
  auto r = req();
  auto resp = r.Execute();
  if (!resp.IsOk())
    return StatusWrapped<T>::FromStatus(resp.GetStatus());
  return resp.Unwrap().Parse<T>();
}

// Accounts

StatusWrapped<GetBalancesResponse>
Client::GetBalancesWithOptions(const std::string &access_token,
                               const GetBalancesOptions &options) {
  if (access_token == "")
    return StatusWrapped<GetBalancesResponse>::FromStatus(
        Status::MissingInfo("missing account token"));
  auto req = [&]() {
    auto req = Request(AppendUrl("accounts/balance/get"));
    auto req_data = GetBalancesRequest();
    req_data.set_client_id(creds_.client_id);
    req_data.set_secret(creds_.secret);
    req_data.set_access_token(access_token);
    for (int i = 0; i < options.account_ids_size(); ++i)
      req_data.mutable_options()->add_account_ids(options.account_ids(i));
    req.SetBody(req_data);
    return req;
  };
  return make_plaid_request<GetBalancesResponse>(req);
}

StatusWrapped<GetBalancesResponse>
Client::GetBalances(const std::string &access_token) {
  return GetBalancesWithOptions(access_token, GetBalancesOptions());
}

StatusWrapped<GetAccountsResponse>
Client::GetAccountsWithOptions(const std::string &access_token,
                               const GetAccountsOptions &options) {
  if (access_token == "")
    return StatusWrapped<GetAccountsResponse>::FromStatus(
        Status::MissingInfo("missing account token"));
  auto req = [&]() {
    auto req = Request(AppendUrl("accounts/get"));
    auto req_data = GetAccountsRequest();
    req_data.set_client_id(creds_.client_id);
    req_data.set_secret(creds_.secret);
    req_data.set_access_token(access_token);
    for (int i = 0; i < options.account_ids_size; ++i)
      req_data.mutable_options()->add_account_ids(options.account_ids(i));
    req.SetBody(req_data);
    return req;
  };
  return make_plaid_request<GetAccountsResponse>(req);
}

StatusWrapped<GetAccountsResponse>
Client::GetAccounts(const std::string &access_token) {
  return GetAccountsWithOptions(access_token, GetAccountsOptions());
}

// Categories

StatusWrapped<GetCategoriesResponse> Client::GetCategories() {
  auto req = [&]() { return Request(AppendUrl("categories/get")); };
  return make_plaid_request<GetCategoriesResponse>(req);
}

} // namespace plaid