#include <sstream>
#include <string>
#include "libscript.h"

int main()
{
    Script script;

    Table global = script.getGlobalTable();
    global.set("t", script.newTable());

    Table t3 = script.newTable();
    t3.set("123", "456");

    Table t2 = script.newTable();
    t2.set("7", "777");
    t2.set("t3", t3);

    Table t = global["t"];

    if (t.isTable())
    {
        t.set("test", "My name is jack");
        t.set(0, 3.1415926);
        t.set(1, 77);
        t.set("t2", t2);

        TableStringDumper dumper = t;

        std::ostringstream stream;

        dumper.dump("t", stream);

        std::cout << stream.str() << std::endl;
    }
}