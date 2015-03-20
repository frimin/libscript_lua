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

#ifndef _H_LIBSCRIPT_ARGS_H_
#define _H_LIBSCRIPT_ARGS_H_

#include "libscript_value.h"

/// @addtogroup script
/// @{

_NAME_BEGIN

/// @brief Represent a argument from function stack.
class EXPORT Arg FINAL : public StackValue
{
public:
    Arg(RawInterface raw, int index);
    Arg(StackInterface stack, int index);

    /// @brief Returns value of copy to registry
    Value toValue();
};

/// @brief Represent function stack.
class EXPORT Args FINAL : public StackInterface
{
public:
    Args(RawInterface raw, int upValueBegin = 0);
    Args(const Args& args, int upValueBegin = 0);

    Arg getArg(int i) { Arg v(*this, i); return v; }
    UpValue getUpValue(int i) { return UpValue(*this, _upvalue_begin + i); }

    /// @brief Get a argument
    /// (Index begin with 1)
    Arg operator[](int i) { return getArg(i); }

    /// @brief Returns arguments quantity
    int count() const { return _top; }

private:
    int _top;
    int _upvalue_begin;
};

/// @}

_NAME_END

#endif
