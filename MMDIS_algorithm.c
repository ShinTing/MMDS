#include <stdio.h>
#include <stdlib.h>
#define NODE  30
#define OVER  0
#define UNDER 1 
#define EQUAL 2
#define true 1
#define false 0
#define file_num 1000

int graph[NODE][NODE]; 

int open_x[NODE]={false};
int benefit_g[NODE]={UNDER};
int action[5]={0};

int require_ary[NODE];

/*找出每回合擁有最小值的點其id*/  
int min_index (int k[])        
{
    int min=0;
    int i=0,j=0;
    int degree=0;
    int index=0;

    min=k[0];
    index=0;
    for (j=1;j<NODE;j++)
    {
        if (min>k[j])
        {
            min=k[j];
            index=j;
        }
    }
    return index;
}

/*找出每回合擁有最小值*/
int min_backoff (int k[],int j)        
{
    int min;
    min=k[j];
    return min;
}

int degree (int k) // 判斷鄰近點的個數
{
    int total=0;
    int i=0;
    for ( i=0;i<NODE;i++)
    {
        if (graph[k][i]==1)
        {
            total++;
        }
    }
    return total;
}

/*計算|N(i)|內的個數*/
int dominator_num(int k)
{
    int i=0,j=0;
    int num;
    num=0;
      
    if (open_x[k]==true)
    {
        num++;                   
    }
      
    for (i=0;i<NODE;i++)
    {
        if (graph[k][i]==1 && open_x[i]==true) // *是相鄰，而且鄰居在set中
        {
            num++;
        }
    }   
        return num;   
    }
/*
int become_dominator(int k) // k是自己
{
    int i=0; // i是鄰居
    int num=0;
    for (i=0;i<NODE;i++)
    {
        if (benefit_g[k]==UNDER) // 自己若為UNDER,則回傳1（將自己加入set） 
        {
            return 1;             
        }              
        if (graph[k][i]==1 && benefit_g[i]==UNDER) // 鄰近存在一個點為UNDER且在set中,則回傳1 （將自己加入set） 
        {
            return 1;                
        }                
    }                     
    return 0; // 否則的話我不用加入set                                
}
*/

int become_dominator(int k)
{
    if (benefit_g[k] == UNDER)
    {
        return 1;
    }

    for (int i = 0; i < NODE; i++)
    {
        // 有鄰居還沒被dominated，所以來判斷能不能true
        if (graph[k][i] == 1 && benefit_g[i] == UNDER) // 鄰近存在一個點為UNDER且在set中,則將自己加入set
        {
            for (int j = 0; j < NODE; j++)
            {
                // 如果有鄰居已經是dominator
                if (graph[k][j] == 1 && open_x[j] == true)
                {
                    double d_current = degree(k);
                    double d_another = degree(j);
                    int id_current = k;
                    int id_another = j;
                   
                    // 輸
                    if (d_current < d_another || (d_current == d_another && id_current > id_another))
                    {
                        return false;
                    }
                }
            }
            // 比贏了
            return true;
        }
    }
    // 鄰居都dominaterd
    return false;
}

/*    
int become_nondominator(int k) // 1的話就是退出set
{
    int i=0;
    int num=0;
    if (benefit_g[k]!=OVER) // 這個省略掉會不會有問題  // 如果我沒有over，那我就不該退出set(return 0)       
    {
        return 0;                               
    }
    for (i=0;i<NODE;i++)
    {                             
        if (graph[k][i]==1) 
        {
            if(benefit_g[i]==OVER) // （新）鄰居已經OVER，那我就要退出set（return 1） <--應為如果「所有」鄰居已經OVER，我才要退出set
            {
                continue; // *return 1;  <--原本正確，不用改                  
            }    
            else
            {
                return 0; // (原）只要找到一個在set中的鄰居沒有OVER，我就不用退出？ <--這樣才對
            }             
        }          
    }                     
    return 1;                                 
}
*/

/*
int become_nondominator(int k) // 注意for loop的問題
{
    if (benefit_g[k]!=OVER) // 這個省略掉會不會有問題  // 如果我沒有over，那我就不該退出set(return 0)       
    {
        return 0;                               
    }
    for (int i = 0; i < NODE; i++)
    {
        if (graph[k][i] == 1 && open_x[i] == true) // 如果鄰居在set中
        {
            double d_current = degree(k);
            double d_another = degree(i);
            int id_current = k;
            int id_another = i;
            if (d_current < d_another || (d_current == d_another && id_current > id_another))
            {
                return true;
            }
        }
    }
    return false;
}
*/
int become_nondominator(int k) // 注意for loop的問題
{
    if (benefit_g[k]!=OVER) // 這個省略掉會不會有問題  // 如果我沒有over，那我就不該退出set(return 0)       
    {
        return 0;                               
    }
    for (int i = 0; i < NODE; i++)
    {                             
        if (graph[k][i]==1) 
        {
            if(benefit_g[i]==OVER) // 如果「所有」鄰居已經OVER，我才要退出set
            {
                continue;                  
            }    
            else
            {
                if (open_x[i] == true) // 如果鄰居在set中
                {
                    double d_current = degree(k);
                    double d_another = degree(i);
                    int id_current = k;
                    int id_another = i;
                    if (d_current > d_another || (d_current == d_another && id_current < id_another))
                    {
                        return 0;
                    }
                }
            }             
        }          
    }                     
    return 1; 
}
    
