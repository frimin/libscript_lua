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

#ifndef _H_LIBSCRIPT_FWD_H_
#define _H_LIBSCRIPT_FWD_H_

#include <tuple>
#include <initializer_list>
#define FINAL final
#define TODELETE = delete

#include <string>
#include <memory>
#include <cassert>

#ifndef _NAME_BEGIN
#define _NAME_BEGIN
#endif

#ifndef _NAME_END
#define _NAME_END
#endif

#ifndef EXPORT
#define EXPORT
#endif

#ifndef SCRIPT_EXCEPTION
#include <exception>
#define SCRIPT_EXCEPTION(s) throw std::logic_error(s)
#endif

struct lua_State;

_NAME_BEGIN

class Script;

typedef lua_State* RawInterface;

typedef int         (*CFunction)(RawInterface raw);
typedef int         (*ChunkWriterFunc) (RawInterface raw, const void* p, size_t sz, void* ud);
typedef const char* (*ChunkReaderFunc) (RawInterface raw, void* data, size_t* size);
typedef void*       (*MemAllocFunc) (void *ud, void *ptr, size_t osize, size_t nsize);

template<typename _Class> struct ClassInfo {
    _Class* obj;
    bool ref;
    bool readonly;
};

struct MemAllocation
{
    virtual void* alloc(std::size_t s) = 0;
    virtual void free(void* ptr, std::size_t s) = 0;
};

struct Class
{
    void* c;
    bool ref;
    bool readonly;
    std::string metaname;
};

typedef struct {} VOID_T;

template<typename _T> struct Wrapper {
    _T value;
};

template<typename _Class>
class RefClass : public Class
{
public:
    RefClass(_Class* _c)
    {
        c = _c;
        metaname = typeid(_Class).name();
        ref = true;
        readonly = false;
    }

    RefClass(_Class& _c)
    {
        c = &_c;
        metaname = typeid(_Class).name();
        ref = true;
        readonly = false;
    }

    // readonly
    RefClass(const _Class* _c)
    {
        c = const_cast<_Class*>(_c);
        metaname = typeid(_Class).name();
        ref = true;
        readonly = true;
    }
};

template<typename _Class>
class GCClass : public Class
{
public:
    GCClass(_Class* _c)
    {
        c = _c;
        metaname = typeid(_Class).name();
        ref = false;
        readonly = false;
    }
};

_NAME_END

#endif
