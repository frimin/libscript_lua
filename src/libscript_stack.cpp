// +----------------------------------------------------------------------
// | Copyright (c) 2013-2014.
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

#include "libscript_stack.h"

#include <cstdarg>
#include "libscript_luaapi.h"

_NAME_BEGIN

int StackInterface::REGISTRYINDEX()
{
    return LUA_REGISTRYINDEX;
}

const StackInterface::P_Nil StackInterface::Nil = StackInterface::P_Nil();

StackInterface::StackInterface(RawInterface state) : _c_state(state)
{

}

StackInterface::StackInterface(const StackInterface& stack) : _c_state(stack._c_state)
{

}

StackInterface::~StackInterface()
{
    
}

int StackInterface::absindex(int index)
{
    return lua_absindex(LUASTATE(_c_state), index);
}

void StackInterface::arith(ARITHOP op)
{
    lua_arith(LUASTATE(_c_state), op);
}

CFunction StackInterface::atpanic(CFunction panicf)
{
    return lua_atpanic(LUASTATE(_c_state), panicf);
}

void StackInterface::call(int nargs, int nresults)
{
    lua_call(LUASTATE(_c_state), nargs, nresults);
}

void StackInterface::callk(int nargs, int nresults, int ctx, CFunction k)
{
    lua_callk(LUASTATE(_c_state), nargs, nresults, ctx, k);
}

void StackInterface::checkstack(int extra)
{
    lua_checkstack(LUASTATE(_c_state), extra);
}

void StackInterface::close(RawInterface L)
{
    lua_close(L);
}

int StackInterface::compare(int index1, int index2, COMPAREOP op)
{
    return lua_compare(LUASTATE(_c_state), index1, index2, op);
}

void StackInterface::concat(int n)
{
    lua_concat(LUASTATE(_c_state), n);
}

void StackInterface::copy(int fromidx, int toidx)
{
    lua_copy(LUASTATE(_c_state), fromidx, toidx);
}

void StackInterface::createtable(int narr, int nrec)
{
    lua_createtable(LUASTATE(_c_state), narr, nrec);
}

int StackInterface::dump(ChunkWriterFunc writer, void* data)
{
    return lua_dump(LUASTATE(_c_state), writer, data);
}

int StackInterface::error()
{
    return lua_error(LUASTATE(_c_state));
}

int StackInterface::gc(GCBEHAVIOR what, int data)
{
    return lua_gc(LUASTATE(_c_state), what, data);
}

int StackInterface::getctx(int* ctx)
{
    return lua_getctx(LUASTATE(_c_state), ctx);
}

void StackInterface::getfield(int index, const char* k)
{
    lua_getfield(LUASTATE(_c_state), index, k);
}

void StackInterface::getglobal(const char* name)
{
    lua_getglobal(LUASTATE(_c_state), name);
}

int StackInterface::getmetatable(int index)
{
    return lua_getmetatable(LUASTATE(_c_state), index);
}

void StackInterface::gettable(int index)
{
    lua_gettable(LUASTATE(_c_state), index);
}

int StackInterface::gettop()
{
    return lua_gettop(LUASTATE(_c_state));
}

void StackInterface::getuservalue(int index)
{
    lua_getuservalue(LUASTATE(_c_state), index);
}

void StackInterface::insert(int index)
{
    lua_insert(LUASTATE(_c_state), index);
}

bool StackInterface::isboolean(int index)
{
    return lua_isboolean(LUASTATE(_c_state), index);
}

bool StackInterface::iscfunction(int index)
{
    return lua_iscfunction(LUASTATE(_c_state), index);
}

bool StackInterface::isfunction(int index)
{
    return lua_isfunction(LUASTATE(_c_state), index);
}

bool StackInterface::islightuserdata(int index)
{
    return lua_islightuserdata(LUASTATE(_c_state), index);
}

bool StackInterface::isnil(int index)
{
    return lua_isnil(LUASTATE(_c_state), index);
}

bool StackInterface::isnone(int index)
{
    return lua_isnone(LUASTATE(_c_state), index);
}

bool StackInterface::isnoneornil(int index)
{
    return lua_isnoneornil(LUASTATE(_c_state), index);
}

