
//For N=256 use ulong4, if N=1024 use ulong16.


#define HASH_TABLE_SIZE 25033
#define HASH_TABLE_SIZE2 25032
#define HASH_TABLE_SIZE3 25031
#define HASH_TABLE_SIZE4 25030
#define HASH_TABLE_SIZE5 25029
#define HASH_TABLE_SIZE6 25028
#define HASH_TABLE_SIZE7 25027

/*
#define HASH_TABLE_SIZE 12043
#define HASH_TABLE_SIZE2 12042
#define HASH_TABLE_SIZE3 12041
#define HASH_TABLE_SIZE4 12040
#define HASH_TABLE_SIZE5 12039
#define HASH_TABLE_SIZE6 12038
#define HASH_TABLE_SIZE7 12037
*/
// HASH_TABLE_SIZE must be prime.  The higher it is, the more bandwidth, but less collisions.  It should also be "far" from a power of 2. 

#define ACCESS_RADIUS_THRESHOLD 104



__kernel void get_active_hard_locations(__global ulong4 *HL_address, __global ulong4 *bitstring, __global uint *distances, __global uint *bin_active_index)
{
  __private uint mem_pos;
  __private ulong4 Aux;
  __private uint bin_pos;
        
  mem_pos = get_global_id(0);

  Aux = HL_address[mem_pos] ^ bitstring[0];
  Aux = popcount(Aux);
  distances [mem_pos] = (uint) (Aux.s0+Aux.s1+Aux.s2+Aux.s3);

  if (distances[mem_pos]<ACCESS_RADIUS_THRESHOLD)   //104 is the one: 128-24: mu-3sigma. With seed = 123456789 (see python code), we get 1153 Active Hard Locations (re-check this)
  {                                                 
    bin_pos = (mem_pos % HASH_TABLE_SIZE);          // Hashing 7 times, see (cormen et al) "introduction to algorihms" section 12.4, on "open addressing". Performance doesn't degrade in the macbook pro.
    if (bin_active_index[bin_pos]>0) {              // 7 reaches diminishing returns; in parallel the system can read 0s simultaneously, and may have a collision..
      bin_pos= (bin_pos + 1 + (mem_pos % HASH_TABLE_SIZE2)) % HASH_TABLE_SIZE;
      if (bin_active_index[bin_pos]>0) {
        bin_pos= (bin_pos + 1 + (mem_pos % HASH_TABLE_SIZE3)) % HASH_TABLE_SIZE;
        if (bin_active_index[bin_pos]>0) {
          bin_pos= (bin_pos + 1 + (mem_pos % HASH_TABLE_SIZE4)) % HASH_TABLE_SIZE; 
          if (bin_active_index[bin_pos]>0) {
          bin_pos= (bin_pos + 1 + (mem_pos % HASH_TABLE_SIZE5)) % HASH_TABLE_SIZE; 
          if (bin_active_index[bin_pos]>0) {
            bin_pos= (bin_pos + 1 + (mem_pos % HASH_TABLE_SIZE6)) % HASH_TABLE_SIZE; 
            if (bin_active_index[bin_pos]>0) {
              bin_pos= (bin_pos + 1 + (mem_pos % HASH_TABLE_SIZE7)) % HASH_TABLE_SIZE; 
              }
            }
          }
        }
      }  
    }
    bin_active_index[bin_pos]=mem_pos;
  }
}

__kernel void clear_bin_active_indexes_gpu(__global uint *bin_active_index_gpu)
{
  __private uint gid;
  gid = get_global_id(0);
  bin_active_index_gpu[gid]=0;
}


__kernel void compute_hammings_hard_locations_256bits(__global ulong4 *HL_address, __global ulong4 *bitstring, __global uint *distances)
{
  __private uint mem_pos;
  __private ulong4 Aux;
        
  mem_pos = get_global_id(0);
  
  Aux = HL_address[mem_pos] ^ bitstring[0];
  Aux = popcount(Aux);
  distances [mem_pos] = (uint) (Aux.s0+Aux.s1+Aux.s2+Aux.s3);
}

/* Implement full read & write to Hard_locations
 try pragma unroll here

inline int bs_bit(bitstring* a, int bit) {
  int i = bit/64, j = bit%64;
  return (a[bs_len-1-i]&((uint64_t)1<<j) ? 1 : 0);
}

int bs_bitsign(bitstring* a, int bit) {
  return (bs_bit(a, bit) ? 1 : -1);
}

void hl_write(hardlocation* hl, bitstring* data) {
  int i, a;
  for(i=0; i<bs_dimension; i++) {
    a = bs_bitsign(data, i);
    if (a > 0) {
      if (hl->adder[i] < 127) hl->adder[i]++;
      else printf("@@ WARNING adder[%d] overflow!\n", i);
    } else if (a < 0) {
      if (hl->adder[i] > -127) hl->adder[i]--;
      else printf("@@ WARNING adder[%d] overflow!\n", i);
    }
  }
}

bitstring* hl_read(hardlocation* hl) {
  return bs_init_adder(bs_alloc(), hl->adder);
}

unsigned int sdm_write(bitstring* address, bitstring* data) {
  unsigned int i, counter = 0;
  unsigned int dist;
  for(i=0; i<sdm_sample; i++) {
    dist = bs_distance(sdm_memory[i]->address, address);
    if (dist <= sdm_radius) {
      hl_write(sdm_memory[i], data);
      counter++;
    }
  }
  //printf("Hardlocations inside radius %d = %d\n", sdm_radius, counter);
  return counter;
}

bitstring* sdm_read(bitstring* address) {
  unsigned int i, j, counter = 0;
  unsigned int dist;
  int32_t adder[bs_dimension];
  adder_t adder2[bs_dimension];
  memset(adder, 0, sizeof(adder));
  for(i=0; i<sdm_sample; i++) {
    dist = bs_distance(sdm_memory[i]->address, address);
    if (dist <= sdm_radius) {
      for(j=0; j<bs_dimension; j++) {
        adder[j] += sdm_memory[i]->adder[j];
      }
      counter++;
    }
  }
  // we can't add all adders in an adder_t type because
  // it will probably overflow.
  for(i=0; i<bs_dimension; i++) {
    if (adder[i] > 0) adder2[i] = 1;
    else if (adder[i] < 0) adder2[i] = -1;
    else adder2[i] = (rand()%2 == 0 ? 1 : -1);
  }
  //printf("Hardlocation inside radius %d = %d\n", sdm_radius, counter);
  return bs_init_adder(bs_alloc(), adder2);
}

*/ 

