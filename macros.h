/*
 * File:   macros.h
 * Author: andres
 *
 * Created on May 31, 2010, 1:12 PM
 */

#ifndef _MACROS_H
#define	_MACROS_H

#define DEBUG 1

/**
 * Print Macros
 *
 * These are used to add some safety checks to basic print operations.
 *
 * Some output format may also be modified here
 *
 * Contains different levels of debug operation
 */

#if DEBUG == 0

#define PRINTLN(str)  printf("%s\n", str)

#define PRINT(str)  printf("%s", str)

#elif DEBUG == 1

#define PRINTLN(str)  assert(str); printf("%s\n", str)

#define PRINT(str)  assert(str); printf("%s", str)

#endif

/**
 * Macros Related to formatting
 */
#define FEDGE "-->[%d:%d]"

#define FVERTEX "[%d] "

#define FPARTITIONHEADER "Partition %d.%d\n"

#define FPARTITION1 "%d"

#define FPARTITION2 "->%d"

/**
 * CLI parameters
 */

#define CLIFILE "-f"
/**
 * Testing Macros
 */
#define TSTART printf("----S %d----\n", testnum);

#define TEND printf("----E %d----\n\n", testnum++);

#define TESTGRAPH(code) TSTART code; TEND



#endif	/* _MACROS_H */

