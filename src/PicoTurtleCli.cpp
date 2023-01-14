#include <zclk.h>
#include <stdio.h>
#include <coll_arraylist.h>

zclk_res basic_handler(zclk_command* cmd, void* handler_args)
{
    if(arraylist_length(cmd->args) > 0)
    {
        zclk_argument* turtle_prog = (zclk_argument*) arraylist_get(cmd->args, 0);
        printf("Program to run: %s\n", zclk_argument_get_val_string(turtle_prog));
    }
    else
    {
        printf("No input program.\n");
    }
    return ZCLK_RES_SUCCESS;
}

int main(int argc, char* argv[])
{
    zclk_command *cmd = new_zclk_command(argv[0], "ptcli",
                            "PicoTurtle CLI", &basic_handler);
    
    zclk_command_string_argument(
        cmd,
        "path_to_program",
        "",
        "",
        "Turtle lua program to execute",
        1
    );
    
    zclk_command_exec(cmd, NULL, argc, argv);
}
