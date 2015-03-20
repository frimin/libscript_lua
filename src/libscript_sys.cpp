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

#include "libscript.h"
#include "libscript_luaapi.h"

_NAME_BEGIN

Script::Script(bool openlibs, MemAllocFunc alloc, void* ud) : StackInterface(NULL)
{
    RawInterface raw = NULL;

    if (alloc == NULL)
        raw = StackInterface::newstate_L();
    else
        raw = StackInterface::newstate(alloc, ud);

    _c_state = raw;

    if (openlibs)
        openlibs_L();
}

static void *l_alloc(void *ud, void *ptr, size_t osize, size_t nsize) 
{
    MemAllocation* alloc = (MemAllocation*)ud;

    if (nsize == 0) {
        alloc->free(ptr, osize);
        return NULL;
    }
    else
        return alloc->alloc(nsize);
}

Script::Script(bool openlibs, MemAllocation* alloc) : StackInterface(NULL)
{
    RawInterface raw = NULL;

    if (alloc == NULL)
        raw = StackInterface::newstate_L();
    else
        raw = StackInterface::newstate(l_alloc, alloc);

    _c_state = raw;

    if (openlibs)
        openlibs_L();
}

Script::~Script()
{
    Value::endOfHandlerRefBuffer();
    StackInterface::close(_c_state);
}

void Script::exec(const char* filemame)
{
    if (dofile_L(filemame))
    {
        std::string errorInfo = tostring(-1); 
        pop(1);
        SCRIPT_EXCEPTION(errorInfo);
    }
}

void Script::execString(const char* str)
{
    if (loadbuffer_L(str, std::strlen(str), str) ||
        pcall(0, 0, 0))
    {
        std::string errorInfo = tostring(-1);
        pop(1);
        SCRIPT_EXCEPTION(errorInfo);
    }
}

bool Script::execSafe(const char* fileName, std::string* errorOut)
{
    if (dofile_L(fileName))
    {
        if(errorOut)
            *errorOut = tostring(-1);
        pop(1);
        return false;
    }
    return true;
}

bool Script::execStringSafe(const char* str, std::string* errorOut)
{
    if (loadbuffer_L(str, std::strlen(str), str) ||
        pcall(0, 0, 0))
    {
        if(errorOut)
            *errorOut = tostring(-1);
        pop(1);
        return false;
    }
    
    return true;
}

Value Script::newThread()
{
    StackInterface::newthread();
    return (Value)*this;
}

Value Script::newThread(Function function)
{
    StackInterface NL = StackInterface::newthread();
    function.pushRef(T_Function);
    xmove(NL, 1);
    return (Value)*this;
}

Value Script::getGlobal(const char* name)
{
    getglobal(name);
    return (Value)*this;
}

Table Script::getGlobalTable()
{
    pushglobaltable();
    return (Value)*this;
}

Value Script::getNil()
{
    pushnil();
    return (Value)*this;
}

Table Script::operator[](int key)
{
    Table global = getGlobalTable();
    
    if (global.pushRefSafe(T_Table))
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

Table Script::operator[](Value& value)
{
    Table global = getGlobalTable();
    
    if (global.sameThreadSafe(value) && global.pushRefSafe(T_Table))
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

Table Script::operator[](const char* key)
{
    Table global = getGlobalTable();
    
    if (global.pushRefSafe(T_Table))
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

_NAME_END
