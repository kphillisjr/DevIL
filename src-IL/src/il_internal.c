//-----------------------------------------------------------------------------
//
// ImageLib Sources
// Copyright (C) 2000-2008 by Denton Woods
// Last modified: 11/08/2008
//
// Filename: src-IL/src/il_internal.c
//
// Description: Internal stuff for DevIL
//
//-----------------------------------------------------------------------------


#include "il_internal.h"
#include <string.h>
#include <stdlib.h>

#ifdef BUILD_MODULES
#include "il_module.h"
#endif /* BUILD_MODULES */

ILimage *iCurImage = NULL;


/* Siigron: added this for Linux... a #define should work, but for some reason
	it doesn't (anyone who knows why?) */
#if !_WIN32 || (_WIN32 && __GNUC__) // Cygwin
	int stricmp(const char *src1, const char *src2)
	{
		return strcasecmp(src1, src2);
	}
	int strnicmp(const char *src1, const char *src2, size_t max)
	{
		return strncasecmp(src1, src2, max);
	}
#elif _WIN32_WCE
	int stricmp(const char *src1, const char *src2)
	{
		return _stricmp(src1, src2);
	}
	int strnicmp(const char *src1, const char *src2, size_t max)
	{
		return _strnicmp(src1, src2, max);
	}
#endif /* _WIN32 */

#ifdef _WIN32_WCE
	char *strdup(const char *src)
	{
		return _strdup(src);
	}
#endif//_WIN32_WCE


#ifdef _UNICODE
	int iStrCmp(ILconst_string src1, ILconst_string src2)
	{
		return wcsicmp(src1, src2);
	}
#else
	int iStrCmp(ILconst_string src1, ILconst_string src2)
	{
		return stricmp(src1, src2);
	}
#endif


//! Glut's portability.txt says to use this...
ILstring ilStrDup(ILconst_string Str)
{
	ILstring copy;

	copy = (ILstring)ialloc((ilStrLen(Str) + 1) * sizeof(ILchar));
	if (copy == NULL)
		return NULL;
	iStrCpy(copy, Str);
	return copy;
}


// Because MSVC++'s version is too stupid to check for NULL...
/* TODO: Maybe return ILsizei? */
ILsizei ilStrLen(ILconst_string Str)
{
	ILconst_string eos = Str;

	if (Str == NULL)
		return 0;

	while (*eos++);

	return((ILsizei)(eos - Str - 1));
}


// Because MSVC++'s version is too stupid to check for NULL...
//  Passing NULL to strlen will definitely cause a crash.
ILuint ilCharStrLen(const char *Str)
{
	const char *eos = Str;

	if (Str == NULL)
		return 0;

	while (*eos++);

	return((int)(eos - Str - 1));
}


// Simple function to test if a filename has a given extension, disregarding case
ILboolean iCheckExtension(ILconst_string Arg, ILconst_string Ext)
{
	ILboolean PeriodFound = IL_FALSE;
	ILint i, Len;
	ILstring Argu = (ILstring)Arg;

	if (Arg == NULL || Ext == NULL || !ilStrLen(Arg) || !ilStrLen(Ext))  // if not a good filename/extension, exit early
		return IL_FALSE;

	Len = ilStrLen(Arg);
	Argu += Len;  // start at the end

	for (i = Len; i >= 0; i--) {
		if (*Argu == '.') {  // try to find a period 
			PeriodFound = IL_TRUE;
			break;
		}
		Argu--;
	}

	if (!PeriodFound)  // if no period, no extension
		return IL_FALSE;

	if (!iStrCmp(Argu+1, Ext))  // extension and ext match?
		return IL_TRUE;

	return IL_FALSE;  // if all else fails, return IL_FALSE
}


