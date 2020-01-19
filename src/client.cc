#include "plaid/client.h"
#include "plaid/transport.h"

#include <functional>
#include <memory>
#include <stdexcept>
#include <vector>

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

// Holdings

StatusWrapped<GetHoldingsResponse>
Client::GetHoldingsWithOptions(const std::string &access_token,
                               const GetHoldingsOptions &options) {
  if (access_token == "")
    return StatusWrapped<GetHoldingsResponse>::FromStatus(
        Status::MissingInfo("missing access token"));
  auto req = [&]() {
    auto req = Request(AppendUrl("investments/holdings/get"));
    auto req_data = GetHoldingsRequest();
    req_data.set_client_id(creds_.client_id);
    req_data.set_secret(creds_.secret);
    req_data.set_access_token(access_token);
    for (int i = 0; i < options.account_ids_size(); ++i)
      req_data.mutable_options()->add_account_ids(options.account_ids(i));
    req.SetBody(req_data);
    return req;
  };
  return make_plaid_request<GetHoldingsResponse>(req);
}

StatusWrapped<GetHoldingsResponse>
Client::GetHoldings(const std::string &access_token) {
  return GetHoldingsWithOptions(access_token, GetHoldingsOptions());
}

// Identity

StatusWrapped<GetIdentityResponse>
Client::GetIdentity(const std::string &access_token) {
  if (access_token == "")
    return StatusWrapped<GetIdentityResponse>::FromStatus(
        Status::MissingInfo("missing access token"));
  auto req = [&]() {
    auto req = Request(AppendUrl("identity/get"));
    auto req_data = GetIdentityRequest();
    req_data.set_client_id(creds_.client_id);
    req_data.set_secret(creds_.secret);
    req_data.set_access_token(access_token);
    req.SetBody(req_data);
    return req;
  };
  return make_plaid_request<GetIdentityResponse>(req);
}

// Income

StatusWrapped<GetIncomeResponse>
Client::GetIncome(const std::string &access_token) {
  if (access_token == "")
    return StatusWrapped<GetIncomeResponse>::FromStatus(
        Status::MissingInfo("missing access token"));
  auto req = [&]() {
    auto req = Request(AppendUrl("income/get"));
    auto req_data = GetIncomeRequest();
    req_data.set_client_id(creds_.client_id);
    req_data.set_secret(creds_.secret);
    req_data.set_access_token(access_token);
    req.SetBody(req_data);
    return req;
  };
  return make_plaid_request<GetIncomeResponse>(req);
}

// Institutions

StatusWrapped<GetInstitutionByIDResponse> Client::GetInstitutionByIDWithOptions(
    const std::string &id, const GetInstitutionByIDOptions &options) {
  if (id == "")
    return StatusWrapped<GetInstitutionByIDResponse>::FromStatus(
        Status::MissingInfo("missing id"));
  auto req = [&]() {
    auto req = Request(AppendUrl("institutions/get_by_id"));
    auto req_data = GetInstitutionByIDRequest();
    req_data.set_institution_id(id);
    req_data.set_public_key(creds_.public_key);
    *req_data.mutable_options() = options;
    req.SetBody(req_data);
    return req;
  };
  return make_plaid_request<GetInstitutionByIDResponse>(req);
}

StatusWrapped<GetInstitutionByIDResponse>
Client::GetInstitutionByID(const std::string &id) {
  return GetInstitutionByIDWithOptions(id, GetInstitutionByIDOptions());
}

StatusWrapped<GetInstitutionsResponse>
Client::GetInstitutionsWithOptions(int count, int offset,
                                   const GetInstitutionsOptions &options) {
  if (count == 0)
    count = 50;
  auto req = [&]() {
    auto req = Request(AppendUrl("institutions/get"));
    auto req_data = GetInstitutionsRequest();
    req_data.set_client_id(creds_.client_id);
    req_data.set_secret(creds_.secret);
    req_data.set_count(count);
    req_data.set_offset(offset);
    *req_data.mutable_options() = options;
    req.SetBody(req_data);
    return req;
  };
  return make_plaid_request<GetInstitutionsResponse>(req);
}

StatusWrapped<GetInstitutionsResponse> Client::GetInstitutions(int count,
                                                               int offset) {
  return GetInstitutionsWithOptions(count, offset, GetInstitutionsOptions());
}

StatusWrapped<SearchInstitutionsResponse> Client::SearchInstitutionsWithOptions(
    const std::string &query, const std::vector<std::string> &products,
    const SearchInstitutionsOptions &options) {
  if (query == "")
    return StatusWrapped<SearchInstitutionsResponse>::FromStatus(
        Status::MissingInfo("missing query"));
  auto req = [&]() {
    auto req = Request(AppendUrl("institutions/search"));
    auto req_data = SearchInstitutionsRequest();
    req_data.set_query(query);
    for (const auto &product : products)
      req_data.add_products(product);
    *req_data.mutable_options() = options;
    req.SetBody(req_data);
    return req;
  };
  return make_plaid_request<SearchInstitutionsResponse>(req);
}

