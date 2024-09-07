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

#include "bedrock/world/attribute/attribute_instance.h"

#include "bedrock/world/attribute/base_attribute_map.h"

float AttributeInstance::getCurrentValue() const
{
    return current_.default_value;
}

void AttributeInstance::setCurrentValue(float value)
{
    current_.default_value = value;
    _setDirty();
}

Attribute *AttributeInstance::getAttribute() const
{
    return attribute_;
}

void AttributeInstance::_setDirty()
{
    if (attribute_map_) {
        attribute_map_->onAttributeModified(*this);
    }
}
