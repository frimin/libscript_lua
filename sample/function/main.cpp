#include <iostream>
#include "libscript.h"

int foo2(Args& args, Pusher& returns)
{
    std::cout << "This is foo2" << std::endl;

    if (args[1].isTable())
    {
        Table t = args[1].toValue();

        for (Table::Accessor acc(t); !acc.end(); acc.next())
        {
			Stack::TYPE type = acc.value().type();

            switch (type)
            {
            case Stack::T_Number:
                std::cout << acc.value().toInteger() << std::endl;
                break;
            case Stack::T_String:
                std::cout << acc.value().toString() << std::endl;
                break;
            case Stack::T_Function:
            {
                Function f = acc.value();
                f();
            }
                break;
            default:
                std::cout << "<" << acc.value().typeName() << ">" << std::endl;
                break;
            }
        }
    }

    return returns.count();
}


int foo3(int n)
{
    std::cout << "This is foo3" << std::endl;
    return n + 1;
}

void foo4(const char* s)
{
    std::cout << "This is foo4" << std::endl;
    std::cout << s << std::endl;
}

int main()
{
    Script script;

    script.getGlobalTable().set("foo1", script.newFunction("print(\"This is foo1\")"));
    script.getGlobalTable().set("foo2", script.newFunction(foo2));
    script.getGlobalTable().set("foo3", script.newFunction(foo3));
    script.getGlobalTable().set("foo4", script.newFunction(foo4));
    script.execString("function foo5(...) local t = {...} print(\"This is foo1\") print(\"have \" .. #t .. \" args\") end");

    script.execString("foo1()");

    Function foo2 = script.getGlobal("foo2");

    foo2(script.newTable(1, 2, 3, "hello", script.newTable(666), script.newFunction("print(3.14)")));

    script.execString("print(foo3(2))");

    script.execString("foo4(\"call foo4\")");

    Function foo5 = script.getGlobal("foo5");
    foo5("1", 22, 999);
}