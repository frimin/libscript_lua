// +----------------------------------------------------------------------
// | Project : ray.
// | All rights reserved.
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

#include "libscript_value.h"

#include <vector>
#include "libscript_args.h"
#include "libscript_pusher.h"

_NAME_BEGIN

struct DataSourcesDispatcher
{
    DataSourcesDispatcher(IValueDataSources* dataSources) : _dataSources(dataSources)
    {
        if (_dataSources)
            _dataSources->pushData();
    }
    ~DataSourcesDispatcher()
    {
        if (_dataSources)
            _dataSources->popData();
    }

    IValueDataSources* _dataSources;
};

StackValue::StackValue(RawInterface raw, int index, IValueDataSources* dataSources) : StackInterface(raw), _index(index), _dataSources(dataSources)
{

}

StackValue::StackValue(const StackInterface& stack, int index, IValueDataSources* dataSources) : StackInterface(stack), _index(index), _dataSources(dataSources)
{

}

StackValue::StackValue(const StackValue& stackvalue) : StackInterface(stackvalue), _index(stackvalue._index), _dataSources(NULL)
{

}

const char* StackValue::typeName()  {
    DataSourcesDispatcher dispatcher(_dataSources);
    return typename_L(_index);
}

StackInterface::TYPE StackValue::type()
{
    DataSourcesDispatcher dispatcher(_dataSources);
    return StackInterface::type(_index);
}

bool StackValue::toBoolean()
{
    DataSourcesDispatcher dispatcher(_dataSources);
    return toboolean(_index);
}

CFunction StackValue::toCFunction()
{
    DataSourcesDispatcher dispatcher(_dataSources);
    return tocfunction(_index);
}

long long StackValue::toInteger()
{
    DataSourcesDispatcher dispatcher(_dataSources);
    return tointeger(_index);
}

const char* StackValue::toLString(std::size_t* len)
{
    DataSourcesDispatcher dispatcher(_dataSources);
    return tolstring(_index, len);
}

double StackValue::toNumber()
{
    DataSourcesDispatcher dispatcher(_dataSources);
    return tonumber(_index);
}

const void* StackValue::toPointer()
{
    DataSourcesDispatcher dispatcher(_dataSources);
    return topointer(_index);
}

void* StackValue::toUserdata()
{
    DataSourcesDispatcher dispatcher(_dataSources);
    return touserdata(_index);
}

const char* StackValue::toString()
{
    DataSourcesDispatcher dispatcher(_dataSources);
    return tostring(_index);
}

void* StackValue::_toClass(const char* metaname)
{
    DataSourcesDispatcher dispatcher(_dataSources);
    checkudata_L(_index, metaname);
    ClassInfo<VOID_T>* info = (ClassInfo<VOID_T>*)touserdata(_index);
    return info->readonly ? NULL : info->obj;
}

void* StackValue::_toReadOnlyClass(const char* metaname)
{
    DataSourcesDispatcher dispatcher(_dataSources);
    checkudata_L(_index, metaname);
    ClassInfo<VOID_T>* info = (ClassInfo<VOID_T>*)touserdata(_index);
    return info->obj;
}

const void* StackValue::_toClassInfo(const char* metaname)
{
    DataSourcesDispatcher dispatcher(_dataSources);
    checkudata_L(_index, metaname);
    return (ClassInfo<VOID_T>*)touserdata(_index);
}

bool StackValue::isBoolean()
{
    DataSourcesDispatcher dispatcher(_dataSources);
    return isboolean(_index);
}

bool StackValue::isCFunction()
{
    DataSourcesDispatcher dispatcher(_dataSources);
    return iscfunction(_index);
}

bool StackValue::isFunction()
{
    DataSourcesDispatcher dispatcher(_dataSources);
    return isfunction(_index);
}

bool StackValue::isNil()
{
    DataSourcesDispatcher dispatcher(_dataSources);
    return isnil(_index);
}

bool StackValue::isNone()
{
    DataSourcesDispatcher dispatcher(_dataSources);
    return isnone(_index);
}

bool StackValue::isNoneOrNil()
{
    DataSourcesDispatcher dispatcher(_dataSources);
    return isnoneornil(_index);
}

bool StackValue::isNumber()
{
    DataSourcesDispatcher dispatcher(_dataSources);
    return isnumber(_index);
}

bool StackValue::isString()
{
    DataSourcesDispatcher dispatcher(_dataSources);
    return isstring(_index);
}

bool StackValue::isTable()
{
    DataSourcesDispatcher dispatcher(_dataSources);
    return istable(_index);
}

bool StackValue::isThread()
{
    DataSourcesDispatcher dispatcher(_dataSources);
    return isthread(_index);
}

bool StackValue::isUserdata()
{
    DataSourcesDispatcher dispatcher(_dataSources);
    return isuserdata(_index);
}

StackValue& StackValue::operator = (const StackValue& copy)
{
    _index = copy._index;
    return *this;
}

UpValue::UpValue(RawInterface raw, int upValueIndex) : StackValue(raw, StackInterface::upvalueindex(upValueIndex)), _pusher(raw)
{
}

