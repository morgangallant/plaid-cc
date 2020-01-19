#include <iostream>

#include "plaid/client.h"

int main() {
  std::cout << "Starting Testbench!" << std::endl;

  auto creds = plaid::Credentials(plaid::Enviroment::Sandbox, "", "", "");
  std::cout << "URL: " << creds.url << std::endl;

  auto client = plaid::Client::Create(creds);
  auto categories = client->GetCategories();
  if (categories.IsOk())
    std::cout << "Fetched " << categories.Unwrap().categories_size()
              << " categories." << std::endl;
  else
    std::cout << "Error: " << categories.DescribeStatus() << std::endl;
}