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

#ifndef _H_LIBSCRIPT_CALL_DISPATCHER_H_
#define _H_LIBSCRIPT_CALL_DISPATCHER_H_

#include "libscript_value.h"
#include "libscript_pusher.h"
#include "libscript_args.h"

/// @addtogroup script
/// @{

// Call dispatcher

_NAME_BEGIN

namespace CD
{

enum class ARGS_EVALUATE
{
    NOT_TEST,
    LEFT_TO_RIGHT,
    RIGHT_TO_LEFT,
};

class EXPORT ArgsIterator : public Stack
{
public:
    ArgsIterator(Args args, bool reverse = false, int ignoreBottom = 0);
    StackValue GetAndToNext();

    static ARGS_EVALUATE getStaticSequence();
    static ARGS_EVALUATE getConstructorSequence();
    static ARGS_EVALUATE getMethodSequence();
private:
    int _begin;
    int _end;
    int _step;
    Args _args;
};

class Function
{
public:
    typedef int(*Forward)(Args& args, Pusher& pusher);
    
    template <typename _Func>
    static void push(Stack& stack, _Func func)
    {
        auto *w = (Wrapper<_Func> *)stack.newuserdata(sizeof(Wrapper<_Func>));
        w->value = func;
        stack.pushcclosure(Function::dispatcher<_Func>, 1);
    }

    static void pushForward(Stack& stack, Function::Forward func)
    {
        auto w = (Wrapper<Function::Forward>*)stack.newuserdata(sizeof(Wrapper<Function::Forward>));
        w->value = func;
        stack.pushcclosure(Function::forwardDispatcher, 1);
    }

private:
    template <typename _Func>
    static int dispatcher(RawInterface raw)
    {
        Stack stack(raw);

        auto w = (Wrapper<_Func>*)stack.touserdata(Stack::upvalueindex(1));

        return Function::caller(stack, w->value);
    }

    static int forwardDispatcher(RawInterface raw)
    {
        Args args(raw);

        auto w = (Wrapper<Function::Forward>*)args.touserdata(Stack::upvalueindex(1));

        Pusher pusher(raw);

        return w->value(args, pusher);
    }
    
    template <typename ... _Args>
    static int caller(Stack& stack, void(*func)(_Args ...))
    {
        Args args(stack.getInterface());
        ArgsIterator argIter(args,
                             ArgsIterator::getStaticSequence() == ARGS_EVALUATE::RIGHT_TO_LEFT, 0);
        
        func(static_cast<_Args>(argIter.GetAndToNext())...);
        
        return 0;
    }
    
    template <typename _Rt, typename ... _Args>
    static int caller(Stack& stack,  _Rt(*func)(_Args ...))
    {
        Args args(stack.getInterface());
        ArgsIterator argIter(args,
            ArgsIterator::getStaticSequence() == ARGS_EVALUATE::RIGHT_TO_LEFT, 0);
        
        _Rt rt = func(static_cast<_Args>(argIter.GetAndToNext())...);
        
        Pusher pusher(stack.getInterface());
        
        pusher.push(rt);
        
        return pusher.count();
    }
};

template<typename _Class>
class Constructor
{
public:
    typedef _Class*(*Forward)(Args& args);

    template <typename ... _Args>
    static void push(Stack& stack)
    {
        stack.pushcfunction(Constructor<_Class>::dispatcher<_Args ...>);
    }

    template <typename _Creator>
    static void push(Stack& stack, _Creator creator)
    {
        auto w =
            (Wrapper<_Creator>*)stack.newuserdata(sizeof(Wrapper<_Creator>));
        w->value = creator;
        stack.pushcclosure(CD::Constructor<_Class>::userDispatcher<_Creator>, 1);
    }