ILstring iGetExtension(ILconst_string FileName)
{
	ILboolean PeriodFound = IL_FALSE;
	ILstring Ext = (ILstring)FileName;
	ILint i, Len = ilStrLen(FileName);

	if (FileName == NULL || !Len)  // if not a good filename/extension, exit early
		return NULL;

	Ext += Len;  // start at the end

	for (i = Len; i >= 0; i--) {
		if (*Ext == '.') {  // try to find a period 
			PeriodFound = IL_TRUE;
			break;
		}
		Ext--;
	}

	if (!PeriodFound)  // if no period, no extension
		return NULL;

	return Ext+1;
}


// Checks if the file exists
ILboolean iFileExists(ILconst_string FileName)
{
#if (!defined(_UNICODE) || !defined(_WIN32))
	FILE *CheckFile = fopen(FileName, "rb");
#else // Windows uses _wfopen instead.
	FILE *CheckFile = _wfopen(FileName, L"rb");
#endif//_UNICODE

	if (CheckFile) {
		fclose(CheckFile);
		return IL_TRUE;
	}
	return IL_FALSE;
}


// Last time I tried, MSVC++'s fgets() was really really screwy
ILbyte *iFgets(char *buffer, ILuint maxlen)
{
	ILuint	counter = 0;
	ILint	temp = '\0';

	while ((temp = igetc()) && temp != '\n' && temp != IL_EOF && counter < maxlen) {
		buffer[counter] = temp;
		counter++;
	}
	buffer[counter] = '\0';
	
	if (temp == IL_EOF && counter == 0)  // Only return NULL if no data was "got".
		return NULL;

	return (ILbyte*)buffer;
}


// A fast integer squareroot, completely accurate for x < 289.
// Taken from http://atoms.org.uk/sqrt/
// There is also a version that is accurate for all integers
// < 2^31, if we should need it

static int table[] = {
	0,    16,  22,  27,  32,  35,  39,  42,  45,  48,  50,  53,  55,  57,
	59,   61,  64,  65,  67,  69,  71,  73,  75,  76,  78,  80,  81,  83,
	84,   86,  87,  89,  90,  91,  93,  94,  96,  97,  98,  99, 101, 102,
	103, 104, 106, 107, 108, 109, 110, 112, 113, 114, 115, 116, 117, 118,
	119, 120, 121, 122, 123, 124, 125, 126, 128, 128, 129, 130, 131, 132,
	133, 134, 135, 136, 137, 138, 139, 140, 141, 142, 143, 144, 144, 145,
	146, 147, 148, 149, 150, 150, 151, 152, 153, 154, 155, 155, 156, 157,
	158, 159, 160, 160, 161, 162, 163, 163, 164, 165, 166, 167, 167, 168,
	169, 170, 170, 171, 172, 173, 173, 174, 175, 176, 176, 177, 178, 178,
	179, 180, 181, 181, 182, 183, 183, 184, 185, 185, 186, 187, 187, 188,
	189, 189, 190, 191, 192, 192, 193, 193, 194, 195, 195, 196, 197, 197,
	198, 199, 199, 200, 201, 201, 202, 203, 203, 204, 204, 205, 206, 206,
	207, 208, 208, 209, 209, 210, 211, 211, 212, 212, 213, 214, 214, 215,
	215, 216, 217, 217, 218, 218, 219, 219, 220, 221, 221, 222, 222, 223,
	224, 224, 225, 225, 226, 226, 227, 227, 228, 229, 229, 230, 230, 231,
	231, 232, 232, 233, 234, 234, 235, 235, 236, 236, 237, 237, 238, 238,
	239, 240, 240, 241, 241, 242, 242, 243, 243, 244, 244, 245, 245, 246,
	246, 247, 247, 248, 248, 249, 249, 250, 250, 251, 251, 252, 252, 253,
	253, 254, 254, 255
};

