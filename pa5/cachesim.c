#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Cache
{
    int valid;
    long int tag;
    long int age;
} Cache;

Cache **cache;
int reads = 0;
int writes = 0;
int hits = 0;
int misses = 0;

long int logTwo(long int x) //returns log2 of number
{
    long int result = 0;
    while (x >>= 1) 
        result++; 
    return result;
}

int isPowerofTwo(int x) //checks if number is power of 2
{
    return x != 0 && (x & (x - 1)) == 0;
}

void incrementAge(long int set_index, int loc, int cache_lines_per_set) //increments age of all other tags in set except current one
{
    for(int i = 0; i < cache_lines_per_set; i++)
        if(i != loc)
            cache[set_index][i].age++;
}

void prefetch(int cache_lines_per_set, long int prefetch_set_index, long int prefetch_tag, int age_policy) //prefetch algorithm
{
    for (int i = 0; i < cache_lines_per_set; i++)
    {
        if (cache[prefetch_set_index][i].valid == 1 && cache[prefetch_set_index][i].tag == prefetch_tag) //valid and prefetch tag already there
            break;
        
        else if (cache[prefetch_set_index][i].valid == 1 && cache_lines_per_set == i + 1) //valid but set full
        {
            reads++;
            int oldest_access = -1;
            for (int j = 0; j < cache_lines_per_set; j++)
                if (oldest_access <= cache[prefetch_set_index][j].age)
                    oldest_access = cache[prefetch_set_index][j].age;
            
            for (int j = 0; j < cache_lines_per_set; j++)
            {
                if(oldest_access == cache[prefetch_set_index][j].age)
                {
                    cache[prefetch_set_index][j].tag = prefetch_tag;
                    cache[prefetch_set_index][j].age = 0;
                    incrementAge(prefetch_set_index, j, cache_lines_per_set);
                    break;
                }
            }
            return;
        }

        else if (cache[prefetch_set_index][i].valid == 0) //invalid and must initialize
        {
            reads++;
            cache[prefetch_set_index][i].valid = 1;
            cache[prefetch_set_index][i].tag = prefetch_tag;
            cache[prefetch_set_index][i].age = 0;
            incrementAge(prefetch_set_index, i, cache_lines_per_set);
            break;
        }
    }
}

void readWriteHitMiss(char read_or_write, long int cache_lines_per_set, long int set_index, long int tag, int do_prefetch, long int prefetch_set_index, long int prefetch_tag, int replacement_policy)
{
    for (int i = 0; i < cache_lines_per_set; i++)
    {
        if (cache[set_index][i].valid == 1 && cache[set_index][i].tag == tag) //valid and found tag
        {
            hits++;
            if (read_or_write == 'W')
                writes++;

            if(replacement_policy == 0) //lru
            {
                incrementAge(set_index, i, cache_lines_per_set);
                cache[set_index][i].age = 0;
            }
            
            break;
        }

        else if (cache[set_index][i].valid == 1 && cache_lines_per_set == i + 1)
        {
            if (read_or_write == 'W')
                writes++;
            misses++;
            reads++;
            
            int oldest_access = -1;
            for (int j = 0; j < cache_lines_per_set; j++) //finds oldest tag in set
                if (oldest_access <= cache[set_index][j].age)
                    oldest_access = cache[set_index][j].age;
            
            for (int j = 0; j < cache_lines_per_set; j++) //replaces oldest tag in set
            {
                if(oldest_access == cache[set_index][j].age)
                {
                    cache[set_index][j].tag = tag;
                    cache[set_index][j].age = 0;
                    incrementAge(set_index, j, cache_lines_per_set);
                    break;
                }
            }

            if(do_prefetch == 1)
                prefetch(cache_lines_per_set, prefetch_set_index, prefetch_tag, replacement_policy);
        }

        else if (cache[set_index][i].valid == 0) //invalid location, must initialize
        {
            if (read_or_write == 'W')
                writes++;
            misses++;
            reads++;
            cache[set_index][i].valid = 1;
            cache[set_index][i].tag = tag;
            cache[set_index][i].age = 0;
            incrementAge(set_index, i, cache_lines_per_set);

            if(do_prefetch == 1)
                prefetch(cache_lines_per_set, prefetch_set_index, prefetch_tag, replacement_policy);

            break;
        }
    }
}

