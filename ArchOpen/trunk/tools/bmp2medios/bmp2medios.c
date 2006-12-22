#include <stdio.h>
#include <stdlib.h>

#include "../../include/kernel/gui_pal.h"

#pragma pack(1)

typedef struct {
   unsigned short type;
   unsigned long size;
   unsigned short reserved1;
   unsigned short reserved2;
   unsigned long offsetbits;
} BITMAPFILEHEADER;

typedef struct {
   unsigned long size;
   unsigned long width;
   unsigned long height;
   unsigned short planes;
   unsigned short bitcount;
   unsigned long compression;
   unsigned long sizeimage;
   long xpelspermeter;
   long ypelspermeter;
   unsigned long colorsused;
   unsigned long colorsimportant;
} BITMAPINFOHEADER;


long dist_3D(unsigned char r,unsigned char g,unsigned char b,
                unsigned char pr,unsigned char pg,unsigned char pb)
{
    long R,G,B;
    long res;

    R=abs(r-pr);
    G=abs(g-pg);
    B=abs(b-pb);
    
    res=R*R+G*G+B*B;
    return res;
}

void doPrint(int index,long val,unsigned char r,unsigned char g,unsigned char b,
                unsigned char pr,unsigned char pg,unsigned char pb)
{
    printf("i=%d dist=%d pix=[0x%02x,0x%02x,0x%02x] pal=[0x%02x,0x%02x,0x%02x]\n",index,val,
                r,g,b,
                pr,pg,pb);
}

int exact_match[256];

int get_nearest(unsigned char r,unsigned char g,unsigned char b)
{
    int min_index=0;
    long min_val=0x7fffffffL;;
    long new_val;
    int i;
    for(i=0;i<256;i++)
    {
        new_val=dist_3D(r,g,b,gui_pal[i][0],gui_pal[i][1],gui_pal[i][2]);
        if(new_val==0)
        {
            min_index=i;
            min_val=new_val;
            exact_match[i]++;
            break;
        }
        if(new_val<min_val)
        {
            min_val=new_val;
            min_index=i;
        }
    }
    
    return min_index;
}

void outputStr(FILE * out,char * str)
{
    while(*str)
    {
        fputc(*str,out);
        str++;
    }
}

int color_used[256];


int main(int argc, char* argv[]) {
    int w,h,i,j,c,s;
    int stop=0;
    unsigned char rgb[3];
    int match_col;
    int binOut=0;
    char * str;

    FILE* infile;
    FILE* outfile;

    BITMAPFILEHEADER bmphdr;
    BITMAPINFOHEADER bmpinfo;

    for(i=0;i<256;i++)
    {
        color_used[i]=0;
        exact_match[i]=0;
    }

    if (argc != 5 || (argv[1][0]!='h' && argv[1][0]!='i'))
    {
        fprintf(stderr,"Info: %s mode (h|i)  source destination unique_name\n",argv[0]);
        exit(1);
    }

    if(argv[1][0]=='h')
        binOut=0;
    else
        binOut=1;

    infile = fopen(argv[2], "rb");

    if (!infile)
    {
        printf("Can't open file '%s' for reading!\n", argv[2]);
        exit(1);
    }

    //parse BMP file

    s=fread(&bmphdr,sizeof(BITMAPFILEHEADER),1,infile);

    if(s!=1 || bmphdr.type!=0x4d42)
    {
        printf("'%s' is not a BMP file!\n", argv[2]);
        exit(1);
    }

    s=fread(&bmpinfo,sizeof(BITMAPINFOHEADER),1,infile);

    if(s!=1 || bmpinfo.planes!=1 || bmpinfo.bitcount!=24 || bmpinfo.compression!=0)
    {
        printf("'%s' is not a 24bit uncompressed BMP file!\n", argv[2]);
        exit(1);
    }

    w=bmpinfo.width;
    h=bmpinfo.height;

    fseek(infile,0,SEEK_END);
    s=ftell(infile);
    if(s!=(w*h*3+bmphdr.offsetbits))
    {
        printf("'%s' size is wrong!\n", argv[2]);
        exit(1);
    }

    // convert

    if(binOut)
        outfile = fopen(argv[3], "wb");
    else
        outfile = fopen(argv[3], "w");

    if(!binOut)
    {
        printf("Starting conversion: mode=header src=%s dest=%s name:%s_da %s dim=(%d,%d)\n",argv[2],argv[3],argv[4],argv[4],w,h);
        fprintf(outfile,"/*\n* %s\n",argv[3]);
        fprintf(outfile,"*\n* File generated by from %s, (w=%d,h=%d)\n",argv[2],w,h);
        fprintf(outfile,"*/\n\n");
        fprintf(outfile,"#include \"sys_def/graphics.h\"\n\n");
        fprintf(outfile,"unsigned char %s_data[%d][%d] = {\n",argv[4],h,w);
   }
   else
   {
       printf("Starting conversion: mode=icon src=%s dest=%s name:%s dim=(%d,%d)\n",argv[2],argv[3],argv[4],w,h);
       outputStr(outfile,"AVICO");
       i=strlen(argv[4]);
       fputc(i,outfile);
       outputStr(outfile,argv[4]);
       fputc(w,outfile);
       fputc(h,outfile);
   }

    //seek to first line start
    fseek(infile,-3*w,SEEK_END);

    for(i=0;i<h && !stop;i++)
    {
        if(!binOut)
        {
            if(i!=0)
                fprintf(outfile,",\n");
            fprintf(outfile,"{ ");
        }
        
        for(j=0;j<w;j++)
        {
            if(!binOut)
            {
                if(j!=0)
                    fprintf(outfile,",");
            }

            c = fread(rgb, 1, 3, infile);

            if (c == 0)
            {
                stop=1;
                break;
            }

            if (c != 3)
            {
                fprintf(stderr,"\nMismatch!\n\n");
                stop=1;
                break;
            }

            match_col=get_nearest(rgb[2],rgb[1],rgb[0]);
            color_used[match_col]++;

            if(!binOut)
            {
                if(!(j % 20)) fprintf(outfile,"\n    ");
                fprintf(outfile,"0x%02x",match_col);
            }
            else
            {
                fputc(match_col,outfile);
            }
        }

        //seek to next line start
        fseek(infile,-6*w,SEEK_CUR);

        if(!binOut)
        {
            fprintf(outfile,"\n}");
        }
    }
    if(!binOut)
    {
        fprintf(outfile,"};\n\n");
        fprintf(outfile,"BITMAP %s = {(unsigned int) %s_data, %d, %d, 0, 0};\n\n",argv[4],argv[4],w,h);
    }

    j=0;c=0;
    
    printf("color used:\n");
    
    for(i=0;i<256;i++)
        if(color_used[i])
        {
            printf("%03d : %04d (%03d,%03d,%03d)\n",i,color_used[i],gui_pal[i][0],gui_pal[i][1],gui_pal[i][2]);
            j++;
        }
    printf("%d color used\n\n",j);
    printf("Exact match:\n");
    for(i=0;i<256;i++)    
        if(exact_match[i])
        {
            printf("%03d : %04d\n",i,exact_match[i]);
            c++;
        }
    printf("%d exact match\n",c);
    
    fclose(infile);
    fclose(outfile);
    
    return 0;
}