/*check node符合哪些rule*/
int check_action(int k)
{
    int i=0,rule_num=0; // 計算幾個rule的條件成立 
    rule_num=0;

    for (i=0;i<5;i++)
    {
        action[i]=0;                       
    }  
   
       
    /*Rule1*/       
    if (dominator_num(k)< require_ary[k] && benefit_g[k]!=UNDER) 
    { 
        action[0]=1;
        rule_num++;
    }
 
    /*Rule2*/   
    if (dominator_num(k)==require_ary[k] && benefit_g[k]!=EQUAL)
    {                            
        action[1]=1; 
        rule_num++;
        //printf("action2\n");                
    }  
    /*Rule3*/    
    if (dominator_num(k)>require_ary[k] && benefit_g[k]!=OVER ) 
    {                      
        action[2]=1; 
        rule_num++; 
        //printf("action3\n");
    }   

    /*Rule4*/   
    if (become_dominator(k)==1 && open_x[k]!=true) 
    {
        action[3]=1;
        rule_num++;
        //printf("action5\n");
    }
    
    /*Rule5*/  
    if ( become_nondominator(k)==1 && open_x[k]!=false)  
    {        
        action[4]=1;
        rule_num++;
        //printf("action6\n");
    }      
    return rule_num;  
}

int isOver()                      
{
	int i;
	for (i=0;i<NODE;i++)
    {
        // 每個點都輪一遍判斷有沒有想做決策，若迴圈跑完則表示沒有
        /*Rule1*/ 
        if (dominator_num(i) < require_ary[i] && benefit_g[i]!=UNDER)
        {
            return 1; // 想要做決策回傳1
        }
        /*Rule2*/     
        else if (dominator_num(i) == require_ary[i] && benefit_g[i]!=EQUAL)  
        {
            return 1;// 想要做決策回傳1
        }
        /*Rule3*/
        else if (dominator_num(i) > require_ary[i] && benefit_g[i]!=OVER )
        {
            return 1;// 想要做決策回傳1              
        }
        /*Rule4*/      
        else if (become_dominator(i)==1 && open_x[i]!=true)     
        {
            return 1;// 想要做決策回傳1               
        }
        /*Rule5*/  
        else if (become_nondominator(i)==1 && open_x[i]!=false)  
        {        
            return 1;// 想要做決策回傳1
        }             
    }//end for(i)       
	return 0;    // 沒有想要做決策，已做完(over)回傳0
} 

int IS_multidomination()
{
    int i=0,j=0;
    int num=0;
    int num2=0; 
    for (i=0;i<NODE;i++)
    {
        num=0; 
        num2=0;
        if (open_x[i]==0) // open_x[i]=0
        {
            for (j=0;j<NODE;j++)
            {
                if (graph[i][j]==1 && open_x[j]==1)
                {
                    num++;  
                    if (num >= require_ary[i])
                    {
                        break;        
                    }               
                }
            } 
            if (num < require_ary[i]) 
            {
                return 0;        
            }       
        }
        else // open_x[i]==1
        {
            for (j=0;j<NODE;j++)
            {
                if (graph[i][j]==1 && open_x[j]==1)
                {
                    num2++;  
                    if (num2>=require_ary[i]-1)
                    {
                        break;        
                    }               
                }                                 
            } 
            if (num2<require_ary[i]-1) 
            {
                return 0;        
            } 
        }             
    }                
    return 1;              
}
    
/*check 是否為minimal multi-domination*/
int IS_minimal_multidomination()
{
    int i,j=0;                        
    int num=0;
    for (i=0;i<NODE;i++)
    {
        if (open_x[i]==1)
        {
            open_x[i]=0;
            if (IS_multidomination()==1) // 代表去掉一個點後,仍為 multi-domination 
            {
                open_x[i]=1;                        
                return 0;                     
            }   
            else
            {
                open_x[i]=1;     
            }           
        }                
    }                     
    return 1;                  
}

/*check 是否為independent*/
int IS_independent()
{
    for (int i = 0; i < NODE; i++)
    {
        for (int j = 0; j < NODE; j++)
        {
            // 如果有鄰居已經是dominator
            if (graph[i][j] == 1 && open_x[i] == true && open_x[j])
            {
                return 0;
            }
        }

    }
    return 1;
}