int main(int argc, char **argv)
{
    int cache_size = atoi(argv[1]); //C
    char *associativity = argv[2];
    char *replacement_policy = argv[3];
    int block_size = atoi(argv[4]); //B
    FILE *trace_file = fopen(argv[5], "r");

    int num_of_sets; //S
    int cache_lines_per_set; //A

    if(!isPowerofTwo(block_size) || !isPowerofTwo(cache_size)) //confirms if block size and cache size is power of 2
        return 1;

    if (strcmp(associativity, "direct") == 0) //direct
    {
        cache_lines_per_set = 1;
        num_of_sets = cache_size / (block_size * cache_lines_per_set);
    }

    else if (associativity[5] == ':') //n-way associative
    {
        cache_lines_per_set = associativity[6] - '0';

        if(!isPowerofTwo(cache_lines_per_set))
            return 1;

        num_of_sets = cache_size / (block_size * cache_lines_per_set);
    }

    else if (associativity[5] != ':') //fully associative
    {
        num_of_sets = 1;
        cache_lines_per_set = cache_size / (num_of_sets * block_size);
    }

    cache = (struct Cache **)malloc(num_of_sets * sizeof(struct Cache)); //initialize cache
    for (int i = 0; i < num_of_sets; i++)
    {
        cache[i] = (struct Cache *) malloc(cache_lines_per_set * sizeof(struct Cache));
        for (int j = 0; j < cache_lines_per_set; j++)
        {
            cache[i][j].valid = 0;
            cache[i][j].tag = 0;
            cache[i][j].age = 0;
        }
    }

    int block_offset = logTwo(block_size);
    int set_loc = logTwo(num_of_sets);
    
    long int program_counter; //not used
    char read_or_write;
    long int memory_address;

    long int set_index;
    long int tag;

    int r_p = strcmp("fifo", replacement_policy) ? 0 : 1; //check replacement policy
    
    while(fscanf(trace_file, "%lx: %c %lx", &program_counter, &read_or_write, &memory_address) == 3) //while number of arguements equals 3
    {
        tag = memory_address >> (block_offset + set_loc); //get tag
        set_index = (memory_address >> block_offset) & ((1 << set_loc) - 1); //get set index
        readWriteHitMiss(read_or_write, cache_lines_per_set, set_index, tag, 0, 0, 0, r_p); //put into cache
    }

    printf("Prefetch 0\n");
    printf("Memory reads: %d\n", reads);
    printf("Memory writes: %d\n", writes);
    printf("Cache hits: %d\n", hits);
    printf("Cache misses: %d\n", misses);

    rewind(trace_file); //return to start of trace file

    for (int i = 0; i < num_of_sets; i++) //reset all values of cache for rerun with prefetching
    {
        for (int j = 0; j < cache_lines_per_set; j++)
        {
            cache[i][j].valid = 0;
            cache[i][j].tag = 0;
            cache[i][j].age = 0;
        }
    }
    reads = 0;
    writes = 0;
    hits = 0;
    misses = 0;

    long int prefetch_tag; //prefetch identifiers
    long int prefetch_set_index;
    long int prefetch_memory_address;

    while (fscanf(trace_file, "%lx: %c %lx", &program_counter, &read_or_write, &memory_address) == 3)
    {
        tag = memory_address >> (block_offset + set_loc);
        set_index = (memory_address >> block_offset) & ((1 << set_loc) - 1);

        prefetch_memory_address = memory_address + block_size;
        prefetch_tag = prefetch_memory_address >> (block_offset + set_loc);
        prefetch_set_index = (prefetch_memory_address >> block_offset) & ((1 << set_loc) - 1);
        
        readWriteHitMiss(read_or_write, cache_lines_per_set, set_index, tag, 1, prefetch_set_index, prefetch_tag, r_p);
    }

    fclose(trace_file);

    printf("Prefetch 1\n");
    printf("Memory reads: %d\n", reads);
    printf("Memory writes: %d\n", writes);
    printf("Cache hits: %d\n", hits);
    printf("Cache misses: %d\n", misses);

    for (int i = 0; i < num_of_sets; i++)
        free(cache[i]);
    free(cache);

    return 0;
}
