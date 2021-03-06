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

#ifndef _DIAMOND_BUFFER_H
#define _DIAMOND_BUFFER_H

#include <ostream>
#include <string>
#include <type_traits>

#include "diamond/endian.h"

namespace diamond {

    class Storage;

    class Buffer {
    public:
        Buffer();
        Buffer(size_t size);
        Buffer(const char* buffer);
        Buffer(const char* buffer, size_t size);
        Buffer(const std::string& str);
        Buffer(Storage& storage, size_t size, uint64_t offset);

        Buffer(const Buffer& other);
        Buffer(Buffer&& other);

        ~Buffer();

        size_t size() const;
        void resize(size_t s);

        char* buffer();
        const char* buffer() const;

        void write_to_storage(Storage& storage, uint64_t offset) const;

        std::string to_str() const;

        char operator[](size_t i) const;

        Buffer& operator=(const Buffer& other);
        Buffer& operator=(Buffer&& other);

        bool operator==(const Buffer& other) const;
        bool operator!=(const Buffer& other) const;

        friend std::ostream& operator<<(std::ostream& os, const Buffer& buffer);

        struct EqualTo {
            bool operator()(const Buffer& lhs, const Buffer& rhs) const {
                return lhs == rhs;
            }
        };

        struct Hash {
            size_t operator()(const Buffer& buffer) const {
                size_t res = 0;
                const size_t prime = 31;
                for (size_t i = 0; i < buffer.size(); i++) {
                    res = buffer[i] + (res * prime);
                }
                return res;
            }
        };

    private:
        size_t _size;
        char* _buffer;
    };

    class BufferReader {
    public:
        BufferReader(
            const Buffer& buffer,
            endian::Endianness endianness = endian::Endianness::BIG);

        size_t bytes_read() const;
        size_t bytes_remaining() const;

        template <class T>
        typename std::enable_if<
            !std::is_enum<T>::value &&
            !std::is_arithmetic<T>::value &&
            std::is_trivially_copyable<T>::value,
            T
        >::type
        read();

        template<class T>
        typename std::enable_if<std::is_enum<T>::value, T>::type
        read();

        template<class T>
        typename std::enable_if<std::is_arithmetic<T>::value, T>::type
        read();

        void read(Buffer& buffer);
        void read(void* val, size_t size);

        template <class T>
        BufferReader& operator>>(T& val);

    private:
        size_t _ptr;
        const Buffer& _buffer;
        endian::Endianness _endianness;
    };

    class BufferWriter {
    public:
        BufferWriter(
            Buffer& buffer,
            endian::Endianness endianness = endian::Endianness::BIG);

        size_t bytes_written() const;
        size_t bytes_remaining() const;

        template <class T>
        typename std::enable_if<
            !std::is_enum<T>::value &&
            !std::is_arithmetic<T>::value &&
            std::is_trivially_copyable<T>::value,
            void
        >::type
        write(T val);

        template<class T>
        typename std::enable_if<std::is_enum<T>::value, void>::type
        write(T val);

        template<class T>
        typename std::enable_if<std::is_arithmetic<T>::value, void>::type
        write(T val);

        void write(const Buffer& buffer);
        void write(const std::string& str);
        void write(const void* val, size_t size);

        template <class T>
        BufferWriter& operator<<(T obj);

    private:
        size_t _ptr;
        Buffer& _buffer;
        endian::Endianness _endianness;
    };

    template <class T>
    typename std::enable_if<
        !std::is_enum<T>::value &&
        !std::is_arithmetic<T>::value &&
        std::is_trivially_copyable<T>::value,
        T
    >::type
    BufferReader::read() {
        T val;
        read(&val, sizeof(T));
        return val;
    }

    template<class T>
    typename std::enable_if<std::is_enum<T>::value, T>::type
    BufferReader::read() {
        return static_cast<T>(read<typename std::underlying_type<T>::type>());
    }

    template<class T>
    typename std::enable_if<std::is_arithmetic<T>::value, T>::type
    BufferReader::read() {
        T val;
        read(&val, sizeof(T));
        if (_endianness != endian::HOST_ORDER) {
            endian::swap_endianness(val);
        }
        return val;
    }

    template <class T>
    BufferReader& BufferReader::operator>>(T& val) {
        val = read<T>();
        return *this;
    }

    template <class T>
    typename std::enable_if<
        !std::is_enum<T>::value &&
        !std::is_arithmetic<T>::value &&
        std::is_trivially_copyable<T>::value,
        void
    >::type
    BufferWriter::write(T val) { 
        write(&val, sizeof(T));
    }

    template<class T>
    typename std::enable_if<std::is_enum<T>::value, void>::type
    BufferWriter::write(T val) {
        write(static_cast<typename std::underlying_type<T>::type>(val));
    }

    template<class T>
    typename std::enable_if<std::is_arithmetic<T>::value, void>::type
    BufferWriter::write(T val) {
        if (_endianness != endian::HOST_ORDER) {
            endian::swap_endianness(val);
        }
        return write(&val, sizeof(T));
    }

    template <class T>
    BufferWriter& BufferWriter::operator<<(T obj) {
        write(obj);
        return *this;
    }

} // namespace diamond

#endif // _DIAMOND_BUFFER_H
