#include <iostream>
#include "libscript.h"

void foo2(Args& args, Pusher& pusher)
{
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
            default:
                break;
            }
        }
    }
}


int foo3(int n)
{
    return n + 1;
}

void foo4(const char* s)
{
    std::cout << s << std::endl;
}


int main()
{
    Script script;

    script.execString("function foo1(s) print(s) end");
    script.getGlobalTable().set("foo2", getForwardFunction(script, foo2));

    script.getGlobalTable().set("foo3", getFunction(script, foo3));
    script.getGlobalTable().set("foo4", getFunction(script, foo4));

    // Call foo1
    Function foo1 = script.getGlobal("foo1");
    foo1("call foo1");
    // Call foo2
    script.execString("foo2({\"\tprint\", \"\ta\", \"\ttable\"})");

    // Call foo3
    script.execString("print(foo3(2))");
    // Call foo4
    script.execString("foo4(\"call foo4\")");
}