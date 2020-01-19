#ifndef PLAID_CLIENT_H_
#define PLAID_CLIENT_H_

#include "plaid/plaid.pb.h"
#include "plaid/status.h"

#include <memory>
#include <string>
#include <vector>

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

  // Assets
  StatusWrapped<GetAssetReportResponse>
  GetAssetReport(const std::string &asset_report_token);
  StatusWrapped<CreateAuditCopyTokenResponse>
  CreateAuditCopy(const std::string &asset_report_token,
                  const std::string &auditor_id);
  StatusWrapped<RemoveAssetReportResponse>
  RemoveAssetReport(const std::string &asset_report_token);

  // Authentication
  StatusWrapped<GetAuthResponse>
  GetAuthWithOptions(const std::string &access_token,
                     const GetAuthOptions &options);
  StatusWrapped<GetAuthResponse> GetAuth(const std::string &access_token);

  // Categories
  StatusWrapped<GetCategoriesResponse> GetCategories();

  // Holdings
  StatusWrapped<GetHoldingsResponse>
  GetHoldingsWithOptions(const std::string &access_token,
                         const GetHoldingsOptions &options);
  StatusWrapped<GetHoldingsResponse>
  GetHoldings(const std::string &access_token);

  // Identity
  StatusWrapped<GetIdentityResponse>
  GetIdentity(const std::string &access_token);

  // Income
  StatusWrapped<GetIncomeResponse> GetIncome(const std::string &access_token);

  // Institutions
  StatusWrapped<GetInstitutionByIDResponse>
  GetInstitutionByIDWithOptions(const std::string &id,
                                const GetInstitutionByIDOptions &options);
  StatusWrapped<GetInstitutionByIDResponse>
  GetInstitutionByID(const std::string &id);
  StatusWrapped<GetInstitutionsResponse>
  GetInstitutionsWithOptions(int count, int offset,
                             const GetInstitutionsOptions &options);
  StatusWrapped<GetInstitutionsResponse> GetInstitutions(int count, int offset);
  StatusWrapped<SearchInstitutionsResponse>
  SearchInstitutionsWithOptions(const std::string &query,
                                const std::vector<std::string> &products,
                                const SearchInstitutionsOptions &options);
  StatusWrapped<SearchInstitutionsResponse>
  SearchInstitutions(const std::string &query,
                     const std::vector<std::string> &products);

  // Investment Transactions
  StatusWrapped<GetInvestmentTransactionsResponse>
  GetInvestmentTransactionsWithOptions(
      const std::string &access_token,
      const GetInvestmentTransactionsOptions &options);
  StatusWrapped<GetInvestmentTransactionsResponse>
  GetInvestmentTransactions(const std::string &access_token);

  // Items
  StatusWrapped<GetItemResponse> GetItem(const std::string &access_token);
  StatusWrapped<RemoveItemResponse> RemoveItem(const std::string &access_token);
  StatusWrapped<UpdateItemWebhookResponse>
  UpdateItemWebhook(const std::string &access_token,
                    const std::string &webhook);
  StatusWrapped<InvalidateAccessTokenResponse>
  InvalidateAccessToken(const std::string &access_token);
  StatusWrapped<UpdateAccessTokenVersionResponse>
  UpdateAccessTokenVersion(const std::string &access_token);
  StatusWrapped<CreatePublicTokenResponse>
  CreatePublicToken(const std::string &access_token);
  StatusWrapped<ExchangePublicTokenResponse>
  ExchangePublicToken(const std::string &public_token);

  // Liabilities
  StatusWrapped<GetLiabilitiesResponse>
  GetLiabilitiesWithOptions(const std::string &access_token,
                            const GetLiabilitiesOptions &options);
  StatusWrapped<GetLiabilitiesResponse>
  GetLiabilities(const std::string &access_token);

  // Payments
  StatusWrapped<CreatePaymentRecipientResponse>
  CreatePaymentRecipient(const std::string &name, const std::string &iban,
                         const PaymentRecipientAddress &address);
  StatusWrapped<GetPaymentRecipientResponse>
  GetPaymentRecipient(const std::string &recipient_id);
  StatusWrapped<ListPaymentRecipientsResponse> ListPaymentRecipients();
  StatusWrapped<CreatePaymentResponse>
  CreatePayment(const std::string &recipient_id, const std::string &reference,
                const PaymentAmount &amount);
  StatusWrapped<CreatePaymentTokenResponse>
  CreatePaymentToken(const std::string &payment_id);
  StatusWrapped<GetPaymentResponse> GetPayment(const std::string &payment_id);
  StatusWrapped<ListPaymentsResponse>
  ListPayments(const ListPaymentsOptions &options);

  // Processors
  StatusWrapped<CreateProcessorTokenResponse>
  CreateApexToken(const std::string &access_token,
                  const std::string &account_id);
  StatusWrapped<CreateProcessorTokenResponse>
  CreateDwollaToken(const std::string &access_token,
                    const std::string &account_id);
  StatusWrapped<CreateProcessorTokenResponse>
  CreateOcrolusToken(const std::string &access_token,
                     const std::string &account_id);
  StatusWrapped<CreateStripeTokenResponse>
  CreateStripeToken(const std::string &access_token,
                    const std::string &account_id);

  // Sandbox
  StatusWrapped<CreateSandboxPublicTokenResponse>
  CreateSandboxPublicToken(const std::string &institution_id,
                           const std::vector<std::string> &initial_products);
  StatusWrapped<ResetSandboxItemResponse>
  ResetSandboxItem(const std::string &access_token);

  // Transactions
  StatusWrapped<GetTransactionsResponse>
  GetTransactionsWithOptions(const std::string &access_token,
                             const GetTransactionsOptions &options);
  StatusWrapped<GetTransactionsResponse>
  GetTransactions(const std::string &access_token,
                  const std::string &start_date, const std::string &end_date);

private:
  Client(const Credentials &creds);

  std::string AppendUrl(const std::string &extra) const;

  Credentials creds_;
};

} // namespace plaid

#endif // PLAID_CLIENT_H_