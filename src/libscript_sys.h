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

#ifndef _H_LIBSCRIPT_SYS_H_
#define _H_LIBSCRIPT_SYS_H_

#include <string>
#include <vector>

#include "libscript_stack.h"
#include "libscript_table.h"
#include "libscript_function.h"
#include "libscript_thread.h"
#include "libscript_cd.h"

/// @addtogroup script
/// @{

_NAME_BEGIN

class EXPORT Script FINAL : public Stack
{
public:
    Script(bool openlibs = true, MemAllocFunc alloc = NULL, void* ud = NULL);
    Script(bool openlibs, MemAllocation* alloc);
    ~Script();

    void exec(const char* filename);
    void execString(const char* str);
    
    bool execSafe(const char* filename, std::string* errorOut = NULL);
    bool execStringSafe(const char* str, std::string* errorOut = NULL);
    
    template <typename _Func> Value newFunction(_Func function)
    {
        CD::Function::push<_Func>(*this, function);
        return (Value)*this;
    }
    
    template <> Value newFunction(const char* script)
    {
        if (loadbuffer_L(script, std::strlen(script), script))
        {
            std::string errorInfo = tostring(-1);
            pop(1);
            SCRIPT_EXCEPTION(errorInfo);
        }
        return (Value)*this;
    }

    template <> Value newFunction(CFunction function)
    {
        pushcclosure(function, 0);
        return (Value)*this;
    }

    template <> Value newFunction(CD::Function::Forward function)
    {
        CD::Function::pushForward(*this, function);
        return (Value)*this;
    }

    template <typename ... _UpValue> Value newClosure(CFunction function, _UpValue ... upValue)
    {
        MultiPusher pusher(*this);
        pusher.push(upValue ...);
        pushcclosure(function, pusher.count());
        return (Value)*this;
    }
    
    template <typename ... _UpValue> Value newClosure(CD::Function::Forward function, _UpValue ... upValue)
    {
        CD::Function::pushForward<_UpValue ...>(*this, function, upValue ...);
        return (Value)*this;
    }
    
    template <typename ... _Value> Value newTable(_Value ... value)
    {
        newtable();

        MultiPusher pusher(*this);
        pusher.push(value ...);

        for (int i = pusher.count(); i != 0; --i)
            rawseti(-(i + 1), i);

        return (Value)*this;
    }

    Value newThread();
    Value newThread(Function function);
    Value getGlobal(const char* name);
    Table getGlobalTable();
    Value getNil();
    
    template <typename _Value>
    void setGlobal(const std::string& name, _Value value)
    {
        Pusher puhser(this->getInterface());
        puhser.push(value);
        setglobal(name.c_str());
    }
    
    /// @brief index value with given the key
    /// If given index is a nonexistent key, will be return a nil
    Table operator[](long long key);
    Table operator[](Value& value);
    Table operator[](const char* key);

private:
    Script(const Script& copy) TODELETE;
    Script& operator=(const Script& copy) TODELETE;
};

/// @}

_NAME_END

#endif
