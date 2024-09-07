// Copyright (c) 2024, The Endstone Project. (https://endstone.dev) All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#pragma once

#include <cmath>

#include <fmt/format.h>

#include "bedrock/core/math/vec3.h"

class BlockPos {
public:
    int x;
    int y;
    int z;

    BlockPos(int x, int y, int z) : x(x), y(y), z(z){};
    BlockPos(double x, double y, double z) : x(std::floor(x)), y(std::floor(y)), z(std::floor(z)){};
    explicit BlockPos(const Vec3 &vec)
        : x(static_cast<int>(std::floorf(vec.x))), y(static_cast<int>(std::floorf(vec.y))),
          z(static_cast<int>(std::floorf(vec.z))){};

    static const BlockPos ZERO;
};

inline const BlockPos BlockPos::ZERO{0, 0, 0};

namespace fmt {
template <>
struct formatter<BlockPos> : formatter<string_view> {
    using Type = BlockPos;

    template <typename FormatContext>
    auto format(const Type &val, FormatContext &ctx) const -> format_context::iterator
    {
        return format_to(ctx.out(), "BlockPos(x={}, y={}, z={})", val.x, val.y, val.z);
    }
};
}  // namespace fmt
