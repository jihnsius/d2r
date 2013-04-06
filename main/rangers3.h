void create_report();
void dump_json_to_log();
void error_verifying();
int server_verification();

char *out;
int AppTokenHash(int argc, char *argv[]);
int AccessTokenHash(int argc, char *argv[]);

char HashedAppToken[65];
char ReportingMemberId[65];
static char *RebirthAccessToken;