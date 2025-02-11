// This file is made available under Elastic License 2.0.
// This file is based on code available under the Apache license here:
//   https://github.com/apache/incubator-doris/blob/master/be/src/olap/decimal12.h

// Licensed to the Apache Software Foundation (ASF) under one
// or more contributor license agreements.  See the NOTICE file
// distributed with this work for additional information
// regarding copyright ownership.  The ASF licenses this file
// to you under the Apache License, Version 2.0 (the
// "License"); you may not use this file except in compliance
// with the License.  You may obtain a copy of the License at
//
//   http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing,
// software distributed under the License is distributed on an
// "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
// KIND, either express or implied.  See the License for the
// specific language governing permissions and limitations
// under the License.

#pragma once

#include <cstdint>
#include <iostream>
#include <string>

#include "storage/olap_define.h"

namespace starrocks {

// the sign of integer must be same as fraction
struct decimal12_t {
    decimal12_t() : integer(0), fraction(0) {}
    decimal12_t(int64_t int_part, int32_t frac_part) {
        integer = int_part;
        fraction = frac_part;
    }

    decimal12_t(const decimal12_t& value) = default;
    decimal12_t& operator=(const decimal12_t& value) = default;

    decimal12_t& operator+=(const decimal12_t& value) {
        fraction += value.fraction;
        integer += value.integer;

        if (fraction >= FRAC_RATIO) {
            integer += 1;
            fraction -= FRAC_RATIO;
        } else if (fraction <= -FRAC_RATIO) {
            integer -= 1;
            fraction += FRAC_RATIO;
        }

        // if sign of fraction is different from integer
        if ((fraction != 0) && (integer != 0) && (fraction ^ integer) < 0) {
            bool sign = integer < 0;
            integer += (sign ? 1 : -1);
            fraction += (sign ? -FRAC_RATIO : FRAC_RATIO);
        }

        return *this;
    }

    bool operator<(const decimal12_t& value) const { return cmp(value) < 0; }

    bool operator<=(const decimal12_t& value) const { return cmp(value) <= 0; }

    bool operator>(const decimal12_t& value) const { return cmp(value) > 0; }

    bool operator>=(const decimal12_t& value) const { return cmp(value) >= 0; }

    bool operator==(const decimal12_t& value) const { return cmp(value) == 0; }

    bool operator!=(const decimal12_t& value) const { return cmp(value) != 0; }

    int32_t cmp(const decimal12_t& other) const {
        if (integer > other.integer) {
            return 1;
        } else if (integer == other.integer) {
            if (fraction > other.fraction) {
                return 1;
            } else if (fraction == other.fraction) {
                return 0;
            }
        }

        return -1;
    }

    std::string to_string() const {
        char buf[128] = {'\0'};

        if (integer < 0 || fraction < 0) {
            snprintf(buf, sizeof(buf), "-%lu.%09u", std::abs(integer), std::abs(fraction));
        } else {
            snprintf(buf, sizeof(buf), "%lu.%09u", std::abs(integer), std::abs(fraction));
        }

        return std::string(buf);
    }

    OLAPStatus from_string(const std::string& str);

    static const int32_t FRAC_RATIO = 1000000000;
    static const int32_t MAX_INT_DIGITS_NUM = 18;
    static const int32_t MAX_FRAC_DIGITS_NUM = 9;

    int64_t integer;
    int32_t fraction;
} __attribute__((packed));

inline std::ostream& operator<<(std::ostream& os, const decimal12_t& val) {
    os << val.to_string();
    return os;
}

} // namespace starrocks