StatusWrapped<SearchInstitutionsResponse>
Client::SearchInstitutions(const std::string &query,
                           const std::vector<std::string> &products) {
  return SearchInstitutionsWithOptions(query, products,
                                       SearchInstitutionsOptions());
}

// Investment Transactions

StatusWrapped<GetInvestmentTransactionsResponse>
Client::GetInvestmentTransactionsWithOptions(
    const std::string &access_token,
    const GetInvestmentTransactionsOptions &options) {
  if (access_token == "")
    return StatusWrapped<GetInvestmentTransactionsResponse>::FromStatus(
        Status::MissingInfo("missing access token"));
  auto req = [&]() {
    auto req = Request(AppendUrl("investments/transactions/get"));
    auto req_data = GetInvestmentTransactionsRequest();
    req_data.set_client_id(creds_.client_id);
    req_data.set_secret(creds_.secret);
    req_data.set_access_token(access_token);
    req_data.set_start_date(options.start_date());
    req_data.set_end_date(options.end_date());
    auto req_ops = GetInvestmentTransactionsRequestOptions();
    req_ops.set_count(options.count());
    req_ops.set_offset(options.offset());
    for (int i = 0; i < options.account_ids_size(); ++i)
      req_ops.add_account_ids(options.account_ids(i));
    *req_data.mutable_options() = req_ops;
    req.SetBody(req_data);
    return req;
  };
  return make_plaid_request<GetInvestmentTransactionsResponse>(req);
}

StatusWrapped<GetInvestmentTransactionsResponse>
Client::GetInvestmentTransactions(const std::string &access_token) {
  return GetInvestmentTransactionsWithOptions(
      access_token, GetInvestmentTransactionsOptions());
}

// Items

StatusWrapped<GetItemResponse>
Client::GetItem(const std::string &access_token) {
  if (access_token == "")
    return StatusWrapped<GetItemResponse>::FromStatus(
        Status::MissingInfo("missing access token"));
  auto req = [&]() {
    auto req = Request(AppendUrl("item/get"));
    auto req_data = GetItemRequest();
    req_data.set_client_id(creds_.client_id);
    req_data.set_secret(creds_.secret);
    req_data.set_access_token(access_token);
    req.SetBody(req_data);
    return req;
  };
  return make_plaid_request<GetItemResponse>(req);
}

StatusWrapped<RemoveItemResponse>
Client::RemoveItem(const std::string &access_token) {
  if (access_token == "")
    return StatusWrapped<RemoveItemResponse>::FromStatus(
        Status::MissingInfo("missing access token"));
  auto req = [&]() {
    auto req = Request(AppendUrl("item/remove"));
    auto req_data = RemoveItemRequest();
    req_data.set_client_id(creds_.client_id);
    req_data.set_secret(creds_.secret);
    req_data.set_access_token(access_token);
    req.SetBody(req_data);
    return req;
  };
  return make_plaid_request<RemoveItemResponse>(req);
}

StatusWrapped<UpdateItemWebhookResponse>
Client::UpdateItemWebhook(const std::string &access_token,
                          const std::string &webhook) {
  if (access_token == "")
    return StatusWrapped<UpdateItemWebhookResponse>::FromStatus(
        Status::MissingInfo("missing access token"));
  if (webhook == "")
    return StatusWrapped<UpdateItemWebhookResponse>::FromStatus(
        Status::MissingInfo("missing webhook"));
  auto req = [&]() {
    auto req = Request(AppendUrl("item/webhook/update"));
    auto req_data = UpdateItemWebhookRequest();
    req_data.set_client_id(creds_.client_id);
    req_data.set_secret(creds_.secret);
    req_data.set_access_token(access_token);
    req_data.set_webhook(webhook);
    req.SetBody(req_data);
    return req;
  };
  return make_plaid_request<UpdateItemWebhookResponse>(req);
}

StatusWrapped<InvalidateAccessTokenResponse>
Client::InvalidateAccessToken(const std::string &access_token) {
  if (access_token == "")
    return StatusWrapped<InvalidateAccessTokenResponse>::FromStatus(
        Status::MissingInfo("missing access token"));
  auto req = [&]() {
    auto req = Request(AppendUrl("item/access_token/invalidate"));
    auto req_data = InvalidateAccessTokenRequest();
    req_data.set_client_id(creds_.client_id);
    req_data.set_secret(creds_.secret);
    req_data.set_access_token(access_token);
    req.SetBody(req_data);
    return req;
  };
  return make_plaid_request<InvalidateAccessTokenResponse>(req);
}

StatusWrapped<UpdateAccessTokenVersionResponse>
Client::UpdateAccessTokenVersion(const std::string &access_token) {
  if (access_token == "")
    return StatusWrapped<UpdateAccessTokenVersionResponse>::FromStatus(
        Status::MissingInfo("missing access token"));
  auto req = [&]() {
    auto req = Request(AppendUrl("item/access_token/update_version"));
    auto req_data = UpdateAccessTokenVersionRequest();
    req_data.set_client_id(creds_.client_id);
    req_data.set_secret(creds_.secret);
    req_data.set_access_token_v1(access_token);
    req.SetBody(req_data);
    return req;
  };
  return make_plaid_request<UpdateAccessTokenVersionResponse>(req);
}

