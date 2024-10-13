#ifndef __RESULT_HPP__
#define __RESULT_HPP__

#include <optional>

/*
 * Result is a useful type which holds either a success value or an error value.
 * Success values can be of one type and error values can be of a different type.
 * 
 * This presents an alternative to traditional C++ exception handling.
 * Instead of throwing an exception, you return an error type.
 * Any function calling a function that returns a Result type is forced to
 * acknowledge the possibility of an error return and deal with it explicitly.
 * It is also expected to be better for performance, and may also have a positive
 * effect on the stability of the program.
 *
 * Since the class is fully templated, this library is header only.
 *
 * Since the purpose of this class is only to pass a message from a function to the caller,
 * we are using an immutable design for it. That means no setters or mutators.
 */

template <typename Suc, typename Err> class Result {
private:
  std::optional<Suc> _success;
  std::optional<Err> _error;
  // Full initialization constructor.
  // To enforce the "exactly one" nature of this class,
  // this constructor is not publicly visible.
  Result(std::optional<Suc> suc, std::optional<Err> err) {
    _success = suc;
    _error = err;
  }
public:
  // Constructors for each case:
  // - Success or error
  // - Get the value directly, or initialize it in dynamic memory and get a pointer instead
  static Result<Suc, Err> make_success(Suc suc) {
    return Result<Suc, Err>(std::optional<Suc>(suc), std::optional<Err>());
  }
  static Result<Suc, Err>* make_success_ptr(Suc suc) {
    return new Result<Suc, Err>(std::optional<Suc>(suc), std::optional<Err>());
  }
  static Result<Suc, Err> make_error(Err err) {
    return Result<Suc, Err>(std::optional<Suc>(), std::optional<Err>(err));
  }
  static Result<Suc, Err>* make_error_ptr(Err err) {
    return new Result<Suc, Err>(std::optional<Suc>(), std::optional<Err>(err));
  }
  // No need to manually define:
  // * Destructor
  // * Copy/move constructor
  // * Assignment operator
  // Getters
  bool is_success() const {
    return _success.has_value();
  }
  bool is_error() const {
    return _error.has_value();
  }
  const Suc& get_success_value() const {
    return _success.value();
  }
  const Err& get_error_value() const {
    return _error.value();
  }
};

#endif
