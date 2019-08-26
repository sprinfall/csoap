#ifndef CSOAP_PARAMETER_H_
#define CSOAP_PARAMETER_H_

#include <string>

#include "webcc/globals.h"

namespace csoap {

// Key-value SOAP parameter.
class Parameter {
public:
  Parameter() : as_cdata_(false) {
  }

  Parameter(const Parameter&) = default;
  Parameter& operator=(const Parameter&) = default;

  Parameter(const std::string& key, const char* value)
      : key_(key), value_(value), as_cdata_(false) {
  }

  Parameter(const std::string& key, const std::string& value,
            bool as_cdata = false)
      : key_(key), value_(value), as_cdata_(as_cdata) {
  }

  Parameter(const std::string& key, std::string&& value,
            bool as_cdata = false)
      : key_(key), value_(std::move(value)), as_cdata_(as_cdata) {
  }

  Parameter(const std::string& key, int value)
      : key_(key), value_(std::to_string(value)), as_cdata_(false) {
  }

  Parameter(const std::string& key, double value)
      : key_(key), value_(std::to_string(value)), as_cdata_(false) {
  }

  Parameter(const std::string& key, bool value)
      : key_(key), value_(value ? "true" : "false"), as_cdata_(false) {
  }

#if WEBCC_DEFAULT_MOVE_COPY_ASSIGN

  Parameter(Parameter&&) = default;
  Parameter& operator=(Parameter&&) = default;

#else

  Parameter(Parameter&& rhs)
      : key_(std::move(rhs.key_)),
        value_(std::move(rhs.value_)),
        as_cdata_(rhs.as_cdata_) {
  }

  Parameter& operator=(Parameter&& rhs) {
    if (&rhs != this) {
      key_ = std::move(rhs.key_);
      value_ = std::move(rhs.value_);
      as_cdata_ = rhs.as_cdata_;
    }
    return *this;
  }

#endif  // WEBCC_DEFAULT_MOVE_COPY_ASSIGN

  const std::string& key() const { return key_; }
  const std::string& value() const { return value_; }

  const char* c_key() const { return key_.c_str(); }
  const char* c_value() const { return value_.c_str(); }

  bool as_cdata() const { return as_cdata_; }

private:
  std::string key_;
  std::string value_;
  bool as_cdata_;
};

}  // namespace csoap

#endif  // CSOAP_PARAMETER_H_
