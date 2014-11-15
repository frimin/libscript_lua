#include "libscript.h"

int main()
{
    Script script;

    script.execString("function foo(t) for i, v in ipairs(t) do print(i, v) end end");

    Function foo = script.getGlobal("foo");

    foo(script.newTable("hello", "world", 3.1415, script.newTable()));
}