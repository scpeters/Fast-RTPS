// Copyright 2019 Proyectos y Sistemas de Mantenimiento SL (eProsima).
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

/**
 * @file TypeSupport.hpp
 */

#ifndef _FASTDDS_TYPE_SUPPORT_HPP_
#define _FASTDDS_TYPE_SUPPORT_HPP_

#include <fastrtps/topic/TopicDataType.h>
#include <fastrtps/types/DynamicPubSubType.h>
#include <string>
#include <functional>
#include <memory>

namespace eprosima {
namespace fastdds {

class DomainParticipant;

/**
 * Class TypeSupport used to provide the DomainRTPSParticipant with the methods to serialize,
 * deserialize and get the key of a specific data type.
 * The user should created a class that inherits from this one,
 * where Serialize and deserialize methods MUST be implemented.
 * @ingroup FASTDDS_MODULE
 */
class TypeSupport : public std::shared_ptr<fastrtps::TopicDataType>
{
public:
    using Base = std::shared_ptr<fastrtps::TopicDataType>;
    using Base::operator->;
    using Base::operator*;
    using Base::operator bool;
    using Base::operator=;

    RTPS_DllAPI TypeSupport()
        : std::shared_ptr<fastrtps::TopicDataType>(nullptr)
    {}

    RTPS_DllAPI TypeSupport(
            const TypeSupport& type)
        : std::shared_ptr<fastrtps::TopicDataType>(type)
    {}

    RTPS_DllAPI TypeSupport(
            fastrtps::TopicDataType* ptr)
        : std::shared_ptr<fastrtps::TopicDataType>(ptr)
    {}

    RTPS_DllAPI TypeSupport(
            fastrtps::types::DynamicPubSubType ptr)
        : std::shared_ptr<fastrtps::TopicDataType>(std::make_shared<fastrtps::types::DynamicPubSubType>(ptr))
    {
        //auto aux_p = std::make_shared<fastrtps::TopicDataType>(std::move(pst));
        //*this = aux_p;
    }

    RTPS_DllAPI bool register_type(
        DomainParticipant* participant,
        std::string type_name) const;

    RTPS_DllAPI const std::string& get_type_name() const
    {
        return get()->m_topicDataTypeName;
    }

    RTPS_DllAPI bool serialize(
            void* data,
            fastrtps::rtps::SerializedPayload_t* payload)
    {
        return get()->serialize(data, payload);
    }

    RTPS_DllAPI bool deserialize(
            fastrtps::rtps::SerializedPayload_t* payload,
            void* data)
    {
        return get()->deserialize(payload, data);
    }

    RTPS_DllAPI std::function<uint32_t()> get_serialized_size_provider(
            void* data)
    {
        return get()->getSerializedSizeProvider(data);
    }

    RTPS_DllAPI void* create_data()
    {
        return get()->createData();
    }

    RTPS_DllAPI void delete_data(
            void * data)
    {
        return get()->deleteData(data);
    }

    RTPS_DllAPI bool getKey(
            void* data,
            fastrtps::rtps::InstanceHandle_t* i_handle,
            bool force_md5 = false)
    {
        return get()->getKey(data, i_handle, force_md5);
    }

    RTPS_DllAPI bool operator==(
            const TypeSupport& type_support)
    {
        return get()->m_typeSize == type_support->m_typeSize
               && get()->m_isGetKeyDefined == type_support->m_isGetKeyDefined
               && get()->m_topicDataTypeName == type_support->m_topicDataTypeName;
    }

    RTPS_DllAPI bool empty() const
    {
        return get() == nullptr;
    }
};

} /* namespace fastdds */
} /* namespace eprosima */

#endif /* _FASTDDS_TYPE_SUPPORT_HPP_ */
