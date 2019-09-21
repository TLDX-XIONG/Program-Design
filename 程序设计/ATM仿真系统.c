#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<conio.h>        //getch()函数头文件
#include<memory.h>		 //memset函数头文件
#include<time.h>         
#include<math.h>
#include<windows.h>      //时间延时头文件
struct Account           
{
	char ID[20];         //账户ID
	char Name[20];       //用户姓名
	char Password[7];    //密码
	double money;        //账户余额
	int judge;           //判断账户是否锁定
};
typedef struct Account Account;
struct Statement         //交易流水结构体       
{
	char accountID[20];  //账户ID    
	char time[30];       //交易时间
	char type[10];       //交易类型
	double money;        //交易金额
	char toAccount[20];  //转账对方账户（如果是存取款，则为NULL）
};
typedef struct Statement Statement;

//函数声明
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

//运行主函数
void runMain(Account str[],Statement sta[],int choice)  //str为账户信息数组指针，sta为交易流水数组指针，choice为用户业务选择
{
	int account_Number;                                 //存储登录用户账户下标
	switch(choice)
	{
	case 1:account_Number=accountLogin(str);            //账户登录，返回值为用户账户下标
		   menuAccount(str,sta,account_Number);         //二级菜单
		   break;
	case 0:break;
	}
}

//用户登录函数
int accountLogin(Account str[])    //形参为账户数组
{
	Account s1;                    //存储用户登录信息
	int password_Length=0;         //判断密码长度
	int password_Number=3;         //密码次数（三次错误将被锁定）
	int account_Number;            //账户ID下标
	printf("\n\n\n\n\n\n\n\n\n");
	printf("\t\t\t\t\tplease input Account_ID:");
	scanf("%s",&s1.ID);
	while(find(str,s1)<0)          //判断ID是否存在
	{
		
		printf("\t\t\t\t\tID input error\n");
		printf("\t\t\t\t\tplease input Account_ID again:");
		scanf("%s",&s1.ID);
	}
	account_Number=find(str,s1);   //查询用户ID数组下标，将其放置在account_Number
	if(str[account_Number].judge==1)  //判断用户是否被锁定
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
			s1.Password[password_Length]+=51;                    //加密算法，使文件密码被隐藏
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
		s1.Password[6]='\0';       //字符串结束标志位，使s1.Password成为字符串，防止出现乱码
		printf("\n");
		if(strcmp(str[account_Number].Password,s1.Password))
		{
			printf("\t\t\t\t\tpassword input error,the left times you have: %d\n",--password_Number);
			if(password_Number==0) //错误次数达到三次，用户被锁定
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
	Sleep(2000);     //延时2秒
	system("cls");   //清屏
	return account_Number;         //返回账户在数组中下标
}

//信息查询模块函数
void menuSearch(Account *str,Statement *sta,int account_Number)  //str为账户数组指针，sta为交易流水指针，account_Number为账户在数组下标
{
	int choice_number;     //进行业务选择
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
	system("cls");                 //清屏
	switch(choice_number)
	{
	case 1:
		searchBalance(str,account_Number);      //查询余额
		menuSearch(str,sta,account_Number);     //返回菜单
		break;
	case 2:
		searchManage(str,sta,account_Number);   //查询交易记录
		menuSearch(str,sta,account_Number);     //返回菜单
		break;
	case 3:
		menuAccount(str,sta,account_Number);    //返回二级菜单
		break;
	}
}

//查询余额函数
void searchBalance(Account *acc,int account_Number)   //acc为账户数组指针，account_Number为账户数组下标
{
	printf("\n\n\n\n\n\n\n\n\n");
	printf("\t\t\t\t\tNow your account balance is：%.2f\n",acc[account_Number].money);
	printf("\t\t\t\t\tClick enter to return.");                 //按下回车键返回上一级
	getch();
	system("cls");
}

//二级菜单，用户办理业务菜单
void menuAccount(Account *str,Statement *sta,int account_Number)  //str为账户指针，sta为交易流水指针，account_Number为账户数组下标
{
	int choice_number;    //进行业务选择
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
	system("cls");        //清屏
	switch(choice_number)
	{
	case 1:
	    MoneyChange(str,sta,choice_number,account_Number);   //调用存取款函数
		break;
	case 2:
		MoneyChange(str,sta,choice_number,account_Number);   //调用存取款函数
		break;
	case 3:
		menuSearch(str,sta,account_Number);                  //用户信息查询函数
		break;
	case 4:
		transferMoney(str,sta,account_Number);               //转账函数
		break;
	case 5:
		ChangePassword(str,sta,account_Number);              //修改密码函数
		break;
    case 6:
		break;
	}
}


//查询账户在数组的下标函数以及判断账户是否存在
int find(Account str[],Account s1)
{
	int i=0;
	for(i=0;i<20;i++)
	{
		if(strcmp(str[i].ID,s1.ID)==0)
			return i;  //返回账户下标
	}
	return -1;//返回-1表示账户不存在
}

//读取账户文件信息函数
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
	fscanf(fp,"%s%s%s%lf%d",&str[i].ID,&str[i].Name,&str[i].Password,&str[i].money,&str[i].judge);   //读入账户数据到str中
	while(!feof(fp))  //判断是否到文件末尾
	{
		i++;
		fscanf(fp,"%s%s%s%lf%d",&str[i].ID,&str[i].Name,&str[i].Password,&str[i].money,&str[i].judge);
	}
	fclose(fp);
}