bool StackInterface::isnumber(int index)
{
    return lua_isnumber(LUASTATE(_c_state), index);
}

bool StackInterface::isstring(int index)
{
    return lua_isstring(LUASTATE(_c_state), index);
}

bool StackInterface::istable(int index)
{
    return lua_istable(LUASTATE(_c_state), index);
}

bool StackInterface::isthread(int index)
{
    return lua_isthread(LUASTATE(_c_state), index);
}

bool StackInterface::isuserdata(int index)
{
    return lua_isuserdata(LUASTATE(_c_state), index);
}

void StackInterface::len(int index)
{
    lua_len(LUASTATE(_c_state), index);
}

int StackInterface::load(ChunkReaderFunc reader, void* data, const char* source, const char* mode)
{
    return lua_load(LUASTATE(_c_state), reader, data, source, mode);
}

RawInterface StackInterface::newstate(MemAllocFunc f, void *ud)
{
    return lua_newstate(f, ud);
}

void StackInterface::newtable()
{
    lua_newtable(LUASTATE(_c_state));
}

RawInterface StackInterface::newthread()
{
    return lua_newthread(LUASTATE(_c_state));
}

void* StackInterface::newuserdata(std::size_t size)
{
    return lua_newuserdata(LUASTATE(_c_state), size);
}

int StackInterface::next(int index)
{
    return lua_next(LUASTATE(_c_state), index);
}

StackInterface::THREADSTATUS StackInterface::pcall(int nargs, int nresults, int msgh)
{
    return (StackInterface::THREADSTATUS)lua_pcall(LUASTATE(_c_state), nargs, nresults, msgh);
}

StackInterface::THREADSTATUS StackInterface::pcallk(int nargs, int nresults, int errfunc, int ctx, CFunction k)
{
    return (StackInterface::THREADSTATUS)lua_pcallk(LUASTATE(_c_state), nargs, nresults, errfunc, ctx, k);
}

void StackInterface::pop(int n)
{
    lua_pop(LUASTATE(_c_state), n);
}

void StackInterface::pushboolean(int b)
{
    lua_pushboolean(LUASTATE(_c_state), b);
}

void StackInterface::pushcclosure(CFunction fn, int n)
{
    lua_pushcclosure(LUASTATE(_c_state), fn, n);
}

void StackInterface::pushcfunction(CFunction f)
{
    lua_pushcclosure(LUASTATE(_c_state), f, 0);
}

const char* StackInterface::pushfstring(const char* fmt, ...)
{
    va_list vlArgs;
    va_start(vlArgs, fmt);
    const char* s = lua_pushfstring(LUASTATE(_c_state), fmt, vlArgs);
    va_end(vlArgs);
    return s;
}

void StackInterface::pushglobaltable()
{
    lua_pushglobaltable(LUASTATE(_c_state));
}

void StackInterface::pushinteger(long long n)
{
    lua_pushinteger(LUASTATE(_c_state), n);
}

void StackInterface::pushlightuserdata(void* p)
{
    lua_pushlightuserdata(LUASTATE(_c_state), p);
}

void StackInterface::pushlstring(const char* s, std::size_t len)
{
    lua_pushlstring(LUASTATE(_c_state), s, len);
}

void StackInterface::pushnil()
{
    lua_pushnil(LUASTATE(_c_state));
}

void StackInterface::pushnumber(double n)
{
    lua_pushnumber(LUASTATE(_c_state), n);
}

const char* StackInterface::pushstring(const char* s)
{
    return lua_pushstring(LUASTATE(_c_state), s);
}

int StackInterface::pushthread()
{
    return lua_pushthread(LUASTATE(_c_state));
}

void StackInterface::pushunsigned(unsigned int n)
{
    lua_pushunsigned(LUASTATE(_c_state), n);
}

void StackInterface::pushvalue(int index)
{
    lua_pushvalue(LUASTATE(_c_state), index);
}

const char* StackInterface::pushvfstring(const char* fmt, va_list argp)
{
    return lua_pushvfstring(LUASTATE(_c_state), fmt, argp);
}

