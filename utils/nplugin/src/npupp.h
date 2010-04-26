/*
 *  npupp.h $Revision$
 *  function call mecahnics needed by platform specific glue code.
 */


#ifndef _NPUPP_H_
#define _NPUPP_H_

#ifndef GENERATINGCFM
#define GENERATINGCFM 0
#endif

#ifndef _NPAPI_H_
#include "npapi.h"
#endif

/******************************************************************************************
   plug-in function table macros
 	        for each function in and out of the plugin API we define
                    typedef NPP_FooUPP
					#define NewNPP_FooProc
					#define CallNPP_FooProc
			for mac, define the UPP magic for PPC/68K calling
 *******************************************************************************************/


/* NPP_Initialize */

#if GENERATINGCFM
typedef UniversalProcPtr NPP_InitializeUPP;

enum {
	uppNPP_InitializeProcInfo = kThinkCStackBased
		| STACK_ROUTINE_PARAMETER(1, SIZE_CODE(0))		
		| RESULT_SIZE(SIZE_CODE(0))
};

#define NewNPP_InitializeProc(FUNC)		\
		(NPP_InitializeUPP) NewRoutineDescriptor((ProcPtr)(FUNC), uppNPP_InitializeProcInfo, GetCurrentArchitecture())
#define CallNPP_InitializeProc(FUNC)		\
		(void)CallUniversalProc((UniversalProcPtr)(FUNC), uppNPP_InitializeProcInfo)
		
#else

typedef void (*NPP_InitializeUPP)(void);
#define NewNPP_InitializeProc(FUNC)		\
		((NPP_InitializeUPP) (FUNC))
#define CallNPP_InitializeProc(FUNC)		\
		(*(FUNC))()

#endif


/* NPP_Shutdown */

#if GENERATINGCFM
typedef UniversalProcPtr NPP_ShutdownUPP;

enum {
	uppNPP_ShutdownProcInfo = kThinkCStackBased
		| STACK_ROUTINE_PARAMETER(1, SIZE_CODE(0))		
		| RESULT_SIZE(SIZE_CODE(0))
};

#define NewNPP_ShutdownProc(FUNC)		\
		(NPP_ShutdownUPP) NewRoutineDescriptor((ProcPtr)(FUNC), uppNPP_ShutdownProcInfo, GetCurrentArchitecture())
#define CallNPP_ShutdownProc(FUNC)		\
		(void)CallUniversalProc((UniversalProcPtr)(FUNC), uppNPP_ShutdownProcInfo)
		
#else

typedef void (*NPP_ShutdownUPP)(void);
#define NewNPP_ShutdownProc(FUNC)		\
		((NPP_ShutdownUPP) (FUNC))
#define CallNPP_ShutdownProc(FUNC)		\
		(*(FUNC))()

#endif


/* NPP_New */

#if GENERATINGCFM
typedef UniversalProcPtr NPP_NewUPP;

enum {
	uppNPP_NewProcInfo = kThinkCStackBased
		| STACK_ROUTINE_PARAMETER(1, SIZE_CODE(sizeof(NPMIMEType)))
		| STACK_ROUTINE_PARAMETER(2, SIZE_CODE(sizeof(NPP)))
		| STACK_ROUTINE_PARAMETER(3, SIZE_CODE(sizeof(uint16)))
		| STACK_ROUTINE_PARAMETER(4, SIZE_CODE(sizeof(int16)))
		| STACK_ROUTINE_PARAMETER(5, SIZE_CODE(sizeof(char **)))
		| STACK_ROUTINE_PARAMETER(6, SIZE_CODE(sizeof(char **)))
		| STACK_ROUTINE_PARAMETER(7, SIZE_CODE(sizeof(NPSavedData *)))
		| RESULT_SIZE(SIZE_CODE(sizeof(NPError)))
};

#define NewNPP_NewProc(FUNC)		\
		(NPP_NewUPP) NewRoutineDescriptor((ProcPtr)(FUNC), uppNPP_NewProcInfo, GetCurrentArchitecture())
#define CallNPP_NewProc(FUNC, ARG1, ARG2, ARG3, ARG4, ARG5, ARG6, ARG7) \
		(NPError)CallUniversalProc((UniversalProcPtr)(FUNC), uppNPP_NewProcInfo, \
								   (ARG1), (ARG2), (ARG3), (ARG4), (ARG5), (ARG6), (ARG7))
#else

typedef NPError	(*NPP_NewUPP)(NPMIMEType pluginType, NPP instance, uint16 mode, int16 argc, char* argn[], char* argv[], NPSavedData* saved);
#define NewNPP_NewProc(FUNC)		\
		((NPP_NewUPP) (FUNC))
#define CallNPP_NewProc(FUNC, ARG1, ARG2, ARG3, ARG4, ARG5, ARG6, ARG7)		\
		(*(FUNC))((ARG1), (ARG2), (ARG3), (ARG4), (ARG5), (ARG6), (ARG7))

