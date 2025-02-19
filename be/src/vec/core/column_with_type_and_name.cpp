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
// This file is copied from
// https://github.com/ClickHouse/ClickHouse/blob/master/src/Core/ColumnWithTypeAndName.cpp
// and modified by Doris

#include <ostream>
#include <sstream>

#include "vec/core/columns_with_type_and_name.h"

namespace doris::vectorized {

ColumnWithTypeAndName ColumnWithTypeAndName::clone_empty() const {
    ColumnWithTypeAndName res;

    res.name = name;
    res.type = type;
    if (column) {
        res.column = column->clone_empty();
    } else {
        res.column = nullptr;
    }

    return res;
}

bool ColumnWithTypeAndName::operator==(const ColumnWithTypeAndName& other) const {
    return name == other.name &&
           ((!type && !other.type) || (type && other.type && type->equals(*other.type))) &&
           ((!column && !other.column) ||
            (column && other.column && column->get_name() == other.column->get_name()));
}

void ColumnWithTypeAndName::dump_structure(std::ostream& out) const {
    if (name.empty()) {
        out << "[Anonymous Column]";
    } else {
        out << name;
    }

    if (type)
        out << " " << type->get_name();
    else
        out << " nullptr";

    if (column)
        out << ' ' << column->dump_structure();
    else
        out << " nullptr";
}

String ColumnWithTypeAndName::dump_structure() const {
    std::stringstream out;
    dump_structure(out);
    return out.str();
}

std::string ColumnWithTypeAndName::to_string(size_t row_num) const {
    return type->to_string(*column->convert_to_full_column_if_const().get(), row_num);
}

void ColumnWithTypeAndName::to_pb_column_meta(PColumnMeta* col_meta) const {
    col_meta->set_name(name);
    type->to_pb_column_meta(col_meta);
}

} // namespace doris::vectorized
