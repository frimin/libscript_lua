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
            .create_forward("MyClass", [](Args& arg)->MyClass*{
            switch (arg.count())
            {
            case 0:
                return new MyClass();
            case 1:
                return new MyClass(arg[1].toInteger());
            default:
                return nullptr;
            }
        })
            .destroy("__gc")
            .destroy("free")
            .method("foo2", &MyClass::foo2)
            .method_forward("foo", [](MyClass* _this, Args& args, Pusher& pusher)->int{
            std::cout << _this->n << std::endl;
            _this->n++;
            return 0;
        });
            ;

        script.execString(R"( 
            a = MyClass()
            a:foo()
            a:foo()
            a:foo()

            b = MyClass(0)
            b:foo()
            b:foo()
            b:foo()

            a:free()
            b:free()

            c = MyClass(6666)
        )");

        MyClass* c = script["c"].toClass<MyClass>();

        std::cout << "c's value is :" << c->n << std::endl;
    }

    std::cout << ":)" << std::endl;
}