#endif


/* NPP_Destroy */

#if GENERATINGCFM

typedef UniversalProcPtr NPP_DestroyUPP;
enum {
	uppNPP_DestroyProcInfo = kThinkCStackBased
		| STACK_ROUTINE_PARAMETER(1, SIZE_CODE(sizeof(NPP)))
		| STACK_ROUTINE_PARAMETER(2, SIZE_CODE(sizeof(NPSavedData **)))
		| RESULT_SIZE(SIZE_CODE(sizeof(NPError)))
};
#define NewNPP_DestroyProc(FUNC)		\
		(NPP_DestroyUPP) NewRoutineDescriptor((ProcPtr)(FUNC), uppNPP_DestroyProcInfo, GetCurrentArchitecture())
#define CallNPP_DestroyProc(FUNC, ARG1, ARG2)		\
		(NPError)CallUniversalProc((UniversalProcPtr)(FUNC), uppNPP_DestroyProcInfo, (ARG1), (ARG2))
#else

typedef NPError	(*NPP_DestroyUPP)(NPP instance, NPSavedData** save);
#define NewNPP_DestroyProc(FUNC)		\
		((NPP_DestroyUPP) (FUNC))
#define CallNPP_DestroyProc(FUNC, ARG1, ARG2)		\
		(*(FUNC))((ARG1), (ARG2))

#endif


/* NPP_SetWindow */

#if GENERATINGCFM

typedef UniversalProcPtr NPP_SetWindowUPP;
enum {
	uppNPP_SetWindowProcInfo = kThinkCStackBased
		| STACK_ROUTINE_PARAMETER(1, SIZE_CODE(sizeof(NPP)))
		| STACK_ROUTINE_PARAMETER(2, SIZE_CODE(sizeof(NPWindow *)))
		| RESULT_SIZE(SIZE_CODE(sizeof(NPError)))
};
#define NewNPP_SetWindowProc(FUNC)		\
		(NPP_SetWindowUPP) NewRoutineDescriptor((ProcPtr)(FUNC), uppNPP_SetWindowProcInfo, GetCurrentArchitecture())
#define CallNPP_SetWindowProc(FUNC, ARG1, ARG2)		\
		(NPError)CallUniversalProc((UniversalProcPtr)(FUNC), uppNPP_SetWindowProcInfo, (ARG1), (ARG2))

#else

typedef NPError	(*NPP_SetWindowUPP)(NPP instance, NPWindow* window);
#define NewNPP_SetWindowProc(FUNC)		\
		((NPP_SetWindowUPP) (FUNC))
#define CallNPP_SetWindowProc(FUNC, ARG1, ARG2)		\
		(*(FUNC))((ARG1), (ARG2))

#endif


/* NPP_NewStream */

#if GENERATINGCFM

typedef UniversalProcPtr NPP_NewStreamUPP;
enum {
	uppNPP_NewStreamProcInfo = kThinkCStackBased
		| STACK_ROUTINE_PARAMETER(1, SIZE_CODE(sizeof(NPP)))
		| STACK_ROUTINE_PARAMETER(2, SIZE_CODE(sizeof(NPMIMEType)))
		| STACK_ROUTINE_PARAMETER(3, SIZE_CODE(sizeof(NPStream *)))
		| STACK_ROUTINE_PARAMETER(4, SIZE_CODE(sizeof(NPBool)))
		| STACK_ROUTINE_PARAMETER(5, SIZE_CODE(sizeof(uint16 *)))
		| RESULT_SIZE(SIZE_CODE(sizeof(NPError)))
};
#define NewNPP_NewStreamProc(FUNC)		\
		(NPP_NewStreamUPP) NewRoutineDescriptor((ProcPtr)(FUNC), uppNPP_NewStreamProcInfo, GetCurrentArchitecture())
#define CallNPP_NewStreamProc(FUNC, ARG1, ARG2, ARG3, ARG4, ARG5)		\
		(NPError)CallUniversalProc((UniversalProcPtr)(FUNC), uppNPP_NewStreamProcInfo, (ARG1), (ARG2), (ARG3), (ARG4), (ARG5))
#else

typedef NPError	(*NPP_NewStreamUPP)(NPP instance, NPMIMEType type, NPStream* stream, NPBool seekable, uint16* stype);
#define NewNPP_NewStreamProc(FUNC)		\
		((NPP_NewStreamUPP) (FUNC))
#define CallNPP_NewStreamProc(FUNC, ARG1, ARG2, ARG3, ARG4, ARG5) \
		(*(FUNC))((ARG1), (ARG2), (ARG3), (ARG4), (ARG5))
#endif


/* NPP_DestroyStream */

#if GENERATINGCFM