UpValue::UpValue(const StackInterface& stack, int upValueIndex) : StackValue(stack, StackInterface::upvalueindex(upValueIndex)), _pusher(stack)
{
}

// To prevent when copy Value to call many Lua APIs
// (Call the luaL_ref and luaL_unref neet so many time)
struct ValueHandler
{
    int value;
    int refCount;

    ValueHandler() : value(0), refCount(0) { }

    static ValueHandler* create(int value)
    {
        ValueHandler* hander = NULL;

        if (_s_free_list.size())
        {
            hander = _s_free_list.back();
            _s_free_list.pop_back();
        }
        else
        {
            hander = new ValueHandler;
        }

        hander->value = value;
        hander->refCount = 1;

        ++_s_using_count;

        return hander;
    }

    static void release(ValueHandler* handler)
    {
        if (handler->refCount == 1)
        {
            handler->refCount = 0;
            _s_free_list.push_back(handler);
            --_s_using_count;
        }
        else
        {
            --handler->refCount;
        }
    }

    static ValueHandler* ref(ValueHandler* handler)
    {
        ++handler->refCount;
        return handler;
    }

    static void clearFree()
    {
        while (_s_free_list.size())
        {
            delete _s_free_list.back();
            _s_free_list.pop_back();
        }
    }

    static std::size_t getUsingCount()
    {
        return _s_using_count;
    }

private:
    static std::vector<ValueHandler*> _s_free_list;
    static std::size_t _s_using_count;
};

std::vector<ValueHandler*> ValueHandler::_s_free_list;
std::size_t ValueHandler::_s_using_count = 0;

Value::Value(RawInterface raw) : StackValue(raw, -1, this)
{
    if (gettop() == 0)
        SCRIPT_EXCEPTION("No element in the stack");

    _handler = ValueHandler::create(ref_L(StackInterface::REGISTRYINDEX()));
}

Value::Value(const StackInterface& stack) : StackValue(stack, -1, this)
{
    if (gettop() == 0)
        SCRIPT_EXCEPTION("No element in the stack");

    _handler = ValueHandler::create(ref_L(StackInterface::REGISTRYINDEX()));
}

Value::Value(const StackValue& value) : StackValue(value, -1, this)
{
    pushvalue(getIndex());

    _handler = ValueHandler::create(ref_L(StackInterface::REGISTRYINDEX()));
}

Value::Value(const Value& copy) : StackValue(copy, -1, this)
{
    sameThread(copy);

    _handler = ValueHandler::ref(copy._handler);
}

Value::~Value()
{
    releaseHandler();
}

void Value::pushRef(TYPE check)
{
    rawgeti(StackInterface::REGISTRYINDEX(), _handler->value);

    if (check != NoneMask)
    {
        if (StackInterface::type(-1) != check)
            SCRIPT_EXCEPTION("Bad stackobj type.");
    }
}

bool Value::pushRefSafe(TYPE check)
{
    rawgeti(StackInterface::REGISTRYINDEX(), _handler->value);

    if (check != NoneMask)
    {
        if (StackInterface::type(-1) != check)
        {
            this->pop(1);
            return false;
        }
    }

    return true;
}

void Value::reset()
{
    if (gettop() == 0)
        pushnil();

    releaseHandler();

    _handler = ValueHandler::create(ref_L(StackInterface::REGISTRYINDEX()));
}

void Value::reset(StackInterface& stack)
{
    if (stack.gettop() == 0)
        stack.pushnil();

    releaseHandler();

    _c_state = stack.getInterface();

    _handler = ValueHandler::create(ref_L(StackInterface::REGISTRYINDEX()));
}

void Value::reset(const StackValue& stack)
{
    releaseHandler();

    if (_c_state != NULL)
        sameThread(stack);
    else
        _c_state = stack._c_state;

    pushvalue(getIndex());

    _handler = ValueHandler::create(ref_L(StackInterface::REGISTRYINDEX()));
}

void Value::pushData()
{
    this->pushRefSafe(NoneMask);
}
void Value::popData()
{
    pop(1);
}

bool Value::operator ==(Value& value)
{
    if (!sameThreadSafe(value))
        return false;

    if (this->_c_state != value.getInterface())
        SCRIPT_EXCEPTION("Different thread");

    this->pushRefSafe(NoneMask);
    value.pushRefSafe(NoneMask);
    int result = rawequal(-2, -1);
    pop(2);
    return result == 1;
}

bool Value::operator != (Value& value)
{
    return !this->operator==(value);
}

Value& Value::operator=(Value& copy)
{
    releaseHandler();

    if (_c_state != NULL)
        sameThread(copy);
    else
        _c_state = copy._c_state;

    _handler = ValueHandler::ref(copy._handler);

    return *this;
}

Value& Value::operator = (StackValue& copy)
{


    return *this;
}

void Value::releaseHandler()
{
    if (_handler->refCount == 1)
    {
        unref_L(StackInterface::REGISTRYINDEX(), _handler->value);
    }

    ValueHandler::release(_handler);

    _handler = NULL;
}

void Value::endOfHandlerRefBuffer()
{
    if (ValueHandler::getUsingCount() != 0)
        SCRIPT_EXCEPTION("Some handler is not release");

    ValueHandler::clearFree();
}

_NAME_END
