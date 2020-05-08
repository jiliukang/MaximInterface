/*******************************************************************************
* Copyright (C) Maxim Integrated Products, Inc., All Rights Reserved.
*
* Permission is hereby granted, free of charge, to any person obtaining a
* copy of this software and associated documentation files (the "Software"),
* to deal in the Software without restriction, including without limitation
* the rights to use, copy, modify, merge, publish, distribute, sublicense,
* and/or sell copies of the Software, and to permit persons to whom the
* Software is furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included
* in all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
* OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
* IN NO EVENT SHALL MAXIM INTEGRATED BE LIABLE FOR ANY CLAIM, DAMAGES
* OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
* ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
* OTHER DEALINGS IN THE SOFTWARE.
*
* Except as contained in this notice, the name of Maxim Integrated
* Products, Inc. shall not be used except as stated in the Maxim Integrated
* Products, Inc. Branding Policy.
*
* The mere transfer of this software does not imply any licenses
* of trade secrets, proprietary technology, copyrights, patents,
* trademarks, maskwork rights, or any other form of intellectual
* property whatsoever. Maxim Integrated Products, Inc. retains all
* ownership rights.
*******************************************************************************/

/// @file
/// @brief
/// Assertions similar to SystemVerilog and reporting similar to UVM.
/// @details
/// Reporting is based on four SystemVerilog DPI compatible hooks:
/// MaximInterfaceTest_report_info, MaximInterfaceTest_report_warning,
/// MaximInterfaceTest_report_error, and MaximInterfaceTest_report_fatal.

#ifndef MaximInterfaceTest_Test_hpp
#define MaximInterfaceTest_Test_hpp

#include <sstream>
#include <string>

namespace MaximInterfaceTest {

/// Info severity for use with assertion and reporting macros.
void info(const char * message, const char * file, int line);

/// Warning severity for use with assertion and reporting macros.
void warning(const char * message, const char * file, int line);

/// Error severity for use with assertion and reporting macros.
void error(const char * message, const char * file, int line);

/// Fatal severity for use with assertion and reporting macros.
void fatal(const char * message, const char * file, int line);

namespace detail {

typedef void Severity(const char * message, const char * file, int line);

void reportMessage(Severity & severity, const char * message, const char * file,
                   int line);

void reportMessage(Severity & severity, const std::string & message,
                   const char * file, int line);

void insertAssertionFailedPrefix(std::ostream & stream, const char * expected);

void insertAssertionFailedSuffix(std::ostream & stream);

template <typename T>
void reportAssertionFailed(Severity & severity, const char * expected,
                           const T & value, const char * file, int line) {
  std::ostringstream message;
  insertAssertionFailedPrefix(message, expected);
  message << value;
  insertAssertionFailedSuffix(message);
  reportMessage(severity, message.str(), file, line);
}

template <typename LHS, typename RHS>
void reportAssertionFailed(Severity & severity, const char * expected,
                           const LHS & lhs, const RHS & rhs, const char * file,
                           int line) {
  std::ostringstream message;
  insertAssertionFailedPrefix(message, expected);
  message << '(' << lhs << ") and (" << rhs << ')';
  insertAssertionFailedSuffix(message);
  reportMessage(severity, message.str(), file, line);
}

template <typename T>
bool compare_true(Severity & severity, const char * expected, const T & value,
                  const char * file, int line) {
  const bool success = static_cast<bool>(value);
  if (!success) {
    reportAssertionFailed(severity, expected, value, file, line);
  }
  return success;
}

template <typename T>
bool compare_false(Severity & severity, const char * expected, const T & value,
                   const char * file, int line) {
  const bool success = !value;
  if (!success) {
    reportAssertionFailed(severity, expected, value, file, line);
  }
  return success;
}

template <typename LHS, typename RHS>
bool compare_equal(Severity & severity, const char * expected, const LHS & lhs,
                   const RHS & rhs, const char * file, int line) {
  const bool success = (lhs == rhs);
  if (!success) {
    reportAssertionFailed(severity, expected, lhs, rhs, file, line);
  }
  return success;
}

template <typename LHS, typename RHS>
bool compare_not_equal(Severity & severity, const char * expected,
                       const LHS & lhs, const RHS & rhs, const char * file,
                       int line) {
  const bool success = (lhs != rhs);
  if (!success) {
    reportAssertionFailed(severity, expected, lhs, rhs, file, line);
  }
  return success;
}

template <typename LHS, typename RHS>
bool compare_less(Severity & severity, const char * expected, const LHS & lhs,
                  const RHS & rhs, const char * file, int line) {
  const bool success = (lhs < rhs);
  if (!success) {
    reportAssertionFailed(severity, expected, lhs, rhs, file, line);
  }
  return success;
}

template <typename LHS, typename RHS>
bool compare_less_equal(Severity & severity, const char * expected,
                        const LHS & lhs, const RHS & rhs, const char * file,
                        int line) {
  const bool success = (lhs <= rhs);
  if (!success) {
    reportAssertionFailed(severity, expected, lhs, rhs, file, line);
  }
  return success;
}

template <typename LHS, typename RHS>
bool compare_greater(Severity & severity, const char * expected,
                     const LHS & lhs, const RHS & rhs, const char * file,
                     int line) {
  const bool success = (lhs > rhs);
  if (!success) {
    reportAssertionFailed(severity, expected, lhs, rhs, file, line);
  }
  return success;
}

template <typename LHS, typename RHS>
bool compare_greater_equal(Severity & severity, const char * expected,
                           const LHS & lhs, const RHS & rhs, const char * file,
                           int line) {
  const bool success = (lhs >= rhs);
  if (!success) {
    reportAssertionFailed(severity, expected, lhs, rhs, file, line);
  }
  return success;
}

} // namespace detail
} // namespace MaximInterfaceTest