int iSqrt(int x) {
	if (x >= 0x10000) {
		if (x >= 0x1000000) {
			if (x >= 0x10000000) {
				if (x >= 0x40000000) {
					return (table[x >> 24] << 8);
				} else {
					return (table[x >> 22] << 7);
				}
			} else if (x >= 0x4000000) {
				return (table[x >> 20] << 6);
			} else {
				return (table[x >> 18] << 5);
			}
		} else if (x >= 0x100000) {
			if (x >= 0x400000) {
				return (table[x >> 16] << 4);
			} else {
				return (table[x >> 14] << 3);
			}
		} else if (x >= 0x40000) {
			return (table[x >> 12] << 2);
		} else {
			return (table[x >> 10] << 1);
		}
	} else if (x >= 0x100) {
		if (x >= 0x1000) {
			if (x >= 0x4000) {
				return (table[x >> 8]);
			} else {
				return (table[x >> 6] >> 1);
			}
		} else if (x >= 0x400) {
			return (table[x >> 4] >> 2);
		} else {
			return (table[x >> 2] >> 3);
		}
	} else if (x >= 0) {
		return table[x] >> 4;
	}

	//hm, x was negative....
	return -1;
}

#if defined BUILD_MODULES && defined MODULES_LST
/**
 * We have to have MODULES_LST macro #defined in order to be able to load anythig. 
 * This macro is typically #defined during compilation using the -D switch
 *
 * \return Pointer to the Modules structure with all relevant information about loaded modules.
 * Remember to free it using destroy_modules when you no longer need it.
 */
