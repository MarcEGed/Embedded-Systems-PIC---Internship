#ifndef VERSION_H
#define VERSION_H

// Revision history
#define	FW_REV_MAJ	0	/*0 to FF*/
#define	FW_REV_MIN	0   /*0 to FF*/
#define	FW_REV_BLD	0	/*0 to FF*/

#if (defined (__PCH__) || defined (__PCD__))    /*CCS C compiler?*/
    #include "build_ccs.h"
#else
    #include "build.h"
#endif

/*Auto-compute date*/
#define   FW_REV_DAY   BUILD_DAY    /*Day*/
#define   FW_REV_MNT   BUILD_MONTH  /*Month*/
#define   FW_REV_YER   BUILD_YEAR   /*Year*/

//	V:0.0.0
    // Alpha

#endif /* VERSION_H */