typedef UniversalProcPtr NPP_DestroyStreamUPP;
enum {
	uppNPP_DestroyStreamProcInfo = kThinkCStackBased
		| STACK_ROUTINE_PARAMETER(1, SIZE_CODE(sizeof(NPP)))
		| STACK_ROUTINE_PARAMETER(2, SIZE_CODE(sizeof(NPStream *)))
		| STACK_ROUTINE_PARAMETER(3, SIZE_CODE(sizeof(NPError)))
		| RESULT_SIZE(SIZE_CODE(sizeof(NPError)))
};
#define NewNPP_DestroyStreamProc(FUNC)		\
		(NPP_DestroyStreamUPP) NewRoutineDescriptor((ProcPtr)(FUNC), uppNPP_DestroyStreamProcInfo, GetCurrentArchitecture())
#define CallNPP_DestroyStreamProc(FUNC,  NPParg, NPStreamPtr, NPErrorArg)		\
		(NPError)CallUniversalProc((UniversalProcPtr)(FUNC), uppNPP_DestroyStreamProcInfo, (NPParg), (NPStreamPtr), (NPErrorArg))

#else

typedef NPError	(*NPP_DestroyStreamUPP)(NPP instance, NPStream* stream, NPError reason);
#define NewNPP_DestroyStreamProc(FUNC)		\
		((NPP_DestroyStreamUPP) (FUNC))
#define CallNPP_DestroyStreamProc(FUNC,  NPParg, NPStreamPtr, NPErrorArg)		\
		(*(FUNC))((NPParg), (NPStreamPtr), (NPErrorArg))

#endif


/* NPP_WriteReady */

#if GENERATINGCFM

typedef UniversalProcPtr NPP_WriteReadyUPP;
enum {
	uppNPP_WriteReadyProcInfo = kThinkCStackBased
		| STACK_ROUTINE_PARAMETER(1, SIZE_CODE(sizeof(NPP)))
		| STACK_ROUTINE_PARAMETER(2, SIZE_CODE(sizeof(NPStream *)))
		| RESULT_SIZE(SIZE_CODE(sizeof(int32)))
};
#define NewNPP_WriteReadyProc(FUNC)		\
		(NPP_WriteReadyUPP) NewRoutineDescriptor((ProcPtr)(FUNC), uppNPP_WriteReadyProcInfo, GetCurrentArchitecture())
#define CallNPP_WriteReadyProc(FUNC,  NPParg, NPStreamPtr)		\
		(int32)CallUniversalProc((UniversalProcPtr)(FUNC), uppNPP_WriteReadyProcInfo, (NPParg), (NPStreamPtr))

#else

typedef int32 (*NPP_WriteReadyUPP)(NPP instance, NPStream* stream);
#define NewNPP_WriteReadyProc(FUNC)		\
		((NPP_WriteReadyUPP) (FUNC))
#define CallNPP_WriteReadyProc(FUNC,  NPParg, NPStreamPtr)		\
		(*(FUNC))((NPParg), (NPStreamPtr))

#endif


/* NPP_Write */

#if GENERATINGCFM

typedef UniversalProcPtr NPP_WriteUPP;
enum {
	uppNPP_WriteProcInfo = kThinkCStackBased
		| STACK_ROUTINE_PARAMETER(1, SIZE_CODE(sizeof(NPP)))
		| STACK_ROUTINE_PARAMETER(2, SIZE_CODE(sizeof(NPStream *)))
		| STACK_ROUTINE_PARAMETER(3, SIZE_CODE(sizeof(int32)))
		| STACK_ROUTINE_PARAMETER(4, SIZE_CODE(sizeof(int32)))
		| STACK_ROUTINE_PARAMETER(5, SIZE_CODE(sizeof(void*)))
		| RESULT_SIZE(SIZE_CODE(sizeof(int32)))
};
#define NewNPP_WriteProc(FUNC)		\
		(NPP_WriteUPP) NewRoutineDescriptor((ProcPtr)(FUNC), uppNPP_WriteProcInfo, GetCurrentArchitecture())
#define CallNPP_WriteProc(FUNC,  NPParg, NPStreamPtr, offsetArg, lenArg, bufferPtr)		\
		(int32)CallUniversalProc((UniversalProcPtr)(FUNC), uppNPP_WriteProcInfo, (NPParg), (NPStreamPtr), (offsetArg), (lenArg), (bufferPtr))

#else

typedef int32 (*NPP_WriteUPP)(NPP instance, NPStream* stream, int32 offset, int32 len, void* buffer);
#define NewNPP_WriteProc(FUNC)		\
		((NPP_WriteUPP) (FUNC))
#define CallNPP_WriteProc(FUNC,  NPParg, NPStreamPtr, offsetArg, lenArg, bufferPtr)		\
		(*(FUNC))((NPParg), (NPStreamPtr), (offsetArg), (lenArg), (bufferPtr))

#endif


/* NPP_StreamAsFile */

#if GENERATINGCFM

