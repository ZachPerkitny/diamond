/*  Diamond - Embedded NoSQL Database
**  Copyright (C) 2020  Zach Perkitny
**
**  This program is free software: you can redistribute it and/or modify
**  it under the terms of the GNU General Public License as published by
**  the Free Software Foundation, either version 3 of the License, or
**  (at your option) any later version.
**
**  This program is distributed in the hope that it will be useful,
**  but WITHOUT ANY WARRANTY; without even the implied warranty of
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**  GNU General Public License for more details.
**
**  You should have received a copy of the GNU General Public License
**  along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include "gmock/gmock.h"

#include "diamond/eviction_policy.h"

namespace {

    class MockEvictionPolicy : public diamond::EvictionPolicy {
    public:
        MOCK_METHOD(void, update, (diamond::Page::ID id), (override));
        MOCK_METHOD(void, add, (diamond::Page::ID id), (override));
        MOCK_METHOD(diamond::Page::ID, next, (diamond::Page::ID id), (override));
        MOCK_METHOD(void, remove, (diamond::Page::ID id), (override));
    };

    class MockEvictionPolicyFactory : public diamond::EvictionPolicyFactory {
    public:
        MOCK_METHOD(std::shared_ptr<diamond::EvictionPolicy>, create, (), (const override));
    };

} // namespace 
