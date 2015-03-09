/* read_wav.c   read_wav  ok.wav  not working for all .wav files */
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char * argv[])
{
  int i;
  FILE * inp;
  FILE * outp; /* .wav */
  FILE * outd; /* .dat */
  char riff[4];
  int  sread, swrite;
  int  fsize;
  char wave[4];
  char fmt[4];
  int  nbytes;
  short  ccode;
  short  channels;
  int rate;
  int avgrate;
  short blockalign;
  short bps; /* bits per sample */
  char data[4];
  int csize;
  char sbyte; /* byte of sound */
  int  ibyte;
  char more[4];
  int smin;
  int smax;
  int savg;
  int bad; /* flags bad data in read */
  int nbread; /* number of bytes read */
  
  printf("read wav.c running \n");
  if(argc<2)
  {
    printf("no file to open, use read_wav  your.wav \n");
    exit(1);
  }
  inp = fopen(argv[1], "rb");
  if(inp == NULL)
  {
    printf("can not open %s for reading. \n", argv[1]);
    exit(1);
  }
  if(argc<3)
  {
    printf("no output file given, opening junk.wav \n");
    outp = fopen("junk.wav","wb");
    if(outp == NULL)
    {
      printf("can not open junk.wav for writing. \n");
      exit(1);
    }
  }
  else /* open users output file */
  {
    outp = fopen(argv[2], "wb");
    if(outp == NULL)
    {
      printf("can not open %s for writing. \n", argv[2]);
      exit(1);
    }
  }  
  if(argc<4)
  {
    printf("no data output file given, opening junk.dat \n");
    outd = fopen("junk.dat","w");
    if(outd == NULL)
    {
      printf("can not open junk.dat for writing. \n");
      exit(1);
    }
  }
  else /* open users output file */
  {
    outd = fopen(argv[3], "w");
    if(outd == NULL)
    {
      printf("can not open %s for writing. \n", argv[3]);
      exit(1);
    }
  }  

  printf("reading %s \n", argv[1]);

  sread = fread(&riff[0], 1, 4, inp);
  swrite = fwrite(&riff[0], 1, 4, outp);
  printf("first 4 bytes should be RIFF, <%c%c%c%c>\n",
         riff[0],riff[1],riff[2],riff[3]);

  sread = fread(&fsize, 1, 4, inp);
  swrite = fwrite(&fsize, 1, 4, outp);
  printf("file has %d +8 bytes \n", fsize);

  sread = fread(&wave[0], 1, 4, inp);
  swrite = fwrite(&wave[0], 1, 4, outp);
  printf("should be WAVE, <%c%c%c%c>\n",wave[0],wave[1],wave[2],wave[3]);

  sread = fread(&fmt[0], 1, 4, inp);
  swrite = fwrite(&fmt[0], 1, 4, outp);
  printf("should be fmt, <%c%c%c%c>\n",fmt[0],fmt[1],fmt[2],fmt[3]);

  sread = fread(&nbytes, 1, 4, inp);
  swrite = fwrite(&nbytes, 1, 4, outp);
  printf("block has %d bytes \n", nbytes);

  sread = fread(&ccode, 1, 2, inp);
  swrite = fwrite(&ccode, 1, 2, outp);
  printf("compression code = %d \n", ccode);

  sread = fread(&channels, 1, 2, inp);
  swrite = fwrite(&channels, 1, 2, outp);
  printf("channels = %d \n", channels);

  sread = fread(&rate, 1, 4, inp);
  swrite = fwrite(&rate, 1, 4, outp);
  printf("rate = %d  \n", rate);

  sread = fread(&avgrate, 1, 4, inp);
  swrite = fwrite(&avgrate, 1, 4, outp);
  printf("avg rate = %d \n", avgrate);

  sread = fread(&blockalign, 1, 2, inp);
  swrite = fwrite(&blockalign, 1, 2, outp);
  printf("blockalign = %d  \n", blockalign);

  sread = fread(&bps, 1, 2, inp);
  swrite = fwrite(&bps, 1, 2, outp);
  printf("bits per sample = %d \n", bps);

  sread = fread(&data[0], 1, 4, inp);
  swrite = fwrite(&data[0], 1, 4, outp);
  printf("should be data, <%c%c%c%c>\n",data[0],data[1],data[2],data[3]);

  sread = fread(&csize, 1, 4, inp);
  swrite = fwrite(&csize, 1, 4, outp);
  printf("chunk has %d bytes \n", csize);
  nbread = 44;
  printf("%d bytes read so far \n", nbread);
  
  bad = 0;
  savg = 0;
  for(i=0; i<csize; i++)
  {
    sread = fread(&sbyte, 1, 1, inp);
    if(sread != 1 && bad==0) { bad=1; printf("no read on byte %d \n", i); } 
    swrite = fwrite(&sbyte, 1, 1, outp);
    fprintf(outd,"%9.5f\n", (double)sbyte/128.0);
    ibyte = sbyte;
    savg = savg + ibyte;
    if(i==0) {smin=ibyte; smax=ibyte;}
    smin = ibyte<smin?ibyte:smin;
    smax = ibyte>smax?ibyte:smax;
    if(i<10 || i>csize-10)
      printf("sound byte %d=%X  %d  %d \n", i, sbyte, sbyte, ibyte);
  }
  savg = savg / csize;
  printf("smin=%d, smax=%d, savg=%d \n", smin, smax, savg);

  nbread = nbread+csize;
  printf("%d bytes read so far \n", nbread);

  while(1) /* just copy remaining chunks */
  {
    sread = fread(&more[0], 1, 4, inp);
    if(sread != 4) goto done;
    swrite = fwrite(&more[0], 1, 4, outp);
    printf("could be , <%c%c%c%c>\n",more[0],more[1],more[2],more[3]);

    sread = fread(&csize, 1, 4, inp); /* check for more chunks */
    if(sread != 4) goto done;
    swrite = fwrite(&csize, 1, 4, outp);
    printf("chunk has %d bytes \n", csize);
    for(i=0; i<csize; i++)
    {
      sread = fread(&sbyte, 1, 1, inp);
      if(sread != 1) goto done;
      swrite = fwrite(&sbyte, 1, 1, outp);
      fprintf(outd,"%9.5f\n", (double)sbyte/128.0);
    }
    nbread = nbread + 8 + csize;
    printf("%d bytes read so far \n", nbread);
  } /* end copying remaining chunks */
  
done: ;
  fclose(inp);
  fflush(outp);
  fclose(outp);
  fflush(outd);
  fclose(outd);

  if(argc<3) printf("read_wav done. new junk.wav file written \n");
  else       printf("read_wav done. new %s file written \n", argv[2]);
  if(argc<4) printf("read_wav done. new junk.dat file written \n");
  else       printf("read_wav done. new %s file written \n", argv[3]);
  return 0;
} /* end read_wav .c */
