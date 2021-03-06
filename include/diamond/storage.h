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

#ifndef _DIAMOND_STORAGE_H
#define _DIAMOND_STORAGE_H

#include <boost/thread.hpp>
#include <boost/utility.hpp>

#include "diamond/buffer.h"

namespace diamond {

    class Storage : boost::noncopyable {
    public:
        Storage() = default;

        void write(const char* buffer, size_t n, uint64_t offset);
        void read(char* buffer, size_t n, uint64_t offset);
        uint64_t size();

    protected:
        virtual void write_impl(const char* buffer, size_t n) = 0;
        virtual void read_impl(char* buffer, size_t n) = 0;
        virtual void seek_impl(size_t n) = 0;
        virtual uint64_t size_impl() = 0;

    private:
        boost::mutex _mutex;
    };

} // namespace diamond

#endif // _DIAMOND_STORAGE_H
