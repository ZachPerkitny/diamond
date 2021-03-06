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

#include <cstring>

#include "diamond/buffer.h"
#include "diamond/storage.h"

namespace diamond {

    Buffer::Buffer()
        : _size(0),
        _buffer(nullptr) {}

    Buffer::Buffer(size_t size)
        : _size(size),
        _buffer(new char[size]) {}

    Buffer::Buffer(const char* buffer)
        : Buffer(buffer, strlen(buffer)) {}

    Buffer::Buffer(const char* buffer, size_t size)
            : _size(size),
            _buffer(new char[size]) {
        std::memcpy(_buffer, buffer, _size);
    }

    Buffer::Buffer(const std::string& str)
        : Buffer(str.c_str(), str.size()) {}

    Buffer::Buffer(Storage& storage, size_t size, uint64_t offset)
            : _size(size),
            _buffer(new char[size]) {
        storage.read(_buffer, size, offset);
    }

    Buffer::Buffer(const Buffer& other)
            : _size(other._size),
            _buffer(new char[other._size]){
        std::memcpy(_buffer, other._buffer, _size);
    }

    Buffer::Buffer(Buffer&& other)
            : _size(other._size),
            _buffer(other._buffer)  {
        other._size = 0;
        other._buffer = nullptr;
    }

    Buffer::~Buffer() {
        if (_buffer) delete[] _buffer;
    }

    size_t Buffer::size() const {
        return _size;
    }

    void Buffer::resize(size_t s) {
        char* new_buffer = new char[s];
        if (_buffer) {
            std::memcpy(new_buffer, _buffer, (_size > s) ? s : _size);
        }
        _buffer = new_buffer;
        _size = s;
    }

    char* Buffer::buffer() {
        return _buffer;
    }

    const char* Buffer::buffer() const {
        return _buffer;
    }

    void Buffer::write_to_storage(Storage& storage, uint64_t offset) const {
        storage.write(_buffer, _size, offset);
    }

    std::string Buffer::to_str() const {
        return std::string(_buffer, _size);
    }

    char Buffer::operator[](size_t i) const {
        return _buffer[i];
    }

    Buffer& Buffer::operator=(const Buffer& other) {
        if (this != &other) {
            if (_buffer) delete[] _buffer;
            _size = other._size;
            _buffer = new char[_size];
            std::memcpy(_buffer, other._buffer, _size);
        }

        return *this;
    }

    Buffer& Buffer::operator=(Buffer&& other) {
        if (this != &other) {
            if (_buffer) delete[] _buffer;
            _size = other._size;
            _buffer = other._buffer;
            other._size = 0;
            other._buffer = nullptr;
        }

        return *this;
    }

    bool Buffer::operator==(const Buffer& other) const {
        if (_size != other._size) return false;
        return std::memcmp(_buffer, other._buffer, _size) == 0;
    }

    bool Buffer::operator!=(const Buffer& other) const {
        return !(*this == other);
    }

    std::ostream& operator<<(std::ostream& os, const Buffer& buffer) {
        os << buffer.to_str();
        return os;
    }

    BufferReader::BufferReader(const Buffer& buffer, endian::Endianness endianness)
        : _ptr(0),
        _buffer(buffer),
        _endianness(endianness) {}

    size_t BufferReader::bytes_read() const {
        return _ptr;
    }

    size_t BufferReader::bytes_remaining() const {
        return _buffer.size() - _ptr;
    }

    void BufferReader::read(Buffer& buffer) {
        read(buffer.buffer(), buffer.size());
    }

    void BufferReader::read(void* val, size_t size) {
        std::memcpy(val, _buffer.buffer() + _ptr, size);
        _ptr += size;
    }

    BufferWriter::BufferWriter(Buffer& buffer, endian::Endianness endianness)
        : _ptr(0),
        _buffer(buffer),
        _endianness(endianness) {}

    size_t BufferWriter::bytes_written() const {
        return _ptr;
    }

    size_t BufferWriter::bytes_remaining() const {
        return _buffer.size() - _ptr;
    }

    void BufferWriter::write(const Buffer& buffer) {
        write(buffer.buffer(), buffer.size());
    }

    void BufferWriter::write(const std::string& str) {
        write(str.c_str(), str.size());
    }

    void BufferWriter::write(const void* val, size_t size) {
        size_t buffer_size = _buffer.size();
        if (!buffer_size) buffer_size = 1;
        while (_ptr + size > buffer_size) {
            buffer_size *= 2;
        }
        _buffer.resize(buffer_size);
        std::memcpy(_buffer.buffer() + _ptr, val, size);
        _ptr += size;
    }

} // namespace diamond
