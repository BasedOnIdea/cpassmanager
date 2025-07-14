#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

#define warn(...) printf("[WARNING] " __VA_ARGS__);
#define info(...) printf("[INFO] " __VA_ARGS__);
#define STRING_SIZE 50
#define COMMANDS_SIZE 1250
#define KEY_SIZE 401
#define DEBUG_MODE 0

void print_banner(void){
	printf("\n\
██████╗ ██╗    ██╗██████╗     ███╗   ███╗ █████╗ ███╗   ██╗ █████╗  ██████╗ ███████╗██████╗\n\
██╔══██╗██║    ██║██╔══██╗    ████╗ ████║██╔══██╗████╗  ██║██╔══██╗██╔════╝ ██╔════╝██╔══██╗\n\
██████╔╝██║ █╗ ██║██║  ██║    ██╔████╔██║███████║██╔██╗ ██║███████║██║  ███╗█████╗  ██████╔╝\n\
██╔═══╝ ██║███╗██║██║  ██║    ██║╚██╔╝██║██╔══██║██║╚██╗██║██╔══██║██║   ██║██╔══╝  ██╔══██╗\n\
██║     ╚███╔███╔╝██████╔╝    ██║ ╚═╝ ██║██║  ██║██║ ╚████║██║  ██║╚██████╔╝███████╗██║  ██║\n\
╚═╝      ╚══╝╚══╝ ╚═════╝     ╚═╝     ╚═╝╚═╝  ╚═╝╚═╝  ╚═══╝╚═╝  ╚═╝ ╚═════╝ ╚══════╝╚═╝  ╚═╝\n\
\n(Wrote by Klaver)\n\
contacts -> zorrintopper@gmail.com\n");
}

void print_menu(void){
	printf("\n|pwm| ");
}

void print_commands_list(void){
	printf("\nCommands list:\n\
\tlist - to print list of passwords\n\
\tcreate - to create a password\n\
\thelp - to print this menu\n\
\tquit - to exit the program\n");
}

char *encrypt(const char *str, const char *key_str, int str_size, int key_str_size){
	char *result = (char *)malloc((sizeof(char) * (str_size + 1))); 
	
	size_t i,c;
	size_t d = 0;
	for(i = 0,c = 0; i < str_size; i++, c++){
		if((str[i] ^ key_str[(c - d) % key_str_size]) && ((str[i] ^ key_str[(c - d) % key_str_size]) != ' ') && ((str[i] ^ key_str[(c - d) % key_str_size]) != '\t') && ((str[i] ^ key_str[(c - d) % key_str_size]) != '\n') && (str[i] != '\n') && (str[i] != ' ')){
			result[i] = str[i] ^ key_str[(c - d) % key_str_size];
			printf("%c Key: %c\n", str[i], key_str[(c - d) % key_str_size]);
		}
		else {
			d++;
			result[i] = str[i];
		}	
		if(str[i] == '\n'){
			c = -1;
			d = 0;
		}
	}
	
	result[i] = '\0';

	return result;
}

char *decrypt(const char *str, const char *key_str, int str_size, int key_str_size){
	char *result = (char *)malloc(sizeof(char) * (str_size + 1)); 
	
	size_t i,c;
	size_t d = 0;
	for(i = 0, c = 0; i < str_size; i++, c++){
		if((str[i] ^ key_str[(c - d) % key_str_size]) && ((str[i] ^ key_str[(c - d) % key_str_size]) != ' ') && ((str[i] ^ key_str[(c - d) % key_str_size]) != '\t') && ((str[i] ^ key_str[(c - d) % key_str_size]) != '\n') && (str[i] != '\n') && (str[i] != ' ')){
			result[i] = str[i] ^ key_str[(c - d) % key_str_size];
			//printf("%c Key: %c\n", str[i], key_str[(i - d) % key_str_size]);
		}
		else {
			d++;
			result[i] = str[i];
		}
		if(str[i] == '\n'){
			c = -1;
			d = 0;
		}
	}
	
	result[i] = '\0';

	return result;
}

unsigned long get_file_size(FILE *filestream){
	unsigned long fsize = 0;
	fseek(filestream, 0, SEEK_END);
	fsize = ftell(filestream);

	return fsize;	
}