int StackInterface::rawequal(int index1, int index2)
{
    return lua_rawequal(LUASTATE(_c_state), index1, index2);
}

void StackInterface::rawget(int index)
{
    lua_rawget(LUASTATE(_c_state), index);
}

void StackInterface::rawgeti(int index, int n)
{
    lua_rawgeti(LUASTATE(_c_state), index, n);
}

void StackInterface::rawgetp(int index, const void* p)
{
    lua_rawgetp(LUASTATE(_c_state), index, p);
}

std::size_t StackInterface::rawlen(int index)
{
    return lua_rawlen(LUASTATE(_c_state), index);
}

void StackInterface::rawset(int index)
{
    lua_rawset(LUASTATE(_c_state), index);
}

void StackInterface::rawseti(int index, int n)
{
    lua_rawseti(LUASTATE(_c_state), index, n);
}

void StackInterface::rawsetp(int index, const void* p)
{
    lua_rawgetp(LUASTATE(_c_state), index, p);
}

void StackInterface::register_(const char* name, CFunction f)
{
    lua_register(LUASTATE(_c_state), name, f);
}

void StackInterface::remove(int index)
{
    lua_remove(LUASTATE(_c_state), index);
}

void StackInterface::replace(int index)
{
    lua_replace(LUASTATE(_c_state), index);
}

StackInterface::THREADSTATUS StackInterface::resume(RawInterface from, int nargs)
{
    return (THREADSTATUS)lua_resume(LUASTATE(_c_state), LUASTATE(from), nargs);
}

void StackInterface::setfield(int index, const char* k)
{
    lua_setfield(LUASTATE(_c_state), index, k);
}

void StackInterface::setglobal(const char* name)
{
    lua_setglobal(LUASTATE(_c_state), name);
}

void StackInterface::setmetatable(int index)
{
    lua_setmetatable(LUASTATE(_c_state), index);
}

void StackInterface::settable(int index)
{
    lua_settable(LUASTATE(_c_state), index);
}

void StackInterface::settop(int index)
{
    lua_settop(LUASTATE(_c_state), index);
}

void StackInterface::setuservalue(int index)
{
    lua_setuservalue(LUASTATE(_c_state), index);
}

StackInterface::THREADSTATUS StackInterface::status()
{
    return (THREADSTATUS)lua_status(LUASTATE(_c_state));
}

bool StackInterface::toboolean(int index)
{
    return lua_toboolean(LUASTATE(_c_state), index) == 1;
}

CFunction StackInterface::tocfunction(int index)
{
    return lua_tocfunction(LUASTATE(_c_state), index);
}

long long StackInterface::tointeger(int index)
{
    return lua_tointeger(LUASTATE(_c_state), index);
}

long long StackInterface::tointegerx(int index, int* isnum)
{
    return lua_tointegerx(LUASTATE(_c_state), index, isnum);
}

const char* StackInterface::tolstring(int index, std::size_t *len)
{
    return lua_tolstring(LUASTATE(_c_state), index, len);
}

double StackInterface::tonumber(int index)
{
    return lua_tonumber(LUASTATE(_c_state), index);
}

double StackInterface::tonumberx(int index, int* isnum)
{
    return lua_tonumberx(LUASTATE(_c_state), index, isnum);
}

const void* StackInterface::topointer(int index)
{
    return lua_topointer(LUASTATE(_c_state), index);
}

const char* StackInterface::tostring(int index)
{
    return lua_tostring(LUASTATE(_c_state), index);
}

RawInterface StackInterface::tothread(int index)
{
    return lua_tothread(LUASTATE(_c_state), index);
}

unsigned int StackInterface::tounsigned(int index)
{
    return lua_tounsigned(LUASTATE(_c_state), index);
}

unsigned int StackInterface::tounsignedx(int index, int* isnum)
{
    return lua_tounsignedx(LUASTATE(_c_state), index, isnum);
}

void* StackInterface::touserdata(int index)
{
    return lua_touserdata(LUASTATE(_c_state), index);
}

StackInterface::TYPE StackInterface::type(int index)
{
    return (TYPE)lua_type(LUASTATE(_c_state), index);
}

const char* StackInterface::typename_(TYPE tp)
{
    return lua_typename(LUASTATE(_c_state), tp);
}

