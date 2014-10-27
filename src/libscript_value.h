// +----------------------------------------------------------------------
// | Copyright (c) 2013-2014.
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

#ifndef _H_LIBSCRIPT_VALUE_H_
#define _H_LIBSCRIPT_VALUE_H_

#include "libscript_stack.h"

/// @addtogroup script
/// @{

_NAME_BEGIN

class StackValue;

class EXPORT Value : public Stack
{
    friend class Script;
public:
    static const struct P_Nil {} Nil;

    Value(RawInterface raw);
    Value(const Stack& stack);
    Value(const StackValue& arg);
    Value(const Value& copy);
    virtual ~Value();

    /// @brief Push value in stack and check type
    /// @warning May be thow a exception
    void pushRef(TYPE check);

    /// @brief Push value in stack and check type
    /// @return The value is or not given type
    bool pushRefSafe(TYPE check);

    /// @brief Set stack top to new value, and pop stack top
    /// @note If stack is empty, Will to push a nil then to reset
    void reset();
    void reset(Stack& stack);

    const char* typeName();
    TYPE type();

    /// Try to conversion to base types
    bool toBoolean();
    long long toInteger();
    double toNumber();
    void* toUserData();
    const char* toString();

    template<typename _Class> _Class* toClass()
    {
        return (_Class*)_toClass(typeid(_Class).name());
    }

    template<typename _Class> const _Class* toConstClass()
    {
        return (_Class*)_toConstClass(typeid(_Class).name());
    }

    template<typename _Class> const UserData<_Class>* toClassInfo()
    {
        return (UserData<_Class>*)_toClassInfo(typeid(_Class).name());
    }

    bool isBoolean();
    bool isCFunction();
    bool isFunction();
    bool isLightUserdata();
    bool isNil();
    bool isNumber();
    bool isString();
    bool isTable();
    bool isThread();
    bool isUserdata();

    operator bool();
    operator int();
    operator long();
    operator long long();
    operator float();
    operator double();
    operator void*();
    operator const char*();

    bool operator ==(Value& value);
    bool operator != (Value& value);
    Value& operator = (Value& copy);

protected:
    ValueHandler* _handler;

private:
    void* _toClass(const char* metaname);
    void* _toConstClass(const char* metaname);
    const void* _toClassInfo(const char* metaname);

private:
    static void endOfHandlerRefBuffer();

    void releaseHandler();
};

class EXPORT StackValue final : public Stack
{
public:
    StackValue(Stack stack, int index);
    StackValue(const StackValue& arg);

    const char* typeName()  { return typename_L(_index); }
    TYPE type()             { return Stack::type(_index); }

    /// Try to conversion to base types
    bool toBoolean()        { return toboolean(_index); }
    long long toInteger()   { return tointeger(_index); }
    double toNumber()       { return tonumber(_index); }
    void* toUserdata() { return touserdata(_index); }
    const char* toString()  { const char* str = tostring(_index); return str ? str : ""; }
    Value toValue()         { pushvalue(_index); return *this; }

    template<typename _Class> _Class* toClass()
    {
        return (_Class*)_toClass(typeid(_Class).name());
    }

    template<typename _Class> const _Class* toConstClass()
    {
        return (_Class*)_toConstClass(typeid(_Class).name());
    }

    template<typename _Class> const UserData<_Class>* toClassInfo()
    {
        return (UserData<_Class>*)_toClassInfo(typeid(_Class).name());
    }

    bool isBoolean()        { return isboolean(_index); }
    bool isCFunction()      { return iscfunction(_index); }
    bool isFunction()       { return isfunction(_index); }
    bool isNil()            { return isnil(_index); }
    bool isNoneOrNil()      { return isnoneornil(_index); }
    bool isNumber()         { return isnumber(_index); }
    bool isString()         { return isstring(_index); }
    bool isTable()          { return istable(_index); }
    bool isThread()         { return isthread(_index); }
    bool isUserdata()       { return isuserdata(_index); }

    operator bool()         { return toBoolean(); }
    operator int()          { return toInteger(); }
    operator long()         { return toInteger(); }
    operator long long()    { return toInteger(); }
    operator float()        { return toNumber(); }
    operator double()       { return toNumber(); }
    operator void*()        { return toUserdata(); }
    operator const char*()  { return toString(); }

    int getIndex() const    { return _index; }

private:
    void* _toClass(const char* metaname);
    void* _toConstClass(const char* metaname);
    const void* _toClassInfo(const char* metaname);

private:
    int _index;
};

/// @}

_NAME_END

#endif
