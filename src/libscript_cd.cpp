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

#include "libscript_cd.h"

_NAME_BEGIN

namespace CD
{

static ARGS_EVALUATE _s_static_call_sequence = ARGS_EVALUATE::NOT_TEST;
static ARGS_EVALUATE _s_constructor_call_sequence = ARGS_EVALUATE::NOT_TEST;
static ARGS_EVALUATE _s_method_call_sequence = ARGS_EVALUATE::NOT_TEST;

class _testClass
{
public:
    _testClass(int a, int b) {  }
    void foo(int a, int b) {  }
};

void _testStaticCall(int a, int b) { }


int _test(bool isLeft, ARGS_EVALUATE& v)
{
    if (v != ARGS_EVALUATE::NOT_TEST)
        return 0;
    if (isLeft)
        v = ARGS_EVALUATE::LEFT_TO_RIGHT;
    else
        v = ARGS_EVALUATE::RIGHT_TO_LEFT;
    return 0;
}

int _doTest()
{
    _testStaticCall(
        _test(true, _s_static_call_sequence),
        _test(false, _s_static_call_sequence)
        );
    _testClass t(
        _test(true, _s_constructor_call_sequence),
        _test(false, _s_constructor_call_sequence));
    t.foo(
        _test(true, _s_method_call_sequence),
        _test(false, _s_method_call_sequence));
    return 0;
}

const int i = _doTest();

ArgsIterator::ArgsIterator(Args args, bool reverse, int ignoreBottom) : Stack(args), _args(args)
{
    if (!reverse)
    {
        _begin = 1 + ignoreBottom;
        _end = _args.count() + 1;
        _step = 1;
    }
    else
    {
        _begin = _args.count();
        _end = 0 + ignoreBottom;
        _step = -1;
    }
}

StackValue ArgsIterator::GetAndToNext()
{
    if (_begin == _end)
        SCRIPT_EXCEPTION("Bad iterator");

    StackValue result(*this, _begin);

    _begin += _step;

    return result;
}

ARGS_EVALUATE ArgsIterator::getStaticSequence()
{
    return _s_static_call_sequence;
}

ARGS_EVALUATE ArgsIterator::getConstructorSequence()
{
    return _s_constructor_call_sequence;
}

ARGS_EVALUATE ArgsIterator::getMethodSequence()
{
    return _s_method_call_sequence;
}

}

_NAME_END