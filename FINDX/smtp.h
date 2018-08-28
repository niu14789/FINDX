



typedef struct {
	char * host;
	char * account;
	char * password;
	char * from;
	char * to;
}email_config;


int send_email(email_config *config,char * subject,char * data);