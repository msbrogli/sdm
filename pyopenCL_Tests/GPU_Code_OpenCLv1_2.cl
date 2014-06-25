
//For N=256 use ulong4, if N=1024 use ulong16.

#define BIN_SIZE 25033
#define BIN_SIZE2 25032
#define BIN_SIZE3 25031
#define BIN_SIZE4 25030
#define BIN_SIZE5 25029

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

  if (distances[mem_pos]<ACCESS_RADIUS_THRESHOLD)    //104 is the one: 128-24: mu-3sigma. With seed = 123456789 (see python code), we get 1153 Active Hard Locations (re-check this)
  {
    bin_pos = (mem_pos % BIN_SIZE); 
    if (bin_active_index[bin_pos]>0) {
      bin_pos= (bin_pos + 1 + (mem_pos % BIN_SIZE2)) % BIN_SIZE;
      if (bin_active_index[bin_pos]>0) {
        bin_pos= (bin_pos + 1 + (mem_pos % BIN_SIZE3)) % BIN_SIZE;
        if (bin_active_index[bin_pos]>0) {
          bin_pos= (bin_pos + 1 + (mem_pos % BIN_SIZE4)) % BIN_SIZE; 
          if (bin_active_index[bin_pos]>0) {
          bin_pos= (bin_pos + 1 + (mem_pos % BIN_SIZE5)) % BIN_SIZE; 
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