Modules * create_modules()
{
	/* Let's take a look whether we did not override the path to the modules */
	ILconst_string env_path = getenv( STRINGIFY_2(IL_MODULES_ENV) );
	ILconst_string pathname = STRINGIFY_2(IL_MODULES_ENV);
	/* and let's do our stuff accordingly then */
	ILconst_string modules_dir = (env_path == NULL ? MODULES_PATH : env_path);
	if (env_path != NULL)
	{	LOG_ADVANCED(IL_LOG_VERBOSE, _("Have detected modules path override '%s' -> '%s'"), MODULES_PATH, env_path); }

	Modules * retval = (Modules *)malloc(sizeof(Modules));
	ILsizei modules_lst_path_length = strlen(modules_dir) + strlen(MODULES_LST) + 2;
	/* We need space for the slash '/' between modules_dir string and MODULES_LST string */
	char * modules_lst_filename = (char *)malloc(sizeof(char) * modules_lst_path_length);
	/*TODO: exception handling */
	strcpy(modules_lst_filename, modules_dir);
	strcat(modules_lst_filename, "/");
	strcat(modules_lst_filename, MODULES_LST);
	/* Ho, let's open the modules.lst - the macro should be passed at compile time */
	FILE * Modules_lst = fopen(modules_lst_filename, "r");
	/* Something went wrong... */
	if (Modules_lst == NULL)
	{
		LOG_ADVANCED(IL_LOG_ERROR, _("We couldn't open the filename '%s' with the modules list. Don't expect anything from DevIL this time."), modules_lst_filename);
		/* TODO: Maybe some error could be set here */
		return NULL;
	}
	/* First let's find out how many modules will we probably load. We will find out the exact number later */
	int max_modules_count = 0, real_modules_count = 0;
	char prev_c, curr_c = '\n', in_comment_block;
	do {
		prev_c = curr_c;
		curr_c = fgetc (Modules_lst);
		/* The new modules have the ':' sign on the line */
		if (curr_c == ':' && in_comment_block == 0) 
			max_modules_count++;
		/* We don't want to count ':' inside comments */
		if (curr_c == '#' && prev_c == '\n') 
			in_comment_block = 1;
		/* The comment ends if the line ends... */
		if (curr_c == '\n') 
			in_comment_block = 0;
	} while (curr_c != EOF);
	/* Now let's allocate the memory. We will probably realloc it later */
	retval->Num_modules = max_modules_count;
	retval->Module_names = (char **)calloc(max_modules_count, sizeof(char *) );
	retval->Module_formats = (char **)calloc(max_modules_count, sizeof(char *) );
	retval->Module_handles = (lt_dlhandle *)calloc(max_modules_count, sizeof(lt_dlhandle) );

	char line_buffer [1024];
	/* We should be generous while we have that Unicode around... */
	char modname_buffer[128], formats_buffer[1024];
	ILsizei modname_length, formats_length, sscanf_success;
	/* go back to the beginning */
	rewind(Modules_lst);
	/* Read a line */
	while (fscanf(Modules_lst, "%1023[^\n]\n", line_buffer) != EOF)
	{
		/* leap ahead, we don't want to run in a circle */
		//fseek(Modules_lst, strlen(line_buffer), SEEK_CUR);
		if (line_buffer[0] == '#')	/* Comment encountered */
			continue;	
		/* Extract the module name and extensions list. */
		sscanf_success = sscanf(line_buffer, "%63[^:]: %1023[^\n]c", modname_buffer, formats_buffer);
		if (sscanf_success == 2)
		{
			/* Copy the short module name string to the appropriate place in the modules struct*/
			modname_length = strlen(modname_buffer) + 1;
			retval->Module_names[real_modules_count] = (char *)malloc(sizeof(char) * modname_length);
			strncpy(retval->Module_names[real_modules_count], modname_buffer, modname_length);
			/* Copy the long formats string to the appropriate place in the modules struct*/
			formats_length = strlen(formats_buffer) + 1;
			retval->Module_formats[real_modules_count] = (char *)malloc(sizeof(char) * formats_length);
			strncpy(retval->Module_formats[real_modules_count], formats_buffer, formats_length);
			/* Well, we have found a line matching the <module>: <formats> pattern */
			/* we are using real_modules_count as index, so it is incremented in the end... */
			real_modules_count++;
		}/* endif(sscanf_success == 2) */
	}
	/* Close the file */
	fclose(Modules_lst);
	free(modules_lst_filename); modules_lst_filename = NULL;
	/* Maybe we have detected wrong number of modules in the modules.lst file... */
	if (max_modules_count != real_modules_count)
	{
		LOG_ADVANCED(IL_LOG_WARNING, _("The loaded '%s' file is slightly confusing. This may be caused by improper manual manipulation."), MODULES_LST);
		LOG_ADVANCED(IL_LOG_VERBOSE, _("Expected %u modules, got %u."), max_modules_count, real_modules_count);
		retval->Num_modules = real_modules_count;
		retval->Module_names = (char **)realloc(retval->Module_names, sizeof(char *) * real_modules_count );
		retval->Module_formats = (char **)realloc(retval->Module_formats, sizeof(char *) * real_modules_count );
		retval->Module_handles = (lt_dlhandle *)realloc(retval->Module_handles, sizeof(lt_dlhandle) * real_modules_count );
	}

	/* How many characters are before the last slash? And we will put one slash at the end later..*/
	ILsizei modules_path_length = strlen(modules_dir) + 2;
	/* Let's make some space to store the directory part of the modules path */
	char * modules_path = (char *)malloc(sizeof(char) * modules_path_length + 1 );
	/* and copy the chars there */
	strcpy(modules_path, modules_dir);
	strcat(modules_path, "/");

	/* Now it is time to load the modules! */
	int i;
	for (i = 0; i < retval->Num_modules; i++)
	{
		char * module_filename;
		/* find out the filename of the module */
		module_filename = (char *)malloc( (strlen(modules_path) + strlen(retval->Module_names[i]) + 1) * sizeof(char) );
		strcpy(module_filename, modules_path);
		strcat(module_filename, retval->Module_names[i]);
		/* Now: Load the module! And store its handle... */
		retval->Module_handles[i] = lt_dlopenext(module_filename); //tends to segfault...

		LOG_ACTION_BEGIN(load_module, IL_LOG_VERBOSE, _("Trying to load module %s"), retval->Module_names[i]);
		if (retval->Module_handles[i] == NULL)
		{
			LOG_ACTION_END(load_module, LOG_RES_FAIL);
			LOG_ADVANCED_DEBUG(_("Reason of failiure: %s"), lt_dlerror());
		}
		else
		{
			LOG_ACTION_END(load_module, (retval->Module_handles[i] != NULL) ? LOG_RES_OK : LOG_RES_FAIL);
		}
		/* throw away the filename */
		free(module_filename);	module_filename = NULL;
	}

	/* We don't need the modules_path any longer...  */
	//free(modules_path);	modules_path = NULL;

	return retval;
}
#elif defined BUILD_MODULES && !defined MODULES_LST /* BUILD_MODULES && !MODULES_LST */
/*
 * if we want modules, but without the modules.lst list.
 * This approach allows extending DevIL functionality by 
 * totally free adding and removing of modules without having to edit
 * modules.lst files.
 * However, the loading of modules is more complicated...
 */