typedef UniversalProcPtr NPP_StreamAsFileUPP;
enum {
	uppNPP_StreamAsFileProcInfo = kThinkCStackBased
		| STACK_ROUTINE_PARAMETER(1, SIZE_CODE(sizeof(NPP)))
		| STACK_ROUTINE_PARAMETER(2, SIZE_CODE(sizeof(NPStream *)))
		| STACK_ROUTINE_PARAMETER(3, SIZE_CODE(sizeof(const char *)))
		| RESULT_SIZE(SIZE_CODE(0))
};
#define NewNPP_StreamAsFileProc(FUNC)		\
		(NPP_StreamAsFileUPP) NewRoutineDescriptor((ProcPtr)(FUNC), uppNPP_StreamAsFileProcInfo, GetCurrentArchitecture())
#define CallNPP_StreamAsFileProc(FUNC, ARG1, ARG2, ARG3)		\
		(void)CallUniversalProc((UniversalProcPtr)(FUNC), uppNPP_StreamAsFileProcInfo, (ARG1), (ARG2), (ARG3))

#else

typedef void (*NPP_StreamAsFileUPP)(NPP instance, NPStream* stream, const char* fname);
#define NewNPP_StreamAsFileProc(FUNC)		\
		((NPP_StreamAsFileUPP) (FUNC))
#define CallNPP_StreamAsFileProc(FUNC,  ARG1, ARG2, ARG3)		\
		(*(FUNC))((ARG1), (ARG2), (ARG3))
#endif


/* NPP_Print */

#if GENERATINGCFM

typedef UniversalProcPtr NPP_PrintUPP;
enum {
	uppNPP_PrintProcInfo = kThinkCStackBased
		| STACK_ROUTINE_PARAMETER(1, SIZE_CODE(sizeof(NPP)))
		| STACK_ROUTINE_PARAMETER(2, SIZE_CODE(sizeof(NPPrint *)))
		| RESULT_SIZE(SIZE_CODE(0))
};
#define NewNPP_PrintProc(FUNC)		\
		(NPP_PrintUPP) NewRoutineDescriptor((ProcPtr)(FUNC), uppNPP_PrintProcInfo, GetCurrentArchitecture())
#define CallNPP_PrintProc(FUNC,  NPParg, voidPtr)		\
		(void)CallUniversalProc((UniversalProcPtr)(FUNC), uppNPP_PrintProcInfo, (NPParg), (voidPtr))

#else

typedef void (*NPP_PrintUPP)(NPP instance, NPPrint* platformPrint);
#define NewNPP_PrintProc(FUNC)		\
		((NPP_PrintUPP) (FUNC))
#define CallNPP_PrintProc(FUNC,  NPParg, NPPrintArg)		\
		(*(FUNC))((NPParg), (NPPrintArg))

#endif


/* NPP_HandleEvent */

#if GENERATINGCFM

typedef UniversalProcPtr NPP_HandleEventUPP;
enum {
	uppNPP_HandleEventProcInfo = kThinkCStackBased
		| STACK_ROUTINE_PARAMETER(1, SIZE_CODE(sizeof(NPP)))
		| STACK_ROUTINE_PARAMETER(2, SIZE_CODE(sizeof(void *)))
		| RESULT_SIZE(SIZE_CODE(sizeof(int16)))
};
#define NewNPP_HandleEventProc(FUNC)		\
		(NPP_HandleEventUPP) NewRoutineDescriptor((ProcPtr)(FUNC), uppNPP_HandleEventProcInfo, GetCurrentArchitecture())
#define CallNPP_HandleEventProc(FUNC,  NPParg, voidPtr)		\
		(int16)CallUniversalProc((UniversalProcPtr)(FUNC), uppNPP_HandleEventProcInfo, (NPParg), (voidPtr))

#else

typedef int16 (*NPP_HandleEventUPP)(NPP instance, void* event);
#define NewNPP_HandleEventProc(FUNC)		\
		((NPP_HandleEventUPP) (FUNC))
#define CallNPP_HandleEventProc(FUNC,  NPParg, voidPtr)		\
		(*(FUNC))((NPParg), (voidPtr))

#endif




/*
 *  Netscape entry points
 */


/* NPN_GetUrl */

#if GENERATINGCFM

typedef UniversalProcPtr NPN_GetURLUPP;
enum {
	uppNPN_GetURLProcInfo = kThinkCStackBased
		| STACK_ROUTINE_PARAMETER(1, SIZE_CODE(sizeof(NPP)))
		| STACK_ROUTINE_PARAMETER(2, SIZE_CODE(sizeof(const char*)))
		| STACK_ROUTINE_PARAMETER(3, SIZE_CODE(sizeof(const char*)))
		| RESULT_SIZE(SIZE_CODE(sizeof(NPError)))
};
#define NewNPN_GetURLProc(FUNC)		\
		(NPN_GetURLUPP) NewRoutineDescriptor((ProcPtr)(FUNC), uppNPN_GetURLProcInfo, GetCurrentArchitecture())
