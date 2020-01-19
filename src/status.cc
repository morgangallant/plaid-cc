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
Status Status::MissingInfo(const std::string &description) {
  return Status(Code::kMissingInfo, description);
}

bool Status::IsOk() const { return code_ == Code::kOk; }
bool Status::IsParseError() const { return code_ == Code::kParseError; }
bool Status::IsRequestError() const { return code_ == Code::kRequestError; }
bool Status::IsMissingInfo() const { return code_ == Code::kMissingInfo; }

std::string Status::CodeDesc() const {
  switch (code_) {
  case Code::kOk:
    return "Ok";
  case Code::kParseError:
    return "Parse Error";
  case Code::kRequestError:
    return "Request Error";
  case Code::kMissingInfo:
    return "Missing Info";
  default:
    throw std::runtime_error("invalid code provided");
  }
}
std::string Status::Description() const { return description_; }

} // namespace plaid