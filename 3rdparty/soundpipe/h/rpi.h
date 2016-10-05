typedef struct sp_rpi{
    sp_data *sp;
    void *ud;
    void (*callback)(sp_data *, void *);
}sp_rpi;
int sp_rpi_process(sp_data *sp, void *ud, void (*callback)(sp_data *, void *));
