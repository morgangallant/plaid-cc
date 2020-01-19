#ifndef PLAID_STATUS_H_
#define PLAID_STATUS_H_

#include <string>

namespace plaid {

class Status {
public:
  ~Status() = default;

  static Status OK();
  static Status ParseError(const std::string &description);
  static Status RequestError(const std::string &description);

  bool IsOk() const;
  bool IsParseError() const;
  bool IsRequestError() const;

  std::string CodeDesc() const;
  std::string Description() const;

private:
  enum class Code { kOk, kParseError, kRequestError };
  Code code_;
  std::string description_;

  Status() noexcept;
  Status(Code code, const std::string &description);
};

template <typename T> class StatusWrapped {
public:
  StatusWrapped(const T &rhs) : value_(rhs), status_(Status::OK()) {}
  StatusWrapped(T &&rhs) : value_(std::move(rhs)), status_(Status::OK()) {}
  StatusWrapped(const StatusWrapped &rhs)
      : status_(rhs.status_), value_(rhs.value_) {}
  StatusWrapped(StatusWrapped &&rhs)
      : value_(std::move(rhs.value_)), status_(rhs.status_) {}

  ~StatusWrapped() = default;

  static StatusWrapped<T> FromStatus(Status status) {
    StatusWrapped<T> res;
    res.status_ = status;
    return res;
  }

  bool IsOk() const { return status_.IsOk(); }
  Status GetStatus() const { return status_; }
  T Unwrap() const { return value_; }

  std::string DescribeStatus() const {
    return status_.CodeDesc() + " (" + status_.Description() + ")";
  }

private:
  StatusWrapped() : status_(Status::OK()) {}

  T value_;
  Status status_;
};

} // namespace plaid

#endif // PLAID_STATUS_H_