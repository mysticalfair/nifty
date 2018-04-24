#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <limits.h>

struct cacheEntry
{
  int valid;
  int tag;
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
    unsigned int tagBits=log((blockNum/setNum))/log(2); //tagbits
    //i need a 2d array that goes set 0 set 1, and then way way way, valid tag data
    //it goes tag index block offset and addresses r 24bitx
    int blockOff=log(blockSize)/log(2);
    int index=log(setNum)/log(2);

    long long* memory=(long long*)malloc(1<<24); //memory array
    struct cacheEntry cache[wayNum][setNum]; //cache array of ways by sets
    for(int r=0; r<wayNum; r++)
    {
      for (int c=0; c<setNum;c++)
      {
        struct cacheEntry* temp = (struct cacheEntry*) malloc(sizeof(struct cacheEntry));
        temp->valid=0; temp->tag=-1;temp->counter=0;

      }
    }

    while(1)
    {
      int address; int access; long long value;
      char word[64];
      char result[64];
      fscanf(file, "%s",word) ;
      if(feof(file))
      {
        break;
      }
      else if(strcmp(word, "store")==0)
      {
        fscanf(file, "%x %x %llx", &address, &access, &value);
        // printf("test save");
        // printf("%s\n", "test");
        // printf("0x%x %d %llx\n", address, access, value);

        int mask = ((1 << tagBits) - 1) << 0;
        unsigned int tag=address&mask; //gets tag bits
        printf("test tag");
        mask = ((1 << index) - 1) << (24-tagBits);
        unsigned int setindex= address&mask; //gets setindex
      //  printf("test index");
        mask = ((1 << blockOff) - 1) << (24-tagBits-index);
        unsigned int blockoffset=address&mask; //gets block offset

        int exists=0;
        int full=1;
        //printf("test full");
        for(int i=0; i<wayNum; i++)
        {
          //printf("test array");
          if((cache[i] + setindex)->tag==tag)
          {
            //printf("%d",cache[i][setindex].tag );
            exists=1;
            strcpy(result, "hit");
            *(memory+((setindex*blockSize)+blockoffset))=value; //writes to memory the new value
            cache[i][setindex].counter=count; //increments this to be new
            count++;
          }

          // if(cache[i][setindex].valid==0)
          // {
          //   full=0; //finds if the set is full or not
          // }

        }
        if(exists==0)// means there's a miss
        {
          strcpy(result, "miss");
          *(memory+((setindex*blockSize)+blockoffset))=value; //just writes it to memory
        }

        printf("%s %d %s\n", word, address, result);

      }
      else if(strcmp(word, "load")==0)
      {
        fscanf(file, "%d %d", &address, &access);
        int mask = ((1 << tagBits) - 1) << 0;
        unsigned int tag=address&mask; //gets tag bits
        mask = ((1 << index) - 1) << (24-tagBits);
        unsigned int setindex= address&mask; //gets setindex
        mask = ((1 << blockOff) - 1) << (24-tagBits-index);
        unsigned int blockoffset=address&mask; //gets block offset
        long long answer;
        int exists=0;
        int full=1;
        for(int i=0; i<wayNum; i++)
        {
          if(cache[i][setindex].tag==tag)
          {
            exists=1;
            strcpy(result, "hit");
            answer=*(memory+((setindex*blockSize)+blockoffset)); //writes to memory the new value
            cache[i][setindex].counter=count; //increments this to be new
            count++;
          }

          if(cache[i][setindex].valid==0)
          {
            full=0; //finds if the set is full or not
          }

        }
        if(exists==0)// means there's a miss
        {
          strcpy(result, "miss");
          answer= *(memory+((setindex*blockSize)+blockoffset));//reads value from memory

          if(full==0)//case there's still space
          {
            for(int i=0; i<wayNum; i++)
            {
              if(cache[i][setindex].valid==0)//finds first available space
              {
                struct cacheEntry new;
                new.tag=tag; new.valid=1; new.counter=count; new.value=answer;
                cache[i][setindex]=new;
                count++;
                break;
          }

        }
      }
      else //kick out the last used one and sub this in
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
      struct cacheEntry new;
      new.tag=tag; new.valid=1; new.counter=count; new.value=answer;
      cache[lruIndex][setindex]=new;
      count++;
    }


    }
    printf("%s %d %s %llx \n", word, address, result, answer);
  }
}
free(memory); fclose(file);
return 0;
}
