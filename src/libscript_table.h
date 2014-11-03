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

#ifndef _H_LIBSCRIPT_TABLE_H_
#define _H_LIBSCRIPT_TABLE_H_

#include "libscript_value.h"
#include "libscript_pusher.h"

/// @addtogroup script
/// @{

_NAME_BEGIN

class EXPORT Table FINAL : public Value
{
public:
    /// @brief Access table contents
    /// Don't to new this class, that will be damage stack.
    class EXPORT Accessor FINAL
    {
    public:
        Accessor(Table table);
        ~Accessor();
        bool end();
        
        /// @brief To next key / value
        /// @warning Do not call Value::toString directly on a key
        /// Visit : http://www.lua.org/manual/5.2/manual.html#lua_next
        void next();
        StackValue& key();
        StackValue& value();

    private:
        Accessor(const Accessor& copy) TODELETE;
        Accessor& operator=(const Accessor& copy) TODELETE;

    private:
        bool _valid;
        Value _table;
        StackValue _key;
        StackValue _value;
    };

    Table(const Stack& stack);
    Table(const Value& value);
    Table(const Table& copy);

    /// @brief Get table[key]
    Value at(long long key);
    Value at(Value& key);
    Value at(const std::string& key);

    /// @brief Set table[key] = value
    template<typename _Key, typename _Value> Table& set(_Key key, _Value value) 
    {
        pushRef(T_Table);
        _pusher.push(key);
        _pusher.push(value);
        rawset(-3);
        pop(1);
        return *this;
    }

    std::size_t len();

    /// @brief index value with given the key
    /// @note When to index a nonexistent key, will be return a nil
    Table operator[](long long key);
    Table operator[](Value& value);
    Table operator[](const char *key);

private:
    Pusher _pusher;
};

/// @}

_NAME_END

#endif
