/*  Diamond - Embedded Relational Database
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

#ifndef _DIAMOND_FILE_STORAGE_H
#define _DIAMOND_FILE_STORAGE_H

#include <fstream>

#include "diamond/storage.h"

namespace diamond {

    class FileStorage final : public Storage {
    public:
        FileStorage(const std::string& file_name);

        void write(const char* buffer, size_t n) override;
        void write(const Buffer& buffer) override;

        void read(char* buffer, size_t n) override;
        void read(Buffer& buffer, size_t n) override;

        void seek(size_t n) override;

        size_t size() override;

    private:
        std::fstream _fs;
    };

} // namespace diamond

#endif // _DIAMOND_FILE_STORAGE_H
