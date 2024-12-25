
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include "cache.h"
using namespace std;

Cache::Cache(int s,int a,int b )
{
   
   ulong i, j;

 lineSize = (ulong)(b);
        sets = 1;               // Only one set for fully associative
        assoc = 10000;        // Large associativity to simulate "infinite" cache
        numLines = assoc;       // The number of lines is equal to associativity
  
   
 
cache = new cacheLine*[sets];
    for (i = 0; i < sets; i++) {
        cache[i] = new cacheLine[assoc];
        for (j = 0; j < assoc; j++) {
            cache[i][j].invalidate();
        }
    }    
   
}

void Cache::MESI_Processor_Access(ulong addr,uchar rw, int copy , Cache **cache, int processor, int num_processors )
{
    cacheLine *line = cache[processor]->findLine(addr);
    if(!line)
    {
        line = cache[processor]->fillLine(addr);
        line->setFlags(0);
    }
    ulong state = line->getFlags();
    
    switch(rw)
    {
        case 'r':
            cache[processor]->reads++;
            if(state<1)
            {
                cache[processor]->MESI_Bus_Snoop(addr, 1,0,0, cache, processor);
                if(copy)
                {
                    cache[processor]->c_to_c_trans++;
                    Total_execution_time += (flush_transfer);
                    line->setFlags(1);
                }
                else
                {
                    Total_execution_time += (memory_latency);
                    cache[processor]->mem_trans++;
                    line->setFlags(3);
                }
                cache[processor]->readMisses++;
            }
            else
            {
                Total_execution_time += read_hit_latency;
                cache[processor]->Readhits++;
            }
            break;

        case 'w':
            cache[processor]->writes++;
            switch (state)
            {
                case 0:
                {
                    cache[processor]->MESI_Bus_Snoop(addr,0,1,0, cache, processor);
                    cache[processor]->busrdxes++;
                    if(copy)
                    {
                        cache[processor]->c_to_c_trans++;
                        Total_execution_time += (flush_transfer);
                    }
                    else
                    {
                        Total_execution_time += (memory_latency);
                        cache[processor]->mem_trans++;
                    }
                    line->setFlags(2);
                    cache[processor]->writeMisses++;
                    break;
                }

                case 1:
                {
                    cache[processor]->MESI_Bus_Snoop(addr,0,0,1, cache, processor);
                    line->setFlags(2);
                    Total_execution_time += write_hit_latency;
                    cache[processor]->Writehits++;
                    break;
                }

                case 2:
                {
                    Total_execution_time += write_hit_latency;
                    cache[processor]->Writehits++;
                    break;
                }

                case 3:
                {
                    Total_execution_time += write_hit_latency;
                    line->setFlags(2);
                    cache[processor]->Writehits++;
                    break;
                }

                default:
                    std::cout<<"Incorrect state of cache! Abort process!\n";
                    break;
            }
            break;

        default:
            std::cout<<"Error reading access bit!\n";
            break;
    }
}

void Cache::MESI_Bus_Snoop(ulong addr , int busread, int busreadx, int busupgrade, Cache **cache, int processor)
{
    for (int i = 0; i < (int)num_processors; i++)
    {
        if(i != processor)
        {
            cacheLine* line = cache[i]->findLine(addr);
            if (line) {
                ulong state = line->getFlags();
                switch (state){
                    case 1:
                    {
                        if(busread)
                            cache[i]->flushes++;
                        else if(busreadx)
                        {
                            line->invalidate();
                            cache[i]->flushes++;
                            cache[i]->invalidations++;
                        }
                        else if(busupgrade)
                        {
                            line->invalidate();
                            cache[i]->invalidations++;
                        }
                        break;
                    }

                    case 2:
                    {
                        if(busread)
                        {
                            line->setFlags(1);
                            cache[i]->flushes++;
                        }
                        else if(busreadx)
                        {
                            cache[i]->flushes++;
                            line->invalidate();
                            cache[i]->invalidations++;
                        }
                        break;
                    }

                    case 3:
                    {
                        if(busread)
                        {
                            line->setFlags(1);
                            cache[i]->flushes++;
                        }
                        else if(busreadx)
                        {
                            line->invalidate();
                            cache[i]->flushes++;
                            cache[i]->invalidations++;
                        }   
                        break;
                    }

                    default:
                        break;
                }
            }
            else
            {
                continue;
            }

        }
    }
}

