// This file is made available under Elastic License 2.0.
// This file is based on code available under the Apache license here:
//   https://github.com/apache/incubator-doris/blob/master/be/src/exprs/utility_functions.cpp

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

#include "exprs/utility_functions.h"

#include "exprs/anyval_util.h"
#include "exprs/expr.h"
#include "gen_cpp/version.h"
#include "runtime/runtime_state.h"
#include "runtime/tuple_row.h"
#include "util/debug_util.h"
#include "util/monotime.h"

namespace starrocks {

// Our new vectorized query executor is more powerful and stable than old query executor,
// The executor query executor related codes could be deleted safely.
// TODO: Remove old query executor related codes before 2021-09-30

void UtilityFunctions::init() {}

StringVal UtilityFunctions::version(FunctionContext* ctx) {
    return AnyValUtil::from_string_temp(ctx, "5.1.0");
}

StringVal UtilityFunctions::current_version(FunctionContext* ctx) {
    static std::string version = std::string(STARROCKS_VERSION) + " " + STARROCKS_COMMIT_HASH;
    return AnyValUtil::from_string_temp(ctx, version);
}

BooleanVal UtilityFunctions::sleep(FunctionContext* ctx, const IntVal& seconds) {
    if (seconds.is_null) {
        return BooleanVal::null();
    }
    SleepFor(MonoDelta::FromSeconds(seconds.val));
    return BooleanVal(true);
}

StringVal UtilityFunctions::last_query_id(FunctionContext* ctx) {
    const std::string& id = ctx->impl()->state()->last_query_id();
    if (!id.empty()) {
        return AnyValUtil::from_string_temp(ctx, id);
    } else {
        return StringVal::null();
    }
}

} // namespace starrocks
