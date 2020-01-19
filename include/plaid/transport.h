#ifndef PLAID_TRANSPORT_H_
#define PLAID_TRANSPORT_H_

#include "plaid/status.h"

#include <curl/curl.h>
#include <google/protobuf/util/json_util.h>
#include <string>
#include <type_traits>

namespace plaid {

class Request;

class Response {
  friend class Request;

public:
  Response() = default;
  ~Response() = default;

  long StatusCode() const;
  std::string Body() const;

  template <class T> StatusWrapped<T> Parse() const {
    static_assert(
        std::is_base_of<google::protobuf::Message, T>::value,
        "message to parse must inherit from google::protobuf::Message");
    T msg;
    using google::protobuf::util::JsonStringToMessage;
    google::protobuf::util::Status s = JsonStringToMessage(body_, &msg);
    if (!s.ok())
      return StatusWrapped<T>::FromStatus(
          Status::ParseError(s.error_message()));
    return msg;
  }

private:
  Response(long status_code, const std::string &body);

  long status_code_;
  std::string body_;
};

class Request {
public:
  explicit Request(const std::string &url);
  ~Request();

  void AddHeader(const std::string &key, const std::string &value);
  void SetBody(const google::protobuf::Message &msg);
  StatusWrapped<Response> Execute();

private:
  CURL *curl_ = nullptr;
  curl_slist *headers_ = nullptr;
  Status status_;
  std::string request_buf_;
  std::string response_buf_;
};

} // namespace plaid

#endif // PLAID_TRANSPORT_H_