#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define DEBUG_PRINT
//#define DEBUG_FPRINT

struct myROB{

	int state;//IF---1  ID---2  IS---3 EX---4 WB----5
	int src_state1,src_state2, oprand_state; ///0---occupied 1--available
	int tag;

	int list_dispatch;
	int list_issue;
	int list_execute;//	dispatch_list---1   issue_list----2 execute_list----3

	int valid;
	unsigned int fu_type;
	int src1, src2, dst;
	unsigned int if_cycle, if_dur;
	unsigned int id_cycle, id_dur;
	unsigned int is_cycle, is_dur;
	unsigned int ex_cycle, ex_dur;
	unsigned int wb_cycle, wb_dur;
	unsigned int i, cycle;

	unsigned int count_ex;//judge the cycles it takes
	int entry;
	int depend_entry1, depend_entry2;
	myROB *nextrob;
	myROB *lastrob;

};

struct RegisterFile
{
	int tag;
	int valid;
		
};



int main( )//int argc,char *argv[]


{
	int i,j,k;


	char	str[5];
	FILE	*tracefile,*out;
	char*	tracename;
	char*	outputname;

	char	seq_no[10];
	int		op,dst,src1,src2;
	int		cycle_final=0;
	int		tag=0,N=0;
	int		in_n, in_s;
	int		clk_cycle;
	int		count_rob,count_rob_id;//increment when move instr into rob[]
	int		count_FU, count_issue;
	static RegisterFile rf[100];//Register File
	double IPC;
	myROB rob[1024];
	myROB *head, *temprob, *temprob2, *tail;

	head=rob;
	tail=head;

	rob[1023].nextrob=&rob[0];
	rob[0].lastrob=&rob[1023];

	int printednum=0;
		 
/////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////Initializing
/////////////////////////////////////////////////////////////////////////////////////////
		   for(i=0;i<1023;i++)
		   {
				rob[i+1].lastrob=&rob[i];
				rob[i].nextrob=&rob[i+1];
				
		   }
		   
		   
		   for(i=0;i<1024;i++)
		   {
				
				rob[i].valid=1;
				//rob[i].temp=i;
				rob[i].list_dispatch=0;
				rob[i].list_issue=0;
				rob[i].list_execute=0;
				rob[i].entry=i;
				rob[i].oprand_state=1;
		   }
			
		   for(j=0;j<100;j++)
				{
					rf[j].valid=1;
				}

		   clk_cycle=0;
		   count_rob=0;
		   count_rob_id=0;
		   
/////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////Read_In File
/////////////////////////////////////////////////////////////////////////////////////////
	
	in_s=256;//2
	in_n=8;//8
	
	int pipestate=0;//0--pipe   1-nonpipe

	tracename = "gcc1.txt";//perl1.txt    gcc1.txt
	tracefile=fopen(tracename,"r");
	if(tracefile==NULL)  { printf("cannot open tracefile\n");}

	outputname="myoutput_256_8_gcc.txt";
	out=fopen(outputname, "w");
	if(!out) {printf("cannot open this out file\n");exit(0);}

	//myInstr instr[9999];
	count_issue = in_s;
	count_FU=in_n+1;
	count_rob=in_n;
	count_rob_id=in_n*2;
	temprob2=head;
	head->lastrob=NULL;
	int issue_rate=0;

/////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////Cycle begins
/////////////////////////////////////////////////////////////////////////////////////////
	
	//while(!feof(tracefile)||count_rob) //ONE clk cycle begins
while(printednum<10000)
	{	
	
		issue_rate=in_n+1;
		if(clk_cycle==196)
		{
			//gets(str);
		}


	printf("CYCLE=%d,TAG=%d\n",clk_cycle,tag);
	
	//printf("TEST_BEGIN ,COUNT_ROB=%d, count_issue=%d, count_ex=%d, CYCLE=%d\n",count_rob, count_issue, count_FU, clk_cycle);
	//gets(str);
	
	
	////////    1.FakeRetire(); WB-> OUT
	
	for(temprob=head;temprob!=tail;temprob=temprob->nextrob)
	{		
		if(head->state==6) head=head->nextrob;
	}
	
	for(temprob=head;temprob!=tail;temprob=temprob->nextrob)
	{
		if(temprob->state==5)//reach WB, completed
		{	

			i++;
			temprob->state=6;//state=6

			temprob->valid=0;

			//printf("i=%d\n",head->temp);

			temprob->wb_dur=1;


			#ifdef DEBUG_PRINT
					if(temprob2->state==6&&temprob2->tag==printednum&&printednum<10000)
		
							{
								if (temprob2->tag==10053)
								{	gets(str);	printf("STOP\n");	}

								fprintf(out,"%d fu{%d} src{%d,%d} dst{%d} IF{%d,%d} ID{%d,%d} IS{%d,%d} EX{%d,%d} WB{%d,%d}\n",temprob2->tag,temprob2->fu_type,temprob2->src1, temprob2->src2,temprob2->dst,temprob2->if_cycle, temprob2->if_dur,
																												temprob2->id_cycle, temprob2->id_dur,temprob2->is_cycle, temprob2->is_dur,temprob2->ex_cycle, temprob2->ex_dur,
																												temprob2->wb_cycle, temprob2->wb_dur);
								if(tag>9990)
										printf("%d fu{%d} src{%d,%d} dst{%d} IF{%d,%d} ID{%d,%d} IS{%d,%d} EX{%d,%d} WB{%d,%d}\n",temprob2->tag,temprob2->fu_type,temprob2->src1, temprob2->src2,temprob2->dst,temprob2->if_cycle, temprob2->if_dur,
																												temprob2->id_cycle, temprob2->id_dur,temprob2->is_cycle, temprob2->is_dur,temprob2->ex_cycle, temprob2->ex_dur,
																												temprob2->wb_cycle, temprob2->wb_dur);
							if (cycle_final<temprob2->wb_cycle+temprob2->wb_dur)
									cycle_final=temprob2->wb_cycle+temprob2->wb_dur;

							printf("pringted: %d temprob2->tag: %d\n",printednum,temprob2->tag);
							printednum++;
							
							temprob2=temprob2->nextrob;
							//gets(str);
					}
			#endif


			#ifdef DEBUG_PRINT1
				//while(printednum<=temprob->tag)
				{printf("%d fu{%d} src{%d,%d} dst{%d} IF{%d,%d} ID{%d,%d} IS{%d,%d} EX{%d,%d} WB{%d,%d}\n",temprob->tag,temprob->fu_type,temprob->src1, temprob->src2,temprob->dst,temprob->if_cycle, temprob->if_dur,
																									temprob->id_cycle, temprob->id_dur,temprob->is_cycle, temprob->is_dur,temprob->ex_cycle, temprob->ex_dur,
																									temprob->wb_cycle, temprob->wb_dur);
				printednum++;
				gets(str);}
			#endif

		}
	
	}

	////////     2.Execute(); EX-> WB
	////////////////////////////LATENCY

	for(temprob=head;temprob!=tail;temprob=temprob->nextrob)
		{
			if (!temprob->count_ex&&temprob->state==4)
			{	
				//if((temprob->lastrob==NULL)||((temprob->src1==-1||temprob->lastrob->dst!=temprob->src1)&&(temprob->src2==-1||temprob->lastrob->dst!=temprob->src2)&&(temprob->lastrob->state!=3)&&(temprob->lastrob->src1==-1||rf[temprob->lastrob->src1]==1)&&(temprob->lastrob->src2==-1||rf[temprob->lastrob->src2]==1)))
				{
				//printf("I'M HERE....");
				temprob->list_execute=0;
				temprob->state=5;//state=5
				count_FU++;
				
				
			if(temprob->dst!=-1)
						{
									if(temprob->entry==rf[temprob->dst].tag) 
									{
											rf[temprob->dst].valid=1; //tag match
									}
						}


				temprob->oprand_state=1;//oprand ready
				temprob->wb_cycle=clk_cycle;
				temprob->ex_dur=(temprob->wb_cycle - temprob->ex_cycle);
				}
			}
		else if(temprob->state==4)
				temprob->count_ex--;//depend on op type
	


	}
/////////////    3.Issue();IS-> EX
////////////////////////////////////////DEPENDENCY &&  MAX=N+1
	//for(k=0;k<34;k++)		printf("rf[%d]=%d  ",k, rf[k]);////Register File

		for(temprob=head;temprob!=tail;temprob=temprob->nextrob)
		{
			//if(pipestate==1)
			if((pipestate==1&&count_FU&&temprob->state==3)||(pipestate==0&&temprob->state==3&&issue_rate))//
				{
					//printf("\ntag=%d rf[src1]=rf[%d]=%d,rf[src2]=rf[%d]=%d\n",temprob->tag, temprob->src1,rf[temprob->src1],temprob->src2,rf[temprob->src2]);
					if((rob[temprob->depend_entry1].oprand_state||temprob->src_state1)&&(rob[temprob->depend_entry2].oprand_state||temprob->src_state2))
					{	
				
						issue_rate--;
						count_FU--;
						temprob->count_ex--;
						temprob->list_issue=0;
						temprob->list_execute=1;
						temprob->state=4;//state=4
						count_issue++;
						temprob->ex_cycle=clk_cycle;
						temprob->is_dur=(temprob->ex_cycle - temprob->is_cycle);
					}
	
				}
	
		}


/////////////    4.Dispatch(); ID->IS
///////////////////////////////////////////S IS FULL OR NOT
	for(temprob=head;temprob!=tail;temprob=temprob->nextrob)
	{
	if(count_issue&&temprob->state==2)
		{
			count_issue--;
			count_rob_id++;
			temprob->list_dispatch=0;
			temprob->list_issue=1;
			temprob->state=3; //state=3
			temprob->is_cycle=clk_cycle;
			temprob->id_dur=(temprob->is_cycle - temprob->id_cycle);
			//if(temprob->dst!=-1) rf[temprob->dst]=0;
			
		}

	}
/////////////   5.Fetch(); IF->ID
	for(temprob=head;temprob!=tail;temprob=temprob->nextrob)
	 {
		 if(count_rob_id&&temprob->state==1)
			{
				temprob->list_dispatch=1;
				//if(temprob->dst!=-1) rf[temprob->dst].valid=0; 
				count_rob++;
				temprob->state=2;//state=2
			}

	}

	

/////////////////////////   6. IN ->IF
	//printf("count_rob=%d, count_rob_id=%d\n",count_rob,count_rob_id);
	
			while(count_rob&&count_rob_id)
			{
		
					count_rob--;
					count_rob_id--;
					fscanf(tracefile,"%s %d %d %d %d\n",&seq_no, &op,&dst,&src1,&src2);
					//printf("N=%d  %d\n",tag,op);
					tail->fu_type=op;
					if (tail->fu_type==0)	tail->count_ex=1;
					else if(tail->fu_type==1) tail->count_ex=2;
					else if(tail->fu_type==2) tail->count_ex=5;

					tail->tag=tag;
					tail->src1=src1;
					tail->src2=src2;
					tail->dst=dst;
					tail->if_cycle=clk_cycle;
					tail->id_cycle=clk_cycle + 1;
					tail->if_dur=1;
					tail->state=1;//IF
					


					
					
					tail->oprand_state=0; //not completed
					

					if ( !rf[tail->src1].valid && tail->src1!=-1) 
						{
									tail->depend_entry1=rf[tail->src1].tag; 
									tail->src_state1 = 0;//occupied by others
						}
						else if(rf[tail->src1].valid || tail->src1==-1) 
							{
								tail->src_state1=1;
								tail->depend_entry1=tail->entry;
							}


					if(!rf[tail->src2].valid && tail->src2!=-1) 
						{
									tail->depend_entry2=rf[tail->src2].tag;// else  tail->depend_entry2=0;
									tail->src_state2 = 0;
						}
						else if(rf[tail->src2].valid || tail->src2==-1) 
								{
									tail->src_state2=1;
									tail->depend_entry2=tail->entry;
								}

					rf[tail->dst].tag = tail->entry;//entry means the entry of ROB
					rf[tail->dst].valid = 0;
					#ifdef DEBUG_FPRINT
						fprintf(out,"%d %d %d %d\n",op,dst,src1,src2);
					#endif
					tag++;
					tail=tail->nextrob;
			}
	
	//for(temprob=head;temprob!=tail;temprob=temprob->nextrob)		printf("tag=%d temprob=%d STATE= %d\n",temprob->tag, temprob, temprob->state);
		//printf("HEAD= %d  TAIL= %d temprob=%d STATE= %d\n", head, tail, temprob, temprob->state);

	clk_cycle++;
	//printf("END_OF_TEST, TAIL= %d\n",tail);
	//gets(str);
}
	

	IPC=(double)printednum/(double)cycle_final;
	
	printf("number of instructions = %d\nnumber of cycles       = %d\nIPC                    = %1.5f",printednum,cycle_final,IPC);
	fprintf(out,"number of instructions = %d\nnumber of cycles       = %d\nIPC                    = %1.5lf",printednum,cycle_final,IPC);
	fclose(out);

	fclose(tracefile);
	
	getchar();
	return(0);


}