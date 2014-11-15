#include "libscript.h"

class MyClass
{
public:
    MyClass()
    {
        n = 256;
    }

    MyClass(int num)
    {
        n = num;
    }

    ~MyClass()
    {
        std::cout << "free" << std::endl;
    }
    
    int foo2()
    {
        return 0;
    }

    int n;
};

int main()
{
    {
        Script script;
    
        BindClass<MyClass>(script)
            .createForward("MyClass", [](Args& arg)->MyClass*{
            switch (arg.count())
            {
            case 0:
                return new MyClass();
            case 1:
                return new MyClass(arg[1].toInteger());
            default:
                return NULL;
            }
        })
            .destroy("__gc")
            .destroy("free")
            .method("foo2", &MyClass::foo2)
            .methodForward("print_and_increment", [](MyClass* _this, Args& args, Pusher& returns) -> int{
            std::cout << _this->n << std::endl;
            _this->n++;
            return returns.count();
        })
            .methodReadOnlyForward("print_only", [](const MyClass* _this, Args& args, Pusher& returns) -> int{
            std::cout << _this->n << std::endl;
            return returns.count();
        })
            ;

        script.execString(R"( 
            a = MyClass()
            a:print_and_increment()
            a:print_and_increment()
            a:print_and_increment()

            b = MyClass(0)
            b:print_and_increment()
            b:print_and_increment()
            b:print_and_increment()

            a:free()
            b:free()

            c = MyClass(6666)
        )");

        MyClass* c = script["c"].toClass<MyClass>();

        std::cout << "c's value is :" << c->n << std::endl;

        MyClass d(31415);

        script.getGlobalTable().set("d", RefClass<MyClass>((const MyClass*)&d));

        script.execString(R"(d:print_only())");

        std::string errorinfo;

        if(!script.execStringSafe(R"(d:print_and_increment())", &errorinfo))
        {
            std::cout << errorinfo << std::endl;
        }
    }

    std::cout << ":)" << std::endl;
}