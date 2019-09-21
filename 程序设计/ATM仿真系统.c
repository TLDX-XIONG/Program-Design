#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<conio.h>        //getch()����ͷ�ļ�
#include<memory.h>		 //memset����ͷ�ļ�
#include<time.h>         
#include<math.h>
#include<windows.h>      //ʱ����ʱͷ�ļ�
struct Account           
{
	char ID[20];         //�˻�ID
	char Name[20];       //�û�����
	char Password[7];    //����
	double money;        //�˻����
	int judge;           //�ж��˻��Ƿ�����
};
typedef struct Account Account;
struct Statement         //������ˮ�ṹ��       
{
	char accountID[20];  //�˻�ID    
	char time[30];       //����ʱ��
	char type[10];       //��������
	double money;        //���׽��
	char toAccount[20];  //ת�˶Է��˻�������Ǵ�ȡ���ΪNULL��
};
typedef struct Statement Statement;

//��������
void runMain(Account str[],Statement sta[],int choice);
int accountLogin(Account str[]);
int find(Account str[],Account s1);
void menuAccount(Account *str,Statement *sta,int account_Number);
void MoneyChange(Account *pa,Statement *ps,int choice,int account_Number);
void menuSearch(Account *str,Statement *sta,int account_Number);
void searchBalance(Account *acc,int account_Number);
void searchManage(Account *acc,Statement *sta,int account_Number);
void transferMoney(Account *pa,Statement *ps,int account_Number);
void ChangePassword(Account *str,Statement *sta,int account_Number);
char* getDate();

void readFile(Account str[]);
void readFile_Transactions(Statement sta[]);
void writeFile(Account str[]);
void add_Account_Transactions(Statement account_Message);

void menu()
{
	printf("\n\n\n\n\n\n\n\n\n");
	printf("\t\t\t\t\t##########################################\n");
	printf("\t\t\t\t\t#                                        #\n");
	printf("\t\t\t\t\t#                                        #\n");
	printf("\t\t\t\t\t#  >>>>>>>>     1.Sign in    <<<<<<<<<<  #\n");
	printf("\t\t\t\t\t#  >>>>>>>>     0.Quit       <<<<<<<<<<  #\n");
	printf("\t\t\t\t\t#                                        #\n");
	printf("\t\t\t\t\t#                                        #\n");
	printf("\t\t\t\t\t##########################################\n");
}

//����������
void runMain(Account str[],Statement sta[],int choice)  //strΪ�˻���Ϣ����ָ�룬staΪ������ˮ����ָ�룬choiceΪ�û�ҵ��ѡ��
{
	int account_Number;                                 //�洢��¼�û��˻��±�
	switch(choice)
	{
	case 1:account_Number=accountLogin(str);            //�˻���¼������ֵΪ�û��˻��±�
		   menuAccount(str,sta,account_Number);         //�����˵�
		   break;
	case 0:break;
	}
}

//�û���¼����
int accountLogin(Account str[])    //�β�Ϊ�˻�����
{
	Account s1;                    //�洢�û���¼��Ϣ
	int password_Length=0;         //�ж����볤��
	int password_Number=3;         //������������δ��󽫱�������
	int account_Number;            //�˻�ID�±�
	printf("\n\n\n\n\n\n\n\n\n");
	printf("\t\t\t\t\tplease input Account_ID:");
	scanf("%s",&s1.ID);
	while(find(str,s1)<0)          //�ж�ID�Ƿ����
	{
		
		printf("\t\t\t\t\tID input error\n");
		printf("\t\t\t\t\tplease input Account_ID again:");
		scanf("%s",&s1.ID);
	}
	account_Number=find(str,s1);   //��ѯ�û�ID�����±꣬���������account_Number
	if(str[account_Number].judge==1)  //�ж��û��Ƿ�����
		{
			printf("\t\t\t\t\tThe account is locked,please contact with Administrator\n");
			exit(1);
		}
	printf("\t\t\t\t\tplease input passwords:");
	while(1)
	{
		while(password_Length<6)
		{
			s1.Password[password_Length]=getch();
			s1.Password[password_Length]+=51;                    //�����㷨��ʹ�ļ����뱻����
			if((s1.Password[password_Length])=='\b'+51)
           {
	             printf("\b \b");
				 password_Length--;
				 continue;
			}
		    printf("*");
		    password_Length++;
		}
		getch();
		s1.Password[6]='\0';       //�ַ���������־λ��ʹs1.Password��Ϊ�ַ�������ֹ��������
		printf("\n");
		if(strcmp(str[account_Number].Password,s1.Password))
		{
			printf("\t\t\t\t\tpassword input error,the left times you have: %d\n",--password_Number);
			if(password_Number==0) //��������ﵽ���Σ��û�������
			{
				printf("\t\t\t\t\tpassword input is equal to three times,the account is locked,please contact with Administrator\n");
				str[account_Number].judge=1;
				writeFile(str);
				exit(1);
			}
			printf("\t\t\t\t\tplease input passwords:");
			password_Length=0;
		}
		else
			break;
	}
	printf("\t\t\t\t\tLogin success! The page will jump in two seconds.\n");
	Sleep(2000);     //��ʱ2��
	system("cls");   //����
	return account_Number;         //�����˻����������±�
}