typedef const char * (* get_string_ptr)();
/*
 * The dirent stuff will have to go here...
 */
Modules * create_modules()
{
	DIR * dir = opendir(MODULES_PATH);
	struct dirent * dp;          /* returned from readdir() */
	ILsizei filecount = 0;       /* number of entries in directory */
	ILsizei i = 0;
	char ** files;

	if (dir == NULL) 
	{
		return NULL;            /* opendir() failed */
	}/* endif (dir == NULL)  */
	/* Pass 1: Count number of files and subdirectories */
	while ((dp = readdir(dir)) != NULL) 
		filecount++;
	/* Allocate space for the result */
	files = (char **)malloc(filecount * sizeof (*files));
	if (files == NULL) 
	{
		return NULL;            /* malloc failed */
	}/* endif (files == NULL)  */

	/* Pass 2: Collect all filenames */
	rewinddir (dir);
	int j;
	for (i = 0; (dp = readdir(dir)) != NULL; i++) 
	{
		const char * extension = strchr(dp->d_name, '.');
		files[i] = strndup(dp->d_name, extension - dp->d_name - 1);
		for (j = 0; j < i; j++)
			if (strstr(files[j], files[i]) != NULL)
			{/* = we have that filename here already */
				/* So we don't want it */
				free(files[i]); files[i] = NULL;
				i--;	/* We can move on and rewrite the current files[i] next time */
				break;
			}
		if (files[i] == NULL) 
		{
		    /* memory allocation failure; free what's been allocated
		     * so far and return NULL.
		     */
		    for (i-- ;i > 0; i--) 
			free(files[i]);
		    free(files);
		    files = NULL;
		    return NULL;
		}/* endif (files[i] == NULL)  */
	}/* endfor (i = 0; (dp = readdir(dir)) != NULL; i++)  */
	closedir (dir);
	/* now we got 'filecount' of 'files' - list of filenames... */
	Modules * retval = (Modules *)malloc(sizeof(Modules));
	int max_modules_count = filecount;
	retval->Num_modules = max_modules_count;
	retval->Module_names = (char **)calloc(max_modules_count, sizeof(char *) );
	retval->Module_formats = (char **)calloc(max_modules_count, sizeof(char *) );
	retval->Module_handles = (lt_dlhandle *)calloc(max_modules_count, sizeof(lt_dlhandle) );
	int real_modules_count = ;
	for (i = 0; i < max_modules_count; i++)
	{
		lt_dlhandle module_handle = lt_dlopenext(files[i]);
		get_string_ptr get_module_name = (get_string_ptr)lt_dlsym(module_handle, STRINGIFY_1(GET_MODULE_NAME));
		get_string_ptr get_module_formats = (get_string_ptr)lt_dlsym(module_handle, STRINGIFY_1(GET_MODULE_FORMATS));
		if (get_module_name == NULL || get_module_formats == NULL)
		{/* oh dear, wrong! */
			LOG_ADVANCED_DEBUG(_("Failed to identify a module") );
			continue;
		}
		retval->Module_handles[i] = module_handle;

		ILconst_string * module_name = get_module_name();
		ILsizei module_name_length = strlen(module_name) + 1;
		retval->Module_names[real_modules_count] = (char *)malloc(sizeof(char) * module_name_length);
		strncpy(retval->Module_names[real_modules_count], module_name, module_name_length);

		ILconst_string * module_formats = get_module_formats();
		ILsizei module_formats_length = strlen(module_formats) + 1;
		retval->Module_formats[real_modules_count] = (char *)malloc(sizeof(char) * module_formats_length);
		strncpy(retval->Module_formats[real_modules_count], module_formats, module_formats_length);
		LOG_ADVANCED_DEBUG(_("Module '%s' supports these formats: '%s'"), module_name, module_formats);

		real_modules_count++;
	}

	return retval;
}
#endif /* NOTHING DEFINED */

