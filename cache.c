//Program calculate cache  hit and miss rate for a given sequence of memory access. 
//32 bits address assumed. Can change the cache size(Block size, no. of sets, 
//no. of lines in eah set can be varied by changing the global variables.

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

#define SIZE 32							//32 sets.
#define LINES 1							//Direct mapping. so only a single line in each set.

unsigned int b=4;							//number of bits in offset.(Block is is 2^b)
unsigned int s=5;							//number of bits in set.
unsigned int t;							//number of bits in tag.
unsigned int b_mask;						//masks to calculate tag,set and b values.
unsigned int tag_mask;
unsigned int set_mask;

//Structure definitions for SET and LINE

struct line
{
	int valid;
	int tag;
};

struct set
{
	struct line *lines;
};


struct line * getLines()
{
	//Creates LINES number of lines and returns the address.
	struct line *l=(struct line *)malloc(sizeof(struct line)*LINES);
	int i;
	for(i=0;i<LINES;i++)
	{
		l[i].valid=0;
		l[i].tag=0;
	}
	return l;
}


struct set * getCache()
{
	//Creates a cache of size SIZE and returns its address.
	struct set *sets=(struct set *)malloc(sizeof(struct set)*SIZE);
	int i;
	for(i=0;i<SIZE;i++)
	{
		sets[i].lines=getLines();
	}
	return sets;
}


int getInt(char *s,int len)
{
	//Converts the address string to hex and returns last 4 bytes of the address.
	char st[9];
	int i,k=2;
	st[0]='0';
	st[1]='x';
	for(i=6;i<len;i++)
		st[k++]=s[i];
	st[k++]='\0';
	int number = (int)strtol(st, NULL, 16);
	return number;
}


int cacheAddress(unsigned int address,struct set *sets)
{
	//This function checks if the requested address is in the cache. If yes, returns 1.
	//Else selects a line from the perticular set for replacement at random
	//Replaces the line with the requested line and returns 0.

	//Calculate offset,tag and set bits for the given address using the masks.
	int offset,tag1,set1;
	offset=address&b_mask;
	tag1=((address&tag_mask)>>(b+s));
	set1=((address&set_mask)>>b);
	
	//Check if the requested address is in the cache.
	int i;
	int flag=0;
	for(i=0;i<LINES;i++)
	{
		if(sets[set1].lines[i].valid==1&&sets[set1].lines[i].tag==tag1)
		{
			//A cache hit. 
			flag=1;
			return 1;
		}
	}
	
	if(!flag)
	{
		//a cache miss. Write the requested block into cache memory.
		//To write, first check if all lines are valid. If not write into an invalid line. 
		//If all are valid select a line at random to be replaced by a new line.
		int flag1=0;
		int i;
		for(i=0;i<LINES;i++)
		{
			if(sets[set1].lines[i].valid==0)
			{
				//An empty(invalid) line. Select this line for replacement.
				sets[set1].lines[i].valid=1;
				sets[set1].lines[i].tag=tag1;
				flag1=1;
				break;
			}
		}

		if(!flag1)
		{
			//No line in the set is invalid. Select a random line for replacement.
			int a=random()%LINES;
			sets[set1].lines[a].valid=1;
			sets[set1].lines[a].tag=tag1;
		}
		return 0;
	}	
}

int main()
{
	//Mask values for all the parameters.
	t=32-(b+s);
	b_mask=(0xffffffff>>(32-b));	
	tag_mask=(0xffffffff<<(32-t));
	set_mask=~(b_mask|tag_mask);
	
	//Create an empty cache.
	struct set *sets=getCache();
	long int i;
	char str[15];
	int hit_count=0,miss_count=0,total=0;

	for(i=0;i<3010000;i++)
	{
		//Reading address
		scanf("%s\n",str);
		unsigned int address=getInt(str,strlen(str));
		
		if(cacheAddress(address,sets)==1)
		{
			hit_count++;
		}
		else
		{
			miss_count++;
		}
		total++;
	}
	printf("%.2f\n",(float)hit_count/total);

	return 0;
}
