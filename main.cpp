#include <stdio.h>
#include <stdlib.h>

#include "tree_lib.h"
#include "akinator_lib.h"
#include "fileLib.h"

int main()
{
    Akinator akinator = {};
    
    akinator_error error = interface(&akinator);

    if (error != akinator_ok)
        error_processing(error);
        
    return 0;
}