//读取账户流水信息函数
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
	fscanf(fp,"%s%s%s%lf%s",&sta[i].accountID,&sta[i].time,&sta[i].type,&sta[i].money,&sta[i].toAccount);  //读入交易流水数据到sta中
	while(!feof(fp))   //判断是否到文件末尾
	{
		i++;
		fscanf(fp,"%s%s%s%lf%s",&sta[i].accountID,&sta[i].time,&sta[i].type,&sta[i].money,&sta[i].toAccount);
	}
	fclose(fp);
}

//存取款后更新账户金额信息
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
		fprintf(fp,"%-12s%-8s%-8s%-8.2f%-4d\n",str[i].ID,str[i].Name,str[i].Password,str[i].money,str[i].judge);  //读入账户数据到文件中
	}
	fclose(fp);
}

//添加交易记录函数，形参为用户信息结构体
void add_Account_Transactions(Statement account_Message)
{
	FILE *fp;
	fp=fopen("account Transaction.txt","a+");
	if(fp==0)
	{
		printf("file open error");
		exit(1);
	}
	fprintf(fp,"%-12s%-16s%-12s%-8.2lf%-8s\n",account_Message.accountID,account_Message.time,account_Message.type,account_Message.money,account_Message.toAccount);   //添加交易流水到文件中
	fclose(fp);
}

//获取当前时间函数
char* getDate()
{
	char*current_time;
	time_t timep;
	struct tm *p;
	time (&timep);
	p=gmtime(&timep);
	current_time=(char*)malloc(sizeof(char)*20);
	sprintf(current_time,"%d/%d/%d/%d:%d",1900+p->tm_year,1+p->tm_mon,p->tm_mday,8+p->tm_hour,p->tm_min);  //将数据以字符串格式化输出
	return current_time;
}

//修改密码函数
void ChangePassword(Account *str,Statement *sta,int account_Number)   //str为账户指针，sta为交易流水指针，account_Number为账户数组下标
{
	int j=0;             //i控制结构体数组下标，j控制密码长度
	Account a1,a2;
	printf("\n\n\n\n\n\n\n\n\n");
	printf("\t\t\t\t\tplease input your new password:");
		while(j<=5)
		{
			a2.Password[j]=getch();
			a2.Password[j]+=51;       //密码加密处理
			printf("*");
			j++;
		}getch();
		a2.Password[6]='\0';
		j=0;
		printf("\n\t\t\t\t\tplease input your new password again:");
		while(j<=5)
		{
			a1.Password[j]=getch();
			a1.Password[j]+=51;       //密码加密处理
			printf("*");
			j++;
		}getch();
		a1.Password[6]='\0';                  //字符串结束标志位，使s1.Password成为字符串，防止出现乱码
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
	    system("cls");                         //单击回车键返回上一级
		menuAccount(str,sta,account_Number);                    //回到二级菜单	
}