//��Ϣ��ѯģ�麯��
void menuSearch(Account *str,Statement *sta,int account_Number)  //strΪ�˻�����ָ�룬staΪ������ˮָ�룬account_NumberΪ�˻��������±�
{
	int choice_number;     //����ҵ��ѡ��
	printf("\n\n\n\n\n\n\n\n\n");
	printf("\t\t\t\t\t##########################################\n");
	printf("\t\t\t\t\t#                                        #\n");
	printf("\t\t\t\t\t#              1.Search Blance           #\n");
	printf("\t\t\t\t\t#              2.Search Transaction      #\n");
	printf("\t\t\t\t\t#              3.Return                  #\n");
	printf("\t\t\t\t\t#                                        #\n");
	printf("\t\t\t\t\t##########################################\n");
	printf("\t\t\t\t\tplease input your choice:");
	scanf("%d",&choice_number);
	system("cls");                 //����
	switch(choice_number)
	{
	case 1:
		searchBalance(str,account_Number);      //��ѯ���
		menuSearch(str,sta,account_Number);     //���ز˵�
		break;
	case 2:
		searchManage(str,sta,account_Number);   //��ѯ���׼�¼
		menuSearch(str,sta,account_Number);     //���ز˵�
		break;
	case 3:
		menuAccount(str,sta,account_Number);    //���ض����˵�
		break;
	}
}

//��ѯ����
void searchBalance(Account *acc,int account_Number)   //accΪ�˻�����ָ�룬account_NumberΪ�˻������±�
{
	printf("\n\n\n\n\n\n\n\n\n");
	printf("\t\t\t\t\tNow your account balance is��%.2f\n",acc[account_Number].money);
	printf("\t\t\t\t\tClick enter to return.");                 //���»س���������һ��
	getch();
	system("cls");
}

//�����˵����û�����ҵ��˵�
void menuAccount(Account *str,Statement *sta,int account_Number)  //strΪ�˻�ָ�룬staΪ������ˮָ�룬account_NumberΪ�˻������±�
{
	int choice_number;    //����ҵ��ѡ��
	printf("\n\n\n\n\n\n\n\n\n");
	printf("\t\t\t\t\t##########################################\n");
	printf("\t\t\t\t\t#                                        #\n");
	printf("\t\t\t\t\t#   1.Deposit          4.Transfer        #\n");
	printf("\t\t\t\t\t#   2.withdrawal       5.PasswordChange  #\n");
	printf("\t\t\t\t\t#   3.UserInformation  6.Return          #\n");
	printf("\t\t\t\t\t#                                        #\n");
	printf("\t\t\t\t\t##########################################\n");
	printf("\t\t\t\t\tplease input your choice:");
	scanf("%d",&choice_number);
	system("cls");        //����
	switch(choice_number)
	{
	case 1:
	    MoneyChange(str,sta,choice_number,account_Number);   //���ô�ȡ���
		break;
	case 2:
		MoneyChange(str,sta,choice_number,account_Number);   //���ô�ȡ���
		break;
	case 3:
		menuSearch(str,sta,account_Number);                  //�û���Ϣ��ѯ����
		break;
	case 4:
		transferMoney(str,sta,account_Number);               //ת�˺���
		break;
	case 5:
		ChangePassword(str,sta,account_Number);              //�޸����뺯��
		break;
    case 6:
		break;
	}
}


