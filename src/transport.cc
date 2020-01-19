#include "plaid/transport.h"
#include "plaid/status.h"

#include <curl/curl.h>
#include <google/protobuf/util/json_util.h>
#include <stdexcept>
#include <string>

namespace plaid {

Response::Response(long status_code, const std::string &body) {
  this->status_code_ = status_code;
  this->body_ = body;
}

long Response::StatusCode() const { return status_code_; }
std::string Response::Body() const { return body_; }

// Credit: https://stackoverflow.com/questions/9786150/ (Joachim Isaksson)
static size_t resp_cb(void *data, size_t len, size_t nmemb, void *userp) {
  size_t real_size = len * nmemb;
  ((std::string *)userp)->append((char *)data, real_size);
  return real_size;
}

Request::Request(const std::string &url) : status_(Status::OK()) {
  curl_ = curl_easy_init();
  if (!curl_)
    throw std::runtime_error("curl_easy_init returned nullptr");

  curl_easy_setopt(curl_, CURLOPT_URL, url.c_str());
  curl_easy_setopt(curl_, CURLOPT_FOLLOWLOCATION, 1L);
  curl_easy_setopt(curl_, CURLOPT_POST, 1L);
  curl_easy_setopt(curl_, CURLOPT_WRITEFUNCTION, resp_cb);
  curl_easy_setopt(curl_, CURLOPT_WRITEDATA, (void *)&response_buf_);
  curl_easy_setopt(curl_, CURLOPT_USERAGENT, "plaid-cc/1.0");

  AddHeader("Content-Type", "application/json");
}

Request::~Request() {
  if (curl_)
    curl_easy_cleanup(curl_);
  if (headers_)
    curl_slist_free_all(headers_);
}

void Request::AddHeader(const std::string &key, const std::string &value) {
  const auto tmp = key + ": " + value;
  headers_ = curl_slist_append(headers_, tmp.c_str());
}

void Request::SetBody(const google::protobuf::Message &msg) {
  using google::protobuf::util::MessageToJsonString;
  google::protobuf::util::Status s = MessageToJsonString(msg, &request_buf_);
  if (s.ok()) {
    curl_off_t req_size = static_cast<curl_off_t>(request_buf_.size());
    curl_easy_setopt(curl_, CURLOPT_POSTFIELDSIZE, req_size);
    curl_easy_setopt(curl_, CURLOPT_POSTFIELDS, request_buf_.c_str());
    status_ = Status::OK();
  } else {
    status_ = Status::ParseError(s.error_message());
  }
}

StatusWrapped<Response> Request::Execute() {
  curl_easy_setopt(curl_, CURLOPT_HTTPHEADER, headers_);

  if (!status_.IsOk())
    return StatusWrapped<Response>::FromStatus(status_);
  if (request_buf_.size() == 0) {
    curl_easy_setopt(curl_, CURLOPT_POSTFIELDSIZE, 2L);
    curl_easy_setopt(curl_, CURLOPT_POSTFIELDS, "{}");
  }

  CURLcode res = curl_easy_perform(curl_);
  if (res != CURLE_OK) {
    const std::string desc = curl_easy_strerror(res);
    return StatusWrapped<Response>::FromStatus(Status::RequestError(desc));
  }

  long code = 0;
  curl_easy_getinfo(curl_, CURLINFO_RESPONSE_CODE, &code);
  return Response(code, response_buf_);
}

} // namespace plaid