StatusWrapped<CreatePublicTokenResponse>
Client::CreatePublicToken(const std::string &access_token) {
  if (access_token == "")
    return StatusWrapped<CreatePublicTokenResponse>::FromStatus(
        Status::MissingInfo("missing access token"));
  auto req = [&]() {
    auto req = Request(AppendUrl("item/public_token/create"));
    auto req_data = CreatePublicTokenRequest();
    req_data.set_client_id(creds_.client_id);
    req_data.set_secret(creds_.secret);
    req_data.set_access_token(access_token);
    req.SetBody(req_data);
    return req;
  };
  return make_plaid_request<CreatePublicTokenResponse>(req);
}

StatusWrapped<ExchangePublicTokenResponse>
Client::ExchangePublicToken(const std::string &public_token) {
  if (public_token == "")
    return StatusWrapped<ExchangePublicTokenResponse>::FromStatus(
        Status::MissingInfo("missing public token"));
  auto req = [&]() {
    auto req = Request(AppendUrl("item/public_token/exchange"));
    auto req_data = ExchangePublicTokenRequest();
    req_data.set_client_id(creds_.client_id);
    req_data.set_secret(creds_.secret);
    req_data.set_public_token(public_token);
    req.SetBody(req_data);
    return req;
  };
  return make_plaid_request<ExchangePublicTokenResponse>(req);
}

// Liabilities

StatusWrapped<GetLiabilitiesResponse>
Client::GetLiabilitiesWithOptions(const std::string &access_token,
                                  const GetLiabilitiesOptions &options) {
  if (access_token == "")
    return StatusWrapped<GetLiabilitiesResponse>::FromStatus(
        Status::MissingInfo("missing access token"));
  auto req = [&]() {
    auto req = Request(AppendUrl("liabilities/get"));
    auto req_data = GetLiabilitiesRequest();
    req_data.set_client_id(creds_.client_id);
    req_data.set_secret(creds_.secret);
    req_data.set_access_token(access_token);
    for (int i = 0; i < options.account_ids_size(); ++i)
      req_data.mutable_options()->add_account_ids(options.account_ids(i));
    req.SetBody(req_data);
    return req;
  };
  return make_plaid_request<GetLiabilitiesResponse>(req);
}

StatusWrapped<GetLiabilitiesResponse>
Client::GetLiabilities(const std::string &access_token) {
  return GetLiabilitiesWithOptions(access_token, GetLiabilitiesOptions());
}

// Payments

StatusWrapped<CreatePaymentRecipientResponse>
Client::Client::CreatePaymentRecipient(const std::string &name,
                                       const std::string &iban,
                                       const PaymentRecipientAddress &address) {
}

StatusWrapped<GetPaymentRecipientResponse>
GetPaymentRecipient(const std::string &recipient_id) {}

StatusWrapped<ListPaymentRecipientsResponse> Client::ListPaymentRecipients() {}

StatusWrapped<CreatePaymentResponse>
Client::CreatePayment(const std::string &recipient_id,
                      const std::string &string, const PaymentAmount &amount) {}

StatusWrapped<CreatePaymentTokenResponse>
Client::CreatePaymentToken(const std::string &payment_id) {}

StatusWrapped<GetPaymentResponse>
Client::GetPayment(const std::string &payment_id) {}

StatusWrapped<ListPaymentsResponse>
Client::ListPayments(const ListPaymentsOptions &options) {}

// Processors

StatusWrapped<CreateProcessorTokenResponse>
Client::CreateApexToken(const std::string &access_token,
                        const std::string &account_id) {}

StatusWrapped<CreateProcessorTokenResponse>
Client::CreateDwollaToken(const std::string &access_token,
                          const std::string &account_id) {}

StatusWrapped<CreateProcessorTokenResponse>
Client::CreateOcrolusToken(const std::string &access_token,
                           const std::string &account_id) {}

StatusWrapped<CreateStripeTokenResponse>
Client::CreateStripeToken(const std::string &access_token,
                          const std::string &account_id) {}

// Sandbox

StatusWrapped<CreateSandboxPublicTokenResponse>
Client::CreateSandboxPublicToken(
    const std::string &institution_id,
    const std::vector<std::string> &initial_products) {}

StatusWrapped<ResetSandboxItemResponse>
Client::ResetSandboxItem(const std::string &access_token) {}

// Transactions

StatusWrapped<GetTransactionsResponse>
Client::GetTransactionsWithOptions(const std::string &access_token,
                                   const GetTransactionsOptions &options) {}

StatusWrapped<GetTransactionsResponse>
Client::GetTransactions(const std::string &access_token,
                        const std::string &start_date,
                        const std::string &end_date) {}

} // namespace plaid