int main()
{
    int i=0,j=0,a=0,value=0;
    int condition=0;
    int index=0;
    int random_backoff[NODE]={0}; // 每回合每個點的隨機數 
    int round_min_id=-1; // 每回合的最小id
    int round_min_value=-1;
    int activity_num=0;  
    int open_num[file_num]={0};
    char filename1[1000]={0}; 
    //char filename2[1000]={0}; 
    FILE *tinput,*fout;
    //FILE *tinput,*rinput,*fout;
    int rinput = 1; // k = 1
    srand((unsigned)time(NULL));
    fout=fopen("activity_num100(P=0,mk=2).txt","w");     
    //fout=fopen("\\Users\\shih\\eclipse-workspace\\MMDS_code\\minimal multi-domination(k不同)\\500_WS\\activity_num100(P=0,mk=2).txt","w");     

    for (value = 1; value <= file_num; value++)
    {
        printf("graph%d開始run\n",value);                          
        sprintf(filename1,"WS/30/1/%d.txt",value);    //讀入topology  
        //sprintf(filename1,"\\Users\\shih\\eclipse-workspace\\MMDS_code\\WS model\\500_topology\\P=0\\mk=2\\500_%d.txt",value);    //讀入topology  
        printf("%s\n", filename1);
        if ((tinput = fopen(filename1, "r"))==NULL) // 因為fopen() 規定要傳入字串來當作檔名
        {
            printf("File open fail... ");
            return -1;
        }
        //sprintf(filename2,"\\Users\\shih\\eclipse-workspace\\MMDS_code\\WS model\\500_require_k\\P=0\\mk=2\\require_num_%d.txt",value);   //讀入需求k  
        /*if((rinput = fopen(filename2, "r"))==NULL)      //因為fopen() 規定要傳入字串來當作檔名
        {
            //printf("File open fail... ");
            return -1;
        }
        */ 
        
        for (i=0;i<NODE;i++) // 讀入graph topology 
        {
            for (j=0;j<NODE;j++)
            {              
                fscanf(tinput,"%d",&graph[i][j]);
            }
        } 
          
        for (i=0;i<NODE;i++) // 讀入k需求 
        {     
            require_ary[i] = 1;

            //fscanf(rinput,"%d",&require_ary[i]);
        }
                   
        //printf("graph%d結束\n",value);
        /*初始化狀態*/    
        for (i=0;i<NODE;i++)
        {
            benefit_g[i]=UNDER ;          
        }

        for (i=0;i<NODE;i++)
        {
            open_x[i]=false;                
        } 
          
        for (i=0;i<NODE;i++) // 隨機assign給每個點一個1~100的數值 
        {
            random_backoff[i]=(rand()%100)+1;
        }
     
        do
        {
            round_min_id=min_index(random_backoff); // 得到每回合backoff最小值其id名稱 
            round_min_value=min_backoff(random_backoff, round_min_id); 
//printf("round_min_id=%d\n",round_min_id);
            if (check_action(round_min_id)>0)
            {
                do
                {
                    condition=(rand()%6);
                }
                while (action[condition]!=1);
                  
                switch (condition)
                {
                          
                    case 0:
                        //printf("case1\n");
                        benefit_g[round_min_id]=UNDER;
                        break;
                    
                    case 1:
                        //printf("case2\n");
                        benefit_g[round_min_id]=EQUAL;
                        break;
            
                    case 2:
                        //printf("case3\n");
                        benefit_g[round_min_id]=OVER;
                        break;
            
                    case 3:
                        //printf("case4\n");
                        open_x[round_min_id]=true;
                        break;

                    case 4:
                        //printf("case5\n");
                        open_x[round_min_id]=false;
                        break;  
                }// end switch                       
            } // end if
            for (i=0;i<NODE;i++)
            {
                random_backoff[i]=random_backoff[i]-round_min_value;
            }
            random_backoff[round_min_id]=(rand()%100)+1;     
        }
        while(isOver());   
//printf("gameover\n");  
        activity_num=0;
        for (i=0;i<NODE;i++)
        {
            if (open_x[i]==1)
            {
                activity_num++;
            }
        }   
        open_num[index]=activity_num;
        index++;
    
        /*check 是否為 multi-domination*/
        if (IS_multidomination()==1)
        {
            printf("此圖為multi-domination\n");                   
        }
        else
        {
            printf("此圖不為multi-domination\n");                                                     
        }   
      
        /*check 是否為 minimal multi-domination*/
        if (IS_minimal_multidomination()==1)
        {
            printf("此圖為minimal multi-domination\n");                   
        }
        else
        {
            printf("此圖不為minimal multi-domination\n");                                                     
        } 

        /* 檢查是否independent */
        if (IS_independent() == 1)
        {
            printf("此圖為independent\n");  
        }
        else
        {
            printf("此圖不為independent\n"); 
        }
       
        /*回復初始狀態*/ 
        for (i=0;i<NODE;i++)
        {
            open_x[i]=false;                
        }      
        for (i=0;i<NODE;i++)
        {
            benefit_g[i]=UNDER ;          
        }   
        printf("graph%d測試結束\n",value);                
        fclose(tinput);     
        //fclose(rinput);    
    }//end for value     

    for (i=0;i<file_num;i++)
    {
        fprintf(fout,"%d\n",open_num[i]);
    }  
   
    printf("%d\n",index);    
    fclose(fout); 
          
    system("PAUSE");	
    return 0;          
}  
  