#define CallNPN_GetURLProc(FUNC, ARG1, ARG2, ARG3) \
		(NPError)CallUniversalProc((UniversalProcPtr)(FUNC), uppNPN_GetURLProcInfo, (ARG1), (ARG2), (ARG3))
#else

typedef NPError	(*NPN_GetURLUPP)(NPP instance, const char* url, const char* window);
#define NewNPN_GetURLProc(FUNC)		\
		((NPN_GetURLUPP) (FUNC))
#define CallNPN_GetURLProc(FUNC, ARG1, ARG2, ARG3)		\
		(*(FUNC))((ARG1), (ARG2), (ARG3))
#endif


/* NPN_PostUrl */

#if GENERATINGCFM

typedef UniversalProcPtr NPN_PostURLUPP;
enum {
	uppNPN_PostURLProcInfo = kThinkCStackBased
		| STACK_ROUTINE_PARAMETER(1, SIZE_CODE(sizeof(NPP)))
		| STACK_ROUTINE_PARAMETER(2, SIZE_CODE(sizeof(const char*)))
		| STACK_ROUTINE_PARAMETER(3, SIZE_CODE(sizeof(const char*)))
		| STACK_ROUTINE_PARAMETER(4, SIZE_CODE(sizeof(uint32)))
		| STACK_ROUTINE_PARAMETER(5, SIZE_CODE(sizeof(const char*)))
		| STACK_ROUTINE_PARAMETER(6, SIZE_CODE(sizeof(NPBool)))
		| RESULT_SIZE(SIZE_CODE(sizeof(NPError)))
};
#define NewNPN_PostURLProc(FUNC)		\
		(NPN_PostURLUPP) NewRoutineDescriptor((ProcPtr)(FUNC), uppNPN_PostURLProcInfo, GetCurrentArchitecture())
#define CallNPN_PostURLProc(FUNC, ARG1, ARG2, ARG3, ARG4, ARG5, ARG6) \
		(NPError)CallUniversalProc((UniversalProcPtr)(FUNC), uppNPN_PostURLProcInfo, (ARG1), (ARG2), (ARG3), (ARG4), (ARG5), (ARG6))
#else

typedef NPError (*NPN_PostURLUPP)(NPP instance, const char* url, const char* window, uint32 len, const char* buf, NPBool file);
#define NewNPN_PostURLProc(FUNC)		\
		((NPN_PostURLUPP) (FUNC))
#define CallNPN_PostURLProc(FUNC, ARG1, ARG2, ARG3, ARG4, ARG5, ARG6) \
		(*(FUNC))((ARG1), (ARG2), (ARG3), (ARG4), (ARG5), (ARG6))
#endif


/* NPN_RequestRead */

#if GENERATINGCFM

typedef UniversalProcPtr NPN_RequestReadUPP;
enum {
	uppNPN_RequestReadProcInfo = kThinkCStackBased
		| STACK_ROUTINE_PARAMETER(1, SIZE_CODE(sizeof(NPStream *)))
		| STACK_ROUTINE_PARAMETER(2, SIZE_CODE(sizeof(NPByteRange *)))
		| RESULT_SIZE(SIZE_CODE(sizeof(NPError)))
};
#define NewNPN_RequestReadProc(FUNC)		\
		(NPN_RequestReadUPP) NewRoutineDescriptor((ProcPtr)(FUNC), uppNPN_RequestReadProcInfo, GetCurrentArchitecture())
#define CallNPN_RequestReadProc(FUNC,  stream, range)		\
		(NPError)CallUniversalProc((UniversalProcPtr)(FUNC), uppNPN_RequestReadProcInfo, (stream), (range))

#else

typedef NPError	(*NPN_RequestReadUPP)(NPStream* stream, NPByteRange* rangeList);
#define NewNPN_RequestReadProc(FUNC)		\
		((NPN_RequestReadUPP) (FUNC))
#define CallNPN_RequestReadProc(FUNC, stream, range)		\
		(*(FUNC))((stream), (range))

#endif


/* NPN_NewStream */

#if GENERATINGCFM

typedef UniversalProcPtr NPN_NewStreamUPP;
enum {
	uppNPN_NewStreamProcInfo = kThinkCStackBased
		| STACK_ROUTINE_PARAMETER(1, SIZE_CODE(sizeof(NPP )))
		| STACK_ROUTINE_PARAMETER(2, SIZE_CODE(sizeof(NPMIMEType)))
		| STACK_ROUTINE_PARAMETER(3, SIZE_CODE(sizeof(NPStream *)))
		| STACK_ROUTINE_PARAMETER(4, SIZE_CODE(sizeof(NPBool)))
		| STACK_ROUTINE_PARAMETER(5, SIZE_CODE(sizeof(uint16*)))
		| RESULT_SIZE(SIZE_CODE(sizeof(NPError)))
};
#define NewNPN_NewStreamProc(FUNC)		\
		(NPN_NewStreamUPP) NewRoutineDescriptor((ProcPtr)(FUNC), uppNPN_NewStreamProcInfo, GetCurrentArchitecture())
