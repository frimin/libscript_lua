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

#include <memory.h>

_NAME_BEGIN

Thread::Thread(const Stack& stack) : Value(stack), _thread(nullptr)
{
    pushRef(T_Thread);
    _thread = Stack(tothread(-1));
    pop(1);

    _pusher = std::make_shared<Pusher>(_thread.getInterface());
}

Thread::Thread(const Value& value) : Value(value), _thread(nullptr)
{
    pushRef(T_Thread);
    _thread = Stack(tothread(-1));
    pop(1);

    _pusher = std::make_shared<Pusher>(_thread.getInterface());
}

void Thread::load(const std::string& fileName)
{
    if (_thread.loadfile_L(fileName.c_str()))
    {
        std::string errorInfo = _thread.tostring(-1);
        _thread.pop(1);
        SCRIPT_EXCEPTION(errorInfo);
    }
}

void Thread::loadString(const std::string& str)
{
    if (_thread.loadbuffer_L(str.c_str(), str.size(), (str.substr(0, 10) + "...").c_str()))
    {
        std::string errorInfo = _thread.tostring(-1);
        _thread.pop(1);
        SCRIPT_EXCEPTION(errorInfo);
    }
}

bool Thread::loadSafe(const std::string& fileName, std::string* errorOut)
{
    if (_thread.loadfile_L(fileName.c_str()))
    {
        if(errorOut)
            *errorOut = _thread.tostring(-1);
        _thread.pop(1);
        return false;
    }
    return true;
}

bool Thread::loadStringSafe(const std::string& str, std::string* errorOut)
{
    if (_thread.loadbuffer_L(str.c_str(), str.size(), (str.substr(0, 10) + "...").c_str()))
    {
        if(errorOut)
            *errorOut = _thread.tostring(-1);
        _thread.pop(1);
        return false;
    }
    
    return true;
}

Value Thread::newFunction(const std::string& script)
{
    if (loadbuffer_L(script.c_str(), script.size(), (script.substr(0, 10) + "...").c_str()))
    {
        std::string errorInfo = tostring(-1);
        pop(1);
        SCRIPT_EXCEPTION(errorInfo);
    }

    return _thread;
}

Value Thread::newFunction(CFunction function)
{
    pushcfunction(function);
    return _thread;
}

Value Thread::newTable()
{
    newtable();
    return _thread;
}

Value Thread::getGlobal(const std::string& name)
{
    getglobal(name.c_str());
    return _thread;
}

Table Thread::getGlobalTable()
{
    pushglobaltable();
    Value value(_thread);
    return value;
}

void Thread::resultReset()
{ 
    _pusher->reset();
    _beform_stack_size = gettop(); 
}

Stack::THREADSTATUS Thread::_resume()
{
    THREADSTATUS result = (THREADSTATUS)_thread.resume(_c_state, _pusher->count());

    return result;
}

int Thread::resultCount()
{
    return gettop() - _beform_stack_size;
}

Stack::THREADSTATUS Thread::status()
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
