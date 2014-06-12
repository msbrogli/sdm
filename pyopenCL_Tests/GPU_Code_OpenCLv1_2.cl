
//For N=256 use ulong4, if N=1024 use ulong16.

#pragma OPENCL EXTENSION cl_khr_global_int32_base_atomics : enable
#pragma OPENCL EXTENSION cl_khr_local_int32_base_atomics : enable
#pragma OPENCL EXTENSION cl_khr_int64_base_atomics : enable

#define MaxActiveHLs 2000

/*
void GetSemaphor(__global int *semaphor);
void ReleaseSemaphor(__global int *semaphor);


void GetSemaphor(__global int *semaphor) {
  int occupied = atom_xchg(semaphor, 1);
  while(occupied > 0) {
      occupied = atom_xchg(semaphor, 1);   }
}
 
void ReleaseSemaphor(__global int *semaphor)   {
    int prevVal = atom_xchg(semaphor, 0);
}
*/


/*
__kernel void get_active_hard_locations_256bits_in_ulong4(__global ulong4 *HL_address,
__global ulong4 *bitstring, volatile __global uint *activeHLs, __global uint *hamming_distances, __global uint *semaphor)
{
  __private uint hamming = 0; 
  __private uint mem_pos;
  __private ulong4 Aux;
        
  mem_pos = get_global_id(0);
  
  Aux = HL_address[mem_pos] ^ bitstring[0];

  Aux = popcount(Aux);
  hamming = (uint) (Aux.s0+Aux.s1+Aux.s2+Aux.s3);
  //hamming = (uint) dot(Xor, (ulong4)(1)); //--> product might be faster, just try
  
  if (activeHLs[0]<(MaxActiveHLs) & hamming<hamming_distances[0])    //104 is the one: 128-24: mu-3sigma. With seed = 123456789 (see python code), we get 1153 Active Hard Locations (re-check this)
  {
    activeHLs[0] = activeHLs[0]+1; //atomic_inc https://stackoverflow.com/questions/6448145/how-to-get-a-list-or-subset-from-an-opencl-kernel/6453564#6453564
    activeHLs[activeHLs[0]]=mem_pos;
    hamming_distances[activeHLs[0]]=hamming;
  }
}

*/


/*
__kernel void get_active_hard_locations_256bits_in_ulong4(__constant ulong4 *HL_address,
__global ulong4 *bitstring,  __global uint *activeHLs, __global uint *hamming_distances, volatile __global uint *semaphor)
{
  __private uint hamming = 0; 
  __private uint mem_pos;
  __private ulong4 Aux;
        
  mem_pos = get_global_id(0);
  
  Aux = HL_address[mem_pos] ^ bitstring[0];

  Aux = popcount(Aux);
  hamming = (uint) (Aux.s0+Aux.s1+Aux.s2+Aux.s3);
  //hamming = (uint) dot(Xor, (ulong4)(1)); //--> product might be faster, just try
  
  if (activeHLs[0]<(MaxActiveHLs) & hamming<hamming_distances[0])    //104 is the one: 128-24: mu-3sigma. With seed = 123456789 (see python code), we get 1153 Active Hard Locations (re-check this)
  {

    uint index = atomic_inc(semaphor);  // or atom_inc if using OpenCL 1.0 extension
    //out[index] = value;
    activeHLs[index]=mem_pos;
    
    //activeHLs[0] = activeHLs[0]+1; //atomic_inc https://stackoverflow.com/questions/6448145/how-to-get-a-list-or-subset-from-an-opencl-kernel/6453564#6453564
    //activeHLs[activeHLs[0]]=mem_pos;
    hamming_distances[index]=hamming;
  }
}

*/ 

/*
__kernel void get_active_hard_locations_256bits_in_ulong4(__global ulong4 *HL_address,
__global ulong4 *bitstring, volatile __global uint *activeHLs, __global uint *hamming_distances, __global uint *semaphor)
{
  __private uint hamming = 0; 
  __private uint mem_pos;
  __private ulong4 Aux;
        
  mem_pos = get_global_id(0);
  
  Aux = HL_address[mem_pos] ^ bitstring[0];

  Aux = popcount(Aux);
  hamming = (uint) (Aux.s0+Aux.s1+Aux.s2+Aux.s3);
  //hamming = (uint) dot(Xor, (ulong4)(1)); //--> product might be faster, just try
  
  if (activeHLs[0]<(MaxActiveHLs) & hamming<hamming_distances[0])    //104 is the one: 128-24: mu-3sigma. With seed = 123456789 (see python code), we get 1153 Active Hard Locations (re-check this)
  {

    activeHLs[0] = activeHLs[0]+1; //atomic_inc https://stackoverflow.com/questions/6448145/how-to-get-a-list-or-subset-from-an-opencl-kernel/6453564#6453564
    activeHLs[activeHLs[0]]=mem_pos;
    hamming_distances[activeHLs[0]]=hamming;
  }
}  */






