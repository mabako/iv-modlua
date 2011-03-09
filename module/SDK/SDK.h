//============== Copyright � 2010 IV:MP Team. All rights reserved. ==============
// File: SDK.h
//===============================================================================

#pragma once

#include <string.h>
#include "Squirrel.h"

#if defined _WIN32 || defined __CYGWIN__
  #ifdef __GNUC__
    #define EXPORT extern "C" __attribute__((dllexport))
  #else
    #define EXPORT extern "C" __declspec(dllexport) // Note: actually gcc seems to also supports this syntax.
  #endif
#else
  #if __GNUC__ >= 4
    #define EXPORT extern "C" __attribute__((visibility("default")))
  #else
    #define EXPORT
  #endif
#endif

enum eModuleCallback
{
};

typedef void (* LogPrintf_t)(const char * szFormat, ...);
#define LogPrintf FuncContainer.myLogPrintf

void RegisterFunction(HSQUIRRELVM pVM, const char * szName, SQFUNCTION pfnFunction);

struct FuncContainer_t
{
	sq_open_t sqopen;
	sq_newthread_t sqnewthread;
	sq_seterrorhandler_t sqseterrorhandler;
	sq_close_t sqclose;
	sq_setforeignptr_t sqsetforeignptr;
	sq_getforeignptr_t sqgetforeignptr;
	sq_setprintfunc_t sqsetprintfunc;
	sq_getprintfunc_t sqgetprintfunc;
	sq_geterrorfunc_t sqgeterrorfunc;
	sq_suspendvm_t sqsuspendvm;
	sq_wakeupvm_t sqwakeupvm;
	sq_getvmstate_t sqgetvmstate;
	sq_compile_t sqcompile;
	sq_compilebuffer_t sqcompilebuffer;
	sq_enabledebuginfo_t sqenabledebuginfo;
	sq_notifyallexceptions_t sqnotifyallexceptions;
	sq_setcompilererrorhandler_t sqsetcompilererrorhandler;
	sq_push_t sqpush;
	sq_pop_t sqpop;
	sq_poptop_t sqpoptop;
	sq_remove_t sqremove;
	sq_gettop_t sqgettop;
	sq_settop_t sqsettop;
	sq_reservestack_t sqreservestack;
	sq_cmp_t sqcmp;
	sq_move_t sqmove;
	sq_newuserdata_t sqnewuserdata;
	sq_newtable_t sqnewtable;
	sq_newarray_t sqnewarray;
	sq_newclosure_t sqnewclosure;
	sq_setparamscheck_t sqsetparamscheck;
	sq_bindenv_t sqbindenv;
	sq_pushstring_t sqpushstring;
	sq_pushfloat_t sqpushfloat;
	sq_pushinteger_t sqpushinteger;
	sq_pushbool_t sqpushbool;
	sq_pushuserpointer_t sqpushuserpointer;
	sq_pushnull_t sqpushnull;
	sq_gettype_t sqgettype;
	sq_getsize_t sqgetsize;
	sq_getbase_t sqgetbase;
	sq_instanceof_t sqinstanceof;
	sq_tostring_t sqtostring;
	sq_tobool_t sqtobool;
	sq_getstring_t sqgetstring;
	sq_getinteger_t sqgetinteger;
	sq_getfloat_t sqgetfloat;
	sq_getbool_t sqgetbool;
	sq_getthread_t sqgetthread;
	sq_getuserpointer_t sqgetuserpointer;
	sq_getuserdata_t sqgetuserdata;
	sq_settypetag_t sqsettypetag;
	sq_gettypetag_t sqgettypetag;
	sq_setreleasehook_t sqsetreleasehook;
	sq_getscratchpad_t sqgetscratchpad;
	sq_getfunctioninfo_t sqgetfunctioninfo;
	sq_getclosureinfo_t sqgetclosureinfo;
	sq_setnativeclosurename_t sqsetnativeclosurename;
	sq_setinstanceup_t sqsetinstanceup;
	sq_getinstanceup_t sqgetinstanceup;
	sq_setclassudsize_t sqsetclassudsize;
	sq_newclass_t sqnewclass;
	sq_createinstance_t sqcreateinstance;
	sq_setattributes_t sqsetattributes;
	sq_getattributes_t sqgetattributes;
	sq_getclass_t sqgetclass;
	sq_weakref_t sqweakref;
	sq_getdefaultdelegate_t sqgetdefaultdelegate;
	sq_pushroottable_t sqpushroottable;
	sq_pushregistrytable_t sqpushregistrytable;
	sq_pushconsttable_t sqpushconsttable;
	sq_setroottable_t sqsetroottable;
	sq_setconsttable_t sqsetconsttable;
	sq_newslot_t sqnewslot;
	sq_deleteslot_t sqdeleteslot;
	sq_set_t sqset;
	sq_get_t sqget;
	sq_rawget_t sqrawget;
	sq_rawset_t sqrawset;
	sq_rawdeleteslot_t sqrawdeleteslot;
	sq_arrayappend_t sqarrayappend;
	sq_arraypop_t sqarraypop;
	sq_arrayresize_t sqarrayresize;
	sq_arrayreverse_t sqarrayreverse;
	sq_arrayremove_t sqarrayremove;
	sq_arrayinsert_t sqarrayinsert;
	sq_setdelegate_t sqsetdelegate;
	sq_getdelegate_t sqgetdelegate;
	sq_clone_t sqclone;
	sq_setfreevariable_t sqsetfreevariable;
	sq_next_t sqnext;
	sq_getweakrefval_t sqgetweakrefval;
	sq_clear_t sqclear;
	sq_call_t sqcall;
	sq_resume_t sqresume;
	sq_getlocal_t sqgetlocal;
	sq_getfreevariable_t sqgetfreevariable;
	sq_throwerror_t sqthrowerror;
	sq_reseterror_t sqreseterror;
	sq_getlasterror_t sqgetlasterror;
	sq_getstackobj_t sqgetstackobj;
	sq_pushobject_t sqpushobject;
	sq_addref_t sqaddref;
	sq_release_t sqrelease;
	sq_resetobject_t sqresetobject;
	sq_objtostring_t sqobjtostring;
	sq_objtobool_t sqobjtobool;
	sq_objtointeger_t sqobjtointeger;
	sq_objtofloat_t sqobjtofloat;
	sq_getobjtypetag_t sqgetobjtypetag;
	sq_collectgarbage_t sqcollectgarbage;
	sq_writeclosure_t sqwriteclosure;
	sq_readclosure_t sqreadclosure;
	sq_malloc_t sqmalloc;
	sq_realloc_t sqrealloc;
	sq_free_t sqfree;
	sq_stackinfos_t sqstackinfos;
	sq_setdebughook_t sqsetdebughook;
	LogPrintf_t myLogPrintf;
};

extern FuncContainer_t FuncContainer;
