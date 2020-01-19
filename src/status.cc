#include "plaid/status.h"

#include <stdexcept>

namespace plaid {

Status::Status() noexcept {
  this->code_ = Code::kOk;
  this->description_ = std::string();
}

Status::Status(Code code, const std::string &description) {
  this->code_ = code;
  this->description_ = description;
}

Status Status::OK() { return Status(); }
Status Status::ParseError(const std::string &description) {
  return Status(Code::kParseError, description);
}
Status Status::RequestError(const std::string &description) {
  return Status(Code::kRequestError, description);
}

bool Status::IsOk() const { return code_ == Code::kOk; }
bool Status::IsParseError() const { return code_ == Code::kParseError; }
bool Status::IsRequestError() const { return code_ == Code::kRequestError; }

std::string Status::CodeDesc() const {
  switch (code_) {
  case Code::kOk:
    return "Ok";
  case Code::kParseError:
    return "Parse Error";
  case Code::kRequestError:
    return "Request Error";
  default:
    throw std::runtime_error("invalid code provided");
  }
}
std::string Status::Description() const { return description_; }

} // namespace plaid