/// Reports a message with the specified severity.
#define MaximInterfaceTest_MESSAGE(severity, message)                          \
  ::MaximInterfaceTest::detail::reportMessage(severity, message, __FILE__,     \
                                              __LINE__)

/// Reports an info message.
#define MaximInterfaceTest_INFO(message)                                       \
  MaximInterfaceTest_MESSAGE(::MaximInterfaceTest::info, message)

/// Reports a warning message.
#define MaximInterfaceTest_WARNING(message)                                    \
  MaximInterfaceTest_MESSAGE(::MaximInterfaceTest::warning, message)

/// Reports an error message.
#define MaximInterfaceTest_ERROR(message)                                      \
  MaximInterfaceTest_MESSAGE(::MaximInterfaceTest::error, message)

/// Reports a fatal message.
#define MaximInterfaceTest_FATAL(message)                                      \
  MaximInterfaceTest_MESSAGE(::MaximInterfaceTest::fatal, message)

/// @brief
/// Asserts that the value expression is true. Otherwise, reports a message
/// with the specified severity.
/// @returns True if the assertion held.
#define MaximInterfaceTest_ASSERT_TRUE(severity, value)                        \
  ::MaximInterfaceTest::detail::compare_true(severity, #value, value,          \
                                             __FILE__, __LINE__)

/// @brief
/// Asserts that the value expression is true. Otherwise, reports an info
/// message and executes the the following block or statement.
#define MaximInterfaceTest_ASSERT_TRUE_ELSE(value)                             \
  if (MaximInterfaceTest_ASSERT_TRUE(::MaximInterfaceTest::info, value))       \
    ;                                                                          \
  else

/// @brief
/// Asserts that the value expression is false. Otherwise, reports a message
/// with the specified severity.
/// @returns True if the assertion held.
#define MaximInterfaceTest_ASSERT_FALSE(severity, value)                       \
  ::MaximInterfaceTest::detail::compare_false(severity, "!(" #value ")",       \
                                              value, __FILE__, __LINE__)

/// @brief
/// Asserts that the value expression is false. Otherwise, reports an info
/// message and executes the the following block or statement.
#define MaximInterfaceTest_ASSERT_FALSE_ELSE(value)                            \
  if (MaximInterfaceTest_ASSERT_FALSE(::MaximInterfaceTest::info, value))      \
    ;                                                                          \
  else