void Cache::MOESI_Processor_Access(ulong addr,uchar rw, int copy, Cache **cache, int processor, int num_processors )
{
	cacheLine *line = cache[processor]->findLine(addr);
    if(!line)
    {
        line = cache[processor]->fillLine(addr);
        line->setFlags(0);
    }
    ulong state = line->getFlags();
    
    switch(rw)
    {
        case 'r':
            cache[processor]->reads++;
            if(state<1)
            {
                cache[processor]->MOESI_Bus_Snoop(addr, 1,0,0, cache, processor);
                if(copy)
                {
                    cache[processor]->c_to_c_trans++;
                    Total_execution_time += (flush_transfer);
                    line->setFlags(1);
                }
                else
                {
                    Total_execution_time += (memory_latency);
                    cache[processor]->mem_trans++;
                    line->setFlags(3);
                }
                cache[processor]->readMisses++;
            }
            else
            {
                Total_execution_time += read_hit_latency;
                cache[processor]->Readhits++;
            }
            break;

        case 'w':
            cache[processor]->writes++;
            switch (state)
            {
                case 0:
                {
                    cache[processor]->MOESI_Bus_Snoop(addr,0,1,0, cache, processor);
                    cache[processor]->busrdxes++;
                    if(copy)
                    {
                        cache[processor]->c_to_c_trans++;
                        Total_execution_time += (flush_transfer);
                    }
                    else
                    {
                        Total_execution_time += (memory_latency);
                        cache[processor]->mem_trans++;
                    }
                    line->setFlags(2);
                    cache[processor]->writeMisses++;
                    break;
                }

                case 1:
                {
                    cache[processor]->MOESI_Bus_Snoop(addr,0,0,1, cache, processor);
                    line->setFlags(2);
                    Total_execution_time += write_hit_latency;
                    cache[processor]->Writehits++;
                    break;
                }

                case 2:
                {
                    Total_execution_time += write_hit_latency;
                    cache[processor]->Writehits++;
                    break;
                }

                case 3:
                {
                    Total_execution_time += write_hit_latency;
                    line->setFlags(2);
                    cache[processor]->Writehits++;
                    break;
                }

                case 4:
                {
                    cache[processor]->MOESI_Bus_Snoop(addr, 0,0,1,cache,processor);
                    Total_execution_time += write_hit_latency;
                    line->setFlags(2);
                    cache[processor]->Writehits++;
                    break;
                }

                default:
                    std::cout<<"Incorrect state of cache! Abort process!\n";
                    break;
            }
            break;

        default:
            std::cout<<"Error reading access bit!\n";
            break;
    }

}

void Cache::MOESI_Bus_Snoop(ulong addr , int busread,int busreadx, int busupgrade, Cache **cache, int processor)
{
	for (int i = 0; i < (int)num_processors; i++)
    {
        if(i != processor)
        {
            cacheLine* line = cache[i]->findLine(addr);
            if (line) {
                ulong state = line->getFlags();
                switch (state){
                    case 1:
                    {
                        if(busreadx)
                        {
                            line->invalidate();
                            cache[i]->invalidations++;
                        }
                        else if(busupgrade)
                        {
                            line->invalidate();
                            cache[i]->invalidations++;
                        }
                        break;
                    }

                    case 2:
                    {
                        if(busread)
                        {
                            line->setFlags(4);
                            cache[i]->flushes++;
                        }
                        else if(busreadx || busupgrade)
                        {
                            cache[i]->flushes++;
                            line->invalidate();
                            cache[i]->invalidations++;
                        }
                        break;
                    }

                    case 3:
                    {
                        if(busread)
                        {
                            line->setFlags(1);
                        }
                        else if(busreadx || busupgrade)
                        {
                            line->invalidate();
                            cache[i]->invalidations++;
                        }   
                        break;
                    }

                    case 4:
                    {
                        if(busread)
                        {
                            cache[i]->flushes++;
                        }
                        else if(busreadx)
                        {
                            cache[i]->flushes++;
                            line->invalidate();
                            cache[i]->invalidations++;
                        }
                        else if(busupgrade)
                        {
                            line->invalidate();
                            cache[i]->invalidations++;
                        }
                    }
                    default:
                        break;
                }
            }
            else
            {
                continue;
            }

        }
    }
}

