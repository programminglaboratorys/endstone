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

#include "bedrock/world/item/registry/item_registry_manager.h"

#include <entt/entt.hpp>

#include "endstone/detail/level/level.h"
#include "endstone/detail/server.h"

using endstone::detail::EndstoneLevel;
using endstone::detail::EndstoneServer;

ItemRegistryRef ItemRegistryManager::getItemRegistry()
{
    // NOTE: This is not the actual implementation in BDS
    auto &server = entt::locator<EndstoneServer>::value();
    if (!server.isPrimaryThread()) {
        server.getLogger().error("ItemRegistryManager::getItemRegistry must be called from the server thread.");
        return {};
    }
    auto *level = server.getLevel();
    if (!level) {
        server.getLogger().error("ItemRegistryManager::getItemRegistry is called before the level is loaded.");
        return {};
    }
    return static_cast<EndstoneLevel *>(level)->getHandle().getItemRegistry();
}
