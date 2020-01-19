#include "plaid/client.h"
#include "plaid/transport.h"

#include <functional>
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
        Status::MissingInfo("missing access token"));
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
        Status::MissingInfo("missing access token"));
  auto req = [&]() {
    auto req = Request(AppendUrl("accounts/get"));
    auto req_data = GetAccountsRequest();
    req_data.set_client_id(creds_.client_id);
    req_data.set_secret(creds_.secret);
    req_data.set_access_token(access_token);
    for (int i = 0; i < options.account_ids_size(); ++i)
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

// Assets

StatusWrapped<GetAssetReportResponse>
Client::GetAssetReport(const std::string &asset_report_token) {
  if (asset_report_token == "")
    return StatusWrapped<GetAssetReportResponse>::FromStatus(
        Status::MissingInfo("missing asset report token"));
  auto req = [&]() {
    auto req = Request(AppendUrl("asset_report/get"));
    auto req_data = GetAssetReportRequest();
    req_data.set_client_id(creds_.client_id);
    req_data.set_secret(creds_.secret);
    req_data.set_asset_report_token(asset_report_token);
    req.SetBody(req_data);
    return req;
  };
  return make_plaid_request<GetAssetReportResponse>(req);
}

StatusWrapped<CreateAuditCopyTokenResponse>
Client::CreateAuditCopy(const std::string &asset_report_token,
                        const std::string &auditor_id) {
  if (asset_report_token == "")
    return StatusWrapped<CreateAuditCopyTokenResponse>::FromStatus(
        Status::MissingInfo("missing asset report token"));
  if (auditor_id == "")
    return StatusWrapped<CreateAuditCopyTokenResponse>::FromStatus(
        Status::MissingInfo("missing auditor id"));
  auto req = [&]() {
    auto req = Request(AppendUrl("asset_report/audit_copy/create"));
    auto req_data = CreateAuditCopyRequest();
    req_data.set_client_id(creds_.client_id);
    req_data.set_secret(creds_.secret);
    req_data.set_asset_report_token(asset_report_token);
    req_data.set_auditor_id(auditor_id);
    req.SetBody(req_data);
    return req;
  };
  return make_plaid_request<CreateAuditCopyTokenResponse>(req);
}

StatusWrapped<RemoveAssetReportResponse>
Client::RemoveAssetReport(const std::string &asset_report_token) {
  if (asset_report_token == "")
    return StatusWrapped<RemoveAssetReportResponse>::FromStatus(
        Status::MissingInfo("missing asset report token"));
  auto req = [&]() {
    auto req = Request(AppendUrl("asset_report/remove"));
    auto req_data = RemoveAssetReportRequest();
    req_data.set_client_id(creds_.client_id);
    req_data.set_secret(creds_.secret);
    req_data.set_asset_report_token(asset_report_token);
    req.SetBody(req_data);
    return req;
  };
  return make_plaid_request<RemoveAssetReportResponse>(req);
}

// Authentication

StatusWrapped<GetAuthResponse>
Client::GetAuthWithOptions(const std::string &access_token,
                           const GetAuthOptions &options) {
  if (access_token == "")
    return StatusWrapped<GetAuthResponse>::FromStatus(
        Status::MissingInfo("missing access token"));
  auto req = [&] {
    auto req = Request(AppendUrl("auth/get"));
    auto req_data = GetAuthRequest();
    req_data.set_client_id(creds_.client_id);
    req_data.set_secret(creds_.secret);
    req_data.set_access_token(access_token);
    for (int i = 0; i < options.account_ids_size(); ++i)
      req_data.mutable_options()->add_account_ids(options.account_ids(i));
    req.SetBody(req_data);
    return req;
  };
  return make_plaid_request<GetAuthResponse>(req);
}

StatusWrapped<GetAuthResponse>
Client::GetAuth(const std::string &access_token) {
  return GetAuthWithOptions(access_token, GetAuthOptions());
}

// Categories

StatusWrapped<GetCategoriesResponse> Client::GetCategories() {
  auto req = [&]() { return Request(AppendUrl("categories/get")); };
  return make_plaid_request<GetCategoriesResponse>(req);
}

} // namespace plaid