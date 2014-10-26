#include <iostream>
#include "libscript.h"

int printtable(Args& args, Pusher& pusher)
{
    if (args[1].isTable())
    {
        Table t = args[1].toValue();

        for (Table::Accessor acc(t); !acc.end(); acc.next())
        {
            auto type = acc.key().type();

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

    return pusher.count();
}

int main()
{
    Script script;
    script.execString("function foo(n) print(n) end");

    Function foo = script["foo"];
    foo(256);

    script.getGlobalTable().set("printtable", getFunction(script, printtable));
    script.execString(R"( printtable({123, "hello", "456"}) )");
}