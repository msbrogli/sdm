
//For N=256 use ulong4, if N=1024 use ulong16.



#define HASH_TABLE_SIZE 25033
#define HASH_TABLE_SIZE2 25032
#define HASH_TABLE_SIZE3 25031
#define HASH_TABLE_SIZE4 25030
#define HASH_TABLE_SIZE5 25029
#define HASH_TABLE_SIZE6 25028
#define HASH_TABLE_SIZE7 25027


/*


#define HASH_TABLE_SIZE 48781
#define HASH_TABLE_SIZE2 48780
#define HASH_TABLE_SIZE3 48779
#define HASH_TABLE_SIZE4 48778
#define HASH_TABLE_SIZE5 48777
#define HASH_TABLE_SIZE6 48776
#define HASH_TABLE_SIZE7 48775
*/


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
    if (bin_active_index[bin_pos]>0) {              // 7 reaches diminishing returns; in parallel the system can read 0s simultaneously, and have a collision..
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
