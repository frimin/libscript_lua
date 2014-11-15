Simple Lua C++ Bind
==============

Its simple to using, like:

```code
#include "libscript.h"

int main()
{
    Script script;
    script.execString("print(\"hello world\")");
}
```

**Using function & closure**

```code
#include "libscript.h"

void foo(const char* s)
{
    std::cout << "This is foo" << std::endl;
}

int foo2(RawInterface raw)
{
    UpValue v(raw, 1);
    std::cout << v.toInteger() << std::endl;
    v.reset(v.toInteger() + 1);
    return 0;
}

int main()
{
    Script script;
    
    script.getGlobalTable().set("foo", 
        script.newFunction(foo));
    script.getGlobalTable().set("foo2",
        script.newClosure(foo2, 1));
    script.execString("foo() for i=1, 10 do foo2() end");
}
```

**Using table**

```code
Script script;

script.execString("function foo(t) for i, v in ipairs(t) do print(i, v) end end");

Function foo = script.getGlobal("foo");

foo(script.newTable("hello", "world", 3.1415, script.newTable()));
```

License
============== 

* Redistribution and use of this software in source and binary forms,
  with or without modification, are permitted provided that the following
  conditions are met:

* Redistributions of source code must retain the above
  copyright notice, this list of conditions and the
  following disclaimer.

* Redistributions in binary form must reproduce the above
  copyright notice, this list of conditions and the
  following disclaimer in the documentation and/or other
  materials provided with the distribution.

* Neither the name of the ray team, nor the names of its
  contributors may be used to endorse or promote products
  derived from this software without specific prior
  written permission of the ray team.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.