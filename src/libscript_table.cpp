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

#include "libscript_table.h"

_NAME_BEGIN

Table::Table(const Stack& stack) : Value(stack), _pusher(NULL)
{
    _pusher.reset(getInterface());
}

Table::Table(const Value& value) : Value(value), _pusher(NULL)
{
    _pusher.reset(getInterface());
}

Table::Table(const Table& copy) : Value(copy), _pusher(NULL)
{
    _pusher.reset(getInterface());
}
 
Value Table::at(long long key)
{ 
    pushRef(T_Table);
    pushinteger(key);
    rawget(-2);
    remove(-2);
    return this->_c_state;
}

Value Table::at(Value& key)
{
    sameThread(key);

    pushRef(T_Table);
    key.pushRef(NoneMask);
    rawget(-2);
    remove(-2);
    return this->_c_state;
}

Value Table::at(const std::string& key)
{
    pushRef(T_Table);
    pushstring(key.c_str());
    rawget(-2);
    remove(-2);
    return this->_c_state;
}

size_t Table::len()
{
    this->pushRef(T_Table);
    std::size_t s = rawlen( -1);
    pop(1);
    return s;
}

Table Table::operator[](long long key)
{
    if (pushRefSafe(T_Table))
    {
        pushinteger(key);
        rawget(-2);
        remove(-2);
    }
    else
    {
        pushnil();
    }

    Value value(getInterface());

    return value;
}

Table Table::operator[](Value& value)
{
    if (sameThreadSafe(value) && this->pushRefSafe(T_Table))
    {
        value.pushRefSafe(NoneMask);
        rawget(-2);
        remove(-2);
    }
    else
    {
        pushnil();
    }

    Value _value(getInterface());

    return _value;
}

Table Table::operator[](const char *key)
{
    if (pushRefSafe(T_Table))
    {
        pushstring(key);
        rawget(-2);
        remove(-2);
    }
    else 
    {
        pushnil();
    }

    Value value(this->getInterface());

    return value;
}

// +----------------------------------------------------------------------
// class Table::Accessor
// +----------------------------------------------------------------------

Table::Accessor::Accessor(Table table) 
    : _valid(true), _table(table), 
    _key(table.getInterface(), 0),
    _value(table.getInterface(), 0)
{
    _table.pushRef(Stack::T_Table);
    _table.pushnil();
    _table.pushnil();

    this->next();
}

Table::Accessor::~Accessor()
{
    // remove table from stack
    _table.pop(1);
}

bool Table::Accessor::end()
{
    return !_valid;
}

void Table::Accessor::next()
{
    if (!_valid)
        SCRIPT_EXCEPTION("Invalid Accessor status");

    // remove value from stack
    _table.pop(1);

    _valid = _table.next(-2) != 0;

    if (_valid) {
        // index of key / value
        _key.setIndex(_table.gettop() - 1);
        _value.setIndex(_table.gettop());
    }
}

StackValue& Table::Accessor::key()
{
    return _key;
}

StackValue& Table::Accessor::value()
{
    return _value;
}

_NAME_END