    static void pushForward(Stack& stack, typename Constructor<_Class>::Forward method)
    {
        auto w =
            (Wrapper<Constructor<_Class>::Forward>*)stack.newuserdata(sizeof(Wrapper<Constructor<_Class>::Forward>));
        w->value = method;
        stack.pushcclosure(CD::Constructor<_Class>::forwardDispatcher, 1);
    }

private:
    template <typename ... _Args>
    static int dispatcher(RawInterface raw)
    {
        Args args(raw);
        ArgsIterator argIter(args,
            ArgsIterator::getConstructorSequence() != ARGS_EVALUATE::RIGHT_TO_LEFT, 0);
        return setupMetaTable(args, new _Class(static_cast<_Args>(argIter.GetAndToNext())...));
    }

    template <typename _Creator>
    static int userDispatcher(RawInterface raw)
    {
        Stack stack(raw);

        auto w = (Wrapper<_Creator>*)stack.touserdata(Stack::upvalueindex(1));

        Args args(raw);
        ArgsIterator argIter(args,
            ArgsIterator::getConstructorSequence() != ARGS_EVALUATE::RIGHT_TO_LEFT, 0);

        return setupMetaTable(argIter, Constructor<_Class>::caller(argIter, w->value));
    }

    static int forwardDispatcher(RawInterface raw)
    {
        Args args(raw);

        auto w = (Wrapper<Constructor<_Class>::Forward>*)args.touserdata(Stack::upvalueindex(1));

        return setupMetaTable(args, w->value(args));
    }

    template <typename ... _Args>
    static _Class* caller(ArgsIterator& argIter, _Class*(*creator)(_Args ...))
    {
        return creator(static_cast<_Args>(argIter.GetAndToNext())...);
    }

    static int setupMetaTable(Stack& stack, _Class* obj)
    {
        if (obj == nullptr)
           return stack.error_L("Bad initialization");;

        std::string metaname(typeid(_Class).name());

        auto info = (ClassInfo<_Class>*)stack.newuserdata(sizeof(ClassInfo<_Class>));

        info->obj = obj;
        info->ref = false;
        info->readonly = false;

        stack.getfield(Stack::REGISTRYINDEX(), metaname.c_str());
        stack.setmetatable(-2);

        return 1;
    }
};

template<typename _Class>
class Method
{
public:
    typedef int(*Forward)(_Class* _this, Args& args, Pusher& pusher);

    template<typename _Method>
    static void push(Stack& stack, _Method method)
    {
        Wrapper<_Method>* w = (Wrapper<_Method>*)stack.newuserdata(sizeof(Wrapper<_Method>));
        w->value = method;
        stack.pushcclosure(Method<_Class>::dispatcher<_Method>, 1);
    }

    static void pushForward(Stack& stack, typename Method<_Class>::Forward method)
    {
        Wrapper<Method<_Class>::Forward>* w =
            (Wrapper<Method<_Class>::Forward>*)stack.newuserdata(sizeof(Wrapper<Method<_Class>::Forward>));
        w->value = method;
        stack.pushcclosure(Method<_Class>::forwardDispatcher, 1);
    }

private:
    template<typename _Method>
    static int dispatcher(RawInterface raw)
    {
        Stack stack(raw);

        auto info = (ClassInfo<_Class>*)stack.touserdata(1);

        auto w = (Wrapper<_Method>*)stack.touserdata(Stack::upvalueindex(1));

        if (info->readonly)
        {
            bool readonlyMethod = stack.tointeger(Stack::upvalueindex(2)) == 1;
            if (readonlyMethod == false)
            {
                return stack.error_L("Try to call readonly class's usually method");
            }
        }

        return Method<_Class>::caller(stack, info, w->value);
    }

    static int forwardDispatcher(RawInterface raw)
    {
        Args args(raw);

        if (!args[1].isUserdata())
        {
            return args.error_L("Bad arg");
        }

        auto info = (ClassInfo<_Class>*)args.touserdata(1);

        auto w = (Wrapper<Method<_Class>::Forward>*)args.touserdata(Stack::upvalueindex(1));

        if (info->readonly)
        {
            bool readonlyMethod = args.tointeger(Stack::upvalueindex(2)) == 1;
            if (readonlyMethod == false)
            {
                return args.error_L("Try to call readonly class's usually method");
            }
        }

        Pusher pusher(raw);

        _Class* _this = args[1].toClass<_Class>();;

        return w->value(_this, args, pusher);
    }

