#include "libscript.h"

int main()
{
    Script script;

    std::string code = "\
        function userthread(n)\
            for i=1, 10 do\
                n = coroutine.yield(i + n)\
            end\
        end\
    ";

    script.execString(code.c_str());

    Thread thread = script.newThread(script["userthread"]);

    thread.type();

    int n = 0;

	Stack::THREADSTATUS state = Stack::STATUS_YIELD;

    while (state == Stack::STATUS_YIELD)
    {
        state = thread.resume(++n);
        
        switch (state)
        {
        
        case Stack::STATUS_YIELD:
            std::cout << thread[1].toInteger() << std::endl;
            break;
        case Stack::STATUS_OK:
            std::cout << "done" << std::endl;
            break;
        default:
            //errors
            std::cout << thread[1].toString() << std::endl;
            break;
        }
    }
}