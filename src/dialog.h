class dialog
{
public:
    dialog ();
    ~dialog ();
    
    char *error_text;                   // error message
    char *npc_text;                     // NPC´s part
    Array<char *> player_text;          // player´s part

    u_int8 init_dialog (char*, u_int32 = 0);
    s_int32 run_dialog (u_int32);
    
protected:
    u_int32 PC;                         // Program counter
    u_int32 str_num;                    // Number of Strings
    u_int32 cmd_num;                    // Number of Commands

    Array<dialog_cmd*> cur_cmds;        // New PC values
    Array<u_int32> used_text;           // Text already spoken -> don´t display again

    char **dlg_text;                    // Text data
    dialog_cmd **dlg_code;              // Program code

    u_int8 read_dialog_data (FILE *);
    u_int8 read_dialog_text (FILE *);
};
