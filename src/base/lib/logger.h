/**
 * \file logger.h
 * \brief Logging utilities.
 * \bug Not multi-thread safe
 *
 * \author Mateusz Boryń <mateusz.boryn@gmail.com>
 */

#ifndef LOGGER_H_
#define LOGGER_H_

#include "base/lib/mrmath/homog_matrix.h"

namespace logger {

/** Is log enabled*/
extern bool log_enabled, log_dbg_enabled;

/**
 * Print message to the console only if logEnabled is set to true.
 * @param fmt printf-like format
 */
void log(const char *fmt, ...)
// Check if arguments follow printf-like format (see GCC documentation).
// 1 - number of argument with string format, 2 - first variable argument to check
__attribute__ ((format (printf, 1, 2)))
;

/**
 * Print Homog_matrix.
 * @param hm
 */
void log(const mrrocpp::lib::Homog_matrix & hm);

/**
 * Print message to the console only if logDbgEnabled is set to true.
 * @param fmt printf-like format
 */
void log_dbg(const char *fmt, ...)
// Check if arguments follow printf-like format (see GCC documentation).
// 1 - number of argument with string format, 2 - first variable argument to check
__attribute__ ((format (printf, 1, 2)))
;

/**
 * Print Homog_matrix.
 * @param hm
 */
void log_dbg(const mrrocpp::lib::Homog_matrix & hm);

} // namespace logger

#endif /* LOGGER_H_ */
