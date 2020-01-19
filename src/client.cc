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

template <class T>
static StatusWrapped<T> make_plaid_request(std::function<Request()> req) {
  auto r = req();
  auto resp = r.Execute();
  if (!resp.IsOk())
    return StatusWrapped<T>::FromStatus(resp.GetStatus());
  return resp.Unwrap().Parse<T>();
}

StatusWrapped<GetCategoriesResponse> Client::GetCategories() {
  auto req = [&]() { return Request(creds_.url + "categories/get"); };
  return make_plaid_request<GetCategoriesResponse>(req);
}

} // namespace plaid