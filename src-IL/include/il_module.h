#ifndef IL_MODULE_H
#define IL_MODULE_H

//#include "il_internal.h"

#define PASS_1(X) X
#define PASS_2(X) PASS_1(X)

#define STRINGIFY_1(X) # X
#define STRINGIFY_2(X) STRINGIFY_1(X)

#define IDENTITY_1(X) X
#define IDENTITY_2(X) IDENTITY_1(X)
#define CONCAT_1(first,second,third) first ## second ## third
#define CONCAT_2(first,second,third) CONCAT_1(first,second,third)

#ifdef HAVE_MODULES
#define EXTERNAL_SYMBOL(sym) CONCAT_2(MODULE_NAME,_LTX_,sym)
#else	/* no modules, no name mangling and no exporting are needed */
#define EXTERNAL_SYMBOL(sym) sym
#endif

#define GET_MODULE_NAME get_module_name
#define GET_MODULE_NAME_DECL EXTERNAL_SYMBOL(GET_MODULE_NAME)

#endif /* IL_MODULE_H */