int StackInterface::upvalueindex(int i)
{
    return lua_upvalueindex(i);
}

void StackInterface::xmove(StackInterface& to, int n)
{
    lua_xmove(LUASTATE(_c_state), LUASTATE(to.getInterface()), n);
}

int StackInterface::yield(int nresults)
{
    return lua_yield(LUASTATE(_c_state), nresults);
}

int StackInterface::yieldk(int nresults, int ctx, CFunction k)
{
    return lua_yieldk(LUASTATE(_c_state), nresults, ctx, k);
}

const char* StackInterface::getupvalue(int funcindex, int n)
{
    return lua_getupvalue(LUASTATE(_c_state), funcindex, n);
}

const char* StackInterface::setupvalue(int funcindex, int n)
{
    return lua_setupvalue(LUASTATE(_c_state), funcindex, n);
}

void StackInterface::sameThread(const StackInterface& stack) const
{
    if (_c_state != stack._c_state)
        SCRIPT_EXCEPTION("Different thread");
}

bool StackInterface::sameThreadSafe(const StackInterface& stack)const
{
    return _c_state == stack._c_state;
}

void StackInterface::argcheck_L(int cond, int arg, const char* extramsg)
{
    luaL_argcheck(LUASTATE(_c_state), cond, arg, extramsg);
}

int StackInterface::argerror_L(int arg, const char* extramsg)
{
    return luaL_argerror(LUASTATE(_c_state), arg, extramsg);
}

int StackInterface::callmeta_L(int obj, const char* e)
{
    return luaL_callmeta(LUASTATE(_c_state), obj, e);
}

void StackInterface::checkany_L(int arg)
{
    luaL_checkany(LUASTATE(_c_state), arg);
}

int StackInterface::checkint_L(int arg)
{
    return luaL_checkint(LUASTATE(_c_state), arg);
}

long long StackInterface::checkinteger_L(int arg)
{
    return luaL_checkinteger(LUASTATE(_c_state), arg);
}

long StackInterface::checklong_L(int arg)
{
    return luaL_checklong(LUASTATE(_c_state), arg);
}

const char* StackInterface::checklstring_L(int arg, std::size_t* l)
{
    return luaL_checklstring(LUASTATE(_c_state), arg, l);
}

double StackInterface::checknumber_L(int arg)
{
    return luaL_checknumber(LUASTATE(_c_state), arg);
}

int StackInterface::checkoption_L(int arg, const char* def, const char* const lst[])
{
    return luaL_checkoption(LUASTATE(_c_state), arg, def, lst);
}

void StackInterface::checkstack_L(int sz, const char* msg)
{
    luaL_checkstack(LUASTATE(_c_state), sz, msg);
}

const char* StackInterface::checkstring_L(int arg)
{
    return luaL_checkstring(LUASTATE(_c_state), arg);
}

void StackInterface::checktype_L(int arg, TYPE t)
{
    luaL_checktype(LUASTATE(_c_state), arg, t);
}

void* StackInterface::checkudata_L(int arg, const char* tname)
{
    return luaL_checkudata(LUASTATE(_c_state), arg, tname);
}

unsigned int StackInterface::checkunsigned_L(int arg)
{
    return luaL_checkunsigned(LUASTATE(_c_state), arg);
}

void StackInterface::checkversion_L()
{
    luaL_checkversion(LUASTATE(_c_state));
}

int StackInterface::dofile_L(const char* filename)
{
    return luaL_dofile(LUASTATE(_c_state), filename);
}

int StackInterface::dostring_L(const char* str)
{
    return luaL_dostring(LUASTATE(_c_state), str);
}

int StackInterface::error_L(const char* fmt, ...)
{
    va_list vlArgs;
    va_start(vlArgs, fmt);
    int n = luaL_error(LUASTATE(_c_state), fmt, vlArgs);
    va_end(vlArgs);
    return n;
}

int StackInterface::execresult_L(int stat)
{
    return luaL_execresult(LUASTATE(_c_state), stat);
}

int StackInterface::fileresult_L(int stat, const char* fname)
{
    return luaL_fileresult(LUASTATE(_c_state), stat, fname);
}

