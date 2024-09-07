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

#include "bedrock/world/level/block_pos.h"
#include "bedrock/world/level/block_source.h"
#include "endstone/block/block.h"

namespace endstone::detail {
class EndstoneBlock : public Block {
public:
    EndstoneBlock(BlockSource &block_source, BlockPos block_pos);
    [[nodiscard]] bool isValid() const override;
    [[nodiscard]] std::string getType() const override;
    void setType(std::string type) override;
    void setType(std::string type, bool apply_physics) override;
    [[nodiscard]] std::shared_ptr<BlockData> getData() const override;
    void setData(std::shared_ptr<BlockData> data) override;
    void setData(std::shared_ptr<BlockData> data, bool apply_physics) override;
    std::unique_ptr<Block> getRelative(int offset_x, int offset_y, int offset_z) override;
    std::unique_ptr<Block> getRelative(BlockFace face) override;
    std::unique_ptr<Block> getRelative(BlockFace face, int distance) override;
    [[nodiscard]] Dimension &getDimension() const override;
    [[nodiscard]] int getX() const override;
    [[nodiscard]] int getY() const override;
    [[nodiscard]] int getZ() const override;
    [[nodiscard]] Location getLocation() const override;

    [[nodiscard]] BlockPos getPosition() const;
    [[nodiscard]] ::Block &getMinecraftBlock() const;

    static std::unique_ptr<EndstoneBlock> at(BlockSource &block_source, BlockPos block_pos);

private:
    [[nodiscard]] bool checkState() const;
    BlockSource &block_source_;
    BlockPos block_pos_;
};
}  // namespace endstone::detail
