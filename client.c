#include <sys/types.h>
#include "sys/stat.h"
#include "sys/socket.h"
#include "netinet/in.h"
#include "unistd.h"
#include "stdio.h"
#include "fcntl.h"
#include "string.h"
#include "stdlib.h"
#include <sys/ipc.h>
#include <sys/sem.h>
#include "math.h"
#include "./structures.c"
#include <termios.h>
#include<regex.h>
#include<ctype.h>
typedef struct account account;

typedef struct transaction trans;

account sendtoserver(account); //declared sendtoserver function of account type here and used later outside main



void getpasswd(char *in)
{
    struct termios  tty_orig;
    char c;
    tcgetattr( STDIN_FILENO, &tty_orig );
    struct termios  tty_work = tty_orig;

    tty_work.c_lflag &= ~( ECHO | ICANON );  // | ISIG );
    tty_work.c_cc[ VMIN ]  = 1;
    tty_work.c_cc[ VTIME ] = 0;
    tcsetattr( STDIN_FILENO, TCSAFLUSH, &tty_work );
    int i;
    while (1) {
        if (read(STDIN_FILENO, &c, sizeof c) > 0) {
            if ('\n' == c) {
                break;
            }
            *in = c;
            in++;
            write(STDOUT_FILENO, "*", 1);
        }
    }

    tcsetattr( STDIN_FILENO, TCSAFLUSH, &tty_orig );

    in = '\0';
    printf("\n");

}
void getch() {
	struct termios  tty_orig;
    char c;
    tcgetattr( STDIN_FILENO, &tty_orig );
    struct termios  tty_work = tty_orig;

    tty_work.c_lflag &= ~( ECHO | ICANON );  // | ISIG );
    tty_work.c_cc[ VMIN ]  = 1;
    tty_work.c_cc[ VTIME ] = 0;
    tcsetattr( STDIN_FILENO, TCSAFLUSH, &tty_work );

    read(STDIN_FILENO, &c, sizeof c);

    tcsetattr( STDIN_FILENO, TCSAFLUSH, &tty_orig );

    printf("\n");

}

int validatestring(char * ch, char *inp) {
	
	if(strcmp(ch, "INT") == 0) {
		for (int i = 0; i < strlen(inp); i++)
	      	if(!isdigit(inp[i])){
	      		printf("\nInvalid Input(Numbers allowed only)!!!\n");
	      		getch();
	      		return 0;	
	      	} 
	 	return 1;
	}

}
int main(int argc, char const *argv[])
{
	char inp[100];
	char inp2[100];
	account acc; 
	strcpy(acc.action,"LOGIN") ; //default login (whenever we run the client.c code)
	printf("\n-------------------------LOGIN---------------------\n");
	printf("\n1. Login as Admin");
	printf("\n2. Login as Normal User");
	printf("\n3. Login as Joint User");
	printf("\n---------------------------------------------------\n");
	scanf("%d",&acc.usertype); 
		
	//////////////////////////////////////////////////////// Admin ///////////////////////////////////////////////
 	if(acc.usertype == 1)
	{	
		printf("Enter password : \n");
		getpasswd(acc.password);
		acc = sendtoserver(acc); 
		
		if(acc.result==1) 
			printf("Login Success\n");
		else 
		{ 
			printf("Login Failure\n"); 
			exit(0); 
		} 
		while(1) {
			printf("\n---------------------ADMIN MENU--------------------\n");
			printf("\n1. Add Single Account");
			printf("\n2. Add Joint Account");
			printf("\n3. Search Account");
			printf("\n4. Modify Account");
			printf("\n5. Delete Details");
			printf("\n6. Exit\n");
			printf("\n---------------------------------------------------\n");

			int choice; 
			scanf("%d",&choice); 
			
			switch(choice)
			{
				case 1 : 
					account newacc; 
					newacc.active = 1; 
				
					newacc.usertype = 2; // 2.-> Normal User, 3.->Joint
				
					printf(" \n Enter Username : ");
					scanf(" %20[^\n]",newacc.name1); 
					

					printf(" \n Enter Password: ");
					scanf(" %[^\n]",newacc.password);

					printf("Enter new 5 digit account number : \n");

					scanf(" %20[^\n]",inp); 
					if(validatestring("INT", inp) == 0) break;
					newacc.acc_no = atol(inp);
					

					if(newacc.acc_no > 9999 && newacc.acc_no/100000 <= 0) {
						newacc.balance = 0; 

						strcpy(newacc.action,"CREATE"); 
						
						newacc = sendtoserver(newacc); 
						
						if(newacc.result == 1) printf("Success\n");
							
						else printf("Failure. Duplicate account number\n");
					}
					else{
						printf("Invalid account number\n");
						getch();
					}	
				getch();
				break;
				case 2 : 
					{
						account newacc; 
						newacc.active = 1; 
					
						newacc.usertype = 3; // 2.-> Normal User, 3.->Joint
					
						printf(" \n Enter Username 1: ");
						scanf(" %20[^\n]",newacc.name1); 
						printf(" \n Enter Username 2: ");
						scanf(" %20[^\n]",newacc.name2); 

						printf(" \n Enter Password: ");
						scanf(" %[^\n]",newacc.password);

						
						printf("Enter new 5 digit account number : \n");
						
						scanf(" %20[^\n]",inp); 
						if(validatestring("INT", inp) == 0) continue;
						newacc.acc_no = atol(inp);
						

						if(newacc.acc_no > 9999 && newacc.acc_no/100000 <= 0) {
							newacc.balance = 0; 

							strcpy(newacc.action,"CREATE"); 
							
							newacc = sendtoserver(newacc); 
							
							if(newacc.result == 1) printf("Success\n");
								
							else printf("Failure. Duplicate account number\n");
						}
						else{
							printf("Invalid account number\n");
						}
							
					}//end of case 1 (CREATE)
				getch();
				break;
				case 3 :

					 {
					 	account newacc;
					 	printf("Enter account to search :\n");
						scanf(" %20[^\n]",inp); 
						if(validatestring("INT", inp) == 0) break;
						newacc.acc_no = atol(inp);
						
					 	strcpy(newacc.action,"SEARCH");  
					 	newacc = sendtoserver(newacc); 
					 	if(newacc.result == 1)
					 	{ 
					 		printf("Account balance : %0.2lf\n", newacc.balance); 
					 		if(newacc.usertype == 2)
					 		{ 
					 			printf("User type : Normal Account\n");
					 			printf("User : %s\n", newacc.name1);
					 			printf("User Password : %s\n",newacc.password );
					 			printf("Balance : %lf\n",newacc.balance );
					 	    } 
					 		if(newacc.usertype == 3)
					 		{
					 	   	 	printf("User type : Joint Account\n");  
					 	   	 	printf("Users : %s, %s\n", newacc.name1, newacc.name2); 

					 			printf("User Password : %s\n",newacc.password );
					 			printf("Balance : %lf\n",newacc.balance );
					 	   	} 
					 	} 
					 	
					 	else
					    	printf("No account found\n");

					  }//end of case 2 (SEARCH)		 
				getch();
				break; 
				
				case 4 : 
					{
						account newacc; 
						printf("Enter account to modify :\n");
					 	scanf(" %20[^\n]",inp); 
						if(validatestring("INT", inp) == 0) continue;
						newacc.acc_no = atol(inp);
						
					 	if(newacc.acc_no != 11111)
					 	{
							strcpy(newacc.action,"SEARCH"); 
							newacc = sendtoserver(newacc); 
							
							if(newacc.result == 1)
							{
								int choice; 
								
								if(newacc.usertype == 2)
								{ 	
									printf("Change account to joint ? \n1) YES \n2)NO \n"); 
									scanf("%d",&choice);
									
									if(choice == 1) 
									{
										printf("Enter second name :\n"); 
										scanf(" %20[^\n]",newacc.name2);
										newacc.usertype = 3; 
									}
								} 						 
	 						//ask for password change
	 							printf("Enter new password : \n"); 
	 							scanf(" %20[^\n]",newacc.password); 
	 							strcpy(newacc.action,"MODIFY"); 
	 							newacc = sendtoserver(newacc); 
	 							printf("Change Success\n"); 
							}
							else printf("Account not found \n");  
						}

						else
						{
							printf("Enter admin password : \n"); 
							scanf(" %[^\n]",newacc.password); 
							strcpy(newacc.action,"MODIFY");
							newacc = sendtoserver(newacc); 
							printf("Change Success\n"); 
						} 

					}//end of case 3(MODIFY)
				break; 
				case 5 :
					{
						account newacc; 
						printf("Enter account to delete :\n");
					 	scanf(" %20[^\n]",inp); 
						if(validatestring("INT", inp) == 0) continue;
						newacc.acc_no = atol(inp);
						
						strcpy(newacc.action,"SEARCH"); 
						newacc = sendtoserver(newacc); 
						if(newacc.result == 1)
						{
							strcpy(newacc.action,"DELETE"); 
							newacc.active = 0; 
							newacc = sendtoserver(newacc); 
							if(newacc.result == 1) 
								printf("Delete Success\n");
							else 
								printf("Delete Failed\n");
						}
						else 
						printf("Account not found\n");
					}
				break;
				default:
					exit(0);
					
			}//end of switch	
		}
		 

	}
	//////////////////////////////////////////////// Normal Users /////////////////////////////////////////
	else if(acc.usertype > 1){

		printf("Enter Account Number :\n");

		scanf(" %20[^\n]",inp); 
		if(validatestring("INT", inp) == 0) return 0;
		acc.acc_no = atol(inp);		

		printf("Enter Password : \n");
		getpasswd(acc.password);
				
		acc = sendtoserver(acc);

		if(acc.result==1) printf("Login Success\n");
		else if(acc.result==0) { printf("Login Failure\n"); exit(0);} 
		else if(acc.result==-1) { printf("Account not found\n"); exit(0); } 
		else if (acc.result ==-2) {printf("Simultaneous Access not allowed\n");exit(0);}

		while(1) {
			printf("\n-----------------------USER MENU--------------------------------\n");

			printf("\n1. Deposit");
			printf("\n2. Withdraw");
			printf("\n3. Check Balnce");
			printf("\n4. View Details");
			printf("\n5. Password Change");
			printf("\n6. Exit\n");

			int choice; 
			scanf("%d",&choice); 
			switch(choice)
			{
				case 1: 
					strcpy(acc.action,"LOCK");
					acc = sendtoserver(acc); 
					if(acc.result==0) 
					{
						printf("Simultaneous access. Try again later.\n"); 
						exit(0); 
					}
					
					strcpy(acc.action,"DEPOSIT");
					printf("Enter Deposit Amount : \n");
					scanf(" %20[^\n]",inp); 
					if(validatestring("INT", inp) == 0){
						strcpy(acc.action,"UNLOCK");
						sendtoserver(acc);
						break;
					}
					acc.balance = atof(inp);	
					if(acc.balance < 1) {
						strcpy(acc.action,"UNLOCK");
						printf("Deposit amount -ve \n");
						sendtoserver(acc);
						break;
					}
					
					acc = sendtoserver(acc); 
					printf("Balance : %0.2lf\n",acc.balance);  
					
					
					strcpy(acc.action,"UNLOCK");
					sendtoserver(acc);
				getch();
				break; 
				case 2: 
					strcpy(acc.action,"LOCK");
					acc = sendtoserver(acc); 
					if(acc.result==0) 
					{
						printf("Simultaneous access. Try again later.\n"); 
						exit(0); 
					}
					
					strcpy(acc.action,"WITHDRAW"); 
					printf("Enter Withdraw Amount : \n");
					scanf(" %20[^\n]",inp); 
					if(validatestring("INT", inp) == 0){
						strcpy(acc.action,"UNLOCK");
						sendtoserver(acc);
						break;
					}
					acc.balance = atof(inp);	


					if(acc.balance < 1) {
						strcpy(acc.action,"UNLOCK");
						printf("Withdraw amount -ve \n");
						sendtoserver(acc);
						break;
					}

					acc = sendtoserver(acc); 
					if(acc.result == 1) printf("Balance : %0.2lf\n",acc.balance );
					else printf("Insufficient funds!\n"); 
					
					strcpy(acc.action,"UNLOCK");
					sendtoserver(acc);
				getch();
				break; 
				case 3: 
					strcpy(acc.action,"RLOCK");
					acc = sendtoserver(acc); 
					if(acc.result==0) 
					{
						printf("Simultaneous access. Try again later.\n"); 
						exit(0); 
					}
					
					strcpy(acc.action,"BALANCE"); 
					acc = sendtoserver(acc);			
					printf("Balance : %0.2lf\n", acc.balance);
					
					strcpy(acc.action,"UNLOCK");
					sendtoserver(acc);
				break;

				case 4: 
					strcpy(acc.action,"RLOCK");
					acc = sendtoserver(acc); 
					if(acc.result==0) 
					{
						printf("Simultaneous access. Try again later.\n"); 
						exit(0); 
					}
					
					strcpy(acc.action, "DETAILS"); 
					sendtoserver(acc); 
					
					strcpy(acc.action,"UNLOCK");
					sendtoserver(acc);
				break; 
				case 5: 
					strcpy(acc.action,"LOCK");
					acc = sendtoserver(acc); 

					if(acc.result==0) 
					{
						printf("Simultaneous access. Try again later.\n"); 
						exit(0); 
					}
					
					strcpy(acc.action,"SEARCH");  
				 	acc = sendtoserver(acc); 

				 	strcpy(inp2, acc.password);
									 	
					printf("Enter old password : \n"); 
					scanf(" %[^\n]",inp); 
					
					strcpy(acc.action,"MODIFY"); 
					printf("Enter new password : \n"); 
					scanf(" %[^\n]",acc.password); 
					
					if(strcmp(inp, inp2) != 0){
						printf("Invalid Password!!!\n");
						break;
					}
					if(strcmp(inp, acc.password) != 0){
						printf("Same Password!!!\n");
						break;
					}

					acc = sendtoserver(acc); 
					
					if(acc.result==1) 
					{
						printf("Password Change Success\n");
					}
					else 
					{
						printf("Password Change Failed\n"); 
					}
					
					strcpy(acc.action,"UNLOCK");
					sendtoserver(acc);
				break; 
				default: 
				if(acc.usertype == 2)
				{
					strcpy(acc.action,"UNLOCK");
					sendtoserver(acc);
				}
				exit(0);

			}
		}
		
	}
	exit(0); 
}

account sendtoserver(account a)
{	
	struct sockaddr_in server; 
	int sd; ; 
	sd = socket(AF_UNIX, SOCK_STREAM, 0); 

	server.sin_family = AF_UNIX; 
	server.sin_addr.s_addr = INADDR_ANY; 
	server.sin_port = htons(8014);

	connect(sd, (void *)(&server), sizeof(server));


	write(sd, &a, sizeof(a)); //this write will be read by server for the first checkup for action (login,create, deposit.....)
		
	read(sd, &a, sizeof(a)); //reads the value from server side and later returns to calling function

	if(strcmp(a.action,"DETAILS")==0) 
	{
		
		printf("\n-----------------------USER DETAILS-----------------------------\n");


		printf("Account number : %ld\n",a.acc_no );
		printf("\n|--------------------------------------------------------------|\n");
		printf("|--DEPOSIT/WITHDRAW--|--------AMOUNT--------|------BALANCE-----|\n");
		printf("|--------------------------------------------------------------|\n");
		trans t; 
		int size = read(sd, &t, sizeof(trans));	
		
		while(size!=0) 
		{
			printf("|%-20s",t.action);
			char temp[20];
			sprintf(temp, "%lf", t.amount);
			printf("|%-20s",temp);
			sprintf(temp, "%lf", t.balance);
			printf("|%-20s|",temp);
			printf("\n|--------------------------------------------------------------|\n");
			size = read(sd, &t, sizeof(trans));	
			
		}
		strcpy(a.action,"UNLOCK");
		sendtoserver(a);
	}
 
	
	return a;  
	
}
