class mappattern
{
    image * frame;
    u_int16 nbr_of_frames;
    u_int16 framecounter;
    u_int16 counterlimit;
    u_int16 currentframe;

     public:
    ~mappattern ();
    void update ();
    s_int8 get (FILE * file);
    s_int8 load (char name[]);
    void putbox (u_int16 x, u_int16 y);
    void putbox_mask (u_int16 x, u_int16 y);
    void putbox_part (u_int16 x, u_int16 y, u_int16 bw, u_int16 bh,
		      u_int16 xo, u_int16 yo);
    void putbox_trans (u_int16 x, u_int16 y, u_int8 alpha);
    void putbox_mask_part (u_int16 x, u_int16 y, u_int16 bw, u_int16 bh,
			   u_int16 xo, u_int16 yo);
    void putbox_mask_trans (u_int16 x, u_int16 y, u_int8 alpha);
    void putbox_part_trans (u_int16 x, u_int16 y, u_int16 bw, 
			    u_int16 bh, u_int16 xo, u_int16 yo, 
			    u_int8 alpha);
    void putbox_mask_part_trans (u_int16 x, u_int16 y, u_int16 bw, u_int16 bh,
				 u_int16 xo, u_int16 yo, u_int8 alpha);
};
