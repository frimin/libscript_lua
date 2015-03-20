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

#include "libscript_thread.h"
#include "libscript_args.h"

#include <memory.h>

_NAME_BEGIN

Thread::Thread(const StackInterface& stack) : Value(stack), _thread(NULL), _pusher(NULL)
{
    pushRef(T_Thread);
    _thread = StackInterface(tothread(-1));
    pop(1);

    _pusher.reset(_thread.getInterface());
}

Thread::Thread(const Value& value) : Value(value), _thread(NULL), _pusher(NULL)
{
    pushRef(T_Thread);
    _thread = StackInterface(tothread(-1));
    pop(1);

    _pusher.reset(_thread.getInterface());
}

void Thread::load(const char* fileName)
{
    if (_thread.loadfile_L(fileName))
    {
        std::string errorInfo = _thread.tostring(-1);
        _thread.pop(1);
        SCRIPT_EXCEPTION(errorInfo);
    }
}

void Thread::loadString(const char* str)
{
    if (_thread.loadbuffer_L(str, std::strlen(str), str))
    {
        std::string errorInfo = _thread.tostring(-1);
        _thread.pop(1);
        SCRIPT_EXCEPTION(errorInfo);
    }
}

bool Thread::loadSafe(const char* fileName, std::string* errorOut)
{
    if (_thread.loadfile_L(fileName))
    {
        if(errorOut)
            *errorOut = _thread.tostring(-1);
        _thread.pop(1);
        return false;
    }
    return true;
}

bool Thread::loadStringSafe(const char* str, std::string* errorOut)
{
    if (_thread.loadbuffer_L(str, std::strlen(str), str))
    {
        if(errorOut)
            *errorOut = _thread.tostring(-1);
        _thread.pop(1);
        return false;
    }
    
    return true;
}

Value Thread::newFunction(const char* script)
{
    if (loadbuffer_L(script, std::strlen(script), script))
    {
        std::string errorInfo = tostring(-1);
        pop(1);
        SCRIPT_EXCEPTION(errorInfo);
    }

    return (Value)_thread;
}

Value Thread::newFunction(CFunction function)
{
    pushcfunction(function);
    return (Value)_thread;
}

Value Thread::newTable()
{
    newtable();
    return (Value)_thread;
}

Value Thread::getGlobal(const std::string& name)
{
    getglobal(name.c_str());
    return (Value)_thread;
}

Table Thread::getGlobalTable()
{
    pushglobaltable();
    return (Value)_thread;
}

void Thread::resultReset()
{ 
    _pusher.reset();
    _beform_stack_size = gettop(); 
}

StackInterface::THREADSTATUS Thread::_resume()
{
    THREADSTATUS result = (THREADSTATUS)_thread.resume(_c_state, _pusher.count());

    return result;
}

int Thread::resultCount()
{
    return gettop() - _beform_stack_size;
}

StackInterface::THREADSTATUS Thread::status()
{
    return _thread.status();
}

Thread::Result Thread::operator[](int i)
{
    Result r(_thread.getInterface(), i + _beform_stack_size); return r;
}

RawInterface Thread::getThreadInterface()   
{ 
    return _thread.getInterface(); 
}

_NAME_END
