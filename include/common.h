
#ifndef common_h
#define common_h

#include "scopeguard.h"

#define    DISALLOW_COPY_AND_ASSIGN(TypeName) \
TypeName(const TypeName&) = delete;                \
TypeName& operator=(const TypeName&)=delete


#endif /* common_h */
