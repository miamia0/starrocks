// This file is made available under Elastic License 2.0.
// This file is based on code available under the Apache license here:
//   https://github.com/apache/incubator-doris/blob/master/fe/fe-core/src/main/java/org/apache/doris/analysis/TupleId.java

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

package com.starrocks.analysis;

import com.starrocks.common.Id;
import com.starrocks.common.IdGenerator;

/**
 * Tuple identifier unique within a single query.
 */
// Our new cost based query optimizer is more powerful and stable than old query optimizer,
// The old query optimizer related codes could be deleted safely.
// TODO: Remove old query optimizer related codes before 2021-09-30
@Deprecated
public class TupleId extends Id<TupleId> {
    public TupleId(int id) {
        super(id);
    }

    public static IdGenerator<TupleId> createGenerator() {
        return new IdGenerator<TupleId>() {
            @Override
            public TupleId getNextId() {
                return new TupleId(nextId_++);
            }

            @Override
            public TupleId getMaxId() {
                return new TupleId(nextId_ - 1);
            }
        };
    }
}
