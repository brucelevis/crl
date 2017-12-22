/*
 * include_everywhere.h
 *
 *  Created on: Dec 22, 2017
 *      Author: joshua
 */

#ifndef INCLUDE_DEPRECATED_H_
#define INCLUDE_DEPRECATED_H_

#ifdef __GNUC__
#define DEPRECATED __attribute__((deprecated))
#elif defined(_MSC_VER)
#define DEPRECATED __declspec(deprecated)
#else
#pragma message("WARNING: You need to implement DEPRECATED for this compiler")
#define DEPRECATED
#endif

#endif /* INCLUDE_DEPRECATED_H_ */
