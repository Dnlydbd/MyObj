

#define COCOL_SAFEFREE(p)	if(p) {free(p),p = NULL;} else {p = NULL;}



int  cocol_string_dup(struct cocol_type_header *pdes,struct cocol_type_header *psrc);
void cocol_string_print(struct cocol_type_header *ptmp);
void cocol_string_free(struct cocol_type_header *ptmp);
int cocol_string_init(struct cocol_type_header *ptmp,void *);

int  cocol_char_dup(struct cocol_type_header *pdes,struct cocol_type_header *psrc);
void cocol_char_free(struct cocol_type_header *ptmp);
void cocol_char_print(struct cocol_type_header *ptmp);
int cocol_char_init(struct cocol_type_header *ptmp,void *);

int cocol_int_dup(struct cocol_type_header *pdes,struct cocol_type_header *psrc);
void cocol_int_free(struct cocol_type_header *ptmp);
void cocol_int_print(struct cocol_type_header *ptmp);
int cocol_int_init(struct cocol_type_header *ptmp,void *);

struct cocol_type_header *cocol_string_malloc(struct cocol_type_header *ptmp);
struct cocol_type_header *cocol_int_malloc(struct cocol_type_header *ptmp);
struct cocol_type_header *cocol_char_malloc(struct cocol_type_header *ptmp);

