class return_cmd : public command
{
    NEW_CMD(return_cmd)

public:
    void init (s_int32 *buffer, u_int32 &i, void *data)
    {
        retval = buffer[i++];
    }
    s_int32 run (u_int32 &PC, void *data)
    {
        return retval;
    }

private:
    s_int32 retval;
};

class import_cmd : public command
{
    NEW_CMD(import_cmd)

public:
    void init (s_int32 *buffer, u_int32 &i, void *data)
    {
        text = buffer[i++];
    }
    s_int32 run (u_int32 &PC, void *data);

private:
    u_int32 text;
};

class clear_cmd : public command
{
    NEW_CMD(clear_cmd)

public:
    void init (s_int32 *buffer, u_int32 &i, void *data) 
    { 
        return;
    }
    s_int32 run (u_int32 &PC, void *data);
};

class npctext_cmd : public command
{
    NEW_CMD(npctext_cmd)

public:
    void init (s_int32 *buffer, u_int32 &i, void *data)
    {
        text = buffer[i++];
    }
    s_int32 run (u_int32 &PC, void *data);

private:
    u_int32 text;
};

class ptext_cmd : public command
{
    NEW_CMD(ptext_cmd)

public:
    void init (s_int32 *buffer, u_int32 &i, void *data)
    {
        text = buffer[i++];
        new_pc = buffer[i++];
    }
    s_int32 run (u_int32 &PC, void *data);

private:
    u_int32 text;
    u_int32 new_pc;
};

class snpctext_cmd : public command
{
    NEW_CMD(snpctext_cmd)

public:
    void init (s_int32 *buffer, u_int32 &i, void *data)
    {
        text = buffer[i++];
        new_pc = buffer[i++];
    }
    s_int32 run (u_int32 &PC, void *data);

private:
    u_int32 text;
    u_int32 new_pc;
};