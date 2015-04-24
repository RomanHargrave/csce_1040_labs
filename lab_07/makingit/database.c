/* ****************************************************************** */
/*                                                                    */
/* main.c --- driver to test the set implementation                   */
/*                                                                    */
/* Author: Philip Sweany                                              */
/* No rights reserved                                                 */
/*                                                                    */
/* ****************************************************************** */

#include "defs.h"
#include "parse.h"

int main()
{
    buildDataBase();
    parseAndExecuteCommands();

    return 0;
}

