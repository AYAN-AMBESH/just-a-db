#pragma once
#include <string>

namespace JustADb {

class Error {
public:
  enum class Kind {
    NoColumnFound,
    Unknown,
  };

  Error(Kind kind = Kind::Unknown) : kind_(kind) {}

  Error(const std::string &message, Kind kind = Kind::Unknown)
      : message_(message), kind_(kind) {}

  auto message() const -> std::string {
    if (message_.has_value()) {
      return message_.value();
    } else {
      return "Unknown error";
    }
  }
  auto kind() const { return kind_; }

private:
  std::optional<std::string> message_;
  Kind kind_;
};

template <typename T> class Result {
public:
  Result() = default;
  Result(const T &value) : value_(value) {}
  Result(const Error &error) : error_(error) {}

  auto value() const { return *value_; }
  auto error() const { return error_; }

private:
  std::optional<T> value_;
  std::optional<Error> error_;
};

} // namespace JustADb
