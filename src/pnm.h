
void * read_pnm(FILE * file, u_int16 * lenght, u_int16 * height);
void put_pnm(FILE * file, void * image, u_int16 lenght, u_int16 height);
int write_pnm(char * filename, void * image, u_int16 lenght, u_int16 height);