/// @brief
/// Asserts that the LHS expression is equal to the RHS expression. Otherwise,
/// reports a message with the specified severity.
/// @returns True if the assertion held.
#define MaximInterfaceTest_ASSERT_EQUAL(severity, lhs, rhs)                    \
  ::MaximInterfaceTest::detail::compare_equal(                                 \
      severity, "(" #lhs ") == (" #rhs ")", lhs, rhs, __FILE__, __LINE__)

/// @brief
/// Asserts that the LHS expression is equal to the RHS expression. Otherwise,
/// reports an info message and executes the following block or statement.
#define MaximInterfaceTest_ASSERT_EQUAL_ELSE(lhs, rhs)                         \
  if (MaximInterfaceTest_ASSERT_EQUAL(::MaximInterfaceTest::info, lhs, rhs))   \
    ;                                                                          \
  else

/// @brief
/// Asserts that the LHS expression is not equal to the RHS expression.
/// Otherwise, reports a message with the specified severity.
/// @returns True if the assertion held.
#define MaximInterfaceTest_ASSERT_NOT_EQUAL(severity, lhs, rhs)                \
  ::MaximInterfaceTest::detail::compare_not_equal(                             \
      severity, "(" #lhs ") != (" #rhs ")", lhs, rhs, __FILE__, __LINE__)

/// @brief
/// Asserts that the LHS expression is not equal to the RHS expression.
/// Otherwise, reports an info message and executes the following block or
/// statement.
#define MaximInterfaceTest_ASSERT_NOT_EQUAL_ELSE(lhs, rhs)                     \
  if (MaximInterfaceTest_ASSERT_NOT_EQUAL(::MaximInterfaceTest::info, lhs,     \
                                          rhs))                                \
    ;                                                                          \
  else

/// @brief
/// Asserts that the LHS expression is less than the RHS expression. Otherwise,
/// reports a message with the specified severity.
/// @returns True if the assertion held.
#define MaximInterfaceTest_ASSERT_LESS(severity, lhs, rhs)                     \
  ::MaximInterfaceTest::detail::compare_less(                                  \
      severity, "(" #lhs ") < (" #rhs ")", lhs, rhs, __FILE__, __LINE__)

/// @brief
/// Asserts that the LHS expression is less than the RHS expression. Otherwise,
/// reports an info message and executes the following block or statement.
#define MaximInterfaceTest_ASSERT_LESS_ELSE(lhs, rhs)                          \
  if (MaximInterfaceTest_ASSERT_LESS(::MaximInterfaceTest::info, lhs, rhs))    \
    ;                                                                          \
  else

/// @brief
/// Asserts that the LHS expression is less than or equal to the RHS expression.
/// Otherwise, reports a message with the specified severity.
/// @returns True if the assertion held.
#define MaximInterfaceTest_ASSERT_LESS_EQUAL(severity, lhs, rhs)               \
  ::MaximInterfaceTest::detail::compare_less_equal(                            \
      severity, "(" #lhs ") <= (" #rhs ")", lhs, rhs, __FILE__, __LINE__)

/// @brief
/// Asserts that the LHS expression is less than or equal to the RHS expression.
/// Otherwise, reports an info message and executes the following block or
/// statement.
#define MaximInterfaceTest_ASSERT_LESS_EQUAL_ELSE(lhs, rhs)                    \
  if (MaximInterfaceTest_ASSERT_LESS_EQUAL(::MaximInterfaceTest::info, lhs,    \
                                           rhs))                               \
    ;                                                                          \
  else

/// @brief
/// Asserts that the LHS expression is greater than the RHS expression.
/// Otherwise, reports a message with the specified severity.
/// @returns True if the assertion held.
#define MaximInterfaceTest_ASSERT_GREATER(severity, lhs, rhs)                  \
  ::MaximInterfaceTest::detail::compare_greater(                               \
      severity, "(" #lhs ") > (" #rhs ")", lhs, rhs, __FILE__, __LINE__)

/// @brief
/// Asserts that the LHS expression is greater than the RHS expression.
/// Otherwise, reports an info message and executes the following block or
/// statement.
#define MaximInterfaceTest_ASSERT_GREATER_ELSE(lhs, rhs)                       \
  if (MaximInterfaceTest_ASSERT_GREATER(::MaximInterfaceTest::info, lhs, rhs)) \
    ;                                                                          \
  else

/// @brief
/// Asserts that the LHS expression is greater than or equal to the RHS
/// expression. Otherwise, reports a message with the specified severity.
/// @returns True if the assertion held.
#define MaximInterfaceTest_ASSERT_GREATER_EQUAL(severity, lhs, rhs)            \
  ::MaximInterfaceTest::detail::compare_greater_equal(                         \
      severity, "(" #lhs ") >= (" #rhs ")", lhs, rhs, __FILE__, __LINE__)

/// @brief
/// Asserts that the LHS expression is greater than or equal to the RHS
/// expression. Otherwise, reports an info message and executes the following
/// block or statement.
#define MaximInterfaceTest_ASSERT_GREATER_EQUAL_ELSE(lhs, rhs)                 \
  if (MaximInterfaceTest_ASSERT_GREATER_EQUAL(::MaximInterfaceTest::info, lhs, \
                                              rhs))                            \
    ;                                                                          \
  else

#endif