#ifdef BUILD_MODULES
void destroy_modules(Modules * modules)
{
	int i;
	for (i = 0; i < modules->Num_modules; i++)
	{/* Firs of all, free all strings */
		free(modules->Module_names[i]);	modules->Module_names[i] = NULL;
		free(modules->Module_formats[i]);	modules->Module_formats[i] = NULL;
	}
	/* And free the list in the end */
	free(modules->Module_names);	modules->Module_names = NULL;
	free(modules->Module_formats);	modules->Module_formats = NULL;
	/* The list of handles next */
	free(modules->Module_handles);	modules->Module_handles = NULL;
	/* Free the module pointer comes next */
	free(modules);	modules = NULL;
}
#endif /* !BUILD_MODULES */

Format Formats[IL_FORMATS_COUNT];

/**
 * \param format Pointer to the Format structure. Typically a library's private static variable. This is the output of the function...
 * \param modules The pointer to modules that we have obtained earlier (probably using the create_modules function)
 * \param format_name The string format ID that is used to identify formats. Like BMP, TGA, JPEG...
 * \param format_extensions Space-separated lowercase extensions list, like "jpeg jpg"
 */
void Set_format(Format * format, const char * format_name, const char * format_extensions)
{
	/* How are we going to divide format extensions? */
	const char delimiter = ' ';
	/* How many tokens are in the format_extensions string? */
	ILsizei i, num_tokens = 1;
	for (i = 0; format_extensions[i] != '\0'; i++)
		if (format_extensions[i] == delimiter)
			num_tokens++;
	/* now we know how many extensions are we going to need */
	format->Extensions = (char **)malloc(sizeof(char *) * (num_tokens + 1));
	/* begin parsing */
	/* let's create the sscanf's format string at runtime! */
	char sscanf_format_str[32];
	/* to store extensions during parsing */
	char extension[32];
	/* flexible creation of the sscanf's format string */
	sprintf(sscanf_format_str, "%%%ds%c%%n", sizeof(extension) - 1, delimiter);
	/* let's point to the first extension in the list... */
	const char * current_extension = format_extensions;
	/* how many characters will we read? */
	int advance;
	for(i = 0; sscanf(current_extension, sscanf_format_str, extension, & advance) != EOF; i++)
	{
		/* Just let's advance the pointer to the next extension in the extensions string */
		current_extension += advance;
		/* We got the extension! Let's make place for it */
		format->Extensions[i] = (char *)malloc(sizeof(char) * (strlen(extension) + 1));
		/* and copy it...*/
		if (format->Extensions[i] != NULL)
			strcpy(format->Extensions[i], extension);
	}
	/* The last extension has to be followed by NULL. */
	format->Extensions[i] = NULL;
	/* Now let's just copy the name where it belongs. */
	format->Name = (char *)malloc(sizeof(char) * (strlen(format_name) + 1));
	if (format->Name != NULL)
		strcpy(format->Name, format_name);
}

#ifdef BUILD_MODULES

void Set_format_modular(Format * format, const Modules * modules, ILconst_string format_name, ILconst_string format_extensions)
{
	/* Do the boring stuff first */
	Set_format(format, format_name, format_extensions);
	/* TODO: What if modules == NULL? */
	load_callbacks(modules, & format->Callbacks, format->Name);
	/* 
	 * OK, now the biggest fun - let's load the callbacks!
	 * However, let's check whether we can do that.
	 * We might want to abuse the Set_format function only to settle name, extensions etc.
	 */
	if (modules != NULL)
		load_callbacks(modules, & format->Callbacks, format->Name);
}