    template <typename ... _Args>
    static int caller(Stack& stack, ClassInfo<_Class>* info, void (_Class::*method)(_Args ...))
    {
        if (info->readonly)
        {
            return stack.error_L("Try to call readonly class's usually method");
        }

        Args args(stack.getInterface());
        ArgsIterator argIter(args,
            ArgsIterator::getMethodSequence() == ARGS_EVALUATE::RIGHT_TO_LEFT, 0);

        (info->obj->*method)(static_cast<_Args>(argIter.GetAndToNext())...);

        return 0;
    }

    template <typename _Rt, typename ... _Args>
    static int caller(Stack& stack, ClassInfo<_Class>* info, _Rt(_Class::*method)(_Args ...))
    {
        if (info->readonly)
        {
            return stack.error_L("Try to call readonly class's usually method");
        }

        Args args(stack.getInterface());
        ArgsIterator argIter(args,
            ArgsIterator::getMethodSequence() == ARGS_EVALUATE::RIGHT_TO_LEFT, 0);

        _Rt rt = (info->obj->*method)(static_cast<_Args>(argIter.GetAndToNext())...);

        Pusher pusher(stack.getInterface());

        pusher.push(rt);

        return pusher.count();
    }

    template <typename ... _Args>
    static int caller(Stack& stack, ClassInfo<_Class>* info, void (_Class::*method)(_Args ...) const)
    {
        Args args(stack.getInterface());
        ArgsIterator argIter(args,
            ArgsIterator::getMethodSequence() == ARGS_EVALUATE::RIGHT_TO_LEFT, 1);

        (info->obj->*method)(static_cast<_Args>(argIter.GetAndToNext())...);

        return 0;
    }

    template <typename _Rt, typename ... _Args>
    static int caller(Stack& stack, ClassInfo<_Class>* info, _Rt(_Class::*method)(_Args ...) const)
    {
        Args args(stack.getInterface());
        ArgsIterator argIter(args,
            ArgsIterator::getMethodSequence() == ARGS_EVALUATE::RIGHT_TO_LEFT, 1);

        _Rt rt = (info->obj->*method)(static_cast<_Args>(argIter.GetAndToNext())...);

        Pusher pusher(stack.getInterface());

        pusher.push(rt);

        return pusher.count();
    }
};

template<typename _Class>
class Destructor
{
public:
    static void push(Stack& stack)
    {
        stack.pushcfunction(Destructor<_Class>::dispatcher);
    }

    static void push(Stack& stack, void (userDispatcher)(_Class*))
    {
        auto w =
            (Wrapper<void(*)(_Class*)>*)stack.newuserdata(sizeof(Wrapper<void(*)(_Class*)>));
        w->value = userDispatcher;
        stack.pushcclosure(Destructor<_Class>::userDispatcher, 1);
    }

    static int dispatcher(RawInterface raw)
    {
        Args args(raw);
        ArgsIterator argIter(args,
            ArgsIterator::getConstructorSequence() != ARGS_EVALUATE::RIGHT_TO_LEFT, 0);

        Stack stack(raw);

        ClassInfo<_Class>* info = (ClassInfo<_Class>*)stack.touserdata(1);

        if (info->ref)
            return 0;

        if (info->obj)
        {
            delete info->obj;
            info->obj = nullptr;
        }

        return 0;
    }

    typedef void(*Destroyer)(_Class*);

    static int userDispatcher(RawInterface raw)
    {
        Stack stack(raw);

        assert(stack.gettop() == 1);

        auto w = (Wrapper<Destroyer>*)stack.touserdata(Stack::upvalueindex(1));

        ClassInfo<_Class>* info = (ClassInfo<_Class>*)stack.touserdata(1);

        if (info->ref)
            return 0;

        if (info->obj)
        {
            w->value(info->obj);
            info->obj = nullptr;
        }

        return 0;
    }
};

}

/// @}

_NAME_END

#endif
