/**
 *
 * @file DequeBuffer class Unit Tests
 *
 * This is part of the DUNE DAQ Application Framework, copyright 2020.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */

#include "app-framework/Buffers/DequeBuffer.hh"

#define BOOST_TEST_MODULE DequeBuffer_test
#include <boost/test/included/unit_test.hpp>

#include <chrono>

// For a first look at the code, you may want to skip past the
// contents of the unnamed namespace and move ahead to the actual test
// cases

namespace {
appframework::DequeBuffer<int> buffer;

constexpr int max_testable_capacity = 1000000000;
constexpr double fractional_timeout_tolerance = 0.1;

// See
// https://www.boost.org/doc/libs/1_73_0/libs/test/doc/html/boost_test/tests_organization/enabling.html
// to better understand CapacityChecker.

struct CapacityChecker {

  boost::test_tools::assertion_result
  operator()(boost::unit_test::test_unit_id) {
    if (buffer.capacity() <= max_testable_capacity) {
      return true;
    } else {
      boost::test_tools::assertion_result result(false);
      result.message() << "Capacity of DequeBuffer (" << buffer.capacity()
                       << ") larger than max value this suite tests ("
                       << max_testable_capacity << ")";
      return result;
    }
  }
};

} // namespace ""

// This test case should run first. Make sure all other test cases depend on this. 

BOOST_AUTO_TEST_CASE(sanity_checks) {

  BOOST_REQUIRE_EQUAL(buffer.size(), 0);
  BOOST_REQUIRE(buffer.empty());

  auto starttime = std::chrono::steady_clock::now();
  buffer.push(999);
  auto push_time_in_us = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - starttime).count();

  if ( push_time_in_us > buffer.get_push_timeout() * (1 - fractional_timeout_tolerance)) {
    BOOST_TEST_REQUIRE(false, "Test failure: pushing element onto empty buffer resulted in a timeout (" << push_time_in_us << " us taken)");
  }

  BOOST_REQUIRE_EQUAL(buffer.size(), 1);

  starttime = std::chrono::steady_clock::now();
  auto popped_value = buffer.pop();
  auto pop_time_in_us = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - starttime).count();

  if ( pop_time_in_us > buffer.get_pop_timeout() * (1 - fractional_timeout_tolerance)) {
    BOOST_TEST_REQUIRE(false, "Test failure: popping element off buffer resulted in a timeout (" << pop_time_in_us << "us taken");
  }

  BOOST_REQUIRE_EQUAL(popped_value, 999);
}

BOOST_AUTO_TEST_CASE(empty_checks,
                     *boost::unit_test::depends_on("sanity_checks")) {

  try {
    while (!buffer.empty()) {
      buffer.pop();
    }
  } catch (const std::runtime_error &err) {
    BOOST_WARN_MESSAGE(true, err.what());
    BOOST_TEST(false, "Exception thrown in call to DequeBuffer::pop(); unable "
                      "to empty the buffer");
  }

  BOOST_REQUIRE(buffer.empty());

  // pop off of an empty buffer

  const size_t pop_timeout_in_milliseconds = 100;
  BOOST_TEST_MESSAGE("Setting the pop timeout on the empty buffer to " << pop_timeout_in_milliseconds << " ms before attempting expected-to-fail pop");
  buffer.set_pop_timeout(pop_timeout_in_milliseconds);

  auto starttime = std::chrono::steady_clock::now();
  BOOST_CHECK_THROW(buffer.pop(), std::runtime_error);
  auto pop_duration = std::chrono::steady_clock::now() - starttime;

  const double fraction_of_timeout_used =
      std::chrono::duration_cast<std::chrono::milliseconds>(pop_duration)
          .count() /
      static_cast<double>(pop_timeout_in_milliseconds);

  BOOST_CHECK_GT(fraction_of_timeout_used, 1 - fractional_timeout_tolerance);
  BOOST_CHECK_LT(fraction_of_timeout_used, 1 + fractional_timeout_tolerance);
}

BOOST_AUTO_TEST_CASE(capacity_checks,
                     *boost::unit_test::precondition(CapacityChecker()) *
                         boost::unit_test::depends_on("sanity_checks")) {

  // TODO, May-6-2020, John Freeman (jcfree@fnal.gov)
  // In the next week, figure out if it makes sense beyond this test to create
  // an insert() function which takes iterators

  try {
    while (buffer.size() < buffer.capacity()) {
      buffer.push(-1);
    }
  } catch (const std::runtime_error &err) {
    BOOST_WARN_MESSAGE(true, err.what());
    BOOST_TEST(false, "Exception thrown in call to DequeBuffer::push(); unable "
                      "to fill the buffer to its alleged capacity of "
                          << buffer.capacity() << " elements");
  }

  BOOST_REQUIRE(buffer.full());

  // Push onto an already-full buffer

  const size_t push_timeout_in_milliseconds = 1000;
  BOOST_TEST_MESSAGE("Setting the push timeout on the at-capacity buffer to " << push_timeout_in_milliseconds << " ms before attempting expected-to-fail push");
  buffer.set_push_timeout(push_timeout_in_milliseconds);

  auto starttime = std::chrono::steady_clock::now();
  try {
    buffer.push(-1);
  } catch (...) { // NOLINT (don't care whether or not a failed push results in an exception)
  }
  auto push_duration = std::chrono::steady_clock::now() - starttime;

  // Trying to push an element onto a buffer at capacity shouldn't change its
  // size

  BOOST_CHECK_EQUAL(buffer.size(), buffer.capacity());

  const double fraction_of_timeout_used =
      std::chrono::duration_cast<std::chrono::milliseconds>(push_duration)
          .count() /
      static_cast<double>(push_timeout_in_milliseconds);

  BOOST_CHECK_GT(fraction_of_timeout_used, 1 - fractional_timeout_tolerance);
  BOOST_CHECK_LT(fraction_of_timeout_used, 1 + fractional_timeout_tolerance);
}