//转账函数
void transferMoney(Account *pa,Statement *ps,int account_Number)    //pa为账户指针，ps为交易流水指针，account_Number为账户数组下标

{
	double amount;                                 //交易数额
	int k;                                       //k为toaccount用户下标
	Statement a1,a2;
	Account toaccount_Number;
	printf("\n\n\n\n\n\n\n\n\n");
	printf("\t\t\t\t\tplease input amount:");
		scanf("%lf",&amount);
	while(amount>pa[account_Number].money)                      //判断账户余额是否大于用户输入的值
		{
			printf("Insufficient Funds! please Input again:");
			scanf("%lf",&amount);
		}
	printf("\t\t\t\t\tplease input toAccount:");
	scanf("%s",toaccount_Number.ID);
	system("cls");                          //清屏
	k=find(pa,toaccount_Number);            //找到转账账户下标
	while(account_Number==k||k==-1)                      //判断用户输入的ID是否有效
	{
		printf("\t\t\t\t\terror input!\n");
		printf("\t\t\t\t\tplease input again:");
		scanf("%s",toaccount_Number.ID);
		system("cls");
		k=find(pa,toaccount_Number);
	}
	pa[account_Number].money=pa[account_Number].money-amount;         //更新账户余额
	pa[k].money=pa[k].money+amount;         //更新被转帐用户的账户余额

	strcpy(a1.accountID,pa[account_Number].ID);          //将登录用户交易记录存储在a1中
	strcpy(a1.time,getDate());
	strcpy(a1.type,"Transfer");
	a1.money=amount;                        
	strcpy(a1.toAccount,toaccount_Number.ID);

	strcpy(a2.accountID,pa[k].ID);          //将被转账用户交易记录存储在a2中
	strcpy(a2.time,getDate());
	strcpy(a2.type,"Input");
	a2.money=amount;                        
	strcpy(a2.toAccount,pa[account_Number].ID);

	writeFile(pa);                          //更新账户金额信息
	add_Account_Transactions(a1);           //将a1中的记录存入文件中
	add_Account_Transactions(a2);           //将a2中的记录存入文件中
	printf("\n\n\n\n\n\n\n\n\n");
	printf("\t\t\t\t\tTransfer success!\n");
	printf("\t\t\t\t\tNow your account balance is:%.2f\n",pa[account_Number].money);
	printf("\t\t\t\t\tThe page will jump in two seconds.\n");
	Sleep(2000);                            //延时两秒
	system("cls");                          //清屏
	menuAccount(pa,ps,account_Number);       //回到业务选择菜单

}


