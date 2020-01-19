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

private:
  Client(const Credentials &creds);

  std::string AppendUrl(const std::string &extra) const;

  Credentials creds_;
};

} // namespace plaid

#endif // PLAID_CLIENT_H_