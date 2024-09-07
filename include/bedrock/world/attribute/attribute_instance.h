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

#include <memory>
#include <vector>

#include "bedrock/forward.h"
#include "bedrock/world/attribute/attribute.h"

class BaseAttributeMap;

class AttributeInstance {
public:
    virtual ~AttributeInstance() = default;
    // virtual void tick() = 0;

    [[nodiscard]] float getCurrentValue() const;
    void setCurrentValue(float value);
    [[nodiscard]] Attribute *getAttribute() const;

private:
    friend class BaseAttributeMap;

    void _setDirty();

    BaseAttributeMap *attribute_map_;                      // +8
    Attribute *attribute_;                                 // +16
    std::vector<void *> modifier_list_;                    // +24 std::vector<AttributeModifier>
    std::vector<void *> temporal_buffs_;                   // +48 std::vector<TemporalAttributeBuff>
    std::vector<void *> listeners_;                        // +72 std::vector<AttributeInstanceHandle>
    std::shared_ptr<AttributeInstanceDelegate> delegate_;  // +96
    union {                                                //
        float default_values[3];                           //
        struct {                                           //
            float min_value;                               // (+112)
            float max_value;                               // (+116)
            float default_value;                           // (+120)
        };                                                 //
    } default_;                                            // +112
    union {                                                //
        float default_values[3];                           //
        struct {                                           //
            float min_value;                               // (+124)
            float max_value;                               // (+128)
            float default_value;                           // (+132)
        };                                                 //
    } current_;                                            // +124
};