/*

WHEN THE THRESHOLD IS OVER 100, THE SEMAPHOR FUCKS IT UP.
*/

/*
__kernel void get_active_hard_locations_256bits_in_ulong4(__global ulong4 *HL_address,
__global ulong4 *bitstring, __global uint *activeHLs, __global uint *hamming_distances, __global uint *semaphor)
{
  __local uint hamming;  
  __private uint Position_list_HL = activeHLs[0];
  __local uint mem_pos;
  __local ulong4 Aux;
        
  hamming=0;
  mem_pos = get_global_id(0);
  
  Aux = HL_address[mem_pos] ^ bitstring[0];

  Aux = popcount(Aux);
  hamming = (uint) (Aux.s0+Aux.s1+Aux.s2+Aux.s3);
  //hamming = (uint) dot(Xor, (ulong4)(1)); //--> product might be faster, just try
  
  if (activeHLs[0]<(MaxActiveHLs) & hamming<hamming_distances[0])    //104 is the one: 128-24: mu-3sigma. With seed = 123456789 (see python code), we get 1153 Active Hard Locations (re-check this)
  {
    GetSemaphor(&semaphor[0]);  
      {
        activeHLs[0]=Position_list_HL+1;
        activeHLs[Position_list_HL+1]=mem_pos;
        hamming_distances[Position_list_HL+1]=hamming;
      }
    ReleaseSemaphor(&semaphor[0]);
  }
}
*/

 


__kernel void compute_hammings_hard_locations_256bits(__global ulong4 *HL_address,
__global ulong4 *bitstring, __global int *distances)
{
  __private uint mem_pos;
  __private ulong4 Aux;
        
  mem_pos = get_global_id(0);
  
  Aux = HL_address[mem_pos] ^ bitstring[0];
  Aux = popcount(Aux);
  distances [mem_pos] = (uint) (Aux.s0+Aux.s1+Aux.s2+Aux.s3);
}








/*
__kernel void get_active_hard_locations_256bits_in_ulong4(__global ulong4 *HL_address,
__global ulong4 *bitstring, __global uint *activeHLs, __global uint *hamming_distances, __global uint *semaphor)
{
  __local uint hamming;  
  __private uint Position_list_HL = activeHLs[0];
  __private uint mem_pos;
  __local ulong4 Aux;
        
  hamming=0;
  mem_pos = get_global_id(0);
  
  Aux = HL_address[mem_pos] ^ bitstring[0];

  Aux = popcount(Aux);
  hamming = (uint) (Aux.s0+Aux.s1+Aux.s2+Aux.s3);
  //hamming = (uint) dot(Xor, (ulong4)(1)); //--> product might be faster, just try
  
  if (activeHLs[0]<(MaxActiveHLs) & hamming<hamming_distances[0])    //104 is the one: 128-24: mu-3sigma. With seed = 123456789 (see python code), we get 1153 Active Hard Locations (re-check this)
  {   
      activeHLs[0]=Position_list_HL+1;
      activeHLs[Position_list_HL+1]=mem_pos;
      hamming_distances[Position_list_HL+1]=hamming;
  }
}

*/




/*

WHEN THE THRESHOLD IS OVER 100, THE SEMAPHOR FUCKS IT UP.


__kernel void get_active_hard_locations_256bits_in_ulong4(__global ulong4 *HL_address,
__global ulong4 *bitstring, __global uint *activeHLs, __global uint *hamming_distances,  __global uint *semaphor)
{
  __private uint hamming = 0; 
  __private uint mem_pos;
  __private ulong4 Aux;
        
  mem_pos = get_global_id(0);
  
  Aux = HL_address[mem_pos] ^ bitstring[0];

  Aux = popcount(Aux);
  hamming = (uint) (Aux.s0+Aux.s1+Aux.s2+Aux.s3);
  //hamming = (uint) dot(Xor, (ulong4)(1)); //--> product might be faster, just try
  
  if (activeHLs[0]<(MaxActiveHLs) & hamming<hamming_distances[0])     //104 is the one: 128-24: mu-3sigma. With seed = 123456789 (see python code), we get 1153 Active Hard Locations (re-check this)
  {
    GetSemaphor(&semaphor[0]);  
      {
        activeHLs[0]=activeHLs[0]+1;
        activeHLs[activeHLs[0]]=mem_pos;
        hamming_distances[activeHLs[0]]=hamming;
      }
    ReleaseSemaphor(&semaphor[0]);
  }
}

*/



