#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <assert.h>
#include <sys/time.h>

#define NUM_BUCKETS 5     // Buckets in hash table
#define NUM_KEYS 100000   // Number of keys inserted per thread
int num_threads = 1;      // Number of threads (configurable)
int keys[NUM_KEYS];       // An integer array of keys
pthread_mutex_t lock[NUM_BUCKETS];	// Create a lock for each bucket

typedef struct _bucket_entry {      // An entry has three fields: key, val and next
  int key;
  int val;
  struct _bucket_entry *next;       // next is a pointer to the next entry in the same bucket
                                    // this creates a linked list of bucket entries
} bucket_entry;

bucket_entry *table[NUM_BUCKETS];   // Create an array of buket_entry pointers

void panic(char *msg) {
  printf("%s\n", msg);
  exit(1);
}

double now() {
  struct timeval tv;
  gettimeofday(&tv, 0);
  return tv.tv_sec + tv.tv_usec / 1000000.0;
}

// Inserts a key-value pair into the table
void insert(int key, int val) {
  int i = key % NUM_BUCKETS;         // Uses mod division to hash key into one of the buckets
  bucket_entry *e = (bucket_entry *) malloc(sizeof(bucket_entry));
  if (!e) panic("No memory to allocate bucket!");
  
  pthread_mutex_lock(&lock[i]);     // Start critical section - lock
  e->next = table[i];
  e->key = key;
  e->val = val;
  table[i] = e;
	pthread_mutex_unlock(&lock[i]);   // End critical section - release lock
}

// Retrieves an entry from the hash table by key
// Returns NULL if the key isn't found in the table
bucket_entry * retrieve(int key) {
  bucket_entry *b;

  for (b = table[key % NUM_BUCKETS]; b != NULL; b = b->next) { // Loop through the linked list
    if (b->key == key) {                                       // Looking for the key
                                                               // If the key is found
      return b;                                                // Return the entry
    }
  }
  return NULL;                                                 // If key is not found return null
}

void * put_phase(void *arg) {
  long tid = (long) arg;                      // arg is the thread index
  int key = 0;

  // If there are k threads, thread with thread index (tid) = i inserts
  //      (i, i), (i+k, i), (i+k*2, i), ...
  for (key = tid ; key < NUM_KEYS; key += num_threads) {
    insert(keys[key], tid);
  }
  pthread_exit(NULL);
}

void * get_phase(void *arg) {
  long tid = (long) arg;                      // arg is the thread index
  int key = 0;
  long lost = 0;

  for (key = tid ; key < NUM_KEYS; key += num_threads) {
    if (retrieve(keys[key]) == NULL) lost++;  // if retrieve returns null increment lost
  }

  printf("[thread %ld] %ld keys lost!\n", tid, lost);
  pthread_exit((void *)lost);
}

int main(int argc, char **argv) {
  long i;
  pthread_t *threads;
  double start, end;

	for(i=0; i<NUM_BUCKETS; i++) {            // For each bucket we have a mutex lock for mutual exclusion
    pthread_mutex_init(&lock[i], NULL);     // Here we initialize each lock
	}

  if (argc != 2) {                          // Check command for correctness
    panic("usage: ./parallel_hashtable <num_threads>");
  }
  if ((num_threads = atoi(argv[1])) <= 0) {	// Check command for correctness
    panic("must enter a valid number of threads to run");
  }

  srandom(time(NULL));
  for (i = 0; i < NUM_KEYS; i++)            // Create keys
    keys[i] = random();

	// Allocates a portion of memory for the threads we will use
  threads = (pthread_t *) malloc(sizeof(pthread_t)*num_threads);
  if (!threads) {
    panic("out of memory allocating thread handles");
  }

  // Insert keys in parallel
  start = now();
  for (i = 0; i < num_threads; i++) {       // Create specified number of threads
                                            // that work on the put_phase function
                                            // and take the argument i
                                            // i is the index of the thread
    pthread_create(&threads[i], NULL, put_phase, (void *)i);
  }
    
  // Barrier - don't continue until all threads are finished with their work
  for (i = 0; i < num_threads; i++) {
    pthread_join(threads[i], NULL);
  }
  end = now();
    
  printf("[main] Inserted %d keys in %f seconds\n", NUM_KEYS, end - start);
    
  // Reset the thread array
  memset(threads, 0, sizeof(pthread_t)*num_threads);

  // Retrieve keys in parallel
  start = now();
  for (i = 0; i < num_threads; i++) {     // Create a thread per bucket
                                          // that works on the get_phase function
                                          // and take the argument i
                                          // i is the index of the thread
    pthread_create(&threads[i], NULL, get_phase, (void *)i);
  }

  // Collect count of lost keys
  long total_lost = 0;
  long *lost_keys = (long *) malloc(sizeof(long) * num_threads);
  for (i = 0; i < num_threads; i++) {
    pthread_join(threads[i], (void **)&lost_keys[i]);
    total_lost += lost_keys[i];
  }
  end = now();

  printf("[main] Retrieved %ld/%d keys in %f seconds\n", NUM_KEYS - total_lost, NUM_KEYS, end - start);

  return 0;
}
