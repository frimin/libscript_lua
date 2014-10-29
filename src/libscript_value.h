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

/// @brief Interface of get stack value
struct IValueDataSources
{
    /// @brief Will be invoked before using the stack value.
    virtual void pushData() = 0;
    /// @brief Will be invoked after using the stack value.
    virtual void popData() = 0;
};

/// @brief Represents a value from stack
class EXPORT StackValue : public Stack
{
public:
    /// @brief Initialization from RawInterface and given index
    StackValue(RawInterface raw, int index, IValueDataSources* dataSource = NULL);
    /// @brief Initialization from other stack and given index
    StackValue(const Stack& stack, int index, IValueDataSources* dataSource = NULL);
    /// @brief Copy other
    StackValue(const StackValue& stackvalue);

    /// @brief Returns the name of the type of the value.
    const char* typeName();

    /// @brief Returns the type of the value,
    TYPE type();

    /// @brief Converts the value to a boolean value, 
    /// returns true for any value different from false and nil
    /// otherwise it returns false, If you want to accept only 
    //// actual boolean values, use StackValue::isBoolean to test the value's type
    bool toBoolean();

    /// @brief Converts a value  to a C function.
    /// That value must be a C function; otherwise, returns NULL.
    CFunction toCFunction();
    
    /// @brief Converts the value to the signed integral type. 
    /// The value must be a number or a string convertible to a number.
    /// otherwise, lua_tointegerx returns 0.
    long long toInteger();

    /// @brief Converts the value to a C string.
    /// If len is not NULL, it also sets *len with the string length
    /// The value must be a string or a number; otherwise, the function returns NULL
    const char* toLString(std::size_t* len);

    /// @brief Converts the value to the double
    /// The value must be a number or a string convertible to a number. otherwise, returns 0.
    double toNumber();

    /// @brief Converts the value to a generic C pointer (void*).
    /// The value can be a ClassInfo, a table, a thread, or a function;
    /// otherwise, returns NULL. Different objects will give different pointers.
    /// There is no way to convert the pointer back to its original value.
    const void* toPointer();

    /// @brief To the ClassInfo value 
    /// If the value is a full ClassInfo returns its block address. 
    /// If the value is a light ClassInfo, returns its pointer. Otherwise, returns NULL.
    void* toUserdata();

    /// @brief Equivalent to StackValue::toLString with len equal to NULL.
    const char* toString();

    /// @brief To a class point.
    /// If the value is a full ClassInfo and have correct metatable in the registry, returns its pointer
    template<typename _Class> _Class* toClass()
    {
        return (_Class*)_toClass(typeid(_Class).name());
    }

    /// @brief To a class const point.
    /// If the value is a full ClassInfo and have correct metatable in the registry, returns its const pointer
    template<typename _Class> const _Class* toConstClass()
    {
        return (_Class*)_toConstClass(typeid(_Class).name());
    }

    /// @brief To a class info block point.
    template<typename _Class> const ClassInfo<_Class>* toClassInfo()
    {
        return (ClassInfo<_Class>*)_toClassInfo(typeid(_Class).name());
    }

    /// @brief Returns true if the value is a boolean, and false otherwise.
    bool isBoolean();

    /// @brief Returns true if the value is a C function, and false otherwise.
    bool isCFunction();

    /// @brief Returns true if the value is a function(either C or Lua), and false otherwise.
    bool isFunction();

    /// @brief Returns true if the value is nil, and false otherwise.
    bool isNil();

    /// @brief Returns true if this value is not valid, and false otherwise.
    bool isNone();

    /// @brief Returns true if this value is not valid or if the value is nil, and false otherwise.
    bool isNoneOrNil();
    
    /// @brief Returns true if the value is a number or a string convertible to a number, and false otherwise.
    bool isNumber();

    /// @brief Returns true if the value is a string or a number (which is always convertible to a string), and false otherwise.
    bool isString();

    /// @brief Returns true if the value is a table, and false otherwise.
    bool isTable();

    /// @brief Returns true if the value is a table, and false otherwise.
    bool isThread();

    /// @brief Returns true if the value is a userdata (either full or light), and false otherwise.
    bool isUserdata();

    /// --- Converts to base types --- 
    operator bool()         { return toBoolean(); }
    operator int()          { return toInteger(); }
    operator long()         { return toInteger(); }
    operator long long()    { return toInteger(); }
    operator float()        { return toNumber(); }
    operator double()       { return toNumber(); }
    operator void*()        { return toUserdata(); }
    operator const char*()  { return toString(); }

    /// @brief Set stack index
    void setIndex(int index){ _index = index; }
    /// @brief Returns stack index
    int getIndex() const    { return _index; }

    StackValue& operator = (const StackValue& copy);

private:
    void* _toClass(const char* metaname);
    void* _toConstClass(const char* metaname);
    const void* _toClassInfo(const char* metaname);

private:
    int _index;
    IValueDataSources* _dataSources;
};

/// @brief Represents a value from registry
class EXPORT Value : public StackValue, private IValueDataSources
{
public:
    /// @brief Initialization from RawInterface
    Value(RawInterface raw);
    /// @brief Initialization from other stack
    Value(const Stack& stack);
    /// @brief Copy other
    Value(const Value& copy);
    virtual ~Value();

    /// @brief Push value to stack and check type
    /// @warning Its may be thow a exception
    void pushRef(TYPE check);

    /// @brief Push value to stack and check type
    /// Returns true if the value is given type, and false otherwise.
    bool pushRefSafe(TYPE check);

    /// @brief Set new value from stack top, and pop top.
    /// If stack is empty, Will to push a nil then to reset
    void reset();
    /// @brief Set new value from given stack top, and pop top.
    /// If stack is empty, Will to push a nil then to reset
    /// You can change current value's stack
    void reset(Stack& stack);

    bool operator ==(Value& value);
    bool operator != (Value& value);
    Value& operator = (Value& copy);

private:
    virtual void pushData();
    virtual void popData();

    ValueHandler* _handler;

private:
    static void endOfHandlerRefBuffer();
    void releaseHandler();
};

/// @}

_NAME_END

#endif
