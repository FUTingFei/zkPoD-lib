
// Copyright (c) 2010-2019 niXman (i dot nixman dog gmail dot com). All
// rights reserved.
//
// This file is part of YAS(https://github.com/niXman/yas) project.
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//
//
// Boost Software License - Version 1.0 - August 17th, 2003
//
// Permission is hereby granted, free of charge, to any person or organization
// obtaining a copy of the software and accompanying documentation covered by
// this license (the "Software") to use, reproduce, display, distribute,
// execute, and transmit the Software, and to prepare derivative works of the
// Software, and to permit third-parties to whom the Software is furnished to
// do so, all subject to the following:
//
// The copyright notices in the Software and this entire statement, including
// the above license grant, this restriction and the following disclaimer,
// must be included in all copies of the Software, in whole or in part, and
// all derivative works of the Software, unless such copies or derivative
// works are solely in the form of machine-executable object code generated by
// a source language processor.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
// SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
// FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.

#ifndef __yas__buffers_hpp
#define __yas__buffers_hpp

#include <yas/detail/config/config.hpp>

#include <cstring>
#include <memory>

namespace yas {

/***************************************************************************/

struct intrusive_buffer {
    intrusive_buffer(const char *data, std::size_t size)
        :data(data)
        ,size(size)
    {}

    intrusive_buffer(const intrusive_buffer& o)
        :data(o.data)
        ,size(o.size)
    {}

    const char *data;
    const std::size_t size;

private:
    intrusive_buffer();
};

/***************************************************************************/

struct shared_buffer {
    typedef std::shared_ptr<char> shared_array_type;

    explicit shared_buffer(std::size_t size = 0)
        :size(0)
    { resize(size); }

    shared_buffer(const void *ptr, std::size_t size)
        :size(0)
    { assign(ptr, size); }

    shared_buffer(shared_array_type buf, std::size_t size)
        :size(size)
    { if ( size ) { data = std::move(buf); } }

    shared_buffer(const shared_buffer& buf)
        :size(buf.size)
    { if ( size ) { data = buf.data; } }

    shared_buffer(shared_buffer&& buf)
        :data(std::move(buf.data))
        ,size(buf.size)
    { buf.size = 0; }

    shared_buffer& operator=(const shared_buffer&) = default;
    shared_buffer& operator=(shared_buffer&&) = default;

    void resize(std::size_t new_size) {
        if ( new_size > size ) {
            data.reset(new char[new_size], &deleter);
        }
        size = new_size;
    }

    void assign(const void *ptr, std::size_t ptr_size) {
        resize(ptr_size);
        if ( ptr_size ) {
            std::memcpy(data.get(), ptr, ptr_size);
        }
    }

    shared_array_type data;
    std::size_t size;

private:
    static void deleter(char *ptr) { delete []ptr; }
};

/***************************************************************************/

} // namespace yas

#endif // __yas__buffers_hpp