//��ѯ�˻���������±꺯���Լ��ж��˻��Ƿ����
int find(Account str[],Account s1)
{
	int i=0;
	for(i=0;i<20;i++)
	{
		if(strcmp(str[i].ID,s1.ID)==0)
			return i;  //�����˻��±�
	}
	return -1;//����-1��ʾ�˻�������
}

//��ȡ�˻��ļ���Ϣ����
void readFile(Account str[])
{
	int i=0;
	FILE *fp;
	fp=fopen("account messages.txt","r");
	if(fp==0)
	{
		printf("source file error");
		exit(1);
	}
	fscanf(fp,"%s%s%s%lf%d",&str[i].ID,&str[i].Name,&str[i].Password,&str[i].money,&str[i].judge);   //�����˻����ݵ�str��
	while(!feof(fp))  //�ж��Ƿ��ļ�ĩβ
	{
		i++;
		fscanf(fp,"%s%s%s%lf%d",&str[i].ID,&str[i].Name,&str[i].Password,&str[i].money,&str[i].judge);
	}
	fclose(fp);
}

//��ȡ�˻���ˮ��Ϣ����
void readFile_Transactions(Statement sta[])
{
	int i=0;
	FILE *fp;
	fp=fopen("account Transaction.txt","r");
	if(fp==0)
	{
		printf("source file error");
		exit(1);
	}
	fscanf(fp,"%s%s%s%lf%s",&sta[i].accountID,&sta[i].time,&sta[i].type,&sta[i].money,&sta[i].toAccount);  //���뽻����ˮ���ݵ�sta��
	while(!feof(fp))   //�ж��Ƿ��ļ�ĩβ
	{
		i++;
		fscanf(fp,"%s%s%s%lf%s",&sta[i].accountID,&sta[i].time,&sta[i].type,&sta[i].money,&sta[i].toAccount);
	}
	fclose(fp);
}

//��ȡ�������˻������Ϣ
void writeFile(Account str[])
{
	int i;
	FILE *fp;
	fp=fopen("account messages.txt","w");
	if(fp==0)
	{
		printf("creat new file error\n");
		exit(1);
	}
	for(i=0;i<20;i++)
	{
		fprintf(fp,"%-12s%-8s%-8s%-8.2f%-4d\n",str[i].ID,str[i].Name,str[i].Password,str[i].money,str[i].judge);  //�����˻����ݵ��ļ���
	}
	fclose(fp);
}

//��ӽ��׼�¼�������β�Ϊ�û���Ϣ�ṹ��
void add_Account_Transactions(Statement account_Message)
{
	FILE *fp;
	fp=fopen("account Transaction.txt","a+");
	if(fp==0)
	{
		printf("file open error");
		exit(1);
	}
	fprintf(fp,"%-12s%-16s%-12s%-8.2lf%-8s\n",account_Message.accountID,account_Message.time,account_Message.type,account_Message.money,account_Message.toAccount);   //��ӽ�����ˮ���ļ���
	fclose(fp);
}

//��ȡ��ǰʱ�亯��
char* getDate()
{
	char*current_time;
	time_t timep;
	struct tm *p;
	time (&timep);
	p=gmtime(&timep);
	current_time=(char*)malloc(sizeof(char)*20);
	sprintf(current_time,"%d/%d/%d/%d:%d",1900+p->tm_year,1+p->tm_mon,p->tm_mday,8+p->tm_hour,p->tm_min);  //���������ַ�����ʽ�����
	return current_time;
}

