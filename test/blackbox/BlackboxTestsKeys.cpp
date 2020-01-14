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

#include "BlackboxTests.hpp"

#include "PubSubReader.hpp"
#include "PubSubWriter.hpp"

TEST(KeyedTopic, RegistrationSuccess)
{
    PubSubWriter<KeyedHelloWorldType> writer(TEST_TOPIC_NAME);

    writer.key(true).init();

    ASSERT_TRUE(writer.isInitialized());

    auto data = default_keyedhelloworld_data_generator(2);

    for (auto data_sample : data)
    {
        // Register instances
        ASSERT_TRUE(writer.register_instance(data_sample));
    }
}

TEST(KeyedTopic, RegistrationFail)
{
    PubSubWriter<KeyedHelloWorldType> writer(TEST_TOPIC_NAME);

    writer.
    key(true).
    resource_limits_max_instances(1).
    init();

    ASSERT_TRUE(writer.isInitialized());

    auto data = default_keyedhelloworld_data_generator(2);

    // Register instances.
    ASSERT_TRUE(writer.register_instance(data.front()));
    ASSERT_FALSE(writer.register_instance(data.back()));
}