//存取款函数
void MoneyChange(Account *pa,Statement *ps,int choice,int account_Number)   //ps为账户指针，ps为交易流水指针，choice为存取款业务选择，account_Number为账户数组下标             
{
	double amount;             //存取款金额
	int i;                     //用于下面菜单业务选择
	Statement account_trans;   //存储本次交易流水信息
	char judge;                //用来忽略回车符
	printf("\n\n\n\n\n\n\n\n\n");
	printf("\t\t\t\t\tplease input the amount:");
	scanf("%lf",&amount);
	getchar();
	if (choice==2)             //判断余额增减情况
	{
		while(amount>pa[account_Number].money)                 //判断账户余额是否大于用户输入的值
		{
			printf("\t\t\t\t\tBalance isn't enough!please input the amount again:");
			scanf("%lf",&amount);
		}
		    amount=-amount;
	}
	pa[account_Number].money=pa[account_Number].money+amount;  //更新数组账户余额
	strcpy(account_trans.toAccount,"NULL");                       //存取款无toaccount
	strcpy(account_trans.accountID,pa[account_Number].ID);     //记录账户ID
	account_trans.money=abs(amount);                           //记录交易金额
	strcpy(account_trans.time,getDate());                      //记录交易时间
	switch (choice)
   {
	case 1:
		strcpy(account_trans.type,"Deposit");
                      break;   
    case 2:
		strcpy(account_trans.type,"Withdrawal");
					  break;                                   //记录资金变动类型
	}
	writeFile(pa);                                             //更新文件账户余额
	add_Account_Transactions(account_trans);                   //添加交易流水
	Sleep(1000);             //延时1秒
	printf("\n");
	printf("\t\t\t\t\tDesposit success!\n");
	printf("\t\t\t\t\tNow your account balance is:%.2f\n",pa[account_Number].money);
	printf("\t\t\t\t\tWhether to print the return form or not?(Y/N):");
	judge=getchar();
	while(judge!='Y'&&judge!='N'&&judge!='y'&&judge!='n')
	{
		printf("\t\t\t\t\tInput error! Whether to print the return form or not?(Y/N):");
		getch();           //忽略回车字符
		judge=getchar();
	}
	if(judge=='Y'||judge=='y')
	{
		printf("\t\t\t\t\tComplete return printing!\n");
	}
	printf("\t\t\t\t\tThe page will jump in two seconds.");
	Sleep(2000);       //延时2秒
	system("cls");     //清屏
	printf("\n\n\n\n\n\n\n\n\n");
	printf("\t\t\t\t\t##########################################\n");
	printf("\t\t\t\t\t#                                        #\n");
	printf("\t\t\t\t\t# 1.Withdraw card 2.Return the main menu #\n");
	printf("\t\t\t\t\t#                                        #\n");
	printf("\t\t\t\t\t##########################################\n");
	printf("\t\t\t\t\tplease input your choice:");
	scanf("%d",&i);
	system("cls");           //清屏
	if(i==2)
	menuAccount(pa,ps,account_Number);  //回到二级菜单
}

//流水信息查询函数
void searchManage(Account *acc,Statement *sta,int account_Number)     //acc为账户指针，sta为交易流水指针，account_Number为账户数组下标
{
	int i=0;                        //用于遍历流水信息数组
	printf("\n\n\n\n\n\n\n\n\n");
    printf("\t\t\t\t\tAccount ID       Time         Type     Amount/$     toAccount ID\n");
	readFile_Transactions(sta);     //读取文件流水信息到数组
	while(sta[i].money>10e-3&&i<=1000)
	{
		if(strcmp(sta[i].accountID,acc[account_Number].ID)==0)
			printf("\t\t\t\t\t%-12s%-16s%-12s%-12.2lf%-10s\n",sta[i].accountID,sta[i].time,sta[i].type,sta[i].money,sta[i].toAccount);
		i++;
	}
	printf("\t\t\t\t\tClick enter to return.");
	getch();
	system("cls");                         //单击回车键返回上一级
}

int main()
{
	int choice;                              //用户业务选择
	struct Account  str[25];                 //存储文件用户账户信息
	struct Statement sta[1000];              //存储交易流水信息
	readFile(str);                           //读入文件
	memset(sta,0,sizeof(Statement)*1000);    //初始化sta数组
	do
	{
		menu();                              //一级菜单
		printf("\t\t\t\t\tPlease input your choice:");
		scanf("%d",&choice);
		system("cls");           //清屏
		if(choice>=0&&choice<=3)
			runMain(str,sta,choice);
		else
			printf("error input,please input your choice again!\n");
	}while(choice);
	printf("\n\n\n\n\n\n\n\n\n\n\t\t\t>>>>>>>>>>  Exit success,please keep your bank card!  <<<<<<<<<<<<\n");
	return 0;
}