/*look up line*/
cacheLine * Cache::findLine(ulong addr)
{
   ulong i, j, tag, pos;
   
   pos = assoc;
   tag = calcTag(addr);
   i   = calcIndex(addr); //will always return 0
  
   for(j=0; j<assoc; j++)
	if(cache[i][j].isValid())
	        if(cache[i][j].getTag() == tag)
		{
		     pos = j; break; 
		}
   if(pos == assoc)
	return NULL;
   else
	return &(cache[i][pos]); 
}


/*Most of these functions are redundant so you can use/change it if you want to*/

/*upgrade LRU line to be MRU line*/
void Cache::updateLRU(cacheLine *line)
{
  line->setSeq(currentCycle);  
}

/*return an invalid line as LRU, if any, otherwise return LRU line*/
cacheLine * Cache::getLRU(ulong addr)
{
   ulong i, j, victim, min;

   victim = assoc;
   min    = currentCycle;
   i      = calcIndex(addr);
   
   for(j=0;j<assoc;j++)
   {
      if(cache[i][j].isValid() == 0) 
	  return &(cache[i][j]);     
   }   
   for(j=0;j<assoc;j++)
   {
	 if(cache[i][j].getSeq() <= min) { victim = j; min = cache[i][j].getSeq();}
   } 
   assert(victim != assoc);
   std::cout << "victim" << victim << std::endl;
   return &(cache[i][victim]);
}

/*find a victim, move it to MRU position*/
cacheLine *Cache::findLineToReplace(ulong addr)
{
   cacheLine * victim = getLRU(addr);
   updateLRU(victim);
  
   return (victim);
}

/*allocate a new line*/
cacheLine *Cache::fillLine(ulong addr)
{ 
   ulong tag;
  
   cacheLine *victim = findLineToReplace(addr);

   assert(victim != 0);
   if ((victim->getFlags() == Modified))
   {
	   writeBack(addr);
   }
   victim->setFlags(Shared);	
   tag = calcTag(addr);   
   victim->setTag(tag);
   return victim;
}

void Cache::printStats()
{ 
	//printf("===== Simulation results      =====\n");
	float miss_rate = (float)(getRM() + getWM()) * 100 / (getReads() + getWrites());
	
printf("01. number of reads:                                 %10lu\n", getReads());
printf("02. number of read misses:                           %10lu\n", getRM());
printf("03. number of writes:                                %10lu\n", getWrites());
printf("04. number of write misses:                          %10lu\n", getWM());
printf("05. number of write hits:                            %10lu\n", getWH());
printf("06. number of read hits:                             %10lu\n", getRH()); // Changed from getRM() to getRH()
printf("07. total miss rate:                                 %10.2f%%\n", miss_rate);
printf("08. number of memory accesses (exclude writebacks):  %10lu\n", mem_trans);
printf("09. number of invalidations:                         %10lu\n", Invalidations());
printf("10. number of flushes:                               %10lu\n", Flushes());

	
}

void Cache::printCacheState(ulong state) {
    switch (state) {
        case INVALID:
            std::cout << "I";
            break;
        case Shared:
            std::cout << "S";
            break;
        case Modified:
            std::cout << "M";
            break;
        case Exclusive:
            std::cout << "E";
            break;
        default:
            std::cout << "-";
            break;
    }
}