int main(int argc, const char const * argv[]){
	print_banner();
	srand(time(NULL));
	FILE *saves = NULL; 
	char current_string[STRING_SIZE];
	char commands[COMMANDS_SIZE];
	char key[KEY_SIZE];
	long fp_pos = 0;
	
	if(argc < 3){
		warn("USAGE: password_manager passwords.txt key\n");
		return EXIT_FAILURE;
	}

	if(strlen(argv[2]) > KEY_SIZE){
		warn("Key max size -> %d\n", KEY_SIZE - 1);
		return EXIT_FAILURE;
	}

	memset(key, 0, KEY_SIZE * sizeof(char));
	strcpy(key, argv[2]);
	saves = fopen(argv[1], "rb+");
	info("Saves file: %s, Key value: %s\n", argv[1], argv[2]);	

	if(!saves){
		warn("Couldn't open saves file!\n");
		return EXIT_FAILURE;
	}

	info("Opened saves.txt\n");
	
	bool is_saves_empty = false;
	if(!fgets(current_string, STRING_SIZE, saves)){
		info("Saves file is empty!\n");
		is_saves_empty = true;
	}

	fp_pos = get_file_size(saves);

	bool is_running = true;
	while(is_running){
		print_menu();
		
		char command[20];
		//www.site.com username password
		char args[3][401];
		memset(args, 0, sizeof(args));

		memset(commands, 0, COMMANDS_SIZE);

		fgets(commands, COMMANDS_SIZE, stdin);
		
		sscanf(commands, "%s %s %s %s", command, args[0], args[1], args[2]);
		
		if(!strcmp(command, "create")){
			if(!strcmp(args[0], "") || !strcmp(args[1], "") || !strcmp(args[2], "")){
				warn("Check usage of create command!\n");
				continue;
			}

			fseek(saves, fp_pos, SEEK_SET);

			int temp_length = strlen(args[0]) + strlen(args[1]) + strlen(args[2]) + 4;
			char *temp = (char *)malloc(temp_length *
						sizeof(char)); // +  ' ' + ' ' + ' '\n' + '\0'
			
			memset(temp, 0, temp_length * sizeof(char));

			sprintf(temp, "%s %s %s\n", args[0], args[1], args[2]);

			char *temp1 = encrypt(temp, key, strlen(temp), strlen(key));

			fwrite(temp1, (temp_length - 1) * sizeof(char), 1, saves);
			
			fp_pos = ftell(saves);

			free(temp);
			free(temp1);

			is_saves_empty = false;
		}
		else if(!strcmp(command, "list")){
			if(is_saves_empty){
				info("Saves file is empty!\n");
			}
			else {
				char site[400] = {0};
				char pass[400] = {0};
				char username[400] = {0};
				char *data = NULL;
				char *dec_data = NULL;

				rewind(saves);
			
				data = (char *)malloc(fp_pos + 1);
				memset(data, 0, fp_pos + 1);

				fread(data, fp_pos, 1, saves);

				dec_data = decrypt(data, key, strlen(data), strlen(key));

				printf("Decrypted data: \n\t%s\n", dec_data);
			
				char *temp = dec_data;	
				while(sscanf(temp, "%s %s %s", site, username, pass) == 3){
					printf("Site -> %s\tUsername -> %s\tPass -> %s\n", site, username, pass);
					temp += strlen(site) + strlen(pass) + strlen(username) + 3;
				}

				free(data);
				free(dec_data);
			}
		}
		else if(!strcmp(command, "remove")){
			if(!remove(args[0])){
				info("File %s removed successfully!\n", args[0]);
			}
			else warn("Couldn't remove %s\n", args[0]);
		}
		else if(!strcmp(command, "help")){
			print_commands_list();
		}
		else if(!strcmp(command, "quit")){
			is_running = false;
		}
		else if(!strcmp(command, "")){
			warn("Please enter the command!\n");
			continue;
		}
		else {
			warn("%s is undefined", command);
			print_commands_list();
		}
	}
	
	fclose(saves);
	return EXIT_SUCCESS;
}




