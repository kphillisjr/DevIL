#include <time.h>
#include <stdarg.h>

#include "il_internal.h"
#include "il_logging.h"

/* Codes of log entries (warnings, errors etc.) */
//static ILstring log_codes;
/* Explanation of those codes */
//static ILconst_string il_log_codes_explanation[IL_LOG_CODES_COUNT];

ILstring ilLogFile = NULL;
ILint ilLogLevel = 0;

/* TRANSLATORS: Results of various actions. This may appear in the log file */
ILconst_string log_results[LOG_RES_COUNT] = {gettext_noop("[ OK ]"), gettext_noop("[BUSY]"), gettext_noop("[FAIL]"), gettext_noop("[ N/A]")};
ILconst_string log_codes_str[IL_LOG_CODES_COUNT] = {"(--)", "(EE)", "(WW)", "(II)", "(VV)", "(DD)"};
ILconst_string log_codes_emo[IL_LOG_CODES_COUNT] = {"d(^_^)", "(>_<)", "(0.o)", "(~_^)", "(^.^)", "(¬_¬)"};

ILconst_string * log_codes = log_codes_str; 


ILLogInfo iLogActionStart(ILenum loglevel, ILconst_string format, ...)
{
	ILLogInfo retval;
	/* TODO: Error checking and stuff*/
	retval.Level = loglevel;
	ILsizei buffer_size = sizeof(retval.Message);
	va_list args;
	va_start (args, format);
	vsnprintf(retval.Message, buffer_size, format, args);
	return retval;
}

void iLogActionResult(ILLogInfo * info_struct, ILenum result_code)
{
	ILsizei buffer_size = sizeof(info_struct->Message);
	ILsizei buffer_occupied = strlen(info_struct->Message);
	snprintf((info_struct->Message + buffer_occupied), buffer_size - buffer_occupied, "\t%s", _(log_results[result_code]));
	ilLogAddEntry(info_struct->Level, info_struct->Message);
}

ILconst_string il_log_codes_explanation[IL_LOG_CODES_COUNT] = 
	{"", /* this one is basically dummy stuff, for symmetry only*/
	gettext_noop("error entry"), 
	gettext_noop("warning entry"), 
	gettext_noop("info entry"), 
	gettext_noop("verbose (not important info) entry"), 
	gettext_noop("debug entry")};

/** The log has to begin somehow. This writes the head.
 */
void ilLogAddHeader()
{	
	if (ilLogFile == NULL)
		return;	/* Nowhere to log. Who could ask us to do so? */
	/* Just to print the full time info */
	time_t rawtime;
	time(& rawtime);
	/* Now we know what day it is etc :-) */

	/* open the file, yes */
	FILE * logfile;
	logfile = fopen(ilLogFile, "a+");

	fprintf(logfile, _("DevIL v. %s log level. %d, %s\n"), PACKAGE_VERSION, ilLogLevel, ctime(& rawtime));
	fputs( _("Build information follows:\n"), logfile);
	/* TRANSLATORS: like Built <day> at <time> using <compiler> */
	fprintf(logfile, _("Built %s at %s, using %s\n"), __DATE__, __TIME__, STRINGIFY_2(C_COMPILER));
	fprintf(logfile, "DevIL CFLAGS: %s\nDevIL LIBS: %s\n", DEVIL_CFLAGS_STR, DEVIL_LIBS_STR );
	#ifdef BUILD_MODULES
	fputs(_("Modules:\tModular build\n"), logfile);
	fprintf(logfile, _("Hardcoded modules list filename:\t%s/%s\n"), MODULES_PATH, MODULES_LST);
	fprintf(logfile, _("Modules flags:\n"));
	#else /* not having any modules */
	fputs(_("Modules:\tStatically linked\n"), logfile);
	fprintf(logfile, _("Convenience libs flags:\n"));
	#endif
	fprintf(logfile, "\tCFLAGS:\t %s\n\tLIBS:\t%s\n" , MODULES_CFLAGS_STR, MODULES_LIBS_STR);
	/* Modular build? What modules, where should they be, where are we looking for them this time, used modules.lst filename, */
	/* Using any ASM?*/

	/* TRANSLATORS: What various log entries mean */
	fputs(_("Logfile entries format description:\n"), logfile);
	int i;
	for(i = 1; i < IL_LOG_CODES_COUNT; i++)
	{
		ILconst_string supression = (i > ilLogLevel) ? _(" (messages supressed now)") : "";
		fprintf(logfile, "\t%s - %s%s,\n", log_codes[i], _(il_log_codes_explanation[i]), supression);
	}
	/* we cover up the trailing comma and put newlines to the end */

	/* TRANSLATORS: This is a placeholder that ends the logfile entries list */
	fputs( _("and that's it.\n"), logfile);
	if (ilLogLevel < IL_LOG_CODES_COUNT - 1)
		fprintf(logfile, _("Hint: Raise the log level (up to %d) to enable messages that are now supressed.\n"), IL_LOG_CODES_COUNT - 1);
	/* TRANSLATORS: This is the last sentence in the logfile header */
	fputs(_("Here we go!\n\n"), logfile);

	fclose(logfile);
}

/** This is intended to get called within ilInit
 * This function will take a look whether we want logging.
 * It will examine according env variables and adjust logfile
 * and loglevel according to the data.
 * If we detect a logfile and no or invalid loglevel spec, we provide
 * the default (IL_LOG_INFO) loglevel
 * */
ILuint ilInitLogging_internal()
{
	/* We init the ilLogFile and ilLogLevel to harmless default values */
	ilLogFile = NULL;
	ilLogLevel = 0;
	/* Take a look whether we are told to log unsing env variables */
	ILconst_string log_path = getenv( STRINGIFY_2(IL_LOGFILE_ENV) );
	if (log_path != NULL)
	{/* OK, so copy our obtained log file path to the global variable! */
		int log_path_length = strlen(log_path);
		ilLogFile = (ILstring)malloc(log_path_length + 1);
		/* the "+1" here means that we want the string to end by '\0' */
		strncpy(ilLogFile, log_path, log_path_length + 1);
		/* We have the logfile, so we change the default to something more meaningful */
		ilLogLevel = IL_LOG_INFO;
	}/* if (log_path != NULL) */
	if (ilLogFile == NULL)
		return 1; /* Not much left to do here, right? */
	/* That's it, now let's take a look at the loglevel... */
	ILconst_string log_level = getenv( STRINGIFY_2(IL_LOGLEVEL_ENV) );
	if (log_level != NULL)
	{
		/* error checking for strtol is best done like this */
		ILint error_check;
		errno = 0;
		/* we convert the value to long int using strtol */
		ilLogLevel = strtol(log_level, NULL, 10);
		error_check = errno;
		if (ilLogLevel < 0)
		{/* gge retsae */
			ilLogLevel *= -1;
			/* sssh... this is sec-ret! */
			log_codes = log_codes_emo;
			ilLogAddEntry(0, _("Yay! Bonus 'happy mode' unlocked!\n"));
		}
		if (error_check != 0 || ilLogLevel >= IL_LOG_CODES_COUNT)
			/* something went wrong */
			ilLogLevel = IL_LOG_INFO;
	}/* if (log_level != NULL) */
	/* OK, loglevel is now settled, too. Let's get started! */
	ilLogAddHeader();
}

/** If you want to control logging, you want to use this function
 * You don't really have a big motivation to use it if you want to enable logging 
 * since you can control logging using enviromental variables.
 * But you have to use it if you want to DISABLE logging even if your
 * user sets the env. variables.
 * To do so, just use ilLog(NULL, 0) after ilInit()
 * \param logfile Filename, where you want to have your log.
 * \param loglevel What to log. Pass (IL_LOG_CODES_COUNT - 1) to log everyting or RTFM
 */
ILAPI void ILAPIENTRY lLogInit(ILconst_string logfile, ILint loglevel)
{
	/* Are we logging? Maybe we only want to change loglevel... */
	if (ilLogFile != NULL && logfile != NULL)
	{/* So we are already logging. Let's end that now! */
		free(ilLogFile);
		ilLogFile = NULL;
	}
	/* We are not logging (any more) and we want to (re)start. OK! */
	if (ilLogFile == NULL && logfile != NULL)
	{
		ILsizei logfile_len = strlen(logfile) + 1;
		ilLogFile = (ILstring)malloc(logfile_len * sizeof(char));
		strncpy(ilLogFile, logfile, logfile_len);
	}
	else /* Well, what to do here... Not a fruitful call... Should be logged :-) */
		return;
	if (loglevel < 0)
	{	/* top sec-ret, ssssh! */
		loglevel *= -1;
		log_codes = log_codes_emo;
	}
	/* So now we change the loglevel... */
	ilLogLevel = loglevel % IL_LOG_CODES_COUNT;
	/* And hey, let's get started! */
	ilLogAddHeader();
}

/** Used to clear the mess - to to free resources taken by logging stuff
 * Currently it just frees the logfile filename and resets logging level to 0 :-)
 */
void ilLogEnd()
{
	if (ilLogFile != NULL)
	{
		free(ilLogFile);
		ilLogFile = NULL;
	}
	ilLogLevel = 0;
}

/** This cute little function returns time in HH:MM:SS format
 * It is static inside, but reentrant. */
ILstring ilGetSimpleTime()
{
	static time_t rawtime;
	static struct tm * timeinfo;
	static char buffer [32];

	time ( &rawtime );
	timeinfo = localtime ( &rawtime );

	strftime (buffer, sizeof(buffer), "%H:%M:%S", timeinfo);

	return buffer;
}

void ilLogAddEntry_ex(ILenum level, ILconst_string format, ...)
{
	ILbyte message[1042];
	va_list args;
	va_start (args, format);
	vsnprintf(message, sizeof(message), format, args);
	ilLogAddEntry(level, message);
}

/** Want to log something? This is the right function to use
 * \param text What to log
 * \param code What sort of message is this? 
 */
void ilLogAddEntry(ILenum level, ILconst_string text)
{
	if (ilLogFile == NULL || ilLogLevel < level)
		return;	/* Nowhere to log. Who could ask us to do so? */
	/* We have to open and then close the file, yes... */
	FILE * logfile;
	logfile = fopen(ilLogFile, "a+");
	/* using everything together here */
	fprintf(logfile, "%s %s: %s\n", log_codes[level], ilGetSimpleTime(), text);
	fclose(logfile);
}

