#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <pwd.h>

#include "../include/main.h"
#include "../include/version.h"
#include "../include/debug.h"

void PrintHelp(){
	printf("usage:\t%s value from to [option]\n"
		"\tunitconv 100 m km\n"
		"\tvalue\t\tvalue to be converted\n"
		"\tfrom\t\tunit to be converted from\n"
		"\tto\t\tunit to be converted to\n"
		"\t-h, --help\tdisplay help\n"
		"\t-f, --file\tpath to config file where conversions are located\n"
		"\t\t\tif not specified, then $XDG_CONFIG_HOME/.unitconv/config or $HOME/.unitconv/config is used\n"
		,NAME);
}

float GetMultiplier(const char *confFile, const char *from, const char *to){
	FILE *file=NULL;
	file=fopen(confFile,"r");
	if(!(file)){
		printf("error reading config file from %s\n",confFile);
		return -99;
	}

	size_t len=0;
	unsigned int reader=0;
	unsigned int i=0;
	char *line=NULL;
	char *entry=NULL;
	char c='\0';
	bool canContinue=true;
	bool foundFrom=false;
	bool foundTo=false;
	bool divide=false;
	float multiplier=0;
	while((reader=getline(&line,&len,file))!=EOF && canContinue){
		entry=strtok(line,";");
		while(entry){
			if(DEBUG){printf("%s\n",entry);}
			switch(i){
				case 0:
					sscanf(entry,"%f%c",&multiplier,&c);
					break;
				case 1:
					if(strcmp(entry,from)==0){
						foundFrom=true;
					}else if(strcmp(entry,to)==0){
						divide=true;
					}
					break;
				case 2:
					if(strcmp(entry,to)==0){
						foundTo=true;
					}else if(strcmp(entry,from)!=0){
						divide=false;
					}
					break;
			}
			if((foundFrom && foundTo) || divide){
				if(DEBUG){printf("from and to found, multiplier is %f, divide is %b\n",multiplier,divide);}
				canContinue=false;
				break;
			}
			entry=strtok(NULL,";");
			i++;
		}
		if(canContinue){
			i=0;
			multiplier=0;
			foundFrom=false;
			foundTo=false;
			divide=false;
		}
	}
	fclose(file);
	free(line);

	if(divide){
		multiplier=1/multiplier;
	}

	if(DEBUG){printf("multiplier is %f\n",multiplier);}

	return multiplier;
}

int main(int argc,char **argv){

	if(argc<2){
		PrintHelp();
		return EXIT_FAILURE;
	}

	unsigned int i;
	float value=0;
	char *from=NULL;
	char *to=NULL;
	char c;
	for(i=1;i<argc;i++){
		if(DEBUG){printf("arg %d is %s\n",i,argv[i]);}
		if(strcmp(argv[i],"-h")==0 || strcmp(argv[i],"--help")==0){
			PrintHelp();
			return EXIT_SUCCESS;
		}else{
			switch(i){
				case 1:
					sscanf(argv[i],"%f%c",&value,&c);
					break;
				case 2:
					from=argv[i];
					break;
				case 3:
					to=argv[i];
					break;
			}
		}
	}
	if(DEBUG){printf("value is %f, from is %s, to is %s\n",value,from,to);}
	
	if(!(value) || value==0 || value==-0 || !(from) || !(to)){
		PrintHelp();
		return EXIT_FAILURE;
	}

	char *confDir;
	if(!(confDir=getenv("XDG_CONFIG_HOME"))){
		if(!(confDir=getenv("HOME"))){
			confDir=getpwuid(getuid())->pw_dir;
		}
	}
	if(DEBUG){printf("config dir is %s\n",confDir);}

	char *confFile;
	if(argc>4 && (strcmp(argv[4],"-f")==0 || strcmp(argv[4],"--file")==0)){
		confFile=argv[5];
	}else{
		confFile=strcat(confDir,"/.unitconv/config");
	}
	if(DEBUG){printf("config file is %s\n",confFile);}

	float multiplier=GetMultiplier(confFile,from,to);
	if(multiplier!=0 && multiplier!=-99){
		printf("%f %s is %f %s\n",value,from,value*multiplier,to);
	}else{
		printf("Couldn't find conversion from %s to %s\n",from,to);
	}

	return EXIT_SUCCESS;
}