#define CallNPN_NewStreamProc(FUNC, npp, type, stream)		\
		(NPError)CallUniversalProc((UniversalProcPtr)(FUNC), uppNPN_NewStreamProcInfo, (npp), (type), (stream))	

#else

typedef NPError	(*NPN_NewStreamUPP)(NPP instance, NPMIMEType type, NPStream* stream);
#define NewNPN_NewStreamProc(FUNC)		\
		((NPN_NewStreamUPP) (FUNC))
#define CallNPN_NewStreamProc(FUNC, npp, type, stream)		\
		(*(FUNC))((npp), (type), (stream))

#endif


/* NPN_Write */

#if GENERATINGCFM

typedef UniversalProcPtr NPN_WriteUPP;
enum {
	uppNPN_WriteProcInfo = kThinkCStackBased
		| STACK_ROUTINE_PARAMETER(1, SIZE_CODE(sizeof(NPP )))
		| STACK_ROUTINE_PARAMETER(2, SIZE_CODE(sizeof(NPStream *)))
		| STACK_ROUTINE_PARAMETER(3, SIZE_CODE(sizeof(int32)))
		| STACK_ROUTINE_PARAMETER(4, SIZE_CODE(sizeof(void*)))
		| RESULT_SIZE(SIZE_CODE(sizeof(int32)))
};
#define NewNPN_WriteProc(FUNC)		\
		(NPN_WriteUPP) NewRoutineDescriptor((ProcPtr)(FUNC), uppNPN_WriteProcInfo, GetCurrentArchitecture())
#define CallNPN_WriteProc(FUNC, npp, stream, len, buffer)		\
		(int32)CallUniversalProc((UniversalProcPtr)(FUNC), uppNPN_WriteProcInfo, (npp), (stream), (len), (buffer))	

#else

typedef int32 (*NPN_WriteUPP)(NPP instance, NPStream* stream, int32 len, void* buffer);
#define NewNPN_WriteProc(FUNC)		\
		((NPN_WriteUPP) (FUNC))
#define CallNPN_WriteProc(FUNC, npp, stream, len, buffer)		\
		(*(FUNC))((npp), (stream), (len), (buffer))

#endif


/* NPN_DestroyStream */

#if GENERATINGCFM

typedef UniversalProcPtr NPN_DestroyStreamUPP;
enum {
	uppNPN_DestroyStreamProcInfo = kThinkCStackBased
		| STACK_ROUTINE_PARAMETER(1, SIZE_CODE(sizeof(NPP )))
		| STACK_ROUTINE_PARAMETER(2, SIZE_CODE(sizeof(NPStream *)))
		| STACK_ROUTINE_PARAMETER(3, SIZE_CODE(sizeof(NPError)))
		| RESULT_SIZE(SIZE_CODE(sizeof(NPError)))
};
#define NewNPN_DestroyStreamProc(FUNC)		\
		(NPN_DestroyStreamUPP) NewRoutineDescriptor((ProcPtr)(FUNC), uppNPN_DestroyStreamProcInfo, GetCurrentArchitecture())
#define CallNPN_DestroyStreamProc(FUNC, npp, stream, err)		\
		(NPError)CallUniversalProc((UniversalProcPtr)(FUNC), uppNPN_DestroyStreamProcInfo, (npp), (stream), (err))	

#else

typedef NPError (*NPN_DestroyStreamUPP)(NPP instance, NPStream* stream, NPError reason);
#define NewNPN_DestroyStreamProc(FUNC)		\
		((NPN_DestroyStreamUPP) (FUNC))
#define CallNPN_DestroyStreamProc(FUNC, npp, stream, err)		\
		(*(FUNC))((npp), (stream), (err))

#endif


/* NPN_Status */

#if GENERATINGCFM

typedef UniversalProcPtr NPN_StatusUPP;
enum {
	uppNPN_StatusProcInfo = kThinkCStackBased
		| STACK_ROUTINE_PARAMETER(1, SIZE_CODE(sizeof(NPP)))
		| STACK_ROUTINE_PARAMETER(2, SIZE_CODE(sizeof(char *)))
};

#define NewNPN_StatusProc(FUNC)		\
		(NPN_StatusUPP) NewRoutineDescriptor((ProcPtr)(FUNC), uppNPN_StatusProcInfo, GetCurrentArchitecture())
#define CallNPN_StatusProc(FUNC, npp, msg)		\
		(void)CallUniversalProc((UniversalProcPtr)(FUNC), uppNPN_StatusProcInfo, (npp), (msg))	

#else

typedef void (*NPN_StatusUPP)(NPP instance, const char* message);
#define NewNPN_StatusProc(FUNC)		\
		((NPN_StatusUPP) (FUNC))