/**
 * \param modules The pointer to modules that we have loaded so far
 * \param callbacks Where to store results
 * \param name Name of the format, modules.lst style
 */
//TODO: load_callbacks needs a #ifndef BUILD_MODULES
void load_callbacks(const Modules * modules, Format_functions * callbacks, const char * name)
{
	/* What if someone, with malicious intentions, passed null modules pointer? */
	if (modules == NULL)
		return;
	/* First find out which module is the good one */
	int i; 
	lt_dlhandle module_handle = 0;
	for (i = 0; i < modules->Num_modules; i++)
		if (strstr(modules->Module_formats[i], name) != NULL)
		{
			/* the format name was in the module's format list */
			module_handle = modules->Module_handles[i];
			break;
		}
	/* Then load everything! */
	/* vectorize everything */
	/* This is not to be translated. */
	const char * operation_type[3] = {"", "F", "L"}; /* examining file, lump? */
	const char * function_type[3] = {"ilIsValid", "ilLoad", "ilSave"}; /*what to do? */
	/* This is OK. lt_dlsym returns generic pointer anyway... */
	/* TODO: This is quite type unsafe. But does it matter? */
	void ** function_pointers [9] =	{
		(void **)(& callbacks->ilIsValid), (void **)(& callbacks->ilIsValidF), (void **)(& callbacks->ilIsValidL),
		(void **)(& callbacks->ilLoad), (void **)(& callbacks->ilLoadF), (void **)(& callbacks->ilLoadL),
       		(void **)(& callbacks->ilSave), (void **)(& callbacks->ilSaveF), (void **)(& callbacks->ilSaveL)};
	/* We have to assemble the symbol name first */
	char symbol_name [64];
	const symname_length = 63;
	int j;
	for (i = 0; i < 3; i++)
		for (j = 0; j < 3; j++)
		{
			/* Now make the symbol name */
			strcpy(symbol_name, function_type[i]);
			/* now we have ilLoad */
			strcat(symbol_name, operation_type[j]);
			/* ilLoadF */
			strcat(symbol_name, "_");
			/* ilLoadF_ */
			strncat(symbol_name, name, symname_length - strlen(symbol_name));
			/* ilLoadF_BMP */
			/* Save the symbol. It doesn't matter if it is NULL... */
			LOG_ADVANCED_DEBUG(_("Loading of function %s: %d"), symbol_name, * function_pointers[i * 3 + j] );
			* function_pointers[i * 3 + j] = lt_dlsym(module_handle, symbol_name);
		}
}

#else /* NO_MODULES */

void Set_format_static(Format * format, const char * format_name, const char * format_extensions 
		,ilIsValid_ptr isvalid, ilIsValidF_ptr isvalidF, ilIsValidL_ptr isvalidL
		,ilLoad_ptr load, ilLoadF_ptr loadF, ilLoadL_ptr loadL
		,ilSave_ptr save, ilSaveF_ptr saveF, ilSaveL_ptr saveL)
{
	Set_format(format, format_name, format_extensions);
	format->Callbacks.ilIsValid = isvalid;
	format->Callbacks.ilIsValidF = isvalidF;
	format->Callbacks.ilIsValidL = isvalidL;
	format->Callbacks.ilLoad  = load;
	format->Callbacks.ilLoadF = loadF;
	format->Callbacks.ilLoadL = loadL;
	format->Callbacks.ilSave  = save;
	format->Callbacks.ilSaveF = saveF;
	format->Callbacks.ilSaveL = saveL;
}
#endif /* NO_MODULES */

void destroy_format(Format * format)
{
	int i;
	if (format == NULL)
		return;
	for(i = 0; format->Extensions[i] != NULL; i++)
	{
		free(format->Extensions[i]);
		format->Extensions[i] = NULL;
	}
	free(format->Name);
	format->Name = NULL;
	/* We don't delete the format itself since it is not typically allocated on the heap */
}

