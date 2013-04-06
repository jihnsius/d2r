#include <stdio.h> 
#include <string.h>
#include "http_lib.h"
#include "rangers3.h"

extern char *http_server;

extern int http_port;

extern char *http_proxy_server;

extern int http_proxy_port;

http_retcode http_put
(
char *filename,
char *data,
int length,
int overwrite,
char *type
);

http_retcode http_get
(
char *filename,
char **pdata,
int *plength,
char *typebuf
);

http_retcode http_head
(
char *filename,
int *plength,
char *typebuf
);

http_retcode http_delete(char *filename);

http_retcode http_parse_url
(
char *url,
char **pfilename
);