//�޸����뺯��
void ChangePassword(Account *str,Statement *sta,int account_Number)   //strΪ�˻�ָ�룬staΪ������ˮָ�룬account_NumberΪ�˻������±�
{
	int j=0;             //i���ƽṹ�������±꣬j�������볤��
	Account a1,a2;
	printf("\n\n\n\n\n\n\n\n\n");
	printf("\t\t\t\t\tplease input your new password:");
		while(j<=5)
		{
			a2.Password[j]=getch();
			a2.Password[j]+=51;       //������ܴ���
			printf("*");
			j++;
		}getch();
		a2.Password[6]='\0';
		j=0;
		printf("\n\t\t\t\t\tplease input your new password again:");
		while(j<=5)
		{
			a1.Password[j]=getch();
			a1.Password[j]+=51;       //������ܴ���
			printf("*");
			j++;
		}getch();
		a1.Password[6]='\0';                  //�ַ���������־λ��ʹs1.Password��Ϊ�ַ�������ֹ��������
		if(strcmp(a2.Password,a1.Password)!=0)
		    {
				printf("\n\t\t\t\t\tOperation failed(the two passwords entered are different!)\n");
		    }
		else
			{
				printf("\n\t\t\t\t\tOperation complete!\n");
				strcpy(str[account_Number].Password,a1.Password);
				writeFile(str);
		}
		printf("\t\t\t\t\tClick enter to return.");
	    getch();
	    system("cls");                         //�����س���������һ��
		menuAccount(str,sta,account_Number);                    //�ص������˵�	
}

//ת�˺���
void transferMoney(Account *pa,Statement *ps,int account_Number)    //paΪ�˻�ָ�룬psΪ������ˮָ�룬account_NumberΪ�˻������±�

{
	double amount;                                 //��������
	int k;                                       //kΪtoaccount�û��±�
	Statement a1,a2;
	Account toaccount_Number;
	printf("\n\n\n\n\n\n\n\n\n");
	printf("\t\t\t\t\tplease input amount:");
		scanf("%lf",&amount);
	while(amount>pa[account_Number].money)                      //�ж��˻�����Ƿ�����û������ֵ
		{
			printf("Insufficient Funds! please Input again:");
			scanf("%lf",&amount);
		}
	printf("\t\t\t\t\tplease input toAccount:");
	scanf("%s",toaccount_Number.ID);
	system("cls");                          //����
	k=find(pa,toaccount_Number);            //�ҵ�ת���˻��±�
	while(account_Number==k||k==-1)                      //�ж��û������ID�Ƿ���Ч
	{
		printf("\t\t\t\t\terror input!\n");
		printf("\t\t\t\t\tplease input again:");
		scanf("%s",toaccount_Number.ID);
		system("cls");
		k=find(pa,toaccount_Number);
	}
	pa[account_Number].money=pa[account_Number].money-amount;         //�����˻����
	pa[k].money=pa[k].money+amount;         //���±�ת���û����˻����

	strcpy(a1.accountID,pa[account_Number].ID);          //����¼�û����׼�¼�洢��a1��
	strcpy(a1.time,getDate());
	strcpy(a1.type,"Transfer");
	a1.money=amount;                        
	strcpy(a1.toAccount,toaccount_Number.ID);

	strcpy(a2.accountID,pa[k].ID);          //����ת���û����׼�¼�洢��a2��
	strcpy(a2.time,getDate());
	strcpy(a2.type,"Input");
	a2.money=amount;                        
	strcpy(a2.toAccount,pa[account_Number].ID);

	writeFile(pa);                          //�����˻������Ϣ
	add_Account_Transactions(a1);           //��a1�еļ�¼�����ļ���
	add_Account_Transactions(a2);           //��a2�еļ�¼�����ļ���
	printf("\n\n\n\n\n\n\n\n\n");
	printf("\t\t\t\t\tTransfer success!\n");
	printf("\t\t\t\t\tNow your account balance is:%.2f\n",pa[account_Number].money);
	printf("\t\t\t\t\tThe page will jump in two seconds.\n");
	Sleep(2000);                            //��ʱ����
	system("cls");                          //����
	menuAccount(pa,ps,account_Number);       //�ص�ҵ��ѡ��˵�

}


//��ȡ���
void MoneyChange(Account *pa,Statement *ps,int choice,int account_Number)   //psΪ�˻�ָ�룬psΪ������ˮָ�룬choiceΪ��ȡ��ҵ��ѡ��account_NumberΪ�˻������±�             
{
	double amount;             //��ȡ����
	int i;                     //��������˵�ҵ��ѡ��
	Statement account_trans;   //�洢���ν�����ˮ��Ϣ
	char judge;                //�������Իس���
	printf("\n\n\n\n\n\n\n\n\n");
	printf("\t\t\t\t\tplease input the amount:");
	scanf("%lf",&amount);
	getchar();
	if (choice==2)             //�ж�����������
	{
		while(amount>pa[account_Number].money)                 //�ж��˻�����Ƿ�����û������ֵ
		{
			printf("\t\t\t\t\tBalance isn't enough!please input the amount again:");
			scanf("%lf",&amount);
		}
		    amount=-amount;
	}
	pa[account_Number].money=pa[account_Number].money+amount;  //���������˻����
	strcpy(account_trans.toAccount,"NULL");                       //��ȡ����toaccount
	strcpy(account_trans.accountID,pa[account_Number].ID);     //��¼�˻�ID
	account_trans.money=abs(amount);                           //��¼���׽��
	strcpy(account_trans.time,getDate());                      //��¼����ʱ��
	switch (choice)
   {
	case 1:
		strcpy(account_trans.type,"Deposit");
                      break;   
    case 2:
		strcpy(account_trans.type,"Withdrawal");
					  break;                                   //��¼�ʽ�䶯����
	}
	writeFile(pa);                                             //�����ļ��˻����
	add_Account_Transactions(account_trans);                   //��ӽ�����ˮ
	Sleep(1000);             //��ʱ1��
	printf("\n");
	printf("\t\t\t\t\tDesposit success!\n");
	printf("\t\t\t\t\tNow your account balance is:%.2f\n",pa[account_Number].money);
	printf("\t\t\t\t\tWhether to print the return form or not?(Y/N):");
	judge=getchar();
	while(judge!='Y'&&judge!='N'&&judge!='y'&&judge!='n')
	{
		printf("\t\t\t\t\tInput error! Whether to print the return form or not?(Y/N):");
		getch();           //���Իس��ַ�
		judge=getchar();
	}
	if(judge=='Y'||judge=='y')
	{
		printf("\t\t\t\t\tComplete return printing!\n");
	}
	printf("\t\t\t\t\tThe page will jump in two seconds.");
	Sleep(2000);       //��ʱ2��
	system("cls");     //����
	printf("\n\n\n\n\n\n\n\n\n");
	printf("\t\t\t\t\t##########################################\n");
	printf("\t\t\t\t\t#                                        #\n");
	printf("\t\t\t\t\t# 1.Withdraw card 2.Return the main menu #\n");
	printf("\t\t\t\t\t#                                        #\n");
	printf("\t\t\t\t\t##########################################\n");
	printf("\t\t\t\t\tplease input your choice:");
	scanf("%d",&i);
	system("cls");           //����
	if(i==2)
	menuAccount(pa,ps,account_Number);  //�ص������˵�
}

//��ˮ��Ϣ��ѯ����
void searchManage(Account *acc,Statement *sta,int account_Number)     //accΪ�˻�ָ�룬staΪ������ˮָ�룬account_NumberΪ�˻������±�
{
	int i=0;                        //���ڱ�����ˮ��Ϣ����
	printf("\n\n\n\n\n\n\n\n\n");
    printf("\t\t\t\t\tAccount ID       Time         Type     Amount/$     toAccount ID\n");
	readFile_Transactions(sta);     //��ȡ�ļ���ˮ��Ϣ������
	while(sta[i].money>10e-3&&i<=1000)
	{
		if(strcmp(sta[i].accountID,acc[account_Number].ID)==0)
			printf("\t\t\t\t\t%-12s%-16s%-12s%-12.2lf%-10s\n",sta[i].accountID,sta[i].time,sta[i].type,sta[i].money,sta[i].toAccount);
		i++;
	}
	printf("\t\t\t\t\tClick enter to return.");
	getch();
	system("cls");                         //�����س���������һ��
}

int main()
{
	int choice;                              //�û�ҵ��ѡ��
	struct Account  str[25];                 //�洢�ļ��û��˻���Ϣ
	struct Statement sta[1000];              //�洢������ˮ��Ϣ
	readFile(str);                           //�����ļ�
	memset(sta,0,sizeof(Statement)*1000);    //��ʼ��sta����
	do
	{
		menu();                              //һ���˵�
		printf("\t\t\t\t\tPlease input your choice:");
		scanf("%d",&choice);
		system("cls");           //����
		if(choice>=0&&choice<=3)
			runMain(str,sta,choice);
		else
			printf("error input,please input your choice again!\n");
	}while(choice);
	printf("\n\n\n\n\n\n\n\n\n\n\t\t\t>>>>>>>>>>  Exit success,please keep your bank card!  <<<<<<<<<<<<\n");
	return 0;
}
