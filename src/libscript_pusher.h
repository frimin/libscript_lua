// +----------------------------------------------------------------------
// | Copyright (c) 2014.
// +----------------------------------------------------------------------
// | * Redistribution and use of this software in source and binary forms,
// |   with or without modification, are permitted provided that the following
// |   conditions are met:
// |
// | * Redistributions of source code must retain the above
// |   copyright notice, this list of conditions and the
// |   following disclaimer.
// |
// | * Redistributions in binary form must reproduce the above
// |   copyright notice, this list of conditions and the
// |   following disclaimer in the documentation and/or other
// |   materials provided with the distribution.
// |
// | * Neither the name of the ray team, nor the names of its
// |   contributors may be used to endorse or promote products
// |   derived from this software without specific prior
// |   written permission of the ray team.
// |
// | THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// | "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// | LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// | A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// | OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// | SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// | LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// | DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// | THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// | (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// | OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
// +----------------------------------------------------------------------

#ifndef _H_LIBSCRIPT_PUSHER_H_
#define _H_LIBSCRIPT_PUSHER_H_

#include "libscript_stack.h"

/// @addtogroup script
/// @{

_NAME_BEGIN


class EXPORT Pusher : public Stack
{
public:
    Pusher(RawInterface raw);
    Pusher(const Stack& stack);

    /// --- Push value into stack ---
    Pusher& push(bool b);
    Pusher& push(long long n);
    Pusher& push(int n);
    Pusher& push(unsigned n);
    Pusher& push(long n);
    Pusher& push(const char* cstr);
    Pusher& push(double f);
    Pusher& push(CFunction func);
    Pusher& push(Stack::P_Nil);
    Pusher& push(Value& value);
    Pusher& push(Class c);

    void reset(RawInterface raw);
    void reset();
    int count() const;

private:
    int _push_count;
};

class EXPORT MultiPusher : public Pusher
{
public:
    MultiPusher(RawInterface raw);
    MultiPusher(const Stack& stack);

    template <typename ... _Value> Pusher& push(_Value ... arg)
    {
        std::tuple<_Value ...> tuple(arg ...);
        _ArgPusher<std::tuple<_Value ...>, sizeof...(_Value)>::multiPush(*this, tuple);
        return *this;
    }

private:
    template<typename _Tuple, std::size_t _N>
    class _ArgPusher {
    public:
        static void multiPush(Pusher& pusher, _Tuple& tuple)
        {
            _ArgPusher<_Tuple, _N - 1>::multiPush(pusher, tuple);
            pusher.push(std::get<_N - 1>(tuple));
        }
    };

    template<typename _Tuple>
    class _ArgPusher<_Tuple, 0> {
    public:
        static void multiPush(Pusher& pusher, _Tuple& tuple) { }
    };
};

/// @}

_NAME_END

#endif
