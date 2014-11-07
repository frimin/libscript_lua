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

#ifndef _H_LIBSCRIPT_BIND_H_
#define _H_LIBSCRIPT_BIND_H_

#include "libscript_sys.h"
#include "libscript_cd.h"


/// @addtogroup script
/// @{

#include <iostream>

_NAME_BEGIN

template <typename _Func>
Value getFunction(Stack& stack, _Func func)
{
    CD::Function::push<_Func>(stack, func);
    return stack;
}

Value getForwardFunction(Stack& stack, CD::Function::Forward func);

/// @brief Bind CPP Class
template<typename _Class>
class BindClass FINAL
{
public:
    typedef typename CD::Constructor<_Class>::Forward Creator;
    typedef typename CD::Method<_Class>::Forward Method;
    typedef typename CD::Method<_Class>::ReadOnlyForward ReadOnlyMethod;
    typedef void (*Destroy)(_Class*);

    BindClass(Script& script) 
        :   _script(script),
            _metaTable(script.newTable()),
            _pusher(script.getInterface())
    {
        std::string metaname(typeid(_Class).name());

        _metaTable.set("__index", _metaTable);
        _metaTable.pushRefSafe(Stack::NoneMask);
        _metaTable.setfield(Stack::REGISTRYINDEX(), metaname.c_str());
    }
    template <typename ... _Args>
    BindClass& create(const char* name)
    {
        CD::Constructor<_Class>::template push<_Args ...>(_script);
        _script.setglobal(name);
        return *this;
    }

    BindClass& createForward(const char* name, Creator creator)
    {
        CD::Constructor<_Class>::pushForward(_script, creator);
        _script.setglobal(name);
        return *this;
    }

    template<typename _Method>
    BindClass& method(const char* name, _Method method)
    {
        _metaTable.pushRef(Stack::T_Table);
        _script.pushstring(name);
        CD::Method<_Class>::template push<_Method>(_script, method);
        _script.rawset(-3);
        _script.pop(1);
        return *this;
    }

    BindClass& methodForward(const char* name, Method method)
    {
        _metaTable.pushRef(Stack::T_Table);
        _script.pushstring(name);
        CD::Method<_Class>::pushForward(_script, method);
        _script.rawset(-3);
        _script.pop(1);
        return *this;
    }

    BindClass& methodReadOnlyForward(const char* name, ReadOnlyMethod method)
    {
        _metaTable.pushRef(Stack::T_Table);
        _script.pushstring(name);
        CD::Method<_Class>::pushForward(_script, method);
        _script.rawset(-3);
        _script.pop(1);
        return *this;
    }

    BindClass& destroy(const char* name)
    {
        _metaTable.pushRef(Stack::T_Table);
        _script.pushstring(name);
        CD::Destructor<_Class>::push(_script);
        _script.rawset(-3);
        _script.pop(1);
        return *this;
    }

    BindClass& destroy(const char* name, void (destroyer)(_Class*))
    {
        _metaTable.pushRef(Stack::T_Table);
        _script.pushstring(name);
        CD::Destructor<_Class>::push(_script, destroyer);
        _script.rawset(-3);
        _script.pop(1);
        return *this;
    }

    template<typename _Key, typename _Value>
    BindClass& set(_Key key, _Value value)
    {
        _metaTable.pushRef(Stack::T_Table);
        _pusher.push(key);
        _pusher.push(value);
        _pusher.rawset(-3);
        _pusher.pop(1);
        return *this;
    }
    
private:
    BindClass(const BindClass& copy) TODELETE;
    BindClass& operator=(const BindClass& copy) TODELETE;

private:
    Script& _script;
    Table _metaTable;
    Pusher _pusher;
};

/// @brief Definition a Module
class EXPORT Module FINAL
{
public:
    /// @brief Definition to global table
    Module(Script& script);
    /// @brief Definition to given table
    Module(Table table);
    /// @brief Definition to create new table
    Module(Script& script, const char* name);
    ~Module();

    template<typename _Key, typename _Value> 
    Module& set(_Key key, _Value value)
    {
        if (!_badFlag)
        {
            _pusher.push(key);
            _pusher.push(value);
            _pusher.rawset(-3);
        }
        return *this;
    }

private:
    Module(const Module& copy) TODELETE;
    Module& operator=(const Module& copy) TODELETE;

private:
    bool _badFlag;
    Table _module;
    Pusher _pusher;
};

/// @}

_NAME_END

#endif
