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

#include "bedrock/network/server_network_handler.h"

#include <variant>

#include <entt/entt.hpp>
#include <magic_enum/magic_enum.hpp>

#include "endstone/detail/hook.h"
#include "endstone/detail/server.h"
#include "endstone/event/player/player_chat_event.h"
#include "endstone/event/player/player_kick_event.h"
#include "endstone/event/player/player_login_event.h"

using endstone::detail::EndstoneServer;

void ServerNetworkHandler::disconnectClient(const NetworkIdentifier &network_id, SubClientId sub_client_id,
                                            Connection::DisconnectFailReason reason, const std::string &message,
                                            std::optional<std::string> filtered_message, bool skip_message)
{
    std::string msg = message;
    if (const auto *server_player = _getServerPlayer(network_id, sub_client_id)) {
        const auto &server = entt::locator<EndstoneServer>::value();
        endstone::PlayerKickEvent e{server_player->getEndstonePlayer(), msg};
        server.getPluginManager().callEvent(e);

        if (e.isCancelled()) {
            return;
        }
        msg = e.getReason();
    }
    ENDSTONE_HOOK_CALL_ORIGINAL(&ServerNetworkHandler::disconnectClient, this, network_id, sub_client_id, reason, msg,
                                std::move(filtered_message), skip_message);
}

void ServerNetworkHandler::updateServerAnnouncement()
{
    ENDSTONE_HOOK_CALL_ORIGINAL(&ServerNetworkHandler::updateServerAnnouncement, this);
}

bool ServerNetworkHandler::trytLoadPlayer(ServerPlayer &server_player, const ConnectionRequest &connection_request)
{
    const auto new_player =
        ENDSTONE_HOOK_CALL_ORIGINAL(&ServerNetworkHandler::trytLoadPlayer, this, server_player, connection_request);
    const auto &server = entt::locator<EndstoneServer>::value();
    auto &endstone_player = server_player.getEndstonePlayer();
    endstone_player.initFromConnectionRequest(&connection_request);

    endstone::PlayerLoginEvent e{endstone_player};
    server.getPluginManager().callEvent(e);

    if (e.isCancelled()) {
        endstone_player.kick(e.getKickMessage());
    }
    return new_player;
}

ServerPlayer &ServerNetworkHandler::_createNewPlayer(const NetworkIdentifier &network_id,
                                                     const SubClientConnectionRequest &sub_client_connection_request,
                                                     SubClientId sub_client_id)
{
    auto &server_player = ENDSTONE_HOOK_CALL_ORIGINAL(&ServerNetworkHandler::_createNewPlayer, this, network_id,
                                                      sub_client_connection_request, sub_client_id);
    auto &server = entt::locator<EndstoneServer>::value();
    auto &endstone_player = server_player.getEndstonePlayer();
    endstone_player.initFromConnectionRequest(&sub_client_connection_request);

    endstone::PlayerLoginEvent e{endstone_player};
    server.getPluginManager().callEvent(e);

    if (e.isCancelled()) {
        endstone_player.kick(e.getKickMessage());
    }
    return server_player;
}

void ServerNetworkHandler::_displayGameMessage(const Player &player, ChatEvent &event)
{
    auto &server = entt::locator<EndstoneServer>::value();
    endstone::PlayerChatEvent e{player.getEndstonePlayer(), event.message};
    server.getPluginManager().callEvent(e);

    if (e.isCancelled()) {
        return;
    }

    event.message = std::move(e.getMessage());
    server.getLogger().info("<{}> {}", e.getPlayer().getName(), e.getMessage());

    ENDSTONE_HOOK_CALL_ORIGINAL(&ServerNetworkHandler::_displayGameMessage, this, player, event);
}

const Bedrock::NonOwnerPointer<ILevel> &ServerNetworkHandler::getLevel() const
{
    return level_;
}
