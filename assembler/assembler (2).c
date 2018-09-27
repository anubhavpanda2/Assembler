#include<stdio.h>
#include<string.h>
#include<stdlib.h>

struct operand{
       char op1[10];
       char op2[10];
};
int make_literal_table(int lit_val);
int get_lenght(char *pseudo,char *op)
{
    char *s=op;
    int mul,val=0;
    if(strcmp(pseudo,"DS")==0)
    {
                              while(*s>='0'&& *s<='9')
                              {
                                              val=val*10+(int)(*s-'0');
                                              s++;
                              }
                              if(*s=='F')
                                val*=4;
							  if(*s=='H')
							    val*=2;
    }
    else
    {
        if(*s=='F')
                                mul=4;
		if(*s=='H')
							    mul=2;
        while(*s!='\0')
        {
                       if(*s==',')
                                  val++;
                       s++;
        }
        val*=mul;
    }
    return val;
}

int get_baseReg(char *op)
{
	int base_reg=0;
	char *s=op;
	while(*s++!=',');
	while(*s!='\0')
	{
		base_reg=base_reg*10+(int)(*s-'0');
		s++;
	}
	return base_reg;
}


int PASS1()
{
     int flag_mnem=0;
     int flag_pseudo=0;
     FILE *f1,*f2,*f3,*f4,*f5,*f6;
     int loc_count,start_add,op1,value,add_loc,base_reg,lit,len=1;
     int no_ltorg=1;
     char mnem[20],label[20],op[20],otp[20],pseudo[10],symbol[10];
     f1=fopen("input.txt","r");
     f4=fopen("symtab.txt","w");
     f6=fopen("obcode.txt","w");
     fscanf(f1,"%s %s %d",label,mnem,&op1);     //reading the first line
     if(strcmp(mnem,"START")==0)
     {
                                start_add=op1;
                                loc_count=start_add;
     }
     else
                                loc_count=start_add=0;
     fprintf(f6,"\n%s\t%s\t%s\t%s\n","LC","label","mnemonics","op");
     fprintf(f4,"\n%s\t%s\t%s\n","value","label","len");      //heading of symbols table
     fscanf(f1,"%s %s",label,mnem);
     while(!feof(f1))
     {
                     fscanf(f1,"%s",op);                                //checking pseudo op start
                     fprintf(f6,"\n%d\t%s\t%s\t%s\n",loc_count,label,mnem,op);
                                           
                     if(strcmp("EQU",mnem)==0)
                     {
                                              if(strcmp(op,"*")==0)
                                                                   value=loc_count;
                                              else
                                                                   value=atoi(op);
                                                                   len = 1;
                                                add_loc=0;
                     }
                         
                                           f3=fopen("POT.txt","r");
                                           fscanf(f3,"%s",pseudo);
                                           while(!feof(f3))
                                           {
                                                           if(strcmp(mnem,pseudo)==0)
                                                           {
                                                                                     add_loc=0;
                                                                                     flag_pseudo=1;
                                                                                     if(strcmp("DS",mnem)==0)
                                                                                     {
                                                                                                             add_loc=get_lenght("DS",op);
                                                                                                             value=loc_count;
                                                                                                             len = 1;
                                                                                                             break;
                                                                                     }
                                                                                     else if(strcmp("DC",mnem)==0)
                                                                                     {
                                                                                                             add_loc=get_lenght("DC",op);
                                                                                                             value=loc_count;
                                                                                                             len = 1;
                                                                                                             break;
                                                                                     }
                                                                                     else if(strcmp("USING",mnem)==0)
                                                                                     {
																							len=1;
																							value=loc_count;
																							base_reg=get_baseReg(op);
																							add_loc=0;
																							break;
																					 }
																					 else if(strcmp("LTORG",mnem)==0)
                                                                                     {
                                                                                                                len=1;
                                                                                                                value=loc_count;
                                                                                                                lit=loc_count-(loc_count%8) + 8;
                                                                                                                fclose(f6);
                                                                                                                fclose(f4);
                                                                                                                printf("\n%d\n",lit);
                                                                                                                add_loc=make_literal_table(lit);
                                                                                                                printf("%d\n",add_loc);
                                                                                                                printf("\n value %d\nadd_loc %d\nlit %d\n",value,add_loc,lit);
                                                                                                                f6=fopen("obcode.txt","a");
                                                                                                                f4=fopen("symtab.txt","a");
                                                                                                                no_ltorg=0;
                                                                                                                break;
                                                                                     }
                                                           }
                                                           fscanf(f3,"%s",pseudo);
                                           }
                                           fclose(f3);                            //checking pseudo op close
                                                                                  
                                           if(flag_pseudo==0)
                                           {   
                                               f2=fopen("MOT.txt","r");       //checking the MOT start
                                               fscanf(f2,"%s %d",otp,&len);
                                               while(!feof(f2))
                                               {
                                                           if(strcmp(mnem,otp)==0)
                                                           {
                                                                                add_loc=len;
                                                                                value=loc_count;
                                                                                flag_mnem=1;
                                                                                break;
                                                           }
                                                           fscanf(f2,"%s %d",otp,&len);
                                               }
                                               fclose(f2);                   //MOT checking over
                                           }
                         if(strcmp(label,"-")!=0)          //if label is present enter into symbols table
                     	{
                                                                           fprintf(f4,"\n%d\t%s\t%d\n",value,label,len);
                     	}
                     	
                     	loc_count+=add_loc;                 //updating location counter

                     fscanf(f1,"%s %s",label,mnem);         //reading next line
                     flag_pseudo=flag_mnem=0;
                     if(strcmp(mnem,"END")==0)              //END has been encountered
       				{
                                printf("\nProgram length = %d \n",(loc_count-start_add));
                                break;
       				}
       }
       /*
       if(no_ltorg==1)
                      add_loc=make_literal_table(lit);*/
       fclose(f1);               
       fclose(f4);
       fclose(f6);
}
 
