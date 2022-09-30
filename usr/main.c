#include <systemInit.h>
#include <pdsTask.h>
void main(void)
{
    /* !!!!!!!MUST BE called firstly here for initializing system parameters !!!!*/
    PDS_Init();
    /* System init for hardwre init */
    SYS_Init();
    /* system main infinite loop */
    for(;;){
      TM_RunTasks();
    }
}
