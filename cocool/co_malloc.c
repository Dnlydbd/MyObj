


char *cocol_malloc(int itotolLen)
{
	return malloc(itotolLen);
}

void cocol_free(char *pdata)
{
    COCOL_SAFEFREE(pdata);
}

char *cocol_realloc(char *pdata,int ilen)
{
	return realloc(pdata,ilen);
}