int make_literal_table(int lit_val)
{      
       FILE *f4,*f2; 
       FILE *f6=fopen("obcode.txt","r");
       if(f6==NULL){
              printf("not opening\n");
              return;}
       else
              printf("opening\n");
       FILE *f5=fopen("littab.txt","w");
       
       int loc_count,val,add_loc,len,value=lit_val;
       char *s,label[10],symbol[10],mnem[10],op[10],opt[10],literal[10];
       memset(op,0,10);
       //filling literals table
       fprintf(f5,"\n%s\t%s\t%s\n","value","label","len");      //heading of symbols table
       fscanf(f6,"%d%s%s%s",&loc_count,label,mnem,op);
       fscanf(f6,"%d%s%s",&loc_count,label,mnem);
       while(!feof(f6))
       {
                    fscanf(f6,"%s",op); 
                    printf("\n%s\n",op);
                    s=op;
                    printf("\n%s\n",s);
                    while(*s!='\0')
                    {
                                  // printf("im in\n");
                                   if(*s=='=')
                                   {
                                                               //       printf("\nim in =\n");
                                                                      char * e="=";
                                                                  //    strcpy(literal,s);
                                                                      strcpy(literal,s);
                                                                      
                                                                      printf("\n= found\n");
                                                                      f2=fopen("MOT.txt","r");       //checking the MOT start
                                                                      fscanf(f2,"%s %d",opt,&len);
                                                                      add_loc=1;
                                                                      while(!feof(f2))
                                                                      {
                                                                                      if(strcmp(mnem,opt)==0)
                                                                                      {
                                                                                                             add_loc=len;
                                                                                                             break;
                                                                                      }
                                                                                      fscanf(f2,"%s %d",opt,&len);
                                                                      }
                                                                      fclose(f2);                   //MOT checking over
                                                                      fprintf(f5,"\n%d\t%s\t%d\n",value,literal,add_loc);
                                                                      value+=add_loc;
                                                                      break;
                                   }
                                   s++;
                    }
                    fscanf(f6,"%d %s %s",&loc_count,label,mnem);
       }   
       fclose(f6);
       fclose(f5);
       return value-lit_val;
       //printf("base register= %d\n",base_reg);
}
struct operand fetch_operand(char op[])
{
       struct operand o;
       strcpy(o.op1,strtok(op,","));
       strcpy(o.op2,strtok(NULL,","));
       return o;
}

void PASS2()
{
     struct operand opr;
     FILE *f1,*f2,*f3,*f4;
     int flag1,flag2;
     flag1=flag2=0;
     char label[10],mnem[10],op[15],val[10],add[10],sym[10],len[10],op1_val[10],op2_val[10];
     f1=fopen("obcode.txt","r");
     f2=fopen("obj_code.txt","w");
     if(f2==NULL)
                 printf("noy opening\n");
     else
                 printf("opening\n");
     fscanf(f1,"%s %s %s %s",add,label,mnem,op);
     printf("\n%s\t%s\t%s\t%s\n",add,label,mnem,op);
     fprintf(f2,"\n%s\t%s\t%s\t%s\n",add,label,mnem,op);
     fscanf(f1,"%s %s %s %s",add,label,mnem,op);
     
     while(!feof(f1))
     {
                    opr=fetch_operand(op);
                    f3=fopen("symtab.txt","r");
                    fscanf(f3,"%s%s%s",val,sym,len);
                    while(!feof(f3))
                    {
                                    
                                    if(strcmp(opr.op1,sym)==0)
                                    {
                                                              strcpy(op1_val,val);
                                                              flag1=1;
                                    }
                                    if(strcmp(opr.op2,sym)==0)
                                    {
                                                              strcpy(op2_val,val);
                                                              flag2=1;
                                    }
                                    fscanf(f3,"%s%s%s",val,sym,len);
                    }
                    fclose(f3);
                    
                    f4=fopen("littab.txt","r");
                    fscanf(f4,"%s%s%s",val,sym,len);

                    while(!feof(f4))
                    {
                                    
                                    if(strcmp(opr.op1,sym)==0)
                                    {
                                                              strcpy(op1_val,val);
                                                              flag1=1;
                                    }
                                    if(strcmp(opr.op2,sym)==0)
                                    {
                                                              strcpy(op2_val,val);
                                                              flag2=1;
                                    }
                                    fscanf(f4,"%s%s%s",val,sym,len);
                    }
                    fclose(f4);
                    if(flag1&&flag2)
                    {
                              strcat(op,op1_val);
                              strcat(op,",");
                              strcat(op,op2_val);
                    }
                    else
                    {
                        if(flag1==flag2)
                        {
                               strcat(op,opr.op1);
                               strcat(op,",");
                               strcat(op,opr.op2);
                        }
                        else if(flag1==0)
                        {
                               strcat(op,opr.op1);
                               strcat(op,",");
                               strcat(op,op2_val);
                        }
                        else
                        {
                               strcat(op,op1_val);
                               strcat(op,",");
                               strcat(op,opr.op2);
                        }
                    }
                    flag1=flag2=0;
                    printf("anubhavpanda");
                    fprintf(f2,"\n%s\t%s\t%s\t%s\n",add,label,mnem,op);
                    fscanf(f1,"%s %s %s %s",add,label,mnem,op);
     }
     fclose(f1);
     fclose(f2);
}

int main()
{
    PASS1();
   // PASS2();
    system("pause");
    return 0;
}
