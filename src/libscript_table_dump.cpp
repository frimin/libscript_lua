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

#include "libscript_table_dump.h"

#include <cctype>
#include <vector>

#include "libscript_thread.h"

_NAME_BEGIN

class Path
{
public:
    Path(Stack stack, const std::string& mainTable)
        : _thread_stack(stack.newthread())
    {
        stack.pop(1);
        _thread_stack.pushstring(mainTable.c_str());
    }

    void enter(const char* str)
    {
        _thread_stack.pushstring(str);
    }

    void enter(double n)
    {
        _thread_stack.pushnumber(n);
    }

    void change(const char* str)
    {
        _thread_stack.pop(1);
        _thread_stack.pushstring(str);
    }

    void change(double n)
    {
        _thread_stack.pop(1);
        _thread_stack.pushnumber(n);
    }

    void leave()
    {
        _thread_stack.pop(1);
    }

    int depth()
    {
        return _thread_stack.gettop();
    }

    const std::string& pathStr()
    {
        _temp_pathStr.clear();

        for (int i = 1; i <= _thread_stack.gettop(); ++i)
        {
            if (i == 1)
            {
                _temp_pathStr.append(_thread_stack.tostring(1));
                continue;
            }

            switch (_thread_stack.type(i))
            {
            case Stack::T_String:
                _temp_pathStr.append("[\"" + std::string(_thread_stack.tostring(i)) + "\"]");
                break;
            case Stack::T_Number:
                _temp_pathStr.append("[" + std::string(_thread_stack.tostring(i)) + "]");
                break;
            default:
                break;
            }
        }

        return _temp_pathStr;
    }

    ~Path()
    {
        _thread_stack.pop(depth());
    }

private:
    Stack _thread_stack;
    std::string _temp_pathStr;
};

TableStringDumper::TableStringDumper(const Stack& stack) : Value(stack)
{
}

TableStringDumper::TableStringDumper(const Value& value) : Value(value)
{
}

TableStringDumper::TableStringDumper(const Table& table) : Value(table)
{
}

#define BEGIN 0.f
#define TABLE_NAME_KEY "@dpname"

bool legalStringKey(const char* str, std::size_t n)
{
    bool result = true;

    for (std::size_t i = 0; i != n; ++i)
    {
        if (!(std::isalnum(str[i]) || str[i] == '_' ))
        {
            result = false;
            break;
        }

        if ((i == 0 ? std::isdigit(str[i]) : false))
        {
            result = false;
            break;
        }
    }

    return result;
}

void tabs(std::ostream& o, std::size_t n)
{
    for (std::size_t i = 0; i != n; ++i)
        o << "    ";
}

bool TableStringDumper::dump(const std::string& tableName, std::ostream& o)
{
    /// Remember current traverse table
    std::vector<int> curTable;

    if (!pushRefSafe(Stack::T_Table))
        return false;

    curTable.push_back(gettop());

    std::vector<std::string> paths;
    Path path(this->getInterface(), tableName);

    pushnil(); /// frist key

    o << tableName << " = \n{\n";

    path.enter(BEGIN);

    while (curTable.size())
    {
        while (next(curTable.back()))
        {
            tabs(o, curTable.size());

            /// key
            switch (Stack::type(-2))
            {
            case T_String:
                if (legalStringKey(tostring(-2), rawlen(-2)))
                    o << tostring(-2);
                else
                    o << "[\"" << tostring(-2) << "\"]";
                path.change(tostring(-2));
                break;
            case T_Number:
                o << "[" << tonumber(-2) << "]";
                path.change(tonumber(-2));
                break;
            default:
                break;
            }

            o << " = ";

            paths.push_back(path.pathStr());

            /// value
            switch (Stack::type(-1))
            {
            case T_String:
                o << "\"" << tostring(-1) << "\"";
                break;
            case T_Number:
                o << tonumber(-1);
                break;
            case T_Table:
                o << "\n";
                tabs(o, curTable.size());
                o << "{\n";
                /// traverse sub table
                path.enter(BEGIN);
                curTable.push_back(gettop());
                pushnil();
                continue;
            default:
                break;
            }

            o << ",\n";

            pop(1);
        }

        pop(1);
        path.leave();
        curTable.pop_back();
        
        tabs(o, curTable.size());
        if (curTable.size())
            o << "},\n";
        else
            o << "}\n";
    }

    return true;
}

bool TableStringDumper::dump(const std::string& tableName, const std::string& fileName)
{
    std::ofstream file(fileName);

    if (!file.is_open())
        return false;

    bool result = this->dump(tableName, file);

    return result;
}

_NAME_END
