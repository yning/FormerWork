/* this ALWAYS GENERATED file contains the definitions for the interfaces */


/* File created by MIDL compiler version 5.01.0164 */
/* at Tue Mar 06 11:17:03 2012
 */
/* Compiler settings for G:\webmind\WebMindMerge1.2\WebMind.idl:
    Oicf (OptLev=i2), W1, Zp8, env=Win32, ms_ext, c_ext
    error checks: allocation ref bounds_check enum stub_data 
*/
//@@MIDL_FILE_HEADING(  )


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 440
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__

#ifndef COM_NO_WINDOWS_H
#include "windows.h"
#include "ole2.h"
#endif /*COM_NO_WINDOWS_H*/

#ifndef __WebMind_i_h__
#define __WebMind_i_h__

#ifdef __cplusplus
extern "C"{
#endif 

/* Forward Declarations */ 

#ifndef __IWebMindNSHandle_FWD_DEFINED__
#define __IWebMindNSHandle_FWD_DEFINED__
typedef interface IWebMindNSHandle IWebMindNSHandle;
#endif 	/* __IWebMindNSHandle_FWD_DEFINED__ */


#ifndef __WebMindNSHandle_FWD_DEFINED__
#define __WebMindNSHandle_FWD_DEFINED__

#ifdef __cplusplus
typedef class WebMindNSHandle WebMindNSHandle;
#else
typedef struct WebMindNSHandle WebMindNSHandle;
#endif /* __cplusplus */

#endif 	/* __WebMindNSHandle_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

void __RPC_FAR * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void __RPC_FAR * ); 

#ifndef __IWebMindNSHandle_INTERFACE_DEFINED__
#define __IWebMindNSHandle_INTERFACE_DEFINED__

/* interface IWebMindNSHandle */
/* [unique][helpstring][uuid][object] */ 


EXTERN_C const IID IID_IWebMindNSHandle;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("25E81C73-A6C1-46B2-A83B-C05565643062")
    IWebMindNSHandle : public IUnknown
    {
    public:
    };
    
#else 	/* C style interface */

    typedef struct IWebMindNSHandleVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IWebMindNSHandle __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IWebMindNSHandle __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IWebMindNSHandle __RPC_FAR * This);
        
        END_INTERFACE
    } IWebMindNSHandleVtbl;

    interface IWebMindNSHandle
    {
        CONST_VTBL struct IWebMindNSHandleVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IWebMindNSHandle_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IWebMindNSHandle_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IWebMindNSHandle_Release(This)	\
    (This)->lpVtbl -> Release(This)


#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IWebMindNSHandle_INTERFACE_DEFINED__ */



#ifndef __WebMindLib_LIBRARY_DEFINED__
#define __WebMindLib_LIBRARY_DEFINED__

/* library WebMindLib */
/* [helpstring][version][uuid] */ 


EXTERN_C const IID LIBID_WebMindLib;

EXTERN_C const CLSID CLSID_WebMindNSHandle;

#ifdef __cplusplus

class DECLSPEC_UUID("B96C53BD-FADC-45C7-A72D-EB5230433A46")
WebMindNSHandle;
#endif
#endif /* __WebMindLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif
