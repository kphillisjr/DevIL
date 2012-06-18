#ifndef IL_LOGGING_H
#define IL_LOGGING_H

#ifdef LOGGING_ENABLED

#ifdef _DEBUG
#define LOG_ADVANCED_DEBUG(format, ...) LOG_ADVANCED(IL_LOG_DEBUG, format, __VA_ARGS__)
#define IL_LOG_IFNEEDED_DEBUG(text) IL_LOG_IFNEEDED((text), IL_LOG_DEBUG)
/* I don't like those two since they change the code quite significantly... */
//#define LOG_ACTION_BEGIN_DEBUG(id, format, ...) LOG_ACTION_BEGIN(id, IL_LOG_DEBUG, format, __VA_ARGS__)
//#define LOG_ACTION_END_DEBUG(id, result) LOG_ACTION_END(id, result)
#else /* NOT _DEBUG*/
#define LOG_ADVANCED_DEBUG(...) /* nothing here */
#define IL_LOG_IFNEEDED_DEBUG(x) /* neither here */
//#define LOG_ACTION_BEGIN_DEBUG(...) /* and so on... */
//#define LOG_ACTION_END_DEBUG(...) /* Zzzz.. */
#endif /* NOT _DEBUG */

#define INIT_LOGGING_IFNEEDED ilInitLogging_internal()
/* TODO: Can anybody like this macro? I don't. */
#define IL_LOG_IFNEEDED(text, level) ilLogAddEntry((level), (text))
/**/
#define LOG_ACTION_BEGIN(id, loglevel, format, ...)\
	ILLogInfo log_action_ ## id = {"", 0};\
	if((loglevel) <= ilLogLevel )\
		log_action_ ## id = iLogActionStart((loglevel), format, __VA_ARGS__);\
	else {} /* our "if" is now neutralized. */

#define LOG_ACTION_END(id, result)\
	if (log_action_ ## id.Message != NUL)\
		iLogActionResult(& log_action_ ## id, result);\
	else {} /* again neutralising effect of "if" */

#define LOG_ADVANCED(loglevel, format, ...)\
	if ((loglevel) <= ilLogLevel) \
		ilLogAddEntry_ex((loglevel), (format), __VA_ARGS__);\
	else {}	/* else do nothing */
#else /* we don't want to log */
/* debug log functions don't have to disturb us since they are using "normal" logging macros*/
#define INIT_LOGGING_IFNEEDED /* No logging this time... */
#define IL_LOG_IFNEEDED(x, y) /* No logging this time... */
#define LOG_ACTION_BEGIN(...) /* No... logging... */
#define LOG_ACTION_END(...) /* no log */
#define LOG_ADVANCED(...) /* nothing */
#endif /* !LOGGING_ENABLED */

enum {LOG_RES_OK, LOG_RES_BUSY, LOG_RES_FAIL, LOG_RES_NA, LOG_RES_COUNT };

/** Stores info about current task that is being logged.
 * The basic idea is that a unique structure (pointer :-() is created
 */
struct IL_LOG_INFO
{
	ILbyte Message[1024];
	ILenum Level;
};
typedef struct IL_LOG_INFO ILLogInfo;

ILuint ilInitLogging_internal();
void ilLogAddEntry(ILenum loglevel, ILconst_string text);
void ilLogAddEntry_ex(ILenum loglevel, ILconst_string format, ...);

void iLogActionResult(ILLogInfo * info_struct, ILenum result_code);
ILLogInfo iLogActionStart(ILenum loglevel, ILconst_string format, ...);

ILstring ilGetSimpleTime();

#endif /* IL_LOGGING_H */
