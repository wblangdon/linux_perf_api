//WBL 30 Dec 2022 C interface for test_olc.py to time main() in olc_all.c

/*compile: gcc demo_perf.c */

/*Modifications:
WBL 21 Jan 2025 Make magpie/q1_diff/demo_perf.c check errno
WBL 16 Jan 2023 based on test_olc.c r1.2
WBL 30 Dec 2022 rewrite to use perf_event_open's PERF_COUNT_HW_INSTRUCTIONS
*/

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

//https://classes.engineering.wustl.edu/cse522/man-pages/perf_event_open.2.pdf
//The following is a short example that measures the total instruction count of a call to printf(3).

//16 Jan 2023
//https://stackoverflow.com/questions/14674463/why-doesnt-perf-report-cache-misses

#include <unistd.h>
#include <string.h>
#include <sys/ioctl.h>
#include <linux/perf_event.h>
#include <asm/unistd.h>

int run_me(){
  int i;
  int j = 0;
  for(i = 0;i<10000;i++){j++;}
  return (j==10000)? 0 : 1;
}

static long
perf_event_open(struct perf_event_attr *hw_event, pid_t pid,
		int cpu, int group_fd, unsigned long flags){
  int ret;
  ret = syscall(__NR_perf_event_open, hw_event, pid, cpu,group_fd, flags);
  return ret;
}

int main(int argc, char* argv[]) {
  printf("Linux perf can measure many things. \n");
  printf("Here we ask it how instruction run_me() consumed. \n");
  printf("Notice this will depend on how run_me() is compiled. \n");
  printf("For example try without -O3 and then with -O3 \n");
  printf("\n");

  struct perf_event_attr pe;
  long long count = -1;
  int fd;

  memset(&pe, 0, sizeof(pe));
  //set up perf's event data structure.
  //There are many things perf can report, here ask for instruction count
  //Note your CPU may not support all perf's options
  pe.type = PERF_TYPE_HARDWARE;	//PERF_TYPE_HW_CACHE; 
  pe.size = sizeof(pe);
  //PERF_COUNT_HW_CACHE_MAX
  //PERF_COUNT_HW_CACHE_OP_MAX
  //PERF_COUNT_HW_CACHE_RESULT_MAX
  //? not yet ? PERF_COUNT_SW_MAX
  pe.config = PERF_COUNT_HW_INSTRUCTIONS; //PERF_COUNT_HW_CACHE_MISSES;
  pe.disabled = 1;
  pe.exclude_kernel = 1;
  pe.exclude_hv = 1;

  fd = perf_event_open(&pe, 0, -1, -1, 0);
  if (fd == -1) {
    fprintf(stderr, "Error on perf_event_open %llx\n", pe.config);
    return EXIT_FAILURE;
  }

  ioctl(fd, PERF_EVENT_IOC_RESET, 0);
  ioctl(fd, PERF_EVENT_IOC_ENABLE, 0);
  const int status = run_me();
  ioctl(fd, PERF_EVENT_IOC_DISABLE, 0);
  if(errno != 0) {//check all is well 
    //Might get for example errno == EBADF 9 /* Bad file number */ bad fd ?
    printf("Opps errno is %d\n",errno);
    return errno;
  }
  read(fd, &count, sizeof(count)); //instructions is first 8 bytes read from fd
  close(fd);

  if(status) printf("run_me() failed! %lld instructions status %d\n",
		    count,status);
  else       printf("run_me() took %lld instructions\n",count);
  return status;
}

