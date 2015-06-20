/* --COPYRIGHT--,EPL
 *  Copyright (c) 2014 Texas Instruments and others.
 *  All rights reserved. This program and the accompanying materials
 *  are made available under the terms of the Eclipse Public License v1.0
 *  which accompanies this distribution, and is available at
 *  http://www.eclipse.org/legal/epl-v10.html
 * 
 *  Contributors:
 *      Texas Instruments - initial implementation
 * 
 * --/COPYRIGHT--*/
/*
 *  ======== main ========
 *  Every module may optionally define a main() function.  If it is defined,
 *  a program can be created with this function as its "entry point".
 */
int main(int argc, char *argv[])
{
    return 0;
}

#ifdef __TI_COMPILER_VERSION__

extern void _c_int00(void);

#pragma WEAK ( ResetISR )
void ResetISR(void)
{
    _c_int00();
}
#endif