int StackInterface::getmetafield_L(int obj, const char* e)
{
    return luaL_getmetafield(LUASTATE(_c_state), obj, e);
}

void StackInterface::getmetatable_L(const char* tname)
{
    luaL_getmetatable(LUASTATE(_c_state), tname);
}

int StackInterface::getsubtable_L(int idx, const char* fname)
{
    return luaL_getsubtable(LUASTATE(_c_state), idx, fname);
}

const char* StackInterface::gsub_L(const char* s, const char* p, const char* r)
{
    return luaL_gsub(LUASTATE(_c_state), s, p, r);
}

int StackInterface::len_L(int index)
{
    return luaL_len(LUASTATE(_c_state), index);
}

int StackInterface::loadbuffer_L(const char* buff, std::size_t sz, const char* name)
{
    return luaL_loadbuffer(LUASTATE(_c_state), buff, sz, name);
}

int StackInterface::loadbufferx_L(const char* buff, std::size_t sz, const char* name, const char* mode)
{
    return luaL_loadbufferx(LUASTATE(_c_state), buff, sz, name, mode);
}

int StackInterface::loadfile_L(const char* filename)
{
    return luaL_loadfile(LUASTATE(_c_state), filename);
}

int StackInterface::loadfilex_L(const char* filename, const char* mode)
{
    return luaL_loadfilex(LUASTATE(_c_state), filename, mode);
}

int StackInterface::loadstring_L(const char* s)
{
    return luaL_loadstring(LUASTATE(_c_state), s);
}

int StackInterface::newmetatable_L(const char* tname)
{
    return luaL_newmetatable(LUASTATE(_c_state), tname);
}

RawInterface StackInterface::newstate_L()
{
    return luaL_newstate();
}

void StackInterface::openlibs_L()
{
    return luaL_openlibs(LUASTATE(_c_state));
}

int StackInterface::optint_L(int arg, int d)
{
    return luaL_optint(LUASTATE(_c_state), arg, d);
}

long long StackInterface::optinteger_L(int arg, long long d)
{
    return luaL_optinteger(LUASTATE(_c_state), arg, d);
}

long StackInterface::optlong_L(int arg, long d)
{
    return luaL_optlong(LUASTATE(_c_state), arg, d);
}

const char* StackInterface::optlstring_L(int arg, const char* d, std::size_t* l)
{
    return luaL_optlstring(LUASTATE(_c_state), arg, d, l);
}

double StackInterface::optnumber_L(int arg, double d)
{
    return luaL_optnumber(LUASTATE(_c_state), arg, d);
}

const char* StackInterface::optstring_L(int arg, const char* d)
{
    return luaL_optstring(LUASTATE(_c_state), arg, d);
}

unsigned int StackInterface::optunsigned_L(int arg, unsigned int u)
{
    return luaL_optunsigned(LUASTATE(_c_state), arg, u);
}

int StackInterface::ref_L(int t)
{
    return luaL_ref(LUASTATE(_c_state), t);
}

void StackInterface::requiref_L(const char* modname, CFunction openf, int glb)
{
    luaL_requiref(LUASTATE(_c_state), modname, openf, glb);
}

void StackInterface::setmetatable_L(const char* tname)
{
    luaL_setmetatable(LUASTATE(_c_state), tname);
}

void* StackInterface::testudata_L(int arg, const char* tname)
{
    return luaL_testudata(LUASTATE(_c_state), arg, tname);
}

const char* StackInterface::tolstring_L(int idx, std::size_t* len)
{
    return luaL_tolstring(LUASTATE(_c_state), idx, len);
}

void StackInterface::traceback_L(RawInterface L1, const char* msg, int level)
{
    luaL_traceback(LUASTATE(_c_state), L1, msg, level);
}

const char* StackInterface::typename_L(int index)
{
    return luaL_typename(LUASTATE(_c_state), index);
}

void StackInterface::unref_L(int t, int ref)
{
    luaL_unref(LUASTATE(_c_state), t,  ref);
}

void StackInterface::where_L(int lvl)
{
    luaL_where(LUASTATE(_c_state), lvl);
}

_NAME_END
