/******************************************************************************
 *                       ____    _    _____                                   *
 *                      / ___|  / \  |  ___|    C++                           *
 *                     | |     / _ \ | |_       Actor                         *
 *                     | |___ / ___ \|  _|      Framework                     *
 *                      \____/_/   \_|_|                                      *
 *                                                                            *
 * Copyright 2011-2018 Dominik Charousset                                     *
 *                                                                            *
 * Distributed under the terms and conditions of the BSD 3-Clause License or  *
 * (at your option) under the terms and conditions of the Boost Software      *
 * License 1.0. See accompanying files LICENSE and LICENSE_ALTERNATIVE.       *
 *                                                                            *
 * If you did not receive a copy of the license files, see                    *
 * http://opensource.org/licenses/BSD-3-Clause and                            *
 * http://www.boost.org/LICENSE_1_0.txt.                                      *
 ******************************************************************************/

#pragma once

#include <array>
#include <cstdint>
#include <string>

#include "caf/byte_address.hpp"
#include "caf/detail/comparable.hpp"
#include "caf/fwd.hpp"

namespace caf {

class ipv6_address : public byte_address<ipv6_address>,
                     detail::comparable<ipv6_address, ipv4_address> {
public:
  // -- constants --------------------------------------------------------------

  static constexpr size_t num_bytes = 16;

  // -- member types -----------------------------------------------------------

  using super = byte_address<ipv6_address>;

  using array_type = std::array<uint8_t, num_bytes>;

  // -- constructors, destructors, and assignment operators --------------------

  ipv6_address();

  explicit ipv6_address(ipv4_address addr);

  ipv6_address(array_type bytes);

  // -- comparison -------------------------------------------------------------

  using super::compare;

  /// Returns a negative number if `*this < other`, zero if `*this == other`
  /// and a positive number if `*this > other`.
  int compare(ipv4_address other) const noexcept;

  // -- properties -------------------------------------------------------------

  /// Returns whether this address embeds an IPv4 address.
  bool embeds_v4() const noexcept;

  /// Returns an embedded IPv4 address.
  /// @pre `embeds_v4()`
  ipv4_address embedded_v4() const noexcept;

  /// Returns whether this is a loopback address.
  bool is_loopback() const noexcept;

  /// Returns the bytes of the IP address as array.
  inline array_type& bytes() noexcept {
    return bytes_;
  }

  /// Returns the bytes of the IP address as array.
  inline const array_type& bytes() const noexcept {
    return bytes_;
  }

  /// Alias for `bytes()`.
  inline array_type& data() noexcept {
    return bytes_;
  }

  /// Alias for `bytes()`.
  inline const array_type& data() const noexcept {
    return bytes_;
  }

  /// Returns the number of bytes of an IPv6 address.
  inline size_t size() const noexcept {
    return bytes_.size();
  }

  /// Returns whether this address contains only zeros, i.e., equals `::`.
  inline bool zero() const noexcept {
    return half_segments_[0] == 0 && half_segments_[1] == 0;
  }

  // -- inspection -------------------------------------------------------------

  template <class Inspector>
  friend typename Inspector::result_type inspect(Inspector& f,
                                                 ipv6_address& x) {
    return f(x.bytes_);
  }

  friend std::string to_string(ipv6_address x);

private:
  // -- member variables -------------------------------------------------------

  union {
    std::array<uint64_t, 2> half_segments_;
    std::array<uint32_t, 4> quad_segments_;
    std::array<uint16_t, 8> oct_segments_;
    array_type bytes_;
  };
};

error parse(string_view str, ipv6_address& dest);

} // namespace caf
