#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
FILE *fp;
bool is_expop,is_termop,is_end,is_id,is_const=false;
char *current_token;

char*  copy (char *org){//custom string copying function
	int n=strlen(org)+1;
	char *cpy=malloc(n*sizeof(char));
	int i;
	for( i=0;i<n;i++){
		cpy[i]=org[i];
	}
	return cpy;
}

typedef struct expression { //AST's node
 	struct expression* left;
	struct expression* right;
	enum Type {expr,term,constant,identifier} type;
	char * token;
}expression;

void printtree (expression * node, int level ){//prints the tree, level should be 0 when called.
	if(node==NULL) return;
	printtree(node->right,level+1);
	for(int i=0;i<level;i++){putchar(' ');putchar(' ');putchar(' ');putchar(' ');}
	putchar('<');
	for(int i=0;i<strlen(node->token);i++)putchar(node->token[i]);
	putchar('>');
	if(node->type==constant)printf(" (constant)"); //print type of factor
	if(node->type==identifier)printf(" (identifier)");
	putchar('\n');

	printtree(node->left,level+1);	
}
void read_token(){//reads from file and creates the complete token. Also sets flags to signal whats the token 
	is_expop = is_termop = is_end = is_const = is_id = false;
	char temp;
	current_token=malloc(sizeof(char)*50); 
	do{
	temp=fgetc(fp);
	if(temp==EOF){printf("incomplete expression \n");return ;}
	}while(temp==' '||temp=='\n'||temp==13); // skip spaces
	
	current_token[0]=temp;
	int i=1;
	if(isalnum(temp)||temp=='-'){
		is_id=true;
		for(i=1;i<50;i++){
			temp=fgetc(fp);
			if(temp==EOF){printf("incomplete expression \n");return ;}
			if(isalnum(temp))
				current_token[i]=temp;
			else{
				ungetc(temp,fp); //if its not part of the term, put it back
				break;
			}
		}
	}
	current_token[i]='\0';
	
	//check and set flags depending on the type.
	if(strlen(current_token)==1){
		switch (current_token[0])
		{
			case '+':;;
			case '-':is_expop=true;is_id=false;break;
			case '*':;;
			case '/':is_termop=true;is_id=false;break;
			case '.':is_end=true;is_id=false;break;
		}
	}
	if(atoi(current_token)!=0){
		is_id=false;
		is_const=true;
	}
}

expression* factor(){//reads and creates a factor node
	expression* p = malloc(sizeof(expression));	
	//printf(" factor read \n");
	read_token();
	//printf(" factor read 2 \n");
	if(is_const){
	p->token=copy(current_token);
	p->type=constant;
	p->left=NULL;
	p->right=NULL;
	}else if(is_id){
			p->token=copy(current_token);
			p->type=identifier;
			p->left=NULL;
			p->right=NULL;
	}else{
		printf("error reading factor \n");
		printf("<%s> is not a constant nor an identifier \n",current_token);
		return NULL;
	}
	return p;
}

expression* make_term(expression * leftp){//is called with factor() as argument
 	//if next token is * or /, make another factor and create the term
	//if not, there is no term to be created and factor is returned
	if(leftp == NULL) return NULL;
	read_token();
	if(is_termop){
		expression* p = malloc(sizeof(expression));	
		p->token=copy(current_token);
		p->type= term;
		p->left=leftp;
		p->right=factor();
		read_token();//read next token, ready for make_exp
		if(p->right==NULL)return NULL; //if there was an error reading right factor, NULL is returned so an error can be detected	
		return p;
	}else if(is_expop||is_end)return leftp;
	else {
		printf("error reading term \n");
		printf("<%s> is not an operrator \n",current_token);
		return NULL;
	}
}

expression* make_exp(expression * leftp){//called witn make_term(factor()) as argument
	//if current_token is + ot - make another expression and make the node
	//vale sto right exp(term(factor()))
	//token is always read before in the make_term() function
	if(leftp != NULL){
		
		if(is_end){
			return leftp;
		}else if(is_expop){
			expression* p = malloc(sizeof(expression)) ;	
			p->token=copy(current_token);
			p->type= expr;
			p->left=leftp;
			p->right= make_exp(make_term(factor()));
			if(p->right==NULL)return NULL; //if there was an error reading right term, NULL is returned so an error can be detected	
			return p;			
		}else {
			printf("error reading expression \n");
			printf("<%s> in not a term operator \n",current_token);
			return NULL;
		}
	}else{
		
		return NULL;
	}
	
}

expression * parse (){
	expression * p = make_exp(make_term(factor()));
	
	if(p==NULL){
		printf("expression is not valid. \n");
		while(!is_end&&!feof(fp))read_token();//skip to next '.' and ignore rest of expression
	}
	else printf("expression successfully parsed! \n");
	return p;
}

void check_tokens(){
	while(!feof(fp)){
		read_token();
		printf("%s \n",current_token);
		printf("is expop %d \n",is_expop);
		printf("is termop %d \n",is_termop);
		printf("is end %d \n",is_end);
		printf("is id %d \n",is_id);
		printf("is const %d \n",is_const);
		
	}

}

int main (int argc, char** argv){	
	char temp;
	int suc=0;
	int total=0;
	expression * tree;
	if(argc==1){
		printf("no input file given \n");
		return 1;
	}
	for(int i=1;i<argc;i++){
		fp=fopen(argv[i],"r");
		if(fp==NULL){
		printf("no such file found \n");
		return 1;
		}
		else{
			//check_tokens();
			temp=getc(fp);
			printf("reading file... \n");
			while(!feof(fp)){	
				ungetc(temp,fp);
				total++;
				tree=parse();
				if(tree!=NULL)suc++; //count successful parses
				printtree(tree,0);
				do temp=getc(fp);
				while(temp==' '||temp=='\n'||temp==13); // skip spaces
				
				//printf("last char is: %d \n",temp);
			}
			printf("successfully parsed %d/%d expressions \n",suc,total);
			fclose(fp);
				
		}
	}
	return 0;
}
