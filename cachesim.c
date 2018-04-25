#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <limits.h>

struct cacheEntry
{
  int valid;
  int  tag;
  int counter;

};

FILE* file;
int main(int argc, char* argv[])
{
    int count=0;
    file=fopen(argv[1], "r");
    unsigned int cacheSize=atoi(argv[2]); //in kb
    unsigned int wayNum=atoi(argv[3]);
    unsigned int blockSize=atoi(argv[4]); // in bytes

    cacheSize=(cacheSize<<10); //turns this to bytes
    unsigned int setNum=(cacheSize/blockSize)/wayNum;  //the set # = frames/ways, and frames=cache/blocksize
    unsigned int blockNum=(cacheSize/blockSize);

    //i need a 2d array that goes set 0 set 1, and then way way way, valid tag data
    //it goes tag index block offset and addresses r 24bitx
    int blockOff=log2(blockSize);
    int index=log2(setNum);

      unsigned int tagBits=24-blockOff-index; //tagbits

    unsigned char* memory=(unsigned char*)calloc(1<<24, 1); //memory array
    //*memory=0;
    struct cacheEntry cache[wayNum][setNum]; //cache array of ways by sets
    for(int r=0; r<wayNum; r++)
    {
      for (int c=0; c<setNum;c++)
      {
        struct cacheEntry* temp = (struct cacheEntry*) malloc(sizeof(struct cacheEntry));
        temp->valid=0; temp->tag=-1;temp->counter=-1;
        cache[r][c]= *temp;

      }
    }

    while(1)
    {

      int address; int access; unsigned char value;
      char word[64];
      char result[64];
      fscanf(file, "%s",word);
      count++;
      if(feof(file))
      {
        break;
      }

      else if(strcmp(word, "store")==0)
      {
        fscanf(file, "%x %d", &address, &access);

        // printf("test save");
        // printf("%s\n", "test");
        // printf("0x%x %d %llx\n", address, access, value);

        int mask = (1 << tagBits) - 1;
        unsigned int x= (address>> (index+blockOff)) & mask;
        unsigned int tag=x; //gets tag bits
        //printf("test tag");
        mask = (1 << index)- 1;
         x= (address>>blockOff) & mask;
        unsigned int setindex= x; //gets setindex
      //  printf("test index");
        mask = (1 << (blockOff-0)) - 1;
        x= (address>> 0) & mask;
        unsigned int blockoffset=x; //gets block offset

        int exists=0;
        int full=1;
        //printf("test full");
        for(int i=0; i<wayNum; i++)
        {
          //printf("test array");
          if(cache[i][setindex].tag==tag && cache[i][setindex].valid==1 /*&& memory[address]!=0*/)
          {
            //printf("%d",cache[i][setindex].tag );
            exists=1;
            strcpy(result, "hit");
            for(int iter=0; iter<access; iter++)
            {
              fscanf(file, "%2hhx", &value);
              memory[address+iter]=value;

            }
               //writes to memory the new value
            cache[i][setindex].counter=count; //increments this to be new

          }

          // if(cache[i][setindex].valid==0)
          // {
          //   full=0; //finds if the set is full or not
          // }

        }
        if(exists==0)// means there's a miss
        {
          strcpy(result, "miss");
          for(int iter=0; iter<access; iter++)
          {
            fscanf(file, "%2hhx", &value);
            memory[address+iter]=value;

          } //just writes it to memory

        }

        printf("%s 0x%x %s\n", word, address, result);
        //printf("The tag is %d for 0x%x\n and index is %d\n", tag, address, setindex);

      }
      else if(strcmp(word, "load")==0)
      {
        fscanf(file, "%x %d", &address, &access);

        int mask = (1 << tagBits) - 1;
        unsigned int x= (address>> (index+blockOff)) & mask;
        unsigned int tag=x; //gets tag bits
        //printf("test tag");
        mask = (1 << index)- 1;
         x= (address>>blockOff) & mask;
        unsigned int setindex= x; //gets setindex
      //  printf("test index");
        mask = (1 << blockOff) - 1;
        x= (address>> 0) & mask;
        unsigned int blockoffset=x; //gets block offset


         //gets block offset


        unsigned char answer=0;
        int exists=0;
        int full=1;
        for(int i=0; i<wayNum; i++)
        {
          if(cache[i][setindex].tag==tag )
          {
            exists=1;
            strcpy(result, "hit");
            answer= memory[(setindex*blockSize)+blockoffset]; //writes to memory the new value
            cache[i][setindex].counter=count; //increments this to be new
            break;
          }}
          for(int i=0; i<wayNum; i++)
          {
          if(cache[i][setindex].valid==0)
          {
            full=0; //finds if the set is full or not
          }

        }
        if(exists==0)// means there's a miss
        {
          strcpy(result, "miss");
          answer= memory[address];//reads value from memory

          if(full==0)//case there's still space
          {
            for(int i=0; i<wayNum; i++)
            {
              if(cache[i][setindex].valid==0)//finds first available space
              {
                struct cacheEntry* ac= &(cache[i][setindex]);
                ac->tag=tag; ac->valid=1; ac->counter=count;

                break;
          }

        }
      }
      else if(full==1)
       //kick out the last used one and sub this in
      {
        int min=INT_MAX;
        int lruIndex;
        for(int i=0; i<wayNum; i++)
        {
          if(cache[i][setindex].counter<min)
          {
            cache[i][setindex].counter=min;
            lruIndex=i;
          }
      }
      struct cacheEntry* ac= &(cache[lruIndex][setindex]);
      ac->tag=tag; ac->valid=1; ac->counter=count;

    }

    }
    printf("%s 0x%x %s ", word, address, result);
    for(int iter=0; iter<access; iter++)
    {
      answer=memory[address+iter];
      printf( "%02hhx", answer);

    }
    printf("\n");
      //printf("The tag is %d for 0x%x\n and index is %d\n", tag, address, setindex);
  }
}
free(memory); fclose(file);
return 0;
}
