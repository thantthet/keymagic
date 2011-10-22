#include "StrTypeFunc.h"

errno_t km_strcat(_Inout_z_cap_(_Size) TCHAR * _Dst, _In_ rsize_t _Size, _In_z_ const TCHAR * _Src){
#ifdef UNICODE
	return wcscat_s(_Dst, _Size, _Src);
#else
	return strcat_s(_Dst, _Size, _Src);
#endif
}