class dialog
{
public:
    dialog ();
    ~dialog ();

    static u_int32 *offset;
    static u_int32 *length;

    u_int32 answer;
    char *text_file;
    char *npc_text;                     // NPC´s part
    Array<char*> player_text;           // player´s part
    Array<u_int32> cur_cmds;            // New PC values
    Array<u_int32> used_text;           // Text already spoken -> don´t display again
    Array<char*> strings;               // Text data
};

u_int32 *dialog::offset;
u_int32 *dialog::length;