#define CallNPN_StatusProc(FUNC, npp, msg)		\
		(*(FUNC))((npp), (msg))	

#endif


/* NPN_UserAgent */
#if GENERATINGCFM

typedef UniversalProcPtr NPN_UserAgentUPP;
enum {
        uppNPN_UserAgentProcInfo = kThinkCStackBased
                | STACK_ROUTINE_PARAMETER(1, SIZE_CODE(sizeof(NPP)))
                | RESULT_SIZE(SIZE_CODE(sizeof(const char *)))
};

#define NewNPN_UserAgentProc(FUNC)              \
                (NPN_UserAgentUPP) NewRoutineDescriptor((ProcPtr)(FUNC), uppNPN_UserAgentProcInfo, GetCurrentArchitecture())
#define CallNPN_UserAgentProc(FUNC, ARG1)               \
                (const char*)CallUniversalProc((UniversalProcPtr)(FUNC), uppNPN_UserAgentProcInfo, (ARG1))

#else

typedef const char*	(*NPN_UserAgentUPP)(NPP instance);
#define NewNPN_UserAgentProc(FUNC)              \
                ((NPN_UserAgentUPP) (FUNC))
#define CallNPN_UserAgentProc(FUNC, ARG1)               \
                (*(FUNC))((ARG1))

#endif


/* NPN_MemAlloc */
#if GENERATINGCFM

typedef UniversalProcPtr NPN_MemAllocUPP;
enum {
	uppNPN_MemAllocProcInfo = kThinkCStackBased
		| STACK_ROUTINE_PARAMETER(1, SIZE_CODE(sizeof(uint32)))
		| RESULT_SIZE(SIZE_CODE(sizeof(void *)))
};

#define NewNPN_MemAllocProc(FUNC)		\
		(NPN_MemAllocUPP) NewRoutineDescriptor((ProcPtr)(FUNC), uppNPN_MemAllocProcInfo, GetCurrentArchitecture())
#define CallNPN_MemAllocProc(FUNC, ARG1)		\
		(void*)CallUniversalProc((UniversalProcPtr)(FUNC), uppNPN_MemAllocProcInfo, (ARG1))	

#else

typedef void* (*NPN_MemAllocUPP)(uint32 size);
#define NewNPN_MemAllocProc(FUNC)		\
		((NPN_MemAllocUPP) (FUNC))
#define CallNPN_MemAllocProc(FUNC, ARG1)		\
		(*(FUNC))((ARG1))	

#endif


/* NPN__MemFree */

#if GENERATINGCFM

typedef UniversalProcPtr NPN_MemFreeUPP;
enum {
	uppNPN_MemFreeProcInfo = kThinkCStackBased
		| STACK_ROUTINE_PARAMETER(1, SIZE_CODE(sizeof(void *)))
};

#define NewNPN_MemFreeProc(FUNC)		\
		(NPN_MemFreeUPP) NewRoutineDescriptor((ProcPtr)(FUNC), uppNPN_MemFreeProcInfo, GetCurrentArchitecture())
#define CallNPN_MemFreeProc(FUNC, ARG1)		\
		(void)CallUniversalProc((UniversalProcPtr)(FUNC), uppNPN_MemFreeProcInfo, (ARG1))

#else

typedef void (*NPN_MemFreeUPP)(void* ptr);
#define NewNPN_MemFreeProc(FUNC)		\
		((NPN_MemFreeUPP) (FUNC))
#define CallNPN_MemFreeProc(FUNC, ARG1)		\
		(*(FUNC))((ARG1))	

#endif


/* NPN_MemFlush */

#if GENERATINGCFM

typedef UniversalProcPtr NPN_MemFlushUPP;
enum {
	uppNPN_MemFlushProcInfo = kThinkCStackBased
		| STACK_ROUTINE_PARAMETER(1, SIZE_CODE(sizeof(uint32)))
		| RESULT_SIZE(SIZE_CODE(sizeof(uint32)))
};

#define NewNPN_MemFlushProc(FUNC)		\
		(NPN_MemFlushUPP) NewRoutineDescriptor((ProcPtr)(FUNC), uppNPN_MemFlushProcInfo, GetCurrentArchitecture())
#define CallNPN_MemFlushProc(FUNC, ARG1)		\
		(uint32)CallUniversalProc((UniversalProcPtr)(FUNC), uppNPN_MemFlushProcInfo, (ARG1))	

#else

typedef uint32 (*NPN_MemFlushUPP)(uint32 size);
#define NewNPN_MemFlushProc(FUNC)		\
		((NPN_MemFlushUPP) (FUNC))
#define CallNPN_MemFlushProc(FUNC, ARG1)		\
		(*(FUNC))((ARG1))	

#endif



/* NPN_ReloadPlugins */

#if GENERATINGCFM

