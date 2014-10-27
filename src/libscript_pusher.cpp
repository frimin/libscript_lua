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

#include "libscript_pusher.h"

/// @addtogroup script
/// @{

_NAME_BEGIN

Pusher::Pusher(RawInterface raw) : Stack(raw), _push_count(0) { }
Pusher& Pusher::push(bool b)         { pushboolean(b); ++_push_count; return *this; }
Pusher& Pusher::push(long long n)    { pushinteger(n); ++_push_count; return *this; }
Pusher& Pusher::push(int n)          { pushinteger(n); ++_push_count; return *this; }
Pusher& Pusher::push(unsigned n)     { pushinteger(n); ++_push_count; return *this; }
Pusher& Pusher::push(long n)         { pushinteger(n); ++_push_count; return *this; }
Pusher& Pusher::push(const char* cstr) { pushstring(cstr); ++_push_count; return *this; }
Pusher& Pusher::push(double f)       { pushnumber(f); ++_push_count; return *this; }
Pusher& Pusher::push(CFunction func) { pushcfunction(func); ++_push_count; return *this; }
Pusher& Pusher::push(Value::P_Nil)   { pushnil(); ++_push_count; return *this; }
Pusher& Pusher::push(Value value)    { sameThread(value); value.pushRefSafe(NoneMask); ++_push_count; return *this; }
Pusher& Pusher::push(Class c)     
{
    auto info = (UserData<VOID_T>*)newuserdata(sizeof(UserData<VOID_T>));

    info->obj = (VOID_T*)c.c;
    info->ref = c.ref;
    info->readonly = c.readonly;

    getfield(Stack::REGISTRYINDEX(), c.metaname.c_str());

    if (!istable(-1))
    {
        SCRIPT_EXCEPTION("Not found metatable : " + c.metaname);
    }

    setmetatable(-2);

    ++_push_count;

    return *this;
}

void Pusher::reset(RawInterface raw){ reset(); _c_state = raw; }
void Pusher::reset()                { _push_count = 0; }
int Pusher::count() const           { return _push_count; }

/// @}

_NAME_END
