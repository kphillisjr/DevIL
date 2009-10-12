#ifndef IL_LOGGING_H
#define IL_LOGGING_H

#ifdef LOGGING_ENABLED
#define INIT_LOGGING_IFNEEDED ilInitLogging_internal()
/* TODO: Can anybody like this macro? I don't. */
#define IL_LOG_IFNEEDED(text, level) ilLogAddEntry((text), (level))
#else /* we don't want to log */
#define INIT_LOGGING_IFNEEDED /* No logging this time... */
#define IL_LOG_IFNEEDED(x, y) /* No logging this time... */
#endif /* !LOGGING_ENABLED */

ILuint ilInitLogging_internal();
void ilLogAddEntry(ILconst_string text, ILuint code);

ILconst_string ilLogAction(ILconst_string str, ILuint action);
/* actions could be: Start (=erase old), append, result (=output). */

#endif /* IL_LOGGING_H */