typedef UniversalProcPtr NPN_ReloadPluginsUPP;
enum {
	uppNPN_ReloadPluginsProcInfo = kThinkCStackBased
		| STACK_ROUTINE_PARAMETER(1, SIZE_CODE(sizeof(NPBool)))
		| RESULT_SIZE(SIZE_CODE(0))
};

#define NewNPN_ReloadPluginsProc(FUNC)		\
		(NPN_ReloadPluginsUPP) NewRoutineDescriptor((ProcPtr)(FUNC), uppNPN_ReloadPluginsProcInfo, GetCurrentArchitecture())
#define CallNPN_ReloadPluginsProc(FUNC, ARG1)		\
		(void)CallUniversalProc((UniversalProcPtr)(FUNC), uppNPN_ReloadPluginsProcInfo, (ARG1))	

#else

typedef void (*NPN_ReloadPluginsUPP)(NPBool reloadPages);
#define NewNPN_ReloadPluginsProc(FUNC)		\
		((NPN_ReloadPluginsUPP) (FUNC))
#define CallNPN_ReloadPluginsProc(FUNC, ARG1)		\
		(*(FUNC))((ARG1))	

#endif




/******************************************************************************************
 * The actual plugin function table definitions
 *******************************************************************************************/

typedef struct _NPPluginFuncs {
	uint16 size;
	uint16 version;
    NPP_NewUPP newp;
    NPP_DestroyUPP destroy;
    NPP_SetWindowUPP setwindow;
    NPP_NewStreamUPP newstream;
    NPP_DestroyStreamUPP destroystream;
	NPP_StreamAsFileUPP asfile;
    NPP_WriteReadyUPP writeready;
    NPP_WriteUPP write;
    NPP_PrintUPP print;
    NPP_HandleEventUPP event;
} NPPluginFuncs;

typedef struct _NPNetscapeFuncs {
	uint16 size;
	uint16 version;
    NPN_GetURLUPP geturl;
    NPN_PostURLUPP posturl;
    NPN_RequestReadUPP requestread;
    NPN_NewStreamUPP newstream;
    NPN_WriteUPP write;
    NPN_DestroyStreamUPP destroystream;
    NPN_StatusUPP status;
    NPN_UserAgentUPP uagent;
	NPN_MemAllocUPP memalloc;
	NPN_MemFreeUPP memfree;
	NPN_MemFlushUPP memflush;
	NPN_ReloadPluginsUPP reloadplugins;
} NPNetscapeFuncs;



#ifdef XP_MAC
/******************************************************************************************
 * Mac platform-specific plugin glue stuff
 *******************************************************************************************/

/*
 * Main entry point of the plugin.
 * This routine will be called when the plugin is loaded. The function
 * tables are passed in and the plugin fills in the NPPluginFuncs table
 * and NPPShutdownUPP for Netscape's use.
 */

#if GENERATINGCFM

typedef UniversalProcPtr NPP_MainEntryUPP;
enum {
	uppNPP_MainEntryProcInfo = kThinkCStackBased
		| STACK_ROUTINE_PARAMETER(1, SIZE_CODE(sizeof(NPNetscapeFuncs*)))
		| STACK_ROUTINE_PARAMETER(2, SIZE_CODE(sizeof(NPPluginFuncs*)))
		| STACK_ROUTINE_PARAMETER(3, SIZE_CODE(sizeof(NPP_ShutdownUPP*)))
		| RESULT_SIZE(SIZE_CODE(sizeof(NPError)))
};
#define NewNPP_MainEntryProc(FUNC)		\
		(NPP_MainEntryUPP) NewRoutineDescriptor((ProcPtr)(FUNC), uppNPP_MainEntryProcInfo, GetCurrentArchitecture())
#define CallNPP_MainEntryProc(FUNC,  netscapeFunc, pluginFunc, shutdownUPP)		\
		CallUniversalProc((UniversalProcPtr)(FUNC), (ProcInfoType)uppNPP_MainEntryProcInfo, (netscapeFunc), (pluginFunc), (shutdownUPP))

#else

typedef NPError (*NPP_MainEntryUPP)(NPNetscapeFuncs*, NPPluginFuncs*, NPP_ShutdownUPP*);
#define NewNPP_MainEntryProc(FUNC)		\
		((NPP_MainEntryUPP) (FUNC))
#define CallNPP_MainEntryProc(FUNC,  netscapeFunc, pluginFunc, shutdownUPP)		\
		(*(FUNC))((netscapeFunc), (pluginFunc), (shutdownUPP))

#endif
#endif /* MAC */


#ifdef _WINDOWS

#ifdef __cplusplus
extern "C" {
#endif

/* plugin meta member functions */

NPError WINAPI NP_GetEntryPoints(NPPluginFuncs* pFuncs);

NPError WINAPI NP_Initialize(NPNetscapeFuncs* pFuncs);

NPError WINAPI NP_Shutdown();

#ifdef __cplusplus
}
#endif

#endif /* _WINDOWS */


#endif /* _NPUPP